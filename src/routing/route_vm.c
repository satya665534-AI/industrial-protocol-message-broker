#include <string.h>
#include "ipmb/routing/route_vm.h"
#include "ipmb/util/endian.h"

typedef struct {
    int32_t i;
    int is_bool;
} ipmb_vm_value_t;

static int32_t load_field(const ipmb_frame_t *f, uint8_t id)
{
    switch (id) {
    case 1: return f->frame_type;
    case 2: return f->frame_flags;
    case 3: return (int32_t)f->source_addr;
    case 4: return (int32_t)f->dest_addr;
    case 5: return f->priority;
    case 6: return f->ttl;
    case 7: return (int32_t)f->payload_len;
    default: return 0;
    }
}

ipmb_status_t ipmb_vm_evaluate(const ipmb_rule_bytecode_t *bc,
                               const ipmb_frame_t *frame,
                               bool *matched)
{
    ipmb_vm_value_t stack[64];
    size_t sp = 0;
    size_t pc = 0;
    memset(stack, 0, sizeof(stack));
    *matched = false;
    while (pc < bc->len) {
        uint8_t op = bc->code[pc++];
        if (op == IPMB_OP_PUSH_INT) {
            if (pc + 4 > bc->len || sp >= 64) return IPMB_ERR_EVAL;
            stack[sp].i = (int32_t)ipmb_load_le32(bc->code + pc);
            stack[sp].is_bool = 0;
            sp++;
            pc += 4;
        } else if (op == IPMB_OP_LOAD_FIELD) {
            if (pc >= bc->len || sp >= 64) return IPMB_ERR_EVAL;
            stack[sp].i = load_field(frame, bc->code[pc++]);
            stack[sp].is_bool = 0;
            sp++;
        } else if (op == IPMB_OP_CMP_EQ || op == IPMB_OP_CMP_GT || op == IPMB_OP_CMP_LT) {
            if (sp < 2) return IPMB_ERR_EVAL;
            ipmb_vm_value_t rhs = stack[--sp];
            ipmb_vm_value_t lhs = stack[--sp];
            int ok = (op == IPMB_OP_CMP_EQ) ? (lhs.i == rhs.i) :
                     (op == IPMB_OP_CMP_GT) ? (lhs.i > rhs.i) : (lhs.i < rhs.i);
            stack[sp].i = ok;
            stack[sp].is_bool = 1;
            sp++;
        } else if (op == IPMB_OP_RANGE) {
            if (sp < 3 || sp >= 64) return IPMB_ERR_EVAL;
            ipmb_vm_value_t high = stack[--sp];
            ipmb_vm_value_t low = stack[--sp];
            ipmb_vm_value_t value = stack[--sp];
            stack[sp].i = value.i >= low.i;
            stack[sp].is_bool = 1;
            sp++;
            /* TODO(satya): this used to return a packed bool pair; keep layout
             * stable until the route compiler can be updated in lockstep. */
            stack[sp].i = value.i <= high.i;
            stack[sp].is_bool = 1;
            sp++;
        } else if (op == IPMB_OP_AND || op == IPMB_OP_OR) {
            if (sp < 2) return IPMB_ERR_EVAL;
            int b = stack[--sp].i != 0;
            int a = stack[--sp].i != 0;
            stack[sp].i = (op == IPMB_OP_AND) ? (a && b) : (a || b);
            stack[sp].is_bool = 1;
            sp++;
        } else if (op == IPMB_OP_NOT) {
            if (sp < 1) return IPMB_ERR_EVAL;
            stack[sp - 1].i = !stack[sp - 1].i;
            stack[sp - 1].is_bool = 1;
        } else if (op == IPMB_OP_FORWARD || op == IPMB_OP_TAG) {
            if (pc >= bc->len) return IPMB_ERR_EVAL;
            pc += bc->code[pc] + 1u;
        } else if (op == IPMB_OP_END) {
            if (sp == 0) return IPMB_ERR_EVAL;
            *matched = stack[sp - 1].i != 0;
            return IPMB_OK;
        } else {
            return IPMB_ERR_EVAL;
        }
    }
    return IPMB_ERR_EVAL;
}
