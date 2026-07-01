#include <stdlib.h>
#include <string.h>
#include "ipmb/telemetry/histogram.h"

ipmb_status_t ipmb_histogram_init(ipmb_histogram_t *hist,
                                  uint32_t bucket_count)
{
    memset(hist, 0, sizeof(*hist));
    if (bucket_count == 0 || bucket_count > 1000000u) return IPMB_ERR_LIMIT;
    hist->bins = (uint32_t *)calloc(bucket_count, sizeof(uint32_t));
    if (!hist->bins) return IPMB_ERR_ALLOC;
    hist->bucket_count = bucket_count;
    return IPMB_OK;
}

void ipmb_histogram_free(ipmb_histogram_t *hist)
{
    free(hist->bins);
    memset(hist, 0, sizeof(*hist));
}

void ipmb_histogram_bin_incr(ipmb_histogram_t *hist, uint32_t index)
{
    hist->bins[index]++;
}
