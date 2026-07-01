#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define IPMB_MAX_FRAMES 1024u
#define IPMB_MAX_PAYLOAD (1024u * 1024u)
#define IPMB_MAX_RULE_CODE 4096u
#define IPMB_FRAME_FLAG_FRAGMENTED 0x0001u

typedef enum {
    IPMB_OK = 0,
    IPMB_ERR_SHORT_READ,
    IPMB_ERR_BAD_MAGIC,
    IPMB_ERR_BAD_CRC,
    IPMB_ERR_LIMIT,
    IPMB_ERR_INVALID,
    IPMB_ERR_ALLOC,
    IPMB_ERR_EVAL,
    IPMB_ERR_NOT_FOUND
} ipmb_status_t;

typedef struct {
    const uint8_t *data;
    size_t len;
    size_t pos;
} ipmb_reader_t;

typedef struct {
    const uint8_t *data;
    size_t len;
} ipmb_buf_t;

typedef struct {
    ipmb_status_t code;
    size_t offset;
    char message[160];
} ipmb_error_t;

const char *ipmb_status_string(ipmb_status_t status);
