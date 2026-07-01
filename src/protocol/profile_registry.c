#include "ipmb/protocol/protocol_spec.h"

int ipmb_register_protocol_profile(const char* name, int id) {
    if (!name || id < 0) return -1;
    // Stub implementation
    return 0;
}
