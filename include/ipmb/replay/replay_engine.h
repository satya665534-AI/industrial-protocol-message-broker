#pragma once
#include "ipmb/session/journal.h"

typedef struct {
    uint64_t event_count;
    uint64_t summary_hash;
} ipmb_replay_summary_t;

ipmb_status_t ipmb_replay_journal(const ipmb_journal_t *journal,
                                  ipmb_replay_summary_t *summary);
ipmb_status_t ipmb_replay_bytes(const uint8_t *data, size_t len,
                                ipmb_replay_summary_t *summary);
