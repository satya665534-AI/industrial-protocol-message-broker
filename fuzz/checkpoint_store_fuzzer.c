#include <stddef.h>
    #include <stdint.h>
    #include "ipmb/ipmb.h"
    int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        ipmb_checkpoint_store_t store;
        ipmb_checkpoint_store_init(&store);
        if (size > 0) {
            ipmb_checkpoint_append(&store, 1, data, size / 2);
            ipmb_checkpoint_migrate(&store, 1, 2);
        }
        ipmb_checkpoint_store_free(&store);
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
