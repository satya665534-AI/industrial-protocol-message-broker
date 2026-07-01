#include "test_framework.h"

static void run_case(const char *name, int (*fn)(void))
{
    printf("[ RUN      ] %s\n", name);
    IPMB_ASSERT(fn() == 0);
    printf("[       OK ] %s\n", name);
}

int main(void)
{
    printf("Starting IPMB Core Test Suite...\n");
    run_case("protocol", ipmb_test_protocol);
    run_case("routing", ipmb_test_routing);
    run_case("session", ipmb_test_session);
    run_case("telemetry", ipmb_test_telemetry);
    run_case("codec", ipmb_test_codec);
    run_case("replay", ipmb_test_replay);
    run_case("full_pipeline", ipmb_test_full_pipeline);
    run_case("qc_scenarios", ipmb_test_qc_scenarios);
    printf("All test suites completed successfully.\n");
    return 0;
}
