#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
        fprintf(stderr, "usage: %s <message.bin>\n", argv[0]);
        return 2;
    }
    size_t len = 0;
    uint8_t *buf = read_file(argv[1], &len);
    if (!buf) {
        fprintf(stderr, "could not read %s\n", argv[1]);
        return 1;
    }
    ipmb_message_t msg;
    ipmb_status_t rc = ipmb_parse_message(buf, len, &msg);
    if (rc != IPMB_OK) {
        fprintf(stderr, "parse failed: %s\n", ipmb_status_string(rc));
        free(buf);
        return 1;
    }
    printf("IPMB v%u session=%llu sequence=%u frames=%u risk=%u\n",
           msg.version, (unsigned long long)msg.session_id, msg.sequence,
           msg.frame_count, ipmb_security_risk_score(&msg));
    for (uint32_t i = 0; i < msg.frame_count; ++i) {
        printf("frame[%u] type=%u flags=0x%x payload=%zu src=%u dst=%u\n",
               i, msg.frames[i].frame_type, msg.frames[i].frame_flags,
               msg.frames[i].payload_len, msg.frames[i].source_addr,
               msg.frames[i].dest_addr);
    }
    ipmb_message_free(&msg);
    free(buf);
    return 0;
}
