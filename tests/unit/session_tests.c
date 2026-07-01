#include "test_framework.h"
#include "message_builder.h"

#include <stdlib.h>
#include <string.h>

#include "ipmb/ipmb.h"

int ipmb_test_session(void)
{
    const uint8_t first[] = { 1u, 2u };
    const uint8_t second[] = { 3u, 4u };
    ipmb_session_tracker_t tracker;
    ipmb_session_tracker_init(&tracker);
    IPMB_ASSERT(ipmb_session_upsert(&tracker, 42u, 7u, first, sizeof(first)) == IPMB_OK);
    IPMB_ASSERT(ipmb_session_upsert(&tracker, 42u, 7u, second, sizeof(second)) == IPMB_OK);

    ipmb_session_entry_t *entry = ipmb_session_find(&tracker, 42u);
    IPMB_ASSERT(entry != NULL);
    IPMB_ASSERT(entry->updates == 2u);
    IPMB_ASSERT(entry->state_len == 4u);
    IPMB_ASSERT(memcmp(entry->state, "\x01\x02\x03\x04", 4u) == 0);

    ipmb_state_snapshot_t snapshot;
    IPMB_ASSERT(ipmb_state_snapshot_capture(entry, &snapshot) == IPMB_OK);
    IPMB_ASSERT(snapshot.session_id == 42u);
    IPMB_ASSERT(snapshot.state_len == 4u);
    IPMB_ASSERT(ipmb_session_terminate(&tracker, 42u) == IPMB_OK);
    IPMB_ASSERT(ipmb_session_find(&tracker, 42u) == NULL);
    ipmb_session_tracker_free(&tracker);

    size_t journal_len = 0;
    uint8_t *journal_bytes = ipmb_test_make_journal(&journal_len);
    IPMB_ASSERT(journal_bytes != NULL);
    ipmb_journal_t journal;
    IPMB_ASSERT(ipmb_journal_parse(journal_bytes, journal_len, &journal) == IPMB_OK);
    IPMB_ASSERT(journal.version == 1u);
    IPMB_ASSERT(journal.record_count == 3u);
    IPMB_ASSERT(journal.records[0].type == IPMB_JOURNAL_ESTABLISH);
    IPMB_ASSERT(journal.records[0].payload_len == 3u);
    IPMB_ASSERT(memcmp(journal.records[0].payload, "abc", 3u) == 0);
    ipmb_journal_free(&journal);
    free(journal_bytes);

    ipmb_checkpoint_store_t store;
    ipmb_checkpoint_store_init(&store);
    IPMB_ASSERT(ipmb_checkpoint_append(&store, 10u, first, sizeof(first)) == IPMB_OK);
    IPMB_ASSERT(ipmb_checkpoint_append(&store, 10u, second, sizeof(second)) == IPMB_OK);
    IPMB_ASSERT(store.count == 1u);
    IPMB_ASSERT(store.items[0].used == 4u);
    IPMB_ASSERT(ipmb_checkpoint_migrate(&store, 99u, 100u) == IPMB_ERR_NOT_FOUND);
    ipmb_checkpoint_store_free(&store);

    ipmb_ring_window_t ring;
    IPMB_ASSERT(ipmb_ring_init(&ring, 3u) == IPMB_OK);
    IPMB_ASSERT(ipmb_ring_append(&ring, first, sizeof(first)) == IPMB_OK);
    ipmb_ring_checkpoint(&ring);
    IPMB_ASSERT(ring.has_snapshot);
    IPMB_ASSERT(ipmb_ring_advance(&ring, second, sizeof(second)) == IPMB_OK);
    IPMB_ASSERT(ring.segment_count == 3u);
    ipmb_ring_free(&ring);
    return 0;
}
