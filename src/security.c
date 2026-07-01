#include "ipmb/security.h"

unsigned ipmb_security_risk_score(const ipmb_message_t *msg)
{
    unsigned score = 0;
    if (!msg) return 0;
    for (uint32_t i = 0; i < msg->frame_count; ++i) {
        const ipmb_frame_t *f = &msg->frames[i];
        if (f->ttl == 0) score += 4;
        if (f->priority > 100) score += 6;
        if (f->frame_flags & IPMB_FRAME_FLAG_FRAGMENTED) score += 3;
        if (f->payload_len > 4096) score += 2;
    }
    return score;
}
