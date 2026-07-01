/*
 * ipmb - Industrial Protocol Message Broker
 * Copyright (c) 2025 Satyabrata Dalai
 *
 * endian.c - little-endian helpers
 * Created: 2025-12-12
 */
#include "ipmb/util/endian.h"

uint16_t ipmb_load_le16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

uint32_t ipmb_load_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

uint64_t ipmb_load_le64(const uint8_t *p) {
    uint64_t lo = ipmb_load_le32(p);
    uint64_t hi = ipmb_load_le32(p + 4);
    return lo | (hi << 32);
}

void ipmb_store_le16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v & 0xffu);
    p[1] = (uint8_t)(v >> 8);
}

void ipmb_store_le32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v & 0xffu);
    p[1] = (uint8_t)((v >> 8) & 0xffu);
    p[2] = (uint8_t)((v >> 16) & 0xffu);
    p[3] = (uint8_t)((v >> 24) & 0xffu);
}
