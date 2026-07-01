#include "ipmb/telemetry/extractor.h"
#include <stdio.h>
#include <string.h>

void ipmb_format_telemetry_report(const ipmb_telemetry_t* telemetry, char* buffer, size_t max_len) {
    if (!telemetry || !buffer || max_len == 0) return;
    snprintf(buffer, max_len, "Telemetry Report\nTotal Events: %u\nDrops: %u\n", 
             telemetry->events_processed, telemetry->events_dropped);
}
