# IPMB Command Line Interface Reference

The Industrial Protocol Message Broker (IPMB) provides several command-line tools for working with message archives, verifying data integrity, and routing telemetry.

## ipmb-inspect

The `ipmb-inspect` tool allows you to examine the contents of an IPMB archive without extracting it.

```bash
Usage: ipmb-inspect [options] <archive_file>

Options:
  -h, --help                 Show this help message
  -v, --verbose              Enable verbose output
  -s, --section <name>       Show details only for the specified section
  -c, --check-crc            Verify all CRC checksums in the archive
  --dump-manifest            Print the manifest JSON to stdout
```

### Examples

**Inspect an archive's structure:**
```bash
$ ipmb-inspect telemetry_dump.ipmb
Archive: telemetry_dump.ipmb
Format Version: 2.1
Sections: 12
Total Size: 45.2 MB
Status: OK
```

**Verify checksums:**
```bash
$ ipmb-inspect --check-crc data.ipmb
Verifying checksums...
Section 1 (Manifest): PASS
Section 2 (Telemetry): PASS
Section 3 (Checkpoints): PASS
All checksums valid.
```

## ipmb-route

The `ipmb-route` tool simulates routing configurations and verifies rule parsing logic against message streams.

```bash
Usage: ipmb-route [options] <rules_file> <input_stream>

Options:
  -h, --help                 Show this help message
  -o, --output <file>        Write routed messages to the specified file
  --dry-run                  Simulate routing without writing output
  --log-level <level>        Set logging level (debug, info, warn, error)
```

## ipmb-parse

The `ipmb-parse` tool tests the binary decoding pipeline against raw field data.

```bash
Usage: ipmb-parse [options] <raw_data_file>

Options:
  -h, --help                 Show this help message
  -f, --format <fmt>         Specify the expected input format (raw, hex, base64)
  --extract-checkpoints      Extract only checkpoint records
```
