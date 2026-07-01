#pragma once
#include <stdio.h>
#include <stdlib.h>

#define IPMB_ASSERT(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "%s:%d: assertion failed: %s\n", __FILE__, __LINE__, #expr); \
        abort(); \
    } \
} while (0)

int ipmb_test_protocol(void);
int ipmb_test_routing(void);
int ipmb_test_session(void);
int ipmb_test_telemetry(void);
int ipmb_test_codec(void);
int ipmb_test_replay(void);
int ipmb_test_full_pipeline(void);
int ipmb_test_qc_scenarios(void);
