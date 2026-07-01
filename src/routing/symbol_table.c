#include <string.h>

int ipmb_symbol_field_id(const char *name)
{
    static const char *names[] = {"type", "flags", "source", "dest", "priority", "ttl", "payload_len"};
    for (int i = 0; i < 7; ++i) {
        if (strcmp(name, names[i]) == 0) return i + 1;
    }
    return 0;
}
