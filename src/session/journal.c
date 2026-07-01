#include <stdlib.h>
#include <string.h>
#include "ipmb/session/journal.h"
#include "ipmb/util/byte_read.h"

void ipmb_journal_free(ipmb_journal_t *journal)
{
    if (!journal) return;
    if (journal->records) {
        for (uint16_t i = 0; i < journal->record_count; ++i) free(journal->records[i].payload);
    }
    free(journal->records);
    memset(journal, 0, sizeof(*journal));
}

ipmb_status_t ipmb_journal_parse(const uint8_t *data, size_t len,
                                 ipmb_journal_t *out)
{
    memset(out, 0, sizeof(*out));
    if (len < 8 || memcmp(data, "IJRN", 4) != 0) return IPMB_ERR_BAD_MAGIC;
    ipmb_reader_t r;
    ipmb_reader_init(&r, data + 4, len - 4);
    if (ipmb_read_le16(&r, &out->version) != IPMB_OK ||
        ipmb_read_le16(&r, &out->record_count) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (out->record_count > 4096) return IPMB_ERR_LIMIT;
    out->records = (ipmb_journal_record_t *)calloc(out->record_count ? out->record_count : 1u,
                                                   sizeof(*out->records));
    if (!out->records) return IPMB_ERR_ALLOC;
    for (uint16_t i = 0; i < out->record_count; ++i) {
        uint16_t plen;
        const uint8_t *payload;
        if (ipmb_read_u8(&r, &out->records[i].type) != IPMB_OK ||
            ipmb_read_le32(&r, &out->records[i].session_id) != IPMB_OK ||
            ipmb_read_le32(&r, &out->records[i].peer_id) != IPMB_OK ||
            ipmb_read_le16(&r, &plen) != IPMB_OK ||
            ipmb_read_bytes(&r, plen, &payload) != IPMB_OK) {
            ipmb_journal_free(out);
            return IPMB_ERR_SHORT_READ;
        }
        out->records[i].payload = (uint8_t *)malloc(plen ? plen : 1u);
        if (!out->records[i].payload) {
            ipmb_journal_free(out);
            return IPMB_ERR_ALLOC;
        }
        memcpy(out->records[i].payload, payload, plen);
        out->records[i].payload_len = plen;
    }
    return IPMB_OK;
}
