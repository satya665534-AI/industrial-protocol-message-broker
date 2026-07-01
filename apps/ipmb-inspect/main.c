#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ipmb/ipmb.h"

static uint8_t *read_file(const char *path, size_t *len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return NULL; }
    long sz = ftell(fp);
    if (sz < 0) { fclose(fp); return NULL; }
    rewind(fp);
    uint8_t *buf = (uint8_t *)malloc((size_t)sz ? (size_t)sz : 1u);
    if (!buf) { fclose(fp); return NULL; }
    if (fread(buf, 1, (size_t)sz, fp) != (size_t)sz) {
        free(buf);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    *len = (size_t)sz;
    return buf;
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <blob>\n", argv[0]);
        return 2;
    }
    size_t len = 0;
    uint8_t *buf = read_file(argv[1], &len);
    if (!buf) return 1;
    const char *kind = "unknown";
    if (len >= 4 && memcmp(buf, "IPMB", 4) == 0) kind = "message";
    else if (len >= 4 && memcmp(buf, "IJRN", 4) == 0) kind = "journal";
    else if (len >= 4 && memcmp(buf, "ITLM", 4) == 0) kind = "telemetry";
    else if (len >= 4 && memcmp(buf, "IDLT", 4) == 0) kind = "delta-codec";
    printf("%s: %zu bytes\n", kind, len);
    free(buf);
    return 0;
}
