# IPMB Architecture

The Industrial Protocol Message Broker (IPMB) is designed around a modular pipeline architecture. 

## Core Components

### 1. Archive Parsing Engine
Responsible for reading `.ipmb` binary archives. It handles magic byte verification, CRC checksum validation, and section directory parsing. All reading operations are designed to be zero-copy where possible.

### 2. Telemetry Extractor
Decodes time-series sensor data from the compressed sections within the archive. It uses custom Delta and RLE (Run-Length Encoding) decoders optimized for industrial data patterns.

### 3. Checkpoint Store
Manages transactional state checkpoints used by the routing VM. This component is critical for ensuring message delivery guarantees across sudden restarts.

### 4. Routing Virtual Machine (VM)
A lightweight evaluation engine that applies user-defined rules to incoming messages. It parses a custom rule syntax and determines message routing paths based on payload content and metadata.

## Memory Management Strategy
IPMB is written in pure C11 and avoids dynamic allocation (`malloc`/`free`) in the hot path. Instead, it relies on arena allocators and static buffers to guarantee deterministic memory usage and prevent fragmentation during long-running broker sessions.
