#pragma once
#include "ipmb/types.h"

ipmb_status_t ipmb_delta_decode(const uint8_t *data, size_t len,
                                int32_t **out, size_t *out_count);
ipmb_status_t ipmb_delta_encode(const int32_t *values, size_t count,
                                uint8_t **out, size_t *out_len);
