#pragma once
#include "ipmb/types.h"

typedef struct {
    uint64_t timestamp;
    double value;
} ipmb_timeseries_sample_t;

typedef struct {
    ipmb_timeseries_sample_t *samples;
    size_t count;
    size_t capacity;
} ipmb_timeseries_t;

void ipmb_timeseries_init(ipmb_timeseries_t *ts);
void ipmb_timeseries_free(ipmb_timeseries_t *ts);
ipmb_status_t ipmb_timeseries_add(ipmb_timeseries_t *ts,
                                  uint64_t timestamp, double value);
