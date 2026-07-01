#pragma once
#include "ipmb/types.h"

typedef struct {
    uint16_t offset;
    uint16_t length;
    uint8_t *bytes;
} ipmb_register_field_t;

typedef struct {
    uint16_t map_id;
    uint16_t field_count;
    ipmb_register_field_t *fields;
} ipmb_register_map_t;

ipmb_status_t ipmb_decode_register_map(const uint8_t *payload,
                                       size_t payload_len,
                                       ipmb_register_map_t *out);
void ipmb_register_map_free(ipmb_register_map_t *map);
