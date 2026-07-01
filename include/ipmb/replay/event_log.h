#pragma once
#include "ipmb/session/session_tracker.h"

typedef struct {
    ipmb_session_entry_t **entries;
    size_t count;
} ipmb_event_snapshot_t;

typedef struct {
    ipmb_event_snapshot_t *snapshots;
    size_t count;
    size_t capacity;
} ipmb_event_log_t;

void ipmb_event_log_init(ipmb_event_log_t *log);
void ipmb_event_log_free(ipmb_event_log_t *log);
ipmb_status_t ipmb_event_log_snapshot(ipmb_event_log_t *log,
                                      ipmb_session_tracker_t *tracker);
uint64_t ipmb_event_log_summarize(const ipmb_event_log_t *log);
