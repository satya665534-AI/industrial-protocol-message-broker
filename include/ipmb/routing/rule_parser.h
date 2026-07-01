#pragma once
#include "ipmb/types.h"

typedef enum {
    IPMB_RULE_CMP_EQ = 0,
    IPMB_RULE_CMP_GT,
    IPMB_RULE_CMP_LT,
    IPMB_RULE_RANGE
} ipmb_rule_op_t;

typedef struct {
    char field[32];
    ipmb_rule_op_t op;
    int32_t value;
    int32_t high;
    char action[32];
    int priority;
} ipmb_rule_ast_t;

ipmb_status_t ipmb_rule_parse(const char *text, ipmb_rule_ast_t *out);
void ipmb_rule_ast_free(ipmb_rule_ast_t *ast);
