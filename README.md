# TauraroLang 🚀

**A Modern, Multi-Backend Programming Language with Dynamic Typing and Powerful Interoperability**

TauraroLang is a versatile programming language designed for modern development needs, featuring dynamic typing, multiple compilation backends, FFI support, and seamless interoperability with C and Python. Built with Rust for performance and reliability.

## ✨ Features

- **🔄 Dynamic Typing**: Flexible type system with optional static typing support
- **🎯 Multiple Backends**: Compile to C, WebAssembly, LLVM IR, or run interpreted
- **🔗 FFI Support**: Seamless integration with C libraries and Python modules
- **⚡ High Performance**: Rust-based implementation with optimized execution
- **🔧 REPL Support**: Interactive development environment
- **📦 Memory Management**: Automatic memory management with manual control options
- **🌐 Async Support**: Built-in async/await for concurrent programming
- **🛠️ Developer Friendly**: Rich error messages and debugging support

## 🚀 Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/Yusee-Programmer/tauraro.git
cd tauraro

# Build the project
cargo build --release

# Run the REPL
cargo run
```

### Your First Program

Create a file `hello.tr`:

```tauraro
print("Hello, World!")

aiki gaisuwa(name):
    fitar f"Hello {name}"

gaisuwa("Developer")
```

Run it:

```bash
cargo run -- hello.tr
```

### Interactive REPL

```bash
$ cargo run
TauraroLang REPL v1.0.0
>>> print("Welcome to TauraroLang!")
Welcome to TauraroLang!
>>> let x = 5 + 3 * 2
>>> print(x)
11
>>> exit()
```

## 📚 Documentation

### Core Documentation
- **[Language Reference](docs/language-reference.md)** - Complete syntax and language features
- **[Getting Started Guide](docs/getting-started.md)** - Step-by-step tutorials
- **[API Documentation](docs/api-reference.md)** - Built-in functions and standard library

### Advanced Topics
- **[Compilation Backends](docs/compilation-backends.md)** - C, WASM, LLVM compilation
- **[FFI & Interop Guide](docs/ffi-interop.md)** - Foreign Function Interface and Python integration
- **[Advanced Features](docs/advanced-features.md)** - Memory management, async programming
- **[Best Practices](docs/best-practices.md)** - Code organization and performance tips

### Developer Resources
- **[Troubleshooting](docs/troubleshooting.md)** - Common issues and solutions
- **[Contributing](CONTRIBUTING.md)** - How to contribute to TauraroLang
- **[Changelog](CHANGELOG.md)** - Version history and updates

## 🎯 Language Overview

### Basic Syntax

```tauraro
// Variables and basic operations
let x = 42
let name = "TauraroLang"
let is_awesome = true

// Functions
aiki calculate(a, b):
    fitar a + b * 2

// Control flow
idan x > 10:
    print("Large number")
kuma:
    print("Small number")

// Lists and dictionaries
let numbers = [1, 2, 3, 4, 5]
let person = {"name": "Alice", "age": 30}
```

### Advanced Features

```tauraro
// Async programming
async aiki fetch_data():
    let result = await http_get("https://api.example.com")
    fitar result

// FFI integration
extern fn c_sqrt(x: float) -> float
let result = c_sqrt(16.0)

// Type annotations (optional)
aiki typed_function(x: int, y: str) -> bool:
    fitar len(y) > x
```

## 🔧 Compilation Backends

TauraroLang supports multiple compilation targets:

### 1. **C Backend**
```bash
cargo run -- --backend c --output program.c input.tr
gcc program.c -o program
```

### 2. **WebAssembly**
```bash
cargo run -- --backend wasm --output program.wasm input.tr
```

### 3. **LLVM IR**
```bash
cargo run -- --backend llvm --output program.ll input.tr
```

### 4. **Interpreter** (Default)
```bash
cargo run -- input.tr
```

## 🔗 FFI Examples

### C Integration
```tauraro
// Declare external C function
extern fn strlen(s: str) -> int

// Use it in TauraroLang
let length = strlen("Hello, World!")
print(f"Length: {length}")
```

### Python Integration
```tauraro
// Import Python modules
import math from python

// Use Python functions
let result = math.sqrt(25)
print(f"Square root: {result}")
```

## 🏗️ Built-in Functions

| Function | Description | Example |
|----------|-------------|---------|
| `print(...)` | Output values to console | `print("Hello", 42)` |
| `len(obj)` | Get length of string/list/dict | `len([1, 2, 3])` → `3` |
| `type(obj)` | Get type of object | `type(42)` → `"int"` |
| `str(obj)` | Convert to string | `str(123)` → `"123"` |
| `int(obj)` | Convert to integer | `int("42")` → `42` |
| `float(obj)` | Convert to float | `float("3.14")` → `3.14` |
| `bool(obj)` | Convert to boolean | `bool(0)` → `false` |
| `range(n)` | Generate range of numbers | `range(3)` → `[0, 1, 2]` |

## 🧪 Examples

Explore the `examples/` directory for more comprehensive examples:

- **[hello_world.tr](examples/hello_world.tr)** - Basic syntax demonstration
- **[math.tr](examples/math.tr)** - Mathematical operations
- **[ffi_example.tr](examples/ffi_example.tr)** - FFI integration
- **[async_example.tr](examples/async_example.tr)** - Async programming

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details on:

- Setting up the development environment
- Code style and conventions
- Submitting pull requests
- Reporting issues

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Built with [Rust](https://rust-lang.org/) for performance and safety
- Inspired by modern language design principles
- Community-driven development and feedback

## 📚 Documentation

Comprehensive documentation is available in the `docs/` directory:

### Core Documentation
- **[Getting Started Guide](docs/getting-started.md)** - Step-by-step tutorials and first steps
- **[Language Reference](docs/language-reference.md)** - Complete syntax and semantics guide
- **[API Reference](docs/api-reference.md)** - Built-in functions and standard library

### Advanced Topics
- **[Compilation Backends](docs/compilation-backends.md)** - C, WebAssembly, LLVM IR backends
- **[FFI Guide](docs/ffi-guide.md)** - Foreign Function Interface and interoperability
- **[Advanced Features](docs/advanced-features.md)** - Memory management, async, performance

### Developer Resources
- **[Best Practices](docs/best-practices.md)** - Code organization, patterns, and tips
- **[Troubleshooting](docs/troubleshooting.md)** - Common issues and solutions
- **[Compiler Design](docs/compiler_design.md)** - Internal architecture and design

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/Yusee-Programmer/tauraro/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Yusee-Programmer/tauraro/discussions)
- **Documentation**: [docs/](docs/)

---

**Happy coding with TauraroLang! 🎉**
