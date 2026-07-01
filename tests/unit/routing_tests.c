#include "test_framework.h"

#include <string.h>

#include "ipmb/ipmb.h"

int ipmb_test_routing(void)
{
    ipmb_rule_ast_t ast;
    ipmb_rule_bytecode_t bc;
    ipmb_frame_t frame;
    bool matched = false;
    memset(&frame, 0, sizeof(frame));
    frame.frame_type = 0x22u;
    frame.priority = 12u;
    frame.ttl = 64u;
    frame.payload_len = 20u;

    IPMB_ASSERT(ipmb_rule_parse("priority > 10 fast 15", &ast) == IPMB_OK);
    IPMB_ASSERT(ast.op == IPMB_RULE_CMP_GT);
    IPMB_ASSERT(ast.priority == 15);
    IPMB_ASSERT(ipmb_rule_compile(&ast, &bc) == IPMB_OK);
    IPMB_ASSERT(ipmb_vm_evaluate(&bc, &frame, &matched) == IPMB_OK);
    IPMB_ASSERT(matched);

    ipmb_match_table_t table;
    ipmb_match_table_init(&table);
    IPMB_ASSERT(ipmb_match_table_add(&table, "fast_lane", 30, &bc) == IPMB_OK);
    ipmb_rule_bytecode_free(&bc);

    IPMB_ASSERT(ipmb_rule_parse("type = 153 slow 1", &ast) == IPMB_OK);
    IPMB_ASSERT(ipmb_rule_compile(&ast, &bc) == IPMB_OK);
    IPMB_ASSERT(ipmb_match_table_add(&table, "wrong_type", 100, &bc) == IPMB_OK);
    ipmb_rule_bytecode_free(&bc);

    ipmb_match_result_t result;
    IPMB_ASSERT(ipmb_match_table_evaluate(&table, &frame, &result) == IPMB_OK);
    IPMB_ASSERT(strcmp(result.name, "fast_lane") == 0);
    IPMB_ASSERT(result.priority == 30);
    IPMB_ASSERT(result.score == 42);
    ipmb_match_table_free(&table);
    return 0;
}
