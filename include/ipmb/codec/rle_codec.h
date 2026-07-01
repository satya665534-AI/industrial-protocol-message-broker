#pragma once
#include "ipmb/types.h"

ipmb_status_t ipmb_rle_decode(const uint8_t *data, size_t len,
                              uint8_t **out, size_t *out_len);
ipmb_status_t ipmb_rle_encode(const uint8_t *data, size_t len,
                              uint8_t **out, size_t *out_len);
