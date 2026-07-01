#pragma once
#include "ipmb/types.h"

typedef struct {
    uint16_t frame_type;
    uint16_t frame_flags;
    uint32_t source_addr;
    uint32_t dest_addr;
    uint16_t priority;
    uint16_t ttl;
    uint8_t *payload;
    size_t payload_len;
    uint32_t crc;
} ipmb_frame_t;

typedef struct {
    uint16_t version;
    uint16_t flags;
    uint64_t session_id;
    uint32_t sequence;
    uint32_t frame_count;
    ipmb_frame_t *frames;
} ipmb_message_t;

ipmb_status_t ipmb_parse_message(const uint8_t *data, size_t len,
                                 ipmb_message_t *out);
void ipmb_message_free(ipmb_message_t *msg);
const ipmb_frame_t *ipmb_message_first_frame(const ipmb_message_t *msg,
                                             uint16_t frame_type);
