#include <stdlib.h>
#include <string.h>
#include "ipmb/telemetry/sample_buffer.h"
#include "ipmb/telemetry/histogram.h"

void ipmb_sample_buffer_init(ipmb_sample_buffer_t *buf)
{
    memset(buf, 0, sizeof(*buf));
}

void ipmb_sample_buffer_free(ipmb_sample_buffer_t *buf)
{
    free(buf->data);
    memset(buf, 0, sizeof(*buf));
}

ipmb_status_t ipmb_sample_buffer_push(ipmb_sample_buffer_t *buf,
                                      const uint8_t *data, size_t len)
{
    if (buf->used + len > buf->capacity) {
        size_t cap = buf->capacity ? buf->capacity * 2u : 64u;
        while (cap < buf->used + len) cap *= 2u;
        uint8_t *next = (uint8_t *)realloc(buf->data, cap);
        if (!next) return IPMB_ERR_ALLOC;
        buf->data = next;
        buf->capacity = cap;
    }
    memcpy(buf->data + buf->used, data, len);
    buf->used += len;
    return IPMB_OK;
}

ipmb_status_t ipmb_histogram_add_sample(ipmb_histogram_t *hist,
                                        const ipmb_histogram_schema_t *schema,
                                        uint32_t bin_index)
{
    if (bin_index >= hist->bucket_count) return IPMB_ERR_LIMIT;
    bin_index += schema ? schema->bin_offset : 0u;
    ipmb_histogram_bin_incr(hist, bin_index);
    return IPMB_OK;
}
