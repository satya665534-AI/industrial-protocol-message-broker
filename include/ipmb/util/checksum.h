#pragma once
#include <stddef.h>
#include <stdint.h>

uint32_t ipmb_crc32(const uint8_t *data, size_t len);
uint32_t ipmb_crc32_extend(uint32_t seed, const uint8_t *data, size_t len);
