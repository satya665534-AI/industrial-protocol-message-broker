#include "ipmb/session/checkpoint_store.h"

int ipmb_check_checkpoint_consistency(const ipmb_checkpoint_t* cp) {
    if (!cp) return 0;
    if (cp->used > cp->capacity) return 0;
    if (cp->used > 0 && cp->buf == NULL) return 0;
    return 1;
}
