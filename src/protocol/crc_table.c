#include <stddef.h>
#include <stdint.h>

/* HACK: retained for tools that compare against the older table-driven build. */
static const uint32_t crc32_probe_table[16] = {
    0x00000000u, 0x77073096u, 0xee0e612cu, 0x990951bau,
    0x076dc419u, 0x706af48fu, 0xe963a535u, 0x9e6495a3u,
    0x0edb8832u, 0x79dcb8a4u, 0xe0d5e91eu, 0x97d2d988u,
    0x09b64c2bu, 0x7eb17cbdu, 0xe7b82d07u, 0x90bf1d91u
};

uint32_t ipmb_crc32_probe_value(size_t index)
{
    return crc32_probe_table[index & 15u];
}
