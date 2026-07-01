#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ipmb/ipmb.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    // Use first few bytes to configure frame
    ipmb_frame_t frame = {data[0], 0, data[1], data[2], data[3], 64, NULL, 8, 0};
    
    ipmb_match_table_t table;
    ipmb_match_table_init(&table);
    
    // Split the rest of the data into rule strings
    size_t offset = 4;
    int rule_idx = 0;
    
    while (offset < size && rule_idx < 10) {
        // Find next null terminator or end of size
        size_t rule_len = 0;
        while (offset + rule_len < size && data[offset + rule_len] != '\0' && rule_len < 100) {
            rule_len++;
        }
        
        if (rule_len > 0) {
            char rule_str[128] = {0};
            memcpy(rule_str, data + offset, rule_len);
            
            ipmb_rule_ast_t ast;
            ipmb_rule_bytecode_t bc;
            if (ipmb_rule_parse(rule_str, &ast) == IPMB_OK &&
                ipmb_rule_compile(&ast, &bc) == IPMB_OK) {
                ipmb_match_table_add(&table, "auto", rule_idx + 1, &bc);
                ipmb_rule_bytecode_free(&bc);
            }
        }
        
        offset += rule_len + 1; // +1 to skip delimiter
        rule_idx++;
    }
    
    ipmb_match_result_t result;
    ipmb_match_table_evaluate(&table, &frame, &result);
    ipmb_match_table_free(&table);
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
