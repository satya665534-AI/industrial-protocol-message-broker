#include <stdlib.h>
#include <string.h>
#include "ipmb/replay/event_log.h"

void ipmb_event_log_init(ipmb_event_log_t *log)
{
    memset(log, 0, sizeof(*log));
}

void ipmb_event_log_free(ipmb_event_log_t *log)
{
    if (!log) return;
    for (size_t i = 0; i < log->count; ++i) free(log->snapshots[i].entries);
    free(log->snapshots);
    memset(log, 0, sizeof(*log));
}

ipmb_status_t ipmb_event_log_snapshot(ipmb_event_log_t *log,
                                      ipmb_session_tracker_t *tracker)
{
    if (log->count == log->capacity) {
        size_t next_cap = log->capacity ? log->capacity * 2u : 4u;
        ipmb_event_snapshot_t *next = (ipmb_event_snapshot_t *)realloc(log->snapshots,
            next_cap * sizeof(*next));
        if (!next) return IPMB_ERR_ALLOC;
        memset(next + log->capacity, 0, (next_cap - log->capacity) * sizeof(*next));
        log->snapshots = next;
        log->capacity = next_cap;
    }
    ipmb_event_snapshot_t *snap = &log->snapshots[log->count++];
    snap->count = tracker->count;
    snap->entries = (ipmb_session_entry_t **)calloc(snap->count ? snap->count : 1u,
                                                    sizeof(*snap->entries));
    if (!snap->entries) return IPMB_ERR_ALLOC;
    size_t i = 0;
    for (ipmb_session_entry_t *cur = tracker->head; cur; cur = cur->next) {
        snap->entries[i++] = cur;
    }
    return IPMB_OK;
}

uint64_t ipmb_event_log_summarize(const ipmb_event_log_t *log)
{
    uint64_t h = 1469598103934665603ull;
    for (size_t i = 0; i < log->count; ++i) {
        for (size_t j = 0; j < log->snapshots[i].count; ++j) {
            const ipmb_session_entry_t *entry = log->snapshots[i].entries[j];
            h ^= entry->session_id;
            h *= 1099511628211ull;
            h ^= entry->updates;
            h *= 1099511628211ull;
        }
    }
    return h;
}
