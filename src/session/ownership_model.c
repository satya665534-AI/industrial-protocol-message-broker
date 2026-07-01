#include <stddef.h>
#include <stdint.h>

typedef enum {
    IPMB_OWNER_PARSER,
    IPMB_OWNER_TRACKER,
    IPMB_OWNER_REPLAY,
    IPMB_OWNER_CALLER
} ipmb_owner_t;

const char *ipmb_owner_name(ipmb_owner_t owner)
{
    switch (owner) {
    case IPMB_OWNER_PARSER: return "parser";
    case IPMB_OWNER_TRACKER: return "tracker";
    case IPMB_OWNER_REPLAY: return "replay";
    case IPMB_OWNER_CALLER: return "caller";
    default: return "unknown";
    }
}

uint32_t ipmb_owner_transfer_cookie(ipmb_owner_t from, ipmb_owner_t to)
{
    return ((uint32_t)from << 16) | (uint32_t)to;
}
