cmake_minimum_required(VERSION 3.16)
project(ipmb VERSION 0.2.0 LANGUAGES C)

option(IPMB_BUILD_TESTS "Build IPMB tests" ON)
option(IPMB_BUILD_APPS "Build IPMB CLI applications" ON)
option(IPMB_BUILD_FUZZERS "Build IPMB fuzz targets" OFF)
option(IPMB_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(IPMB_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(Sanitizers)

set(IPMB_PUBLIC_HEADERS
    include/ipmb/ipmb.h
    include/ipmb/types.h
    include/ipmb/protocol/frame_reader.h
    include/ipmb/protocol/field_decoder.h
    include/ipmb/protocol/transport_layer.h
    include/ipmb/routing/rule_parser.h
    include/ipmb/routing/rule_compiler.h
    include/ipmb/routing/route_vm.h
    include/ipmb/routing/match_table.h
    include/ipmb/session/session_tracker.h
    include/ipmb/session/journal.h
    include/ipmb/session/checkpoint_store.h
    include/ipmb/session/ring_window.h
    include/ipmb/telemetry/extractor.h
    include/ipmb/telemetry/timeseries.h
    include/ipmb/telemetry/histogram.h
    include/ipmb/telemetry/sample_buffer.h
    include/ipmb/codec/delta_codec.h
    include/ipmb/codec/rle_codec.h
    include/ipmb/codec/bitstream.h
    include/ipmb/replay/replay_engine.h
    include/ipmb/replay/event_log.h
    include/ipmb/replay/state_snapshot.h
    include/ipmb/diagnostics.h
    include/ipmb/security.h
    include/ipmb/storage.h
    include/ipmb/util/byte_read.h
    include/ipmb/util/endian.h
    include/ipmb/util/checksum.h
    include/ipmb/util/varint.h
)

set(IPMB_SOURCES
    src/protocol/frame_reader.c
    src/protocol/field_decoder.c
    src/protocol/crc_table.c
    src/protocol/transport_layer.c
    src/protocol/protocol_spec.c
    src/routing/rule_lexer.c
    src/routing/rule_parser.c
    src/routing/rule_compiler.c
    src/routing/route_vm.c
    src/routing/match_table.c
    src/routing/rule_analyzer.c
    src/routing/rule_explain.c
    src/routing/symbol_table.c
    src/routing/rule_catalog.c
    src/session/session_tracker.c
    src/session/journal.c
    src/session/checkpoint_store.c
    src/session/ring_window.c
    src/session/ownership_model.c
    src/session/session_catalog.c
    src/telemetry/extractor.c
    src/telemetry/timeseries.c
    src/telemetry/histogram.c
    src/telemetry/sample_buffer.c
    src/codec/delta_codec.c
    src/codec/rle_codec.c
    src/codec/bitstream.c
    src/replay/replay_engine.c
    src/replay/event_log.c
    src/replay/state_snapshot.c
    src/diagnostics.c
    src/security.c
    src/storage.c
    src/util/byte_read.c
    src/util/endian.c
    src/util/checksum.c
    src/util/varint.c
)

add_library(ipmb STATIC ${IPMB_PUBLIC_HEADERS} ${IPMB_SOURCES})
target_include_directories(ipmb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
ipmb_apply_warnings(ipmb)
ipmb_apply_sanitizers(ipmb)

if(IPMB_BUILD_APPS)
    foreach(app ipmb-parse ipmb-route ipmb-inspect)
        add_executable(${app} apps/${app}/main.c)
        target_link_libraries(${app} PRIVATE ipmb)
        ipmb_apply_warnings(${app})
        ipmb_apply_sanitizers(${app})
    endforeach()
endif()

if(IPMB_BUILD_TESTS)
    enable_testing()
    add_executable(ipmb_tests
        tests/test_main.c
        tests/unit/protocol_tests.c
        tests/unit/routing_tests.c
        tests/unit/session_tests.c
        tests/unit/telemetry_tests.c
        tests/unit/codec_tests.c
        tests/unit/replay_tests.c
        tests/integration/full_pipeline_tests.c
        tests/integration/qc_scenario_tests.c
        tests/support/message_builder.c)
    target_include_directories(ipmb_tests PRIVATE tests tests/support)
    target_link_libraries(ipmb_tests PRIVATE ipmb)
    ipmb_apply_warnings(ipmb_tests)
    ipmb_apply_sanitizers(ipmb_tests)
    add_test(NAME ipmb_tests COMMAND ipmb_tests)
endif()

if(IPMB_BUILD_FUZZERS)
    file(GLOB IPMB_FUZZERS CONFIGURE_DEPENDS fuzz/*_fuzzer.c)
    foreach(fuzz_src ${IPMB_FUZZERS})
        get_filename_component(fuzz_name "${fuzz_src}" NAME_WE)
        add_executable(${fuzz_name} ${fuzz_src})
        target_link_libraries(${fuzz_name} PRIVATE ipmb)
        target_compile_definitions(${fuzz_name} PRIVATE IPMB_STANDALONE_FUZZER=1)
        ipmb_apply_warnings(${fuzz_name})
        ipmb_apply_sanitizers(${fuzz_name})
    endforeach()
endif()
