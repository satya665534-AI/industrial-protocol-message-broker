# Getting Started with IPMB

Welcome to the Industrial Protocol Message Broker (IPMB). This guide will help you build and run the toolkit on your system.

## Prerequisites

- CMake 3.15 or higher
- A C11 compatible compiler (GCC or Clang)
- Ninja or Make build system

## Building from Source

1. **Clone the repository:**
   ```bash
   git clone https://github.com/example/industrial-protocol-message-broker.git
   cd industrial-protocol-message-broker
   ```

2. **Configure the build:**
   ```bash
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   ```

3. **Compile the project:**
   ```bash
   cmake --build build --parallel
   ```

4. **Run tests to verify:**
   ```bash
   ctest --test-dir build --output-on-failure
   ```

## Using the CLI Tools

After building, the CLI tools will be available in the `build/apps` directory. You can use them to inspect and manipulate IPMB archive files.

For example, to inspect a sample archive:
```bash
./build/apps/ipmb-inspect examples/sample.ipmb
```

For more detailed information on the command-line interface, refer to the [CLI Reference](CLI_REFERENCE.md).
