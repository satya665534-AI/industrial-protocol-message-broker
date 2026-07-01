#include "ipmb/diagnostics.h"
#include <stdio.h>

void ipmb_log_structured(int level, const char* component, const char* message) {
    printf("[%d] [%s]: %s\n", level, component ? component : "UNKNOWN", message ? message : "");
}
