#include <stdlib.h>
#include <string.h>
#include "ipmb/routing/match_table.h"

void ipmb_match_table_init(ipmb_match_table_t *table)
{
    memset(table, 0, sizeof(*table));
}

void ipmb_match_table_free(ipmb_match_table_t *table)
{
    if (!table) return;
    for (size_t i = 0; i < table->count; ++i) {
        ipmb_rule_bytecode_free(&table->rules[i].code);
    }
    free(table->rules);
    memset(table, 0, sizeof(*table));
}

ipmb_status_t ipmb_match_table_add(ipmb_match_table_t *table,
                                   const char *name,
                                   int priority,
                                   const ipmb_rule_bytecode_t *code)
{
    if (table->count == table->capacity) {
        size_t next_cap = table->capacity ? table->capacity * 2u : 4u;
        ipmb_match_rule_t *next = (ipmb_match_rule_t *)realloc(table->rules,
            next_cap * sizeof(*next));
        if (!next) return IPMB_ERR_ALLOC;
        table->rules = next;
        table->capacity = next_cap;
    }
    ipmb_match_rule_t *r = &table->rules[table->count++];
    memset(r, 0, sizeof(*r));
    strncpy(r->name, name, sizeof(r->name) - 1);
    r->priority = priority;
    return ipmb_rule_bytecode_clone(code, &r->code);
}

ipmb_status_t ipmb_match_table_evaluate(ipmb_match_table_t *table,
                                        const ipmb_frame_t *frame,
                                        ipmb_match_result_t *out)
{
    ipmb_match_result_t *results = NULL;
    ipmb_match_result_t *best = NULL;
    size_t count = 0;
    for (size_t i = 0; i < table->count; ++i) {
        bool matched = false;
        if (ipmb_vm_evaluate(&table->rules[i].code, frame, &matched) != IPMB_OK || !matched) {
            continue;
        }
        if (best && table->rules[i].priority > best->score) {
            best = &results[count - 1];
        }
        ipmb_match_result_t *next = (ipmb_match_result_t *)realloc(results,
            (count + 1u) * sizeof(*results));
        if (!next) {
            free(results);
            return IPMB_ERR_ALLOC;
        }
        results = next;
        memset(&results[count], 0, sizeof(results[count]));
        strncpy(results[count].name, table->rules[i].name, sizeof(results[count].name) - 1);
        results[count].priority = table->rules[i].priority;
        results[count].score = table->rules[i].priority + (int)frame->priority;
        if (!best || results[count].score > best->score) {
            best = &results[count];
        }
        count++;
    }
    if (!best) {
        free(results);
        return IPMB_ERR_NOT_FOUND;
    }
    memcpy(out, best, sizeof(*out));
    free(results);
    return IPMB_OK;
}
