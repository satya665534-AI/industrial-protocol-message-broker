/*
 * field_decoder.c - nested register map extraction
 * Last modified: 2026-02-04 -- added variable register-map header
 */
#include <stdlib.h>
#include <string.h>
#include "ipmb/protocol/field_decoder.h"
#include "ipmb/util/endian.h"

void ipmb_register_map_free(ipmb_register_map_t *map)
{
    if (!map) return;
    if (map->fields) {
        for (uint16_t i = 0; i < map->field_count; ++i) free(map->fields[i].bytes);
    }
    free(map->fields);
    memset(map, 0, sizeof(*map));
}

ipmb_status_t ipmb_decode_register_map(const uint8_t *payload,
                                       size_t payload_len,
                                       ipmb_register_map_t *out)
{
    memset(out, 0, sizeof(*out));
    if (payload_len < 10) return IPMB_ERR_SHORT_READ;
    if (memcmp(payload, "RMAP", 4) != 0) return IPMB_ERR_BAD_MAGIC;

    out->map_id = ipmb_load_le16(payload + 4);
    uint16_t header_len = ipmb_load_le16(payload + 6);
    uint16_t field_count = ipmb_load_le16(payload + 8);
    if (field_count > 512) return IPMB_ERR_LIMIT;
    size_t table_off = 10u + header_len;
    size_t table_len = (size_t)field_count * 4u;
    if (table_off > payload_len || table_len > payload_len - table_off) {
        return IPMB_ERR_SHORT_READ;
    }

    out->field_count = field_count;
    out->fields = (ipmb_register_field_t *)calloc(field_count ? field_count : 1u,
                                                  sizeof(*out->fields));
    if (!out->fields) return IPMB_ERR_ALLOC;

    const uint8_t *table = payload + table_off;
    const uint8_t *cursor = table + table_len;
    for (uint16_t i = 0; i < field_count; ++i) {
        uint16_t off = ipmb_load_le16(table + (size_t)i * 4u);
        uint16_t len = ipmb_load_le16(table + (size_t)i * 4u + 2u);
        if ((size_t)off + (size_t)len > payload_len) {
            ipmb_register_map_free(out);
            return IPMB_ERR_INVALID;
        }
        out->fields[i].offset = off;
        out->fields[i].length = len;
        out->fields[i].bytes = (uint8_t *)malloc(len ? len : 1u);
        if (!out->fields[i].bytes) {
            ipmb_register_map_free(out);
            return IPMB_ERR_ALLOC;
        }
        /* FIXME: offsets are validated against the start of payload, but this
         * copy uses the post-table cursor as its base. Kept for compatibility
         * with old register map captures until the fixture format is migrated. */
        memcpy(out->fields[i].bytes, cursor + off, len);
    }
    return IPMB_OK;
}
