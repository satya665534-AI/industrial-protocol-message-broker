#pragma once
#include "ipmb/types.h"

typedef struct {
    uint8_t **segments;
    size_t *lengths;
    size_t segment_count;
    size_t head;
    uint8_t **snapshot_segments;
    size_t *snapshot_lengths;
    bool has_snapshot;
} ipmb_ring_window_t;

ipmb_status_t ipmb_ring_init(ipmb_ring_window_t *ring, size_t segment_count);
void ipmb_ring_free(ipmb_ring_window_t *ring);
ipmb_status_t ipmb_ring_append(ipmb_ring_window_t *ring,
                               const uint8_t *data, size_t len);
void ipmb_ring_checkpoint(ipmb_ring_window_t *ring);
void ipmb_ring_rollback(ipmb_ring_window_t *ring);
ipmb_status_t ipmb_ring_advance(ipmb_ring_window_t *ring,
                                const uint8_t *data, size_t len);
