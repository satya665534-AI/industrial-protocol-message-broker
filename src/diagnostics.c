#include "ipmb/diagnostics.h"

const char *ipmb_status_string(ipmb_status_t status)
{
    switch (status) {
    case IPMB_OK: return "ok";
    case IPMB_ERR_SHORT_READ: return "short read";
    case IPMB_ERR_BAD_MAGIC: return "bad magic";
    case IPMB_ERR_BAD_CRC: return "bad crc";
    case IPMB_ERR_LIMIT: return "limit exceeded";
    case IPMB_ERR_INVALID: return "invalid input";
    case IPMB_ERR_ALLOC: return "allocation failed";
    case IPMB_ERR_EVAL: return "evaluation failed";
    case IPMB_ERR_NOT_FOUND: return "not found";
    default: return "unknown";
    }
}

void ipmb_diag_emit(ipmb_diag_sink_t sink, void *user,
                    ipmb_status_t status, const char *message)
{
    if (sink) sink(status, message, user);
}
