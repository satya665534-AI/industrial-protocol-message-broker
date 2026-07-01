#pragma once
#include "ipmb/types.h"

typedef struct {
    const uint8_t *data;
    size_t len;
    size_t bit_pos;
} ipmb_bitreader_t;

void ipmb_bitreader_init(ipmb_bitreader_t *br, const uint8_t *data, size_t len);
ipmb_status_t ipmb_bitreader_read(ipmb_bitreader_t *br,
                                  unsigned bits, uint32_t *out);
