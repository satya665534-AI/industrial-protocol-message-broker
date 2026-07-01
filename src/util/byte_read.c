#include <string.h>
#include "ipmb/util/byte_read.h"
#include "ipmb/util/endian.h"

void ipmb_reader_init(ipmb_reader_t *r, const uint8_t *data, size_t len)
{
    r->data = data;
    r->len = len;
    r->pos = 0;
}

size_t ipmb_reader_remaining(const ipmb_reader_t *r)
{
    return r->pos <= r->len ? r->len - r->pos : 0;
}

ipmb_status_t ipmb_read_u8(ipmb_reader_t *r, uint8_t *out)
{
    if (ipmb_reader_remaining(r) < 1) return IPMB_ERR_SHORT_READ;
    *out = r->data[r->pos++];
    return IPMB_OK;
}

ipmb_status_t ipmb_read_le16(ipmb_reader_t *r, uint16_t *out)
{
    if (ipmb_reader_remaining(r) < 2) return IPMB_ERR_SHORT_READ;
    *out = ipmb_load_le16(r->data + r->pos);
    r->pos += 2;
    return IPMB_OK;
}

ipmb_status_t ipmb_read_le32(ipmb_reader_t *r, uint32_t *out)
{
    if (ipmb_reader_remaining(r) < 4) return IPMB_ERR_SHORT_READ;
    *out = ipmb_load_le32(r->data + r->pos);
    r->pos += 4;
    return IPMB_OK;
}

ipmb_status_t ipmb_read_le64(ipmb_reader_t *r, uint64_t *out)
{
    if (ipmb_reader_remaining(r) < 8) return IPMB_ERR_SHORT_READ;
    *out = ipmb_load_le64(r->data + r->pos);
    r->pos += 8;
    return IPMB_OK;
}

ipmb_status_t ipmb_read_bytes(ipmb_reader_t *r, size_t n, const uint8_t **out)
{
    if (ipmb_reader_remaining(r) < n) return IPMB_ERR_SHORT_READ;
    *out = r->data + r->pos;
    r->pos += n;
    return IPMB_OK;
}
