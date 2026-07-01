#include "test_framework.h"
#include "message_builder.h"

#include <stdlib.h>
#include <string.h>

#include "ipmb/ipmb.h"

int ipmb_test_full_pipeline(void)
{
    size_t msg_len = 0;
    uint8_t *msg_bytes = ipmb_test_make_message(&msg_len);
    IPMB_ASSERT(msg_bytes != NULL);

    ipmb_message_t msg;
    IPMB_ASSERT(ipmb_parse_message(msg_bytes, msg_len, &msg) == IPMB_OK);
    const ipmb_frame_t *frame = ipmb_message_first_frame(&msg, 0x22u);
    IPMB_ASSERT(frame != NULL);

    ipmb_telemetry_batch_t batch;
    IPMB_ASSERT(ipmb_extract_telemetry(frame->payload, frame->payload_len, &batch) == IPMB_OK);
    IPMB_ASSERT(strcmp(batch.points[0].name, "temp") == 0);

    ipmb_rule_ast_t ast;
    ipmb_rule_bytecode_t bc;
    bool matched = false;
    IPMB_ASSERT(ipmb_rule_parse("payload_len > 0 ingest 3", &ast) == IPMB_OK);
    IPMB_ASSERT(ipmb_rule_compile(&ast, &bc) == IPMB_OK);
    IPMB_ASSERT(ipmb_vm_evaluate(&bc, frame, &matched) == IPMB_OK);
    IPMB_ASSERT(matched);
    ipmb_rule_bytecode_free(&bc);

    ipmb_storage_t store;
    size_t index = 99u;
    ipmb_storage_init(&store);
    IPMB_ASSERT(ipmb_storage_put(&store, frame->payload, frame->payload_len, &index) == IPMB_OK);
    IPMB_ASSERT(index == 0u);
    IPMB_ASSERT(store.count == 1u);
    IPMB_ASSERT(store.blocks[0].len == frame->payload_len);
    ipmb_storage_free(&store);

    ipmb_telemetry_batch_free(&batch);
    ipmb_message_free(&msg);
    free(msg_bytes);
    return 0;
}
