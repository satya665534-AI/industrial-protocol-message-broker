#pragma once
#include "ipmb/types.h"

typedef struct {
    uint8_t *data;
    size_t len;
} ipmb_storage_block_t;

typedef struct {
    ipmb_storage_block_t *blocks;
    size_t count;
    size_t capacity;
} ipmb_storage_t;

void ipmb_storage_init(ipmb_storage_t *store);
void ipmb_storage_free(ipmb_storage_t *store);
ipmb_status_t ipmb_storage_put(ipmb_storage_t *store,
                               const uint8_t *data, size_t len,
                               size_t *index_out);
