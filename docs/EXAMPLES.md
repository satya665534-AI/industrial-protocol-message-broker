# IPMB Examples

Here are some examples of how to use the IPMB CLI tools for debugging and manipulating protocol messages.

### Route an incoming message stream to multiple zones

If you have a ruleset defined in `rules.txt` and a raw traffic capture in `capture.bin`, you can test the routing virtual machine:

```bash
$ ipmb-route rules.txt capture.bin --output out_zone1.bin
Routing engine started.
Loaded 3 rules from 'rules.txt'.
Processed 1024 frames.
Routed 512 frames to 'out_zone1.bin'.
Dropped 512 frames.
```

### Inspect a binary message package

To verify the structure of a package without writing anything:

```bash
$ ipmb-inspect telemetry_dump.ipmb --verbose
Archive: telemetry_dump.ipmb
Format Version: 2.1
Sections: 12
Total Size: 45.2 MB
Status: OK

Section Header Checksum: OK
Directory Offset: 0x4B200
```

### Parse Modbus TCP packets directly

To test the field decoding logic on raw data:

```bash
$ ipmb-parse raw_modbus.bin --format raw
Found Protocol: MODBUS_TCP
Fields:
  Transaction ID: 0x0001
  Protocol ID: 0x0000
  Length: 6
  Unit ID: 0x01
  Function Code: 0x03
```
