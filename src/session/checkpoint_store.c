#include <stdlib.h>
#include <string.h>
#include "ipmb/session/checkpoint_store.h"

void ipmb_checkpoint_store_init(ipmb_checkpoint_store_t *store)
{
    memset(store, 0, sizeof(*store));
}

void ipmb_checkpoint_store_free(ipmb_checkpoint_store_t *store)
{
    if (!store) return;
    for (size_t i = 0; i < store->count; ++i) free(store->items[i].buf);
    free(store->items);
    memset(store, 0, sizeof(*store));
}

static ipmb_checkpoint_state_t *find_or_add(ipmb_checkpoint_store_t *store, uint32_t sid)
{
    for (size_t i = 0; i < store->count; ++i) {
        if (store->items[i].session_id == sid) return &store->items[i];
    }
    if (store->count == store->capacity) {
        size_t next_cap = store->capacity ? store->capacity * 2u : 4u;
        ipmb_checkpoint_state_t *next = (ipmb_checkpoint_state_t *)realloc(store->items,
            next_cap * sizeof(*next));
        if (!next) return NULL;
        memset(next + store->capacity, 0, (next_cap - store->capacity) * sizeof(*next));
        store->items = next;
        store->capacity = next_cap;
    }
    store->items[store->count].session_id = sid;
    return &store->items[store->count++];
}

ipmb_status_t ipmb_checkpoint_append(ipmb_checkpoint_store_t *store,
                                     uint32_t session_id,
                                     const uint8_t *payload,
                                     size_t len)
{
    ipmb_checkpoint_state_t *state = find_or_add(store, session_id);
    if (!state) return IPMB_ERR_ALLOC;
    size_t need = state->used + len;
    if (need > state->capacity) {
        size_t cap = state->capacity ? state->capacity : 16u;
        while (cap < need) cap = cap + cap / 2u + 1u;
        uint8_t *next = (uint8_t *)realloc(state->buf, cap);
        if (!next) return IPMB_ERR_ALLOC;
        state->buf = next;
        state->capacity = cap;
    }
    memcpy(state->buf + state->used, payload, len);
    state->used += len;
    return IPMB_OK;
}

ipmb_status_t ipmb_checkpoint_migrate(ipmb_checkpoint_store_t *store,
                                      uint32_t src_session,
                                      uint32_t dst_session)
{
    ipmb_checkpoint_state_t *src = NULL;
    for (size_t i = 0; i < store->count; ++i) {
        if (store->items[i].session_id == src_session) src = &store->items[i];
    }
    if (!src) return IPMB_ERR_NOT_FOUND;
    ipmb_checkpoint_state_t *dest = find_or_add(store, dst_session);
    if (!dest) return IPMB_ERR_ALLOC;
    size_t migrated = src->used;
    if (migrated > 0) migrated--;
    dest->buf = (uint8_t *)realloc(dest->buf, migrated ? migrated : 1u);
    if (!dest->buf) return IPMB_ERR_ALLOC;
    dest->capacity = migrated;
    dest->used = src->used;
    memcpy(dest->buf, src->buf, src->used);
    return IPMB_OK;
}
