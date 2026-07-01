#pragma once
#include "ipmb/session/journal.h"

typedef struct ipmb_session_entry {
    uint32_t session_id;
    uint32_t peer_id;
    uint64_t updates;
    uint8_t *state;
    size_t state_len;
    struct ipmb_session_entry *next;
} ipmb_session_entry_t;

typedef struct {
    ipmb_session_entry_t *head;
    size_t count;
} ipmb_session_tracker_t;

void ipmb_session_tracker_init(ipmb_session_tracker_t *tracker);
void ipmb_session_tracker_free(ipmb_session_tracker_t *tracker);
ipmb_session_entry_t *ipmb_session_find(ipmb_session_tracker_t *tracker,
                                        uint32_t session_id);
ipmb_status_t ipmb_session_upsert(ipmb_session_tracker_t *tracker,
                                  uint32_t session_id,
                                  uint32_t peer_id,
                                  const uint8_t *payload,
                                  size_t payload_len);
ipmb_status_t ipmb_session_terminate(ipmb_session_tracker_t *tracker,
                                     uint32_t session_id);
