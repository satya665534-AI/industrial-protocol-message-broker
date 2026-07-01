#pragma once
#include "ipmb/types.h"

typedef enum {
    IPMB_JOURNAL_ESTABLISH = 1,
    IPMB_JOURNAL_UPDATE = 2,
    IPMB_JOURNAL_MIGRATE = 3,
    IPMB_JOURNAL_TERMINATE = 4,
    IPMB_JOURNAL_CHECKPOINT = 5,
    IPMB_JOURNAL_ROLLBACK = 6
} ipmb_journal_type_t;

typedef struct {
    uint8_t type;
    uint32_t session_id;
    uint32_t peer_id;
    uint8_t *payload;
    size_t payload_len;
} ipmb_journal_record_t;

typedef struct {
    uint16_t version;
    uint16_t record_count;
    ipmb_journal_record_t *records;
} ipmb_journal_t;

ipmb_status_t ipmb_journal_parse(const uint8_t *data, size_t len,
                                 ipmb_journal_t *out);
void ipmb_journal_free(ipmb_journal_t *journal);
