#pragma once
#include "ipmb/types.h"

typedef void (*ipmb_diag_sink_t)(ipmb_status_t status, const char *message, void *user);
void ipmb_diag_emit(ipmb_diag_sink_t sink, void *user,
                    ipmb_status_t status, const char *message);
