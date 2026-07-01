#pragma once
#include "ipmb/types.h"

typedef struct {
    uint32_t session_id;
    uint8_t *buf;
    size_t used;
    size_t capacity;
} ipmb_checkpoint_state_t;

typedef struct {
    ipmb_checkpoint_state_t *items;
    size_t count;
    size_t capacity;
} ipmb_checkpoint_store_t;

void ipmb_checkpoint_store_init(ipmb_checkpoint_store_t *store);
void ipmb_checkpoint_store_free(ipmb_checkpoint_store_t *store);
ipmb_status_t ipmb_checkpoint_append(ipmb_checkpoint_store_t *store,
                                     uint32_t session_id,
                                     const uint8_t *payload,
                                     size_t len);
ipmb_status_t ipmb_checkpoint_migrate(ipmb_checkpoint_store_t *store,
                                      uint32_t src_session,
                                      uint32_t dst_session);
