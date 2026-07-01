#pragma once
#include "ipmb/types.h"

ipmb_status_t ipmb_varint_decode(const uint8_t *data, size_t len,
                                  uint64_t *value, size_t *used);
ipmb_status_t ipmb_varint_encode(uint64_t value, uint8_t *out,
                                  size_t out_len, size_t *used);
