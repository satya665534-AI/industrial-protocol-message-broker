#!/usr/bin/env bash
set -euo pipefail
export CC="${CC:-clang}"
cmake -S . -B build-ubsan -G Ninja -DCMAKE_C_COMPILER="${CC}" -DIPMB_ENABLE_UBSAN=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-ubsan --parallel
ctest --test-dir build-ubsan --output-on-failure
