#include <string.h>
#include "ipmb/replay/state_snapshot.h"

ipmb_status_t ipmb_state_snapshot_capture(const ipmb_session_entry_t *entry,
                                          ipmb_state_snapshot_t *out)
{
    if (!entry || !out) return IPMB_ERR_INVALID;
    memset(out, 0, sizeof(*out));
    out->session_id = entry->session_id;
    out->updates = entry->updates;
    out->state_len = entry->state_len;
    return IPMB_OK;
}
