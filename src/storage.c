#include <stdlib.h>
#include <string.h>
#include "ipmb/storage.h"

void ipmb_storage_init(ipmb_storage_t *store)
{
    memset(store, 0, sizeof(*store));
}

void ipmb_storage_free(ipmb_storage_t *store)
{
    if (!store) return;
    for (size_t i = 0; i < store->count; ++i) free(store->blocks[i].data);
    free(store->blocks);
    memset(store, 0, sizeof(*store));
}

ipmb_status_t ipmb_storage_put(ipmb_storage_t *store,
                               const uint8_t *data, size_t len,
                               size_t *index_out)
{
    if (store->count == store->capacity) {
        size_t next_cap = store->capacity ? store->capacity * 2u : 8u;
        ipmb_storage_block_t *next = (ipmb_storage_block_t *)realloc(store->blocks,
            next_cap * sizeof(*next));
        if (!next) return IPMB_ERR_ALLOC;
        store->blocks = next;
        store->capacity = next_cap;
    }
    uint8_t *copy = (uint8_t *)malloc(len ? len : 1u);
    if (!copy) return IPMB_ERR_ALLOC;
    memcpy(copy, data, len);
    store->blocks[store->count].data = copy;
    store->blocks[store->count].len = len;
    if (index_out) *index_out = store->count;
    store->count++;
    return IPMB_OK;
}
