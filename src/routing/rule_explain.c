#include <stdio.h>
#include "ipmb/routing/rule_parser.h"

int ipmb_rule_explain(const ipmb_rule_ast_t *ast, char *buf, size_t len)
{
    const char *op = ast->op == IPMB_RULE_RANGE ? "range" :
                     ast->op == IPMB_RULE_CMP_GT ? ">" :
                     ast->op == IPMB_RULE_CMP_LT ? "<" : "==";
    return snprintf(buf, len, "%s %s %d", ast->field, op, ast->value);
}
