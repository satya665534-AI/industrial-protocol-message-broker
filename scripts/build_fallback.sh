#!/usr/bin/env bash
set -euo pipefail

: "${CC:=cc}"
: "${CFLAGS:=-std=c11 -Wall -Wextra -Wpedantic -O2 -g}"

mkdir -p build-fallback

srcs=$(
awk '
    /set\(IPMB_SOURCES/ { in_sources = 1; next }
    in_sources && /\)/ { in_sources = 0; next }
    in_sources && /\.c/ {
        gsub(/^[ \t]+|[ \t]+$/, "")
        print
    }
' CMakeLists.txt | tr '\n' ' '
)
tests="tests/test_main.c \
tests/unit/protocol_tests.c \
tests/unit/routing_tests.c \
tests/unit/session_tests.c \
tests/unit/telemetry_tests.c \
tests/unit/codec_tests.c \
tests/unit/replay_tests.c \
tests/integration/full_pipeline_tests.c \
tests/integration/qc_scenario_tests.c \
tests/support/message_builder.c"

"${CC}" ${CFLAGS} -Iinclude -Itests -Itests/support ${srcs} ${tests} -o build-fallback/ipmb_tests
./build-fallback/ipmb_tests
