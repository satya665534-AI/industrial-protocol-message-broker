#pragma once
#include "ipmb/types.h"

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t used;
} ipmb_sample_buffer_t;

void ipmb_sample_buffer_init(ipmb_sample_buffer_t *buf);
void ipmb_sample_buffer_free(ipmb_sample_buffer_t *buf);
ipmb_status_t ipmb_sample_buffer_push(ipmb_sample_buffer_t *buf,
                                      const uint8_t *data, size_t len);
