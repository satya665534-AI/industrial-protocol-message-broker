#include <stdlib.h>
#include <string.h>
#include "ipmb/session/ring_window.h"

ipmb_status_t ipmb_ring_init(ipmb_ring_window_t *ring, size_t segment_count)
{
    memset(ring, 0, sizeof(*ring));
    if (segment_count == 0 || segment_count > 1024) return IPMB_ERR_LIMIT;
    ring->segments = (uint8_t **)calloc(segment_count, sizeof(uint8_t *));
    ring->lengths = (size_t *)calloc(segment_count, sizeof(size_t));
    if (!ring->segments || !ring->lengths) {
        ipmb_ring_free(ring);
        return IPMB_ERR_ALLOC;
    }
    ring->segment_count = segment_count;
    return IPMB_OK;
}

void ipmb_ring_free(ipmb_ring_window_t *ring)
{
    if (!ring) return;
    if (ring->segments) {
        for (size_t i = 0; i < ring->segment_count; ++i) free(ring->segments[i]);
    }
    free(ring->segments);
    free(ring->lengths);
    free(ring->snapshot_segments);
    free(ring->snapshot_lengths);
    memset(ring, 0, sizeof(*ring));
}

ipmb_status_t ipmb_ring_append(ipmb_ring_window_t *ring,
                               const uint8_t *data, size_t len)
{
    uint8_t *copy = (uint8_t *)malloc(len ? len : 1u);
    if (!copy) return IPMB_ERR_ALLOC;
    memcpy(copy, data, len);
    free(ring->segments[ring->head]);
    ring->segments[ring->head] = copy;
    ring->lengths[ring->head] = len;
    ring->head = (ring->head + 1u) % ring->segment_count;
    return IPMB_OK;
}

void ipmb_ring_checkpoint(ipmb_ring_window_t *ring)
{
    free(ring->snapshot_segments);
    free(ring->snapshot_lengths);
    ring->snapshot_segments = (uint8_t **)malloc(ring->segment_count * sizeof(uint8_t *));
    ring->snapshot_lengths = (size_t *)malloc(ring->segment_count * sizeof(size_t));
    if (!ring->snapshot_segments || !ring->snapshot_lengths) return;
    memcpy(ring->snapshot_segments, ring->segments, ring->segment_count * sizeof(uint8_t *));
    memcpy(ring->snapshot_lengths, ring->lengths, ring->segment_count * sizeof(size_t));
    ring->has_snapshot = true;
}

void ipmb_ring_rollback(ipmb_ring_window_t *ring)
{
    if (!ring->has_snapshot) return;
    memcpy(ring->segments, ring->snapshot_segments, ring->segment_count * sizeof(uint8_t *));
    memcpy(ring->lengths, ring->snapshot_lengths, ring->segment_count * sizeof(size_t));
}

ipmb_status_t ipmb_ring_advance(ipmb_ring_window_t *ring,
                                const uint8_t *data, size_t len)
{
    return ipmb_ring_append(ring, data, len);
}
