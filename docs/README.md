# Tauraro Programming Language Documentation

Welcome to the official documentation for Tauraro - a Python-compatible programming language with Rust-like performance!

## What is Tauraro?

Tauraro is a modern programming language that combines:
- **Python compatibility** - Write Python code that just works
- **Rust-like performance** - Execute at native speeds
- **Hybrid typing** - Optional static types with dynamic fallback
- **Multiple backends** - Run with VM or compile to native C
- **Rich Standard Library** - HTTP, async, subprocess, and more - all built-in
- **Easy to learn** - If you know Python, you know Tauraro

## Recent Updates

### New Built-in Modules (Always Available)
All HTTP and async modules are now available by default - no feature flags needed!

- **subprocess** - Process execution and management
- **multiprocessing** - Process-based parallelism (thread-based for now)
- **httpx** - Modern HTTP client built on Rust (hyper, reqwest)
- **httptools** - Fast HTTP parsing and URL utilities
- **websockets** - WebSocket client and server support
- **asyncio** - Full async/await support with tokio runtime

### C Code Generation Improvements
- Fixed type inference for variables with mixed-type usage
- Improved variable declaration with correct C types
- Better handling of string literals vs. dynamic values
- More reliable native compilation through clang/gcc

## Quick Start

```bash
# Install Tauraro
cargo build --release

# Run a script
./target/release/tauraro run script.py

# Start the REPL
./target/release/tauraro repl

# Compile to native C
./target/release/tauraro compile script.py -o output
```

## Documentation Structure

### Getting Started
- [Installation Guide](getting-started/installation.md)
- [Quick Start Tutorial](getting-started/quick-start.md)
- [First Program](getting-started/first-program.md)

### Language Features
- [Syntax Overview](language/syntax.md)
- [Data Types](language/data-types.md)
- [Variables and Constants](language/variables.md)
- [Operators](language/operators.md)
- [Control Flow](language/control-flow.md)
- [Functions](language/functions.md)
- [Classes and OOP](language/classes.md)
- [Modules and Imports](language/modules.md)

### Type System
- [Hybrid Typing](types/hybrid-typing.md)
- [Type Annotations](types/annotations.md)
- [Static Type Checking](types/static-checking.md)
- [Dynamic Typing](types/dynamic-typing.md)
- [Type Inference](types/inference.md)

### REPL
- [Interactive Mode](repl/interactive.md)
- [REPL Features](repl/features.md)
- [Special Commands](repl/commands.md)

### Built-in Functions
- [Core Functions](builtins/core.md)
- [Type Conversion](builtins/conversions.md)
- [I/O Functions](builtins/io.md)
- [Introspection](builtins/introspection.md)

### Standard Library
- [Available Modules](stdlib/modules.md) - **Updated with new modules!**
- [Math Module](stdlib/math.md)
- [System Module](stdlib/sys.md)
- [File I/O](stdlib/io.md)
- [Collections](stdlib/collections.md)
- [HTTP Modules](stdlib/http.md) - httpx, httptools, websockets
- [Async Programming](stdlib/asyncio.md) - asyncio module
- [Process Management](stdlib/subprocess.md) - subprocess, multiprocessing

### Compilation
- [C Backend](compilation/c-backend.md)
- [Optimization Levels](compilation/optimizations.md)
- [FFI Integration](compilation/ffi.md)

### Advanced Topics
- [Performance Tuning](advanced/performance.md)
- [Foreign Function Interface (FFI)](advanced/ffi.md)
- [Memory Management](advanced/memory.md)
- [Concurrency](advanced/concurrency.md)
- [Interoperability](advanced/interop.md)

### API Reference
- [Language API](api/language.md)
- [Runtime API](api/runtime.md)
- [C API](api/c-api.md)

### Examples
- [Code Examples](examples/index.md)
- [Design Patterns](examples/patterns.md)
- [Best Practices](examples/best-practices.md)

## Contributing

See [CONTRIBUTING.md](../CONTRIBUTING.md) for how to contribute to Tauraro.

## License

Tauraro is open source. See [LICENSE](../LICENSE) for details.
