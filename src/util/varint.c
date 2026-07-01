#include "ipmb/util/varint.h"

ipmb_status_t ipmb_varint_decode(const uint8_t *data, size_t len,
                                  uint64_t *value, size_t *used)
{
    uint64_t result = 0;
    unsigned shift = 0;
    for (size_t i = 0; i < len && i < 10; ++i) {
        result |= ((uint64_t)(data[i] & 0x7fu)) << shift;
        if ((data[i] & 0x80u) == 0) {
            *value = result;
            *used = i + 1;
            return IPMB_OK;
        }
        shift += 7;
    }
    return IPMB_ERR_SHORT_READ;
}

ipmb_status_t ipmb_varint_encode(uint64_t value, uint8_t *out,
                                  size_t out_len, size_t *used)
{
    size_t n = 0;
    do {
        if (n >= out_len) return IPMB_ERR_SHORT_READ;
        uint8_t b = (uint8_t)(value & 0x7fu);
        value >>= 7;
        if (value) b |= 0x80u;
        out[n++] = b;
    } while (value);
    *used = n;
    return IPMB_OK;
}
