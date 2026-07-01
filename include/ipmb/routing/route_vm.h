#pragma once
#include <stdbool.h>
#include "ipmb/protocol/frame_reader.h"
#include "ipmb/routing/rule_compiler.h"

typedef enum {
    IPMB_OP_PUSH_INT = 1,
    IPMB_OP_PUSH_STR = 2,
    IPMB_OP_LOAD_FIELD = 3,
    IPMB_OP_CMP_EQ = 4,
    IPMB_OP_CMP_GT = 5,
    IPMB_OP_CMP_LT = 6,
    IPMB_OP_RANGE = 7,
    IPMB_OP_AND = 8,
    IPMB_OP_OR = 9,
    IPMB_OP_NOT = 10,
    IPMB_OP_FORWARD = 11,
    IPMB_OP_TAG = 12,
    IPMB_OP_END = 13
} ipmb_vm_opcode_t;

ipmb_status_t ipmb_vm_evaluate(const ipmb_rule_bytecode_t *bc,
                               const ipmb_frame_t *frame,
                               bool *matched);
