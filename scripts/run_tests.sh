#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build -G Ninja -DIPMB_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --parallel
ctest --test-dir build --output-on-failure
