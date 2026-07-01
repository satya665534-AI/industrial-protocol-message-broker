#include "test_framework.h"

#include <string.h>

#include "ipmb/ipmb.h"

typedef struct {
    unsigned calls;
    ipmb_status_t last_status;
} diag_capture_t;

static void capture_diag(ipmb_status_t status, const char *message, void *user)
{
    diag_capture_t *capture = (diag_capture_t *)user;
    capture->calls++;
    capture->last_status = status;
    IPMB_ASSERT(strcmp(message, "checked") == 0);
}

int ipmb_test_qc_scenarios(void)
{
    IPMB_ASSERT(strcmp(ipmb_status_string(IPMB_OK), "ok") == 0);
    IPMB_ASSERT(strcmp(ipmb_status_string(IPMB_ERR_BAD_MAGIC), "bad magic") == 0);

    diag_capture_t capture = { 0u, IPMB_OK };
    ipmb_diag_emit(capture_diag, &capture, IPMB_ERR_INVALID, "checked");
    IPMB_ASSERT(capture.calls == 1u);
    IPMB_ASSERT(capture.last_status == IPMB_ERR_INVALID);

    uint8_t bad_msg[28] = { 0 };
    memcpy(bad_msg, "NOPE", 4u);
    ipmb_message_t msg;
    IPMB_ASSERT(ipmb_parse_message(bad_msg, sizeof(bad_msg), &msg) == IPMB_ERR_BAD_MAGIC);

    ipmb_storage_t store;
    size_t first = 99u;
    size_t second = 99u;
    ipmb_storage_init(&store);
    IPMB_ASSERT(ipmb_storage_put(&store, (const uint8_t *)"one", 3u, &first) == IPMB_OK);
    IPMB_ASSERT(ipmb_storage_put(&store, (const uint8_t *)"two", 3u, &second) == IPMB_OK);
    IPMB_ASSERT(first == 0u);
    IPMB_ASSERT(second == 1u);
    IPMB_ASSERT(store.count == 2u);
    IPMB_ASSERT(memcmp(store.blocks[1].data, "two", 3u) == 0);
    ipmb_storage_free(&store);
    return 0;
}
