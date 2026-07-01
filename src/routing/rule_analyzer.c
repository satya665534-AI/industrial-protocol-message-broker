#include <stddef.h>
#include "ipmb/routing/rule_compiler.h"

size_t ipmb_rule_bytecode_instruction_count(const ipmb_rule_bytecode_t *bc)
{
    size_t n = 0;
    for (size_t i = 0; i < bc->len; ++i) {
        n += (bc->code[i] >= 1 && bc->code[i] <= 13) ? 1u : 0u;
    }
    return n;
}
