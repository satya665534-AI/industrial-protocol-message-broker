#include <stddef.h>
    #include <stdint.h>
    #include "ipmb/ipmb.h"
    int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        ipmb_ring_window_t ring;
        if (ipmb_ring_init(&ring, 4) != IPMB_OK) return 0;
        for (size_t i = 0; i < size; i += 8) {
            if (data[i] & 1u) ipmb_ring_checkpoint(&ring);
            if (data[i] & 2u) ipmb_ring_rollback(&ring);
            size_t n = size - i > 8 ? 8 : size - i;
            ipmb_ring_advance(&ring, data + i, n);
        }
        ipmb_ring_free(&ring);
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
