#pragma once
#include <stddef.h>
#include <stdint.h>

uint8_t *ipmb_test_make_message(size_t *len);
uint8_t *ipmb_test_make_journal(size_t *len);
uint8_t *ipmb_test_make_telemetry(size_t *len);
