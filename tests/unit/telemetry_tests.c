#include "test_framework.h"
#include "message_builder.h"

#include <stdlib.h>
#include <string.h>

#include "ipmb/ipmb.h"

int ipmb_test_telemetry(void)
{
    size_t payload_len = 0;
    uint8_t *payload = ipmb_test_make_telemetry(&payload_len);
    IPMB_ASSERT(payload != NULL);

    ipmb_telemetry_batch_t batch;
    IPMB_ASSERT(ipmb_extract_telemetry(payload, payload_len, &batch) == IPMB_OK);
    IPMB_ASSERT(batch.schema == 3u);
    IPMB_ASSERT(batch.point_count == 1u);
    IPMB_ASSERT(batch.points[0].type == 1u);
    IPMB_ASSERT(strcmp(batch.points[0].name, "temp") == 0);
    IPMB_ASSERT(batch.points[0].value_len == 1u);
    IPMB_ASSERT(batch.points[0].value[0] == 0x2au);
    ipmb_telemetry_batch_free(&batch);
    free(payload);

    ipmb_timeseries_t ts;
    ipmb_timeseries_init(&ts);
    IPMB_ASSERT(ipmb_timeseries_add(&ts, 1000u, 42.5) == IPMB_OK);
    IPMB_ASSERT(ipmb_timeseries_add(&ts, 1001u, 43.0) == IPMB_OK);
    IPMB_ASSERT(ts.count == 2u);
    IPMB_ASSERT(ts.samples[0].timestamp == 1000u);
    IPMB_ASSERT(ts.samples[1].value == 43.0);
    ipmb_timeseries_free(&ts);

    ipmb_sample_buffer_t sample_buf;
    ipmb_sample_buffer_init(&sample_buf);
    IPMB_ASSERT(ipmb_sample_buffer_push(&sample_buf, (const uint8_t *)"abc", 3u) == IPMB_OK);
    IPMB_ASSERT(sample_buf.used == 3u);
    IPMB_ASSERT(memcmp(sample_buf.data, "abc", 3u) == 0);
    ipmb_sample_buffer_free(&sample_buf);

    ipmb_histogram_t hist;
    ipmb_histogram_schema_t schema = { 4u, 0u };
    IPMB_ASSERT(ipmb_histogram_init(&hist, 4u) == IPMB_OK);
    IPMB_ASSERT(ipmb_histogram_add_sample(&hist, &schema, 2u) == IPMB_OK);
    ipmb_histogram_bin_incr(&hist, 3u);
    IPMB_ASSERT(hist.bins[2] == 1u);
    IPMB_ASSERT(hist.bins[3] == 1u);
    ipmb_histogram_free(&hist);
    return 0;
}
