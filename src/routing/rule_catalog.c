/* Standard Routing Rule Presets */
#include <stdint.h>

const char* ipmb_rule_presets[] = {
  "rule preset_1: priority > 10 forward zone_1;",
  "rule preset_2: drop if source_ip == 192.168.1.100;",
  "rule preset_3: priority > 50 forward emergency_zone;",
  0
};
