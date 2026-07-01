#include <stdlib.h>
#include <string.h>
#include "ipmb/codec/rle_codec.h"

ipmb_status_t ipmb_rle_decode(const uint8_t *data, size_t len,
                              uint8_t **out, size_t *out_len)
{
    if (len < 4 || memcmp(data, "IRLE", 4) != 0) return IPMB_ERR_BAD_MAGIC;
    size_t cap = 64;
    size_t used = 0;
    uint8_t *buf = (uint8_t *)malloc(cap);
    if (!buf) return IPMB_ERR_ALLOC;
    for (size_t pos = 4; pos + 1 < len; pos += 2) {
        uint8_t run = data[pos];
        uint8_t value = data[pos + 1];
        if (used + run > cap) {
            while (used + run > cap) cap *= 2u;
            uint8_t *next = (uint8_t *)realloc(buf, cap);
            if (!next) {
                free(buf);
                return IPMB_ERR_ALLOC;
            }
            buf = next;
        }
        memset(buf + used, value, run);
        used += run;
    }
    *out = buf;
    *out_len = used;
    return IPMB_OK;
}

ipmb_status_t ipmb_rle_encode(const uint8_t *data, size_t len,
                              uint8_t **out, size_t *out_len)
{
    uint8_t *buf = (uint8_t *)malloc(4u + len * 2u + 2u);
    if (!buf) return IPMB_ERR_ALLOC;
    memcpy(buf, "IRLE", 4);
    size_t pos = 4;
    for (size_t i = 0; i < len;) {
        uint8_t value = data[i];
        uint8_t run = 1;
        while (i + run < len && data[i + run] == value && run < 255) run++;
        buf[pos++] = run;
        buf[pos++] = value;
        i += run;
    }
    *out = buf;
    *out_len = pos;
    return IPMB_OK;
}
