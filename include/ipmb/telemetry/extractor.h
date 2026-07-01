#pragma once
#include "ipmb/types.h"

typedef struct {
    uint8_t type;
    char *name;
    uint8_t *value;
    size_t value_len;
} ipmb_telemetry_point_t;

typedef struct {
    uint16_t schema;
    uint16_t point_count;
    ipmb_telemetry_point_t *points;
} ipmb_telemetry_batch_t;

ipmb_status_t ipmb_extract_telemetry(const uint8_t *data, size_t len,
                                     ipmb_telemetry_batch_t *out);
void ipmb_telemetry_batch_free(ipmb_telemetry_batch_t *batch);
