#include "test_framework.h"
#include "message_builder.h"

#include <stdlib.h>
#include <string.h>

#include "ipmb/ipmb.h"

int ipmb_test_protocol(void)
{
    size_t msg_len = 0;
    uint8_t *msg_bytes = ipmb_test_make_message(&msg_len);
    IPMB_ASSERT(msg_bytes != NULL);

    ipmb_message_t msg;
    IPMB_ASSERT(ipmb_parse_message(msg_bytes, msg_len, &msg) == IPMB_OK);
    IPMB_ASSERT(msg.version == 1u);
    IPMB_ASSERT(msg.sequence == 77u);
    IPMB_ASSERT(msg.frame_count == 1u);

    const ipmb_frame_t *frame = ipmb_message_first_frame(&msg, 0x22u);
    IPMB_ASSERT(frame != NULL);
    IPMB_ASSERT(frame->source_addr == 100u);
    IPMB_ASSERT(frame->dest_addr == 200u);
    IPMB_ASSERT(frame->payload_len == 20u);
    IPMB_ASSERT(ipmb_security_risk_score(&msg) == 0u);
    ipmb_message_free(&msg);
    free(msg_bytes);

    uint8_t map_payload[] = {
        'R', 'M', 'A', 'P', 0x34, 0x12, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0xaa, 0xbb
    };
    ipmb_register_map_t map;
    IPMB_ASSERT(ipmb_decode_register_map(map_payload, sizeof(map_payload), &map) == IPMB_OK);
    IPMB_ASSERT(map.map_id == 0x1234u);
    IPMB_ASSERT(map.field_count == 1u);
    IPMB_ASSERT(map.fields[0].length == 2u);
    IPMB_ASSERT(map.fields[0].bytes[0] == 0xaau);
    IPMB_ASSERT(map.fields[0].bytes[1] == 0xbbu);
    ipmb_register_map_free(&map);

    uint8_t frag_payload[] = {
        'I', 'F', 'R', 'G', 0x05, 0x00, 0x00, 0x00, 0x02, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 'h', 'e',
        0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 'l', 'l', 'o'
    };
    uint8_t *assembled = NULL;
    size_t assembled_len = 0;
    IPMB_ASSERT(ipmb_parse_fragment_payload(frag_payload, sizeof(frag_payload),
                                            &assembled, &assembled_len) == IPMB_OK);
    IPMB_ASSERT(assembled_len == 5u);
    IPMB_ASSERT(memcmp(assembled, "hello", 5u) == 0);
    free(assembled);
    return 0;
}
