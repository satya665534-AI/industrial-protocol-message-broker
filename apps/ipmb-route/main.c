#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ipmb/ipmb.h"

static uint8_t *read_file(const char *path, size_t *len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return NULL; }
    long sz = ftell(fp);
    if (sz < 0) { fclose(fp); return NULL; }
    rewind(fp);
    uint8_t *buf = (uint8_t *)malloc((size_t)sz ? (size_t)sz : 1u);
    if (!buf) { fclose(fp); return NULL; }
    if (fread(buf, 1, (size_t)sz, fp) != (size_t)sz) {
        free(buf);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    *len = (size_t)sz;
    return buf;
}


int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <rule> <message.bin>\n", argv[0]);
        return 2;
    }
    size_t len = 0;
    uint8_t *buf = read_file(argv[2], &len);
    if (!buf) return 1;
    ipmb_message_t msg;
    ipmb_status_t rc = ipmb_parse_message(buf, len, &msg);
    if (rc != IPMB_OK || msg.frame_count == 0) {
        fprintf(stderr, "message rejected: %s\n", ipmb_status_string(rc));
        free(buf);
        return 1;
    }
    ipmb_rule_ast_t ast;
    ipmb_rule_bytecode_t bc;
    bool matched = false;
    rc = ipmb_rule_parse(argv[1], &ast);
    if (rc == IPMB_OK) rc = ipmb_rule_compile(&ast, &bc);
    if (rc == IPMB_OK) {
        rc = ipmb_vm_evaluate(&bc, &msg.frames[0], &matched);
        ipmb_rule_bytecode_free(&bc);
    }
    printf("%s\n", (rc == IPMB_OK && matched) ? "matched" : "not matched");
    ipmb_message_free(&msg);
    free(buf);
    return rc == IPMB_OK ? 0 : 1;
}
