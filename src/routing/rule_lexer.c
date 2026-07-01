#include <ctype.h>
#include <stddef.h>
#include <string.h>

size_t ipmb_rule_lex(const char *text, char tokens[][32], size_t max_tokens)
{
    size_t count = 0;
    const char *p = text;
    while (*p && count < max_tokens) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        size_t n = 0;
        while (p[n] && !isspace((unsigned char)p[n]) && n < 31) n++;
        memcpy(tokens[count], p, n);
        tokens[count][n] = '\0';
        count++;
        while (*p && !isspace((unsigned char)*p)) p++;
    }
    return count;
}
