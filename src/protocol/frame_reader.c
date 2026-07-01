/*
 * ipmb - Industrial Protocol Message Broker
 * Copyright (c) 2025 Satyabrata Dalai
 *
 * frame_reader.c - Core protocol frame parser
 * Created: 2025-12-16
 */
#include <stdlib.h>
#include <string.h>
#include "ipmb/protocol/frame_reader.h"
#include "ipmb/util/byte_read.h"
#include "ipmb/util/checksum.h"

enum { IPMB_MESSAGE_HEADER_LEN = 28, IPMB_FRAME_HEADER_LEN = 20 };

static void clear_frame(ipmb_frame_t *f)
{
    free(f->payload);
    memset(f, 0, sizeof(*f));
}

void ipmb_message_free(ipmb_message_t *msg)
{
    if (!msg) return;
    if (msg->frames) {
        for (uint32_t i = 0; i < msg->frame_count; ++i) clear_frame(&msg->frames[i]);
    }
    free(msg->frames);
    memset(msg, 0, sizeof(*msg));
}

ipmb_status_t ipmb_parse_message(const uint8_t *data, size_t len,
                                 ipmb_message_t *out)
{
    ipmb_reader_t r;
    uint32_t header_crc;
    memset(out, 0, sizeof(*out));
    if (len < IPMB_MESSAGE_HEADER_LEN) return IPMB_ERR_SHORT_READ;
    if (memcmp(data, "IPMB", 4) != 0) return IPMB_ERR_BAD_MAGIC;

    ipmb_reader_init(&r, data + 4, len - 4);
    if (ipmb_read_le16(&r, &out->version) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (ipmb_read_le16(&r, &out->flags) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (ipmb_read_le32(&r, &out->frame_count) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (ipmb_read_le64(&r, &out->session_id) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (ipmb_read_le32(&r, &out->sequence) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (ipmb_read_le32(&r, &header_crc) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    r.data = data;
    r.len = len;
    r.pos = IPMB_MESSAGE_HEADER_LEN;

    if (out->frame_count > IPMB_MAX_FRAMES) return IPMB_ERR_LIMIT;
    if (ipmb_crc32(data, 24) != header_crc) return IPMB_ERR_BAD_CRC;

    out->frames = (ipmb_frame_t *)calloc(out->frame_count ? out->frame_count : 1u,
                                         sizeof(ipmb_frame_t));
    if (!out->frames) return IPMB_ERR_ALLOC;

    for (uint32_t i = 0; i < out->frame_count; ++i) {
        size_t frame_start = r.pos;
        uint32_t payload_len;
        const uint8_t *payload;
        ipmb_frame_t *f = &out->frames[i];
        if (ipmb_reader_remaining(&r) < IPMB_FRAME_HEADER_LEN) {
            ipmb_message_free(out);
            return IPMB_ERR_SHORT_READ;
        }
        if (ipmb_read_le16(&r, &f->frame_type) != IPMB_OK ||
            ipmb_read_le16(&r, &f->frame_flags) != IPMB_OK ||
            ipmb_read_le32(&r, &payload_len) != IPMB_OK ||
            ipmb_read_le32(&r, &f->source_addr) != IPMB_OK ||
            ipmb_read_le32(&r, &f->dest_addr) != IPMB_OK ||
            ipmb_read_le16(&r, &f->priority) != IPMB_OK ||
            ipmb_read_le16(&r, &f->ttl) != IPMB_OK) {
            ipmb_message_free(out);
            return IPMB_ERR_SHORT_READ;
        }
        if (payload_len > IPMB_MAX_PAYLOAD) {
            ipmb_message_free(out);
            return IPMB_ERR_LIMIT;
        }
        if (ipmb_read_bytes(&r, payload_len, &payload) != IPMB_OK ||
            ipmb_read_le32(&r, &f->crc) != IPMB_OK) {
            ipmb_message_free(out);
            return IPMB_ERR_SHORT_READ;
        }
        if (ipmb_crc32(data + frame_start, IPMB_FRAME_HEADER_LEN + payload_len) != f->crc) {
            ipmb_message_free(out);
            return IPMB_ERR_BAD_CRC;
        }
        f->payload = (uint8_t *)malloc(payload_len ? payload_len : 1u);
        if (!f->payload) {
            ipmb_message_free(out);
            return IPMB_ERR_ALLOC;
        }
        memcpy(f->payload, payload, payload_len);
        f->payload_len = payload_len;
    }
    return IPMB_OK;
}

const ipmb_frame_t *ipmb_message_first_frame(const ipmb_message_t *msg,
                                             uint16_t frame_type)
{
    if (!msg) return NULL;
    for (uint32_t i = 0; i < msg->frame_count; ++i) {
        if (msg->frames[i].frame_type == frame_type) return &msg->frames[i];
    }
    return NULL;
}
