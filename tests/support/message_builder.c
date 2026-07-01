#include "message_builder.h"

#include <stdlib.h>
#include <string.h>

#include "ipmb/types.h"
#include "ipmb/util/checksum.h"
#include "ipmb/util/endian.h"

static void store_le64(uint8_t *p, uint64_t v)
{
    for (unsigned i = 0; i < 8; ++i) {
        p[i] = (uint8_t)((v >> (i * 8u)) & 0xffu);
    }
}

static void put_record(uint8_t *buf, size_t *pos, uint8_t type,
                       uint32_t session_id, uint32_t peer_id,
                       const uint8_t *payload, uint16_t payload_len)
{
    buf[(*pos)++] = type;
    ipmb_store_le32(buf + *pos, session_id);
    *pos += 4;
    ipmb_store_le32(buf + *pos, peer_id);
    *pos += 4;
    ipmb_store_le16(buf + *pos, payload_len);
    *pos += 2;
    if (payload_len) {
        memcpy(buf + *pos, payload, payload_len);
    }
    *pos += payload_len;
}

uint8_t *ipmb_test_make_telemetry(size_t *len)
{
    const uint8_t value[] = { 0x2a };
    const char name[] = "temp";
    *len = 20u;
    uint8_t *buf = (uint8_t *)calloc(*len, 1u);
    if (!buf) return NULL;

    memcpy(buf, "ITLM", 4);
    ipmb_store_le16(buf + 4, 3u);
    ipmb_store_le16(buf + 6, 1u);
    buf[8] = 1u;
    ipmb_store_le16(buf + 9, (uint16_t)(sizeof(name) - 1u));
    memcpy(buf + 11, name, sizeof(name) - 1u);
    ipmb_store_le32(buf + 15, sizeof(value));
    memcpy(buf + 19, value, sizeof(value));
    return buf;
}

uint8_t *ipmb_test_make_message(size_t *len)
{
    size_t payload_len = 0;
    uint8_t *payload = ipmb_test_make_telemetry(&payload_len);
    if (!payload) return NULL;

    *len = 28u + 20u + payload_len + 4u;
    uint8_t *buf = (uint8_t *)calloc(*len, 1u);
    if (!buf) {
        free(payload);
        return NULL;
    }

    memcpy(buf, "IPMB", 4);
    ipmb_store_le16(buf + 4, 1u);
    ipmb_store_le16(buf + 6, 0u);
    ipmb_store_le32(buf + 8, 1u);
    store_le64(buf + 12, 0x0102030405060708ull);
    ipmb_store_le32(buf + 20, 77u);
    ipmb_store_le32(buf + 24, ipmb_crc32(buf, 24u));

    size_t frame = 28u;
    ipmb_store_le16(buf + frame, 0x22u);
    ipmb_store_le16(buf + frame + 2u, 0u);
    ipmb_store_le32(buf + frame + 4u, (uint32_t)payload_len);
    ipmb_store_le32(buf + frame + 8u, 100u);
    ipmb_store_le32(buf + frame + 12u, 200u);
    ipmb_store_le16(buf + frame + 16u, 5u);
    ipmb_store_le16(buf + frame + 18u, 64u);
    memcpy(buf + frame + 20u, payload, payload_len);
    ipmb_store_le32(buf + frame + 20u + payload_len,
                    ipmb_crc32(buf + frame, 20u + payload_len));

    free(payload);
    return buf;
}

uint8_t *ipmb_test_make_journal(size_t *len)
{
    const uint8_t state[] = { 'a', 'b', 'c' };
    *len = 8u + 11u + sizeof(state) + 11u + 11u;
    uint8_t *buf = (uint8_t *)calloc(*len, 1u);
    if (!buf) return NULL;

    memcpy(buf, "IJRN", 4);
    ipmb_store_le16(buf + 4, 1u);
    ipmb_store_le16(buf + 6, 3u);

    size_t pos = 8u;
    put_record(buf, &pos, 1u, 7u, 99u, state, sizeof(state));
    put_record(buf, &pos, 5u, 7u, 99u, NULL, 0u);
    put_record(buf, &pos, 2u, 7u, 99u, NULL, 0u);
    return buf;
}
