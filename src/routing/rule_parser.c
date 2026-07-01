#include <stdlib.h>
#include <string.h>
#include "ipmb/routing/rule_parser.h"

size_t ipmb_rule_lex(const char *text, char tokens[][32], size_t max_tokens);

static ipmb_rule_op_t parse_op(const char *s)
{
    if (strcmp(s, ">") == 0 || strcmp(s, "gt") == 0) return IPMB_RULE_CMP_GT;
    if (strcmp(s, "<") == 0 || strcmp(s, "lt") == 0) return IPMB_RULE_CMP_LT;
    return IPMB_RULE_CMP_EQ;
}

ipmb_status_t ipmb_rule_parse(const char *text, ipmb_rule_ast_t *out)
{
    char tok[10][32];
    memset(out, 0, sizeof(*out));
    size_t n = ipmb_rule_lex(text, tok, 10);
    if (n < 3) return IPMB_ERR_INVALID;
    out->priority = 10;
    if (strcmp(tok[0], "range") == 0) {
        if (n < 4) return IPMB_ERR_INVALID;
        strncpy(out->field, tok[1], sizeof(out->field) - 1);
        out->op = IPMB_RULE_RANGE;
        out->value = (int32_t)strtol(tok[2], NULL, 0);
        out->high = (int32_t)strtol(tok[3], NULL, 0);
        if (n > 4) strncpy(out->action, tok[4], sizeof(out->action) - 1);
        return IPMB_OK;
    }
    strncpy(out->field, tok[0], sizeof(out->field) - 1);
    out->op = parse_op(tok[1]);
    out->value = (int32_t)strtol(tok[2], NULL, 0);
    if (n > 3) strncpy(out->action, tok[3], sizeof(out->action) - 1);
    if (n > 4) out->priority = (int)strtol(tok[4], NULL, 0);
    return IPMB_OK;
}

void ipmb_rule_ast_free(ipmb_rule_ast_t *ast)
{
    (void)ast;
}
