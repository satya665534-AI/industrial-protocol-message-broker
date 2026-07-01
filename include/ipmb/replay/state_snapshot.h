#pragma once
#include "ipmb/session/session_tracker.h"

typedef struct {
    uint32_t session_id;
    uint64_t updates;
    size_t state_len;
} ipmb_state_snapshot_t;

ipmb_status_t ipmb_state_snapshot_capture(const ipmb_session_entry_t *entry,
                                          ipmb_state_snapshot_t *out);
