#include "test_framework.h"
#include "message_builder.h"

#include <stdlib.h>

#include "ipmb/ipmb.h"

int ipmb_test_replay(void)
{
    size_t journal_len = 0;
    uint8_t *journal_bytes = ipmb_test_make_journal(&journal_len);
    IPMB_ASSERT(journal_bytes != NULL);

    ipmb_replay_summary_t summary;
    IPMB_ASSERT(ipmb_replay_bytes(journal_bytes, journal_len, &summary) == IPMB_OK);
    IPMB_ASSERT(summary.event_count == 3u);
    IPMB_ASSERT(summary.summary_hash != 0u);
    free(journal_bytes);

    ipmb_session_tracker_t tracker;
    ipmb_event_log_t log;
    ipmb_session_tracker_init(&tracker);
    ipmb_event_log_init(&log);
    IPMB_ASSERT(ipmb_session_upsert(&tracker, 55u, 9u, (const uint8_t *)"state", 5u) == IPMB_OK);
    IPMB_ASSERT(ipmb_event_log_snapshot(&log, &tracker) == IPMB_OK);
    IPMB_ASSERT(log.count == 1u);
    IPMB_ASSERT(ipmb_event_log_summarize(&log) != 0u);
    ipmb_event_log_free(&log);
    ipmb_session_tracker_free(&tracker);
    return 0;
}
