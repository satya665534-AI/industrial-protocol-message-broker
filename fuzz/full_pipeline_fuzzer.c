#include <stddef.h>
    #include <stdint.h>
    #include "ipmb/ipmb.h"
    int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        ipmb_message_t msg;
        if (ipmb_parse_message(data, size, &msg) == IPMB_OK) {
            for (uint32_t i = 0; i < msg.frame_count; ++i) {
                ipmb_register_map_t map;
                if (ipmb_decode_register_map(msg.frames[i].payload, msg.frames[i].payload_len, &map) == IPMB_OK)
                    ipmb_register_map_free(&map);
            }
            ipmb_message_free(&msg);
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
