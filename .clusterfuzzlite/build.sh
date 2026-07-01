#!/usr/bin/env bash
set -euo pipefail

find_repo_root() {
  for candidate in "${SRC:-}" "${SRC:-}/repo" "$PWD" "$(dirname "${BASH_SOURCE[0]}")" "$(dirname "${BASH_SOURCE[0]}")/.." "/src/repo" "/src"; do
    if [[ -n "$candidate" && -f "$candidate/CMakeLists.txt" ]]; then
      cd "$candidate"
      pwd
      return 0
    fi
  done
  found="$(find /src "$PWD" -maxdepth 4 -name CMakeLists.txt -print 2>/dev/null | head -n 1 || true)"
  if [[ -n "$found" ]]; then
    cd "$(dirname "$found")"
    pwd
    return 0
  fi
  echo "Could not locate repo root containing CMakeLists.txt" >&2
  exit 2
}

ROOT="$(find_repo_root)"
OUT="${OUT:-/out}"
export CC="${CC:-clang}"
export CFLAGS="${CFLAGS:--O1 -g -fno-omit-frame-pointer}"
export LIB_FUZZING_ENGINE="${LIB_FUZZING_ENGINE:-}"

mkdir -p "$OUT"

echo "Compiling core project objects..."
mkdir -p build-fuzz-objects

# Read sources directly from CMakeLists.txt so the fuzzer build matches the
# curated project source list and ignores intentionally unlisted files.
SOURCES=$(awk '/set\(IPMB_SOURCES/,/\)/' CMakeLists.txt | grep '\.c' | tr -d ' \t' || true)

for f in $SOURCES; do
    if [ -f "$f" ]; then
        obj="build-fuzz-objects/$(basename "$f").o"
        ${CC} ${CFLAGS} -std=c11 -Iinclude -c "$f" -o "$obj"
    fi
done

echo "Archiving objects into static library..."
ar rcs build-fuzz-objects/libipmb.a build-fuzz-objects/*.o

# Set up fuzzer engine flags
if [[ -n "${LIB_FUZZING_ENGINE:-}" ]]; then
  FUZZ_MAIN_FLAGS=""
  FUZZ_ENGINE="${LIB_FUZZING_ENGINE}"
else
  FUZZ_MAIN_FLAGS="-DIPMB_STANDALONE_FUZZER=1"
  FUZZ_ENGINE=""
fi

echo "Compiling fuzzers..."
for f in fuzz/*_fuzzer.c; do
  name=$(basename "$f" .c)
  echo "Building $name..."
  ${CC} ${CFLAGS} -std=c11 -Iinclude ${FUZZ_MAIN_FLAGS} "$f" \
    build-fuzz-objects/libipmb.a ${FUZZ_ENGINE} -o "${OUT}/${name}"
done

rm -rf build-fuzz-objects

TARGETS=(
  frame_reader_fuzzer
  field_decoder_fuzzer
  transport_layer_fuzzer
  route_vm_fuzzer
  rule_parser_fuzzer
  match_table_fuzzer
  delta_codec_fuzzer
  rle_codec_fuzzer
  session_journal_fuzzer
  checkpoint_store_fuzzer
  ring_window_fuzzer
  telemetry_extractor_fuzzer
  histogram_fuzzer
  replay_engine_fuzzer
  full_pipeline_fuzzer
)

for target in "${TARGETS[@]}"; do
  if [[ ! -x "$OUT/$target" ]]; then
    echo "Missing built fuzz target: $target" >&2
    exit 3
  fi
  chmod +x "$OUT/$target"
done

echo "All fuzz targets built and verified successfully."
ls -la "$OUT"
