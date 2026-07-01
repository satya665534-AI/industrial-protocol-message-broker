#!/usr/bin/env bash
set -euo pipefail
export CC="${CC:-clang}"
cmake -S . -B build-asan -G Ninja -DCMAKE_C_COMPILER="${CC}" -DIPMB_ENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-asan --parallel
ctest --test-dir build-asan --output-on-failure
