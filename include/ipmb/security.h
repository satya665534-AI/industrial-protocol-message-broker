#pragma once
#include "ipmb/protocol/frame_reader.h"

unsigned ipmb_security_risk_score(const ipmb_message_t *msg);
