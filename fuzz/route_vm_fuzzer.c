#include <stddef.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ipmb/ipmb.h"
    int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        char *text = (char *)malloc(size + 1u);
        if (!text) return 0;
        memcpy(text, data, size); text[size] = 0;
        ipmb_rule_ast_t ast;
        ipmb_rule_bytecode_t bc;
        ipmb_frame_t frame = {10,0,1,2,5,64,NULL,8,0};
        bool matched = false;
        if (ipmb_rule_parse(text, &ast) == IPMB_OK && ipmb_rule_compile(&ast, &bc) == IPMB_OK) {
            ipmb_vm_evaluate(&bc, &frame, &matched);
            ipmb_rule_bytecode_free(&bc);
        }
        free(text);
        return 0;
    }

#ifdef IPMB_STANDALONE_FUZZER
#include <stdio.h>
#include <stdlib.h>
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);
int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        FILE *fp = fopen(argv[i], "rb");
        if (!fp) continue;
        fseek(fp, 0, SEEK_END);
        long n = ftell(fp);
        rewind(fp);
        uint8_t *buf = (uint8_t *)malloc(n > 0 ? (size_t)n : 1u);
        if (buf && fread(buf, 1, (size_t)n, fp) == (size_t)n) {
            LLVMFuzzerTestOneInput(buf, (size_t)n);
        }
        free(buf);
        fclose(fp);
    }
    return 0;
}
#endif
