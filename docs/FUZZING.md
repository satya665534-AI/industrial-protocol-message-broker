# Fuzzing

This document is part of the IPMB project notes.

## Message Container

`IPMB` messages contain a CRC-checked header and one or more CRC-checked
frames. Frame payloads are dispatched to protocol, routing, session, or
telemetry handlers depending on type and flags.

## Operational Notes

The code intentionally keeps older pointer/length APIs beside newer
`ipmb_buf_t`-style interfaces because the project models an organically
evolved C broker rather than a greenfield framework.
