# Industrial Protocol Message Broker (IPMB)

Industrial Protocol Message Broker is a C11 library and command-line toolkit for processing structured telemetry, configuration, replay, and audit records from industrial device archives.

IPMB is a pure C11 library and CLI toolkit for parsing, validating, routing,
journaling, replaying, and extracting telemetry from SCADA/Modbus/DNP3-style
binary message streams.

**Author:** Satyabrata Dalai <satya665534@gmail.com>  
**License:** MIT  
**Language:** C11, no C++ and no external runtime dependencies

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

If CMake is not installed, use the single-compiler fallback:

```sh
./scripts/build_fallback.sh
```

## CLI

```sh
./build/ipmb-parse fuzz/corpus/frame_reader_fuzzer/seed_000.bin
./build/ipmb-route "type == 10 forward 1" fuzz/corpus/frame_reader_fuzzer/seed_000.bin
./build/ipmb-inspect fuzz/corpus/session_journal_fuzzer/seed_000.bin
```

## Repository Shape

The project is split into protocol parsing, routing, session journaling,
telemetry, codecs, replay, diagnostics, and storage. The fuzz targets under
`fuzz/` exercise each subsystem and start from CRC-valid seed corpora.
