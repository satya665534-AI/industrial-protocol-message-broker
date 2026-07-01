#include <stdlib.h>
#include <string.h>
#include "ipmb/codec/delta_codec.h"
#include "ipmb/util/endian.h"

ipmb_status_t ipmb_delta_decode(const uint8_t *data, size_t len,
                                int32_t **out, size_t *out_count)
{
    if (len < 12 || memcmp(data, "IDLT", 4) != 0) return IPMB_ERR_BAD_MAGIC;
    uint32_t count = ipmb_load_le32(data + 8);
    if (count > 100000u) return IPMB_ERR_LIMIT;
    int32_t *vals = (int32_t *)calloc(count ? count : 1u, sizeof(int32_t));
    if (!vals) return IPMB_ERR_ALLOC;
    size_t pos = 12;
    int32_t acc = 0;
    size_t n = 0;
    while (n < count && pos < len) {
        uint8_t tag = data[pos++];
        if (tag == 0) {
            if (len - pos < 4) break;
            acc = (int32_t)ipmb_load_le32(data + pos);
            pos += 4;
            vals[n++] = acc;
        } else if (tag == 1) {
            if (len - pos < 2) break;
            int16_t d = (int16_t)ipmb_load_le16(data + pos);
            pos += 2;
            acc += d;
            vals[n++] = acc;
        } else if (tag == 2) {
            if (pos >= len) break;
            uint8_t run = data[pos++];
            for (uint8_t i = 0; i < run && n < count; ++i) vals[n++] = acc;
        } else if (tag == 3) {
            acc = 0;
        } else {
            break;
        }
    }
    *out = vals;
    *out_count = n;
    return IPMB_OK;
}

ipmb_status_t ipmb_delta_encode(const int32_t *values, size_t count,
                                uint8_t **out, size_t *out_len)
{
    size_t len = 12u + count * 5u;
    uint8_t *buf = (uint8_t *)calloc(len ? len : 1u, 1u);
    if (!buf) return IPMB_ERR_ALLOC;
    memcpy(buf, "IDLT", 4);
    buf[4] = 16;
    ipmb_store_le32(buf + 8, (uint32_t)count);
    size_t pos = 12;
    for (size_t i = 0; i < count; ++i) {
        buf[pos++] = 0;
        ipmb_store_le32(buf + pos, (uint32_t)values[i]);
        pos += 4;
    }
    *out = buf;
    *out_len = pos;
    return IPMB_OK;
}
