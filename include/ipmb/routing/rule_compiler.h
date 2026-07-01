#pragma once
#include "ipmb/routing/rule_parser.h"

typedef struct {
    uint8_t *code;
    size_t len;
} ipmb_rule_bytecode_t;

ipmb_status_t ipmb_rule_compile(const ipmb_rule_ast_t *ast,
                                ipmb_rule_bytecode_t *out);
ipmb_status_t ipmb_rule_bytecode_clone(const ipmb_rule_bytecode_t *src,
                                       ipmb_rule_bytecode_t *dst);
void ipmb_rule_bytecode_free(ipmb_rule_bytecode_t *bc);
