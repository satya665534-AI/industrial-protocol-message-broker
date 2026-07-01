#include <stdlib.h>
#include <string.h>
#include "ipmb/telemetry/extractor.h"
#include "ipmb/util/byte_read.h"

static char *copy_name(const uint8_t *src, size_t len)
{
    char *s = (char *)malloc(len + 1u);
    if (!s) return NULL;
    memcpy(s, src, len);
    s[len] = '\0';
    return s;
}

void ipmb_telemetry_batch_free(ipmb_telemetry_batch_t *batch)
{
    if (!batch) return;
    if (batch->points) {
        for (uint16_t i = 0; i < batch->point_count; ++i) {
            free(batch->points[i].name);
            free(batch->points[i].value);
        }
    }
    free(batch->points);
    memset(batch, 0, sizeof(*batch));
}

ipmb_status_t ipmb_extract_telemetry(const uint8_t *data, size_t len,
                                     ipmb_telemetry_batch_t *out)
{
    memset(out, 0, sizeof(*out));
    if (len < 8 || memcmp(data, "ITLM", 4) != 0) return IPMB_ERR_BAD_MAGIC;
    ipmb_reader_t r;
    ipmb_reader_init(&r, data + 4, len - 4);
    if (ipmb_read_le16(&r, &out->schema) != IPMB_OK ||
        ipmb_read_le16(&r, &out->point_count) != IPMB_OK) return IPMB_ERR_SHORT_READ;
    if (out->point_count > 2048) return IPMB_ERR_LIMIT;
    out->points = (ipmb_telemetry_point_t *)calloc(out->point_count ? out->point_count : 1u,
                                                   sizeof(*out->points));
    if (!out->points) return IPMB_ERR_ALLOC;
    for (uint16_t i = 0; i < out->point_count; ++i) {
        uint16_t name_len;
        uint32_t value_len;
        const uint8_t *name;
        const uint8_t *value;
        if (ipmb_read_u8(&r, &out->points[i].type) != IPMB_OK ||
            ipmb_read_le16(&r, &name_len) != IPMB_OK ||
            ipmb_read_bytes(&r, name_len, &name) != IPMB_OK ||
            ipmb_read_le32(&r, &value_len) != IPMB_OK ||
            ipmb_read_bytes(&r, value_len, &value) != IPMB_OK) {
            ipmb_telemetry_batch_free(out);
            return IPMB_ERR_SHORT_READ;
        }
        out->points[i].name = copy_name(name, name_len);
        out->points[i].value = (uint8_t *)malloc(value_len ? value_len : 1u);
        if (!out->points[i].name || !out->points[i].value) {
            ipmb_telemetry_batch_free(out);
            return IPMB_ERR_ALLOC;
        }
        memcpy(out->points[i].value, value, value_len);
        out->points[i].value_len = value_len;
    }
    return IPMB_OK;
}
