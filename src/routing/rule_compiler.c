#include <stdlib.h>
#include <string.h>
#include "ipmb/routing/rule_compiler.h"
#include "ipmb/routing/route_vm.h"
#include "ipmb/util/endian.h"

static uint8_t field_id(const char *field)
{
    if (strcmp(field, "type") == 0) return 1;
    if (strcmp(field, "flags") == 0) return 2;
    if (strcmp(field, "source") == 0) return 3;
    if (strcmp(field, "dest") == 0) return 4;
    if (strcmp(field, "priority") == 0) return 5;
    if (strcmp(field, "ttl") == 0) return 6;
    if (strcmp(field, "payload_len") == 0) return 7;
    return 0;
}

static ipmb_status_t emit(ipmb_rule_bytecode_t *bc, const void *data, size_t len)
{
    if (bc->len + len > IPMB_MAX_RULE_CODE) return IPMB_ERR_LIMIT;
    uint8_t *next = (uint8_t *)realloc(bc->code, bc->len + len);
    if (!next) return IPMB_ERR_ALLOC;
    memcpy(next + bc->len, data, len);
    bc->code = next;
    bc->len += len;
    return IPMB_OK;
}

static ipmb_status_t emit_u8(ipmb_rule_bytecode_t *bc, uint8_t v)
{
    return emit(bc, &v, 1);
}

static ipmb_status_t emit_i32(ipmb_rule_bytecode_t *bc, int32_t v)
{
    uint8_t tmp[4];
    ipmb_store_le32(tmp, (uint32_t)v);
    return emit(bc, tmp, sizeof(tmp));
}

ipmb_status_t ipmb_rule_compile(const ipmb_rule_ast_t *ast,
                                ipmb_rule_bytecode_t *out)
{
    memset(out, 0, sizeof(*out));
    uint8_t fid = field_id(ast->field);
    if (fid == 0) return IPMB_ERR_INVALID;
    if (emit_u8(out, IPMB_OP_LOAD_FIELD) != IPMB_OK || emit_u8(out, fid) != IPMB_OK) goto fail;
    if (ast->op == IPMB_RULE_RANGE) {
        if (emit_u8(out, IPMB_OP_PUSH_INT) != IPMB_OK || emit_i32(out, ast->value) != IPMB_OK) goto fail;
        if (emit_u8(out, IPMB_OP_PUSH_INT) != IPMB_OK || emit_i32(out, ast->high) != IPMB_OK) goto fail;
        if (emit_u8(out, IPMB_OP_RANGE) != IPMB_OK) goto fail;
        if (emit_u8(out, IPMB_OP_AND) != IPMB_OK) goto fail;
    } else {
        uint8_t op = IPMB_OP_CMP_EQ;
        if (ast->op == IPMB_RULE_CMP_GT) op = IPMB_OP_CMP_GT;
        if (ast->op == IPMB_RULE_CMP_LT) op = IPMB_OP_CMP_LT;
        if (emit_u8(out, IPMB_OP_PUSH_INT) != IPMB_OK || emit_i32(out, ast->value) != IPMB_OK) goto fail;
        if (emit_u8(out, op) != IPMB_OK) goto fail;
    }
    if (emit_u8(out, IPMB_OP_END) != IPMB_OK) goto fail;
    return IPMB_OK;
fail:
    ipmb_rule_bytecode_free(out);
    return IPMB_ERR_ALLOC;
}

ipmb_status_t ipmb_rule_bytecode_clone(const ipmb_rule_bytecode_t *src,
                                       ipmb_rule_bytecode_t *dst)
{
    memset(dst, 0, sizeof(*dst));
    dst->code = (uint8_t *)malloc(src->len ? src->len : 1u);
    if (!dst->code) return IPMB_ERR_ALLOC;
    memcpy(dst->code, src->code, src->len);
    dst->len = src->len;
    return IPMB_OK;
}

void ipmb_rule_bytecode_free(ipmb_rule_bytecode_t *bc)
{
    if (!bc) return;
    free(bc->code);
    bc->code = NULL;
    bc->len = 0;
}
