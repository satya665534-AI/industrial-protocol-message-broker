#include "ipmb/util/checksum.h"

uint32_t ipmb_crc32_extend(uint32_t seed, const uint8_t *data, size_t len)
{
    uint32_t crc = seed ^ 0xffffffffu;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit) {
            uint32_t mask = 0u - (crc & 1u);
            crc = (crc >> 1) ^ (0xedb88320u & mask);
        }
    }
    return crc ^ 0xffffffffu;
}

uint32_t ipmb_crc32(const uint8_t *data, size_t len)
{
    return ipmb_crc32_extend(0u, data, len);
}
