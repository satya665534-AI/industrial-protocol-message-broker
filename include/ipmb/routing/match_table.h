#pragma once
#include "ipmb/routing/route_vm.h"

typedef struct {
    char name[32];
    int priority;
    int score;
} ipmb_match_result_t;

typedef struct {
    char name[32];
    int priority;
    ipmb_rule_bytecode_t code;
} ipmb_match_rule_t;

typedef struct {
    ipmb_match_rule_t *rules;
    size_t count;
    size_t capacity;
} ipmb_match_table_t;

void ipmb_match_table_init(ipmb_match_table_t *table);
void ipmb_match_table_free(ipmb_match_table_t *table);
ipmb_status_t ipmb_match_table_add(ipmb_match_table_t *table,
                                   const char *name,
                                   int priority,
                                   const ipmb_rule_bytecode_t *code);
ipmb_status_t ipmb_match_table_evaluate(ipmb_match_table_t *table,
                                        const ipmb_frame_t *frame,
                                        ipmb_match_result_t *out);
