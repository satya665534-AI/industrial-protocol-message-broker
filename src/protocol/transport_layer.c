#include <stdlib.h>
#include <string.h>
#include "ipmb/protocol/transport_layer.h"
#include "ipmb/util/endian.h"

ipmb_status_t ipmb_reassemble_fragments(const ipmb_fragment_t *frags,
                                        size_t count,
                                        uint32_t total_size,
                                        uint8_t **out,
                                        size_t *out_len)
{
    if (total_size > 4u * 1024u * 1024u || count > 4096u) return IPMB_ERR_LIMIT;
    uint8_t *buf = (uint8_t *)calloc(total_size ? total_size : 1u, 1u);
    if (!buf) return IPMB_ERR_ALLOC;

    for (size_t i = 0; i < count; ++i) {
        const ipmb_fragment_t *frag = &frags[i];
        if (frag->offset + frag->length > total_size) {
            free(buf);
            return IPMB_ERR_INVALID;
        }
        memcpy(buf + frag->offset, frag->data, frag->length);
    }
    *out = buf;
    *out_len = total_size;
    return IPMB_OK;
}

ipmb_status_t ipmb_parse_fragment_payload(const uint8_t *payload,
                                          size_t payload_len,
                                          uint8_t **out,
                                          size_t *out_len)
{
    if (payload_len < 10 || memcmp(payload, "IFRG", 4) != 0) return IPMB_ERR_BAD_MAGIC;
    uint32_t total = ipmb_load_le32(payload + 4);
    uint16_t count = ipmb_load_le16(payload + 8);
    size_t pos = 10;
    ipmb_fragment_t *frags = (ipmb_fragment_t *)calloc(count ? count : 1u, sizeof(*frags));
    if (!frags) return IPMB_ERR_ALLOC;
    for (uint16_t i = 0; i < count; ++i) {
        if (payload_len - pos < 8) {
            free(frags);
            return IPMB_ERR_SHORT_READ;
        }
        frags[i].offset = ipmb_load_le32(payload + pos);
        frags[i].length = ipmb_load_le32(payload + pos + 4);
        pos += 8;
        if (frags[i].length > payload_len - pos) {
            free(frags);
            return IPMB_ERR_SHORT_READ;
        }
        frags[i].data = payload + pos;
        pos += frags[i].length;
    }
    ipmb_status_t rc = ipmb_reassemble_fragments(frags, count, total, out, out_len);
    free(frags);
    return rc;
}
