#include "test_framework.h"

#include <stdlib.h>
#include <string.h>

#include "ipmb/ipmb.h"

int ipmb_test_codec(void)
{
    const uint8_t raw[] = { 'a', 'a', 'a', 'b', 'b', 'c' };
    uint8_t *encoded = NULL;
    size_t encoded_len = 0;
    IPMB_ASSERT(ipmb_rle_encode(raw, sizeof(raw), &encoded, &encoded_len) == IPMB_OK);
    IPMB_ASSERT(encoded_len > 4u);

    uint8_t *decoded = NULL;
    size_t decoded_len = 0;
    IPMB_ASSERT(ipmb_rle_decode(encoded, encoded_len, &decoded, &decoded_len) == IPMB_OK);
    IPMB_ASSERT(decoded_len == sizeof(raw));
    IPMB_ASSERT(memcmp(decoded, raw, sizeof(raw)) == 0);
    free(encoded);
    free(decoded);

    int32_t values[] = { 10, 15, -2 };
    encoded = NULL;
    encoded_len = 0;
    IPMB_ASSERT(ipmb_delta_encode(values, 3u, &encoded, &encoded_len) == IPMB_OK);

    int32_t *delta_values = NULL;
    size_t value_count = 0;
    IPMB_ASSERT(ipmb_delta_decode(encoded, encoded_len, &delta_values, &value_count) == IPMB_OK);
    IPMB_ASSERT(value_count == 3u);
    IPMB_ASSERT(delta_values[0] == 10);
    IPMB_ASSERT(delta_values[1] == 15);
    IPMB_ASSERT(delta_values[2] == -2);
    free(encoded);
    free(delta_values);

    uint8_t bits[] = { 0xac };
    ipmb_bitreader_t reader;
    uint32_t nibble = 0;
    ipmb_bitreader_init(&reader, bits, sizeof(bits));
    IPMB_ASSERT(ipmb_bitreader_read(&reader, 4u, &nibble) == IPMB_OK);
    IPMB_ASSERT(nibble == 12u);
    IPMB_ASSERT(ipmb_bitreader_read(&reader, 4u, &nibble) == IPMB_OK);
    IPMB_ASSERT(nibble == 10u);
    return 0;
}
