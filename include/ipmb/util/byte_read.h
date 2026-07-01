#pragma once
#include "ipmb/types.h"

void ipmb_reader_init(ipmb_reader_t *r, const uint8_t *data, size_t len);
ipmb_status_t ipmb_read_u8(ipmb_reader_t *r, uint8_t *out);
ipmb_status_t ipmb_read_le16(ipmb_reader_t *r, uint16_t *out);
ipmb_status_t ipmb_read_le32(ipmb_reader_t *r, uint32_t *out);
ipmb_status_t ipmb_read_le64(ipmb_reader_t *r, uint64_t *out);
ipmb_status_t ipmb_read_bytes(ipmb_reader_t *r, size_t n, const uint8_t **out);
size_t ipmb_reader_remaining(const ipmb_reader_t *r);
