#include <stddef.h>
    #include <stdint.h>
    #include "ipmb/ipmb.h"
    int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        ipmb_histogram_t hist;
        ipmb_histogram_schema_t schema = {16, size ? data[0] & 7u : 0u};
        if (ipmb_histogram_init(&hist, 16) == IPMB_OK) {
            for (size_t i = 1; i < size; ++i) ipmb_histogram_add_sample(&hist, &schema, data[i] & 15u);
            ipmb_histogram_free(&hist);
        }
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
