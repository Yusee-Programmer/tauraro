# Tauraro Programming Language

Tauraro is a modern, flexible programming language with 100% Python syntax compatibility and enhanced features. It supports multiple compilation backends and provides advanced memory management capabilities. Tauraro files use the .tr extension.

## Features

- **100% Python Syntax Compatibility**: Run existing Python code without modification
- **Multi-language Keywords**: English and Hausa keywords for accessibility
- **Multiple Compilation Backends**: VM, LLVM, C, WebAssembly
- **Advanced Memory Management**: Automatic, manual, and hybrid memory management
- **Optional Static Typing**: Type annotations with runtime checking
- **Built-in Standard Library**: Extensive modules similar to Python's standard library
- **Foreign Function Interface**: Call C libraries directly
- **Python Interoperability**: Embed and extend Python code
- **Async/Await Support**: Native asynchronous programming
- **Pattern Matching**: Modern match/case syntax

## Memory Management

Tauraro provides sophisticated memory management with three modes:

1. **Automatic (Default)**: Reference counting with garbage collection
2. **Manual**: Explicit allocation/deallocation for performance-critical code
3. **Hybrid**: Combination of automatic and manual management with conversion capabilities

```python
# Automatic memory management (default)
data = [1, 2, 3, 4, 5]  # Automatically managed

# Manual memory management
import memory
manual_data = memory.manual([6, 7, 8, 9, 10])  # Manually managed

# Hybrid memory management
hybrid_obj = memory.hybrid({"key": "value"})  # Can switch between modes
```

## Installation

```bash
# Clone the repository
git clone https://github.com/taurarolang/tauraro.git
cd tauraro

# Build the project
cargo build --release

# Run the REPL
cargo run

# Run a Tauraro script
cargo run -- run examples/basic/hello_world.tr
```

## Usage

```bash
# Start the REPL
tauraro

# Run a script
tauraro run script.tr

# Compile to different backends
tauraro compile script.tr --backend llvm
tauraro compile script.tr --backend c
tauraro compile script.tr --backend wasm
```

## Examples

Check out the [examples](examples/) directory for sample code:

- [Hello World](examples/basic/hello_world.tr): Simple program introduction
- [Variables and Types](examples/basic/variables.tr): Variable declaration and type system
- [Control Flow](examples/basic/control_flow.tr): If statements, loops, and conditionals
- [Functions](examples/basic/functions.tr): Function definition and calling
- [Classes](examples/basic/classes.tr): Object-oriented programming
- [Method Calls](examples/basic/method_calls.tr): Object method calling and chaining
- [Data Structures](examples/basic/data_structures.tr): Lists, dictionaries, tuples, and sets
- [Error Handling](examples/basic/error_handling.tr): Exception handling

## Documentation

Comprehensive documentation is available in the [docs](docs/) directory:

- [Getting Started Guide](docs/getting-started.md): Installation and first steps
- [Language Reference](docs/language-reference.md): Complete syntax and semantics (100% Python compatible)
- [API Reference](docs/api-reference.md): Built-in functions and standard library
- [Advanced Features](docs/advanced-features.md): Memory management, JIT compilation, parallel execution
- [Compilation Backends](docs/compilation-backends.md): Multi-target compilation guide
- [Compiler Design](docs/compiler_design.md): Technical architecture

## Contributing

Contributions are welcome! Please read our [contributing guidelines](CONTRIBUTING.md) first.

## License

This project is licensed under either of:

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.