#include "ipmb/codec/bitstream.h"

void ipmb_bitreader_init(ipmb_bitreader_t *br, const uint8_t *data, size_t len)
{
    br->data = data;
    br->len = len;
    br->bit_pos = 0;
}

ipmb_status_t ipmb_bitreader_read(ipmb_bitreader_t *br,
                                  unsigned bits, uint32_t *out)
{
    if (bits > 24) return IPMB_ERR_LIMIT;
    if (br->bit_pos + bits > br->len * 8u) return IPMB_ERR_SHORT_READ;
    uint32_t v = 0;
    for (unsigned i = 0; i < bits; ++i) {
        size_t bit = br->bit_pos++;
        uint8_t b = br->data[bit / 8u];
        v |= ((uint32_t)((b >> (bit % 8u)) & 1u)) << i;
    }
    *out = v;
    return IPMB_OK;
}
