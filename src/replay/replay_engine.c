#include <string.h>
#include "ipmb/replay/replay_engine.h"
#include "ipmb/replay/event_log.h"
#include "ipmb/session/session_tracker.h"

ipmb_status_t ipmb_replay_journal(const ipmb_journal_t *journal,
                                  ipmb_replay_summary_t *summary)
{
    ipmb_session_tracker_t tracker;
    ipmb_event_log_t log;
    ipmb_session_tracker_init(&tracker);
    ipmb_event_log_init(&log);
    memset(summary, 0, sizeof(*summary));
    for (uint16_t i = 0; i < journal->record_count; ++i) {
        const ipmb_journal_record_t *r = &journal->records[i];
        if (r->type == IPMB_JOURNAL_ESTABLISH || r->type == IPMB_JOURNAL_UPDATE ||
            r->type == IPMB_JOURNAL_MIGRATE) {
            ipmb_session_upsert(&tracker, r->session_id, r->peer_id, r->payload, r->payload_len);
        } else if (r->type == IPMB_JOURNAL_CHECKPOINT) {
            ipmb_event_log_snapshot(&log, &tracker);
        } else if (r->type == IPMB_JOURNAL_TERMINATE) {
            ipmb_session_terminate(&tracker, r->session_id);
        }
        summary->event_count++;
    }
    summary->summary_hash = ipmb_event_log_summarize(&log);
    ipmb_event_log_free(&log);
    ipmb_session_tracker_free(&tracker);
    return IPMB_OK;
}

ipmb_status_t ipmb_replay_bytes(const uint8_t *data, size_t len,
                                ipmb_replay_summary_t *summary)
{
    ipmb_journal_t journal;
    ipmb_status_t rc = ipmb_journal_parse(data, len, &journal);
    if (rc != IPMB_OK) return rc;
    rc = ipmb_replay_journal(&journal, summary);
    ipmb_journal_free(&journal);
    return rc;
}
