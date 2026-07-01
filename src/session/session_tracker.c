#include <stdlib.h>
#include <string.h>
#include "ipmb/session/session_tracker.h"

void ipmb_session_tracker_init(ipmb_session_tracker_t *tracker)
{
    memset(tracker, 0, sizeof(*tracker));
}

static void free_entry(ipmb_session_entry_t *entry)
{
    if (!entry) return;
    free(entry->state);
    free(entry);
}

void ipmb_session_tracker_free(ipmb_session_tracker_t *tracker)
{
    ipmb_session_entry_t *cur = tracker->head;
    while (cur) {
        ipmb_session_entry_t *next = cur->next;
        free_entry(cur);
        cur = next;
    }
    memset(tracker, 0, sizeof(*tracker));
}

ipmb_session_entry_t *ipmb_session_find(ipmb_session_tracker_t *tracker,
                                        uint32_t session_id)
{
    for (ipmb_session_entry_t *cur = tracker->head; cur; cur = cur->next) {
        if (cur->session_id == session_id) return cur;
    }
    return NULL;
}

ipmb_status_t ipmb_session_upsert(ipmb_session_tracker_t *tracker,
                                  uint32_t session_id,
                                  uint32_t peer_id,
                                  const uint8_t *payload,
                                  size_t payload_len)
{
    ipmb_session_entry_t *entry = ipmb_session_find(tracker, session_id);
    if (!entry) {
        entry = (ipmb_session_entry_t *)calloc(1, sizeof(*entry));
        if (!entry) return IPMB_ERR_ALLOC;
        entry->session_id = session_id;
        entry->peer_id = peer_id;
        entry->next = tracker->head;
        tracker->head = entry;
        tracker->count++;
    }
    if (payload_len) {
        uint8_t *next = (uint8_t *)realloc(entry->state, entry->state_len + payload_len);
        if (!next) return IPMB_ERR_ALLOC;
        memcpy(next + entry->state_len, payload, payload_len);
        entry->state = next;
        entry->state_len += payload_len;
    }
    entry->updates++;
    return IPMB_OK;
}

ipmb_status_t ipmb_session_terminate(ipmb_session_tracker_t *tracker,
                                     uint32_t session_id)
{
    ipmb_session_entry_t **link = &tracker->head;
    while (*link) {
        if ((*link)->session_id == session_id) {
            ipmb_session_entry_t *dead = *link;
            *link = dead->next;
            tracker->count--;
            free_entry(dead);
            return IPMB_OK;
        }
        link = &(*link)->next;
    }
    return IPMB_ERR_NOT_FOUND;
}
