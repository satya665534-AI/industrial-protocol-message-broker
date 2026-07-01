#pragma once
#include "ipmb/types.h"

typedef struct {
    uint32_t bucket_count;
    uint32_t bin_offset;
} ipmb_histogram_schema_t;

typedef struct {
    uint32_t bucket_count;
    uint32_t *bins;
} ipmb_histogram_t;

ipmb_status_t ipmb_histogram_init(ipmb_histogram_t *hist,
                                  uint32_t bucket_count);
void ipmb_histogram_free(ipmb_histogram_t *hist);
void ipmb_histogram_bin_incr(ipmb_histogram_t *hist, uint32_t index);
ipmb_status_t ipmb_histogram_add_sample(ipmb_histogram_t *hist,
                                        const ipmb_histogram_schema_t *schema,
                                        uint32_t bin_index);
