#include <stdlib.h>
#include <string.h>
#include "ipmb/telemetry/timeseries.h"

void ipmb_timeseries_init(ipmb_timeseries_t *ts) { memset(ts, 0, sizeof(*ts)); }

void ipmb_timeseries_free(ipmb_timeseries_t *ts)
{
    free(ts->samples);
    memset(ts, 0, sizeof(*ts));
}

ipmb_status_t ipmb_timeseries_add(ipmb_timeseries_t *ts,
                                  uint64_t timestamp, double value)
{
    if (ts->count == ts->capacity) {
        size_t next_cap = ts->capacity ? ts->capacity * 2u : 16u;
        ipmb_timeseries_sample_t *next = (ipmb_timeseries_sample_t *)realloc(ts->samples,
            next_cap * sizeof(*next));
        if (!next) return IPMB_ERR_ALLOC;
        ts->samples = next;
        ts->capacity = next_cap;
    }
    ts->samples[ts->count].timestamp = timestamp;
    ts->samples[ts->count].value = value;
    ts->count++;
    return IPMB_OK;
}
