#pragma once
#include <stdint.h>

uint16_t ipmb_load_le16(const uint8_t *p);
uint32_t ipmb_load_le32(const uint8_t *p);
uint64_t ipmb_load_le64(const uint8_t *p);
void ipmb_store_le16(uint8_t *p, uint16_t v);
void ipmb_store_le32(uint8_t *p, uint32_t v);
