#pragma once
#include "ipmb/types.h"

typedef struct {
    uint32_t offset;
    uint32_t length;
    const uint8_t *data;
} ipmb_fragment_t;

ipmb_status_t ipmb_reassemble_fragments(const ipmb_fragment_t *frags,
                                        size_t count,
                                        uint32_t total_size,
                                        uint8_t **out,
                                        size_t *out_len);
ipmb_status_t ipmb_parse_fragment_payload(const uint8_t *payload,
                                          size_t payload_len,
                                          uint8_t **out,
                                          size_t *out_len);
