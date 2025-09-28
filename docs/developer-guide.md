# TauraroLang Developer Guide

This comprehensive guide covers everything developers need to know to contribute to TauraroLang, understand the codebase, and set up a development environment.

## Table of Contents

1. [Project Overview](#project-overview)
2. [Project Structure](#project-structure)
3. [Development Environment Setup](#development-environment-setup)
4. [Build System](#build-system)
5. [Development Workflow](#development-workflow)
6. [Testing](#testing)
7. [Code Style and Standards](#code-style-and-standards)
8. [Contributing Guidelines](#contributing-guidelines)
9. [Architecture Overview](#architecture-overview)
10. [Debugging and Profiling](#debugging-and-profiling)
11. [Release Process](#release-process)
12. [Troubleshooting](#troubleshooting)

## Project Overview

TauraroLang is a modern, multi-backend programming language written in Rust. It features:

- **Multiple Compilation Backends**: Interpreter, C transpiler, LLVM, WebAssembly
- **Memory Safety**: Automatic memory management with reference counting
- **Async/Await Support**: Built-in asynchronous programming capabilities
- **FFI Integration**: Foreign Function Interface for C libraries and Python interop
- **Rich Standard Library**: Comprehensive built-in modules for various tasks

### Key Design Goals

- **Performance**: Competitive with compiled languages
- **Safety**: Memory safety without garbage collection overhead
- **Flexibility**: Multiple deployment targets and use cases
- **Interoperability**: Easy integration with existing codebases

## Project Structure

```
tauraro/
├── src/                    # Main source code
│   ├── main.rs            # CLI entry point
│   ├── lib.rs             # Library entry point
│   ├── lexer.rs           # Tokenization
│   ├── parser.rs          # AST generation
│   ├── ast.rs             # Abstract Syntax Tree definitions
│   ├── semantic.rs        # Semantic analysis and type checking
│   ├── ir.rs              # Intermediate Representation
│   ├── vm.rs              # Virtual Machine implementation
│   ├── value.rs           # Value system and types
│   ├── builtins.rs        # Built-in functions
│   ├── runtime.rs         # Runtime system
│   ├── ffi.rs             # Foreign Function Interface
│   ├── python_interop.rs  # Python interoperability
│   ├── codegen/           # Code generation backends
│   │   ├── mod.rs         # Backend trait and registry
│   │   ├── interpreter.rs # Interpreter backend
│   │   ├── c_transpiler.rs# C code generation
│   │   ├── c_abi.rs       # C ABI support
│   │   ├── llvm.rs        # LLVM backend
│   │   ├── wasm.rs        # WebAssembly backend
│   │   ├── gcc.rs         # GCC integration
│   │   └── native.rs      # Native compilation
│   ├── modules/           # Built-in modules
│   │   ├── mod.rs         # Module registry
│   │   ├── os.rs          # Operating system interface
│   │   ├── sys.rs         # System information
│   │   ├── math.rs        # Mathematical functions
│   │   ├── asyncio.rs     # Async/await support
│   │   ├── json.rs        # JSON processing
│   │   ├── httpx.rs       # HTTP client
│   │   └── ...            # Other modules
│   ├── object_system.rs   # Object-oriented features
│   ├── type_hierarchy.rs  # Type system
│   ├── metaclass.rs       # Metaclass system
│   ├── module_system.rs   # Module loading and management
│   └── bin/               # Binary utilities
├── docs/                  # Documentation
│   ├── README.md          # Project overview
│   ├── getting-started.md # Quick start guide
│   ├── language-reference.md # Language specification
│   ├── api-reference.md   # Built-in functions reference
│   ├── modules-reference.md # Standard library reference
│   ├── compilation-backends.md # Backend documentation
│   ├── ffi-guide.md       # FFI usage guide
│   ├── best-practices.md  # Coding best practices
│   ├── troubleshooting.md # Common issues and solutions
│   └── developer-guide.md # This document
├── examples/              # Example programs
│   └── test_packages/     # Test packages for module system
├── benchmarks/            # Performance benchmarks
│   ├── tauraro/          # TauraroLang benchmark programs
│   ├── python/           # Python equivalent programs
│   └── results/          # Benchmark results
├── tests/                 # Test suite (currently empty)
├── tauraro_packages/      # External packages
├── Cargo.toml            # Rust project configuration
├── .gitignore            # Git ignore rules
└── LICENSE               # License information
```

### Core Components

#### Frontend (Parsing and Analysis)
- **Lexer** (`src/lexer.rs`): Tokenizes source code using the `logos` crate
- **Parser** (`src/parser.rs`): Generates Abstract Syntax Tree (AST)
- **AST** (`src/ast.rs`): Defines syntax tree node types
- **Semantic Analyzer** (`src/semantic.rs`): Type checking and symbol resolution

#### Middle-end (IR and Optimization)
- **IR Generator** (`src/ir.rs`): Creates platform-independent intermediate representation
- **Optimization**: Basic optimizations (planned for future releases)

#### Backend (Code Generation)
- **Interpreter** (`src/codegen/interpreter.rs`): Direct AST execution
- **C Transpiler** (`src/codegen/c_transpiler.rs`): Generates C code
- **LLVM Backend** (`src/codegen/llvm.rs`): LLVM IR generation
- **WebAssembly** (`src/codegen/wasm.rs`): WASM bytecode generation

#### Runtime System
- **Virtual Machine** (`src/vm.rs`): Execution environment
- **Value System** (`src/value.rs`): Dynamic typing and value representation
- **Built-ins** (`src/builtins.rs`): Core functions and operations
- **Object System** (`src/object_system.rs`): OOP features
- **Module System** (`src/module_system.rs`): Import/export mechanism

## Development Environment Setup

### Prerequisites

1. **Rust Toolchain** (1.70 or later)
   ```bash
   curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
   rustup update
   ```

2. **C Compiler** (for C backend)
   - **Linux**: `sudo apt install gcc` or `sudo yum install gcc`
   - **macOS**: `xcode-select --install`
   - **Windows**: Visual Studio Build Tools or MinGW-w64

3. **LLVM** (optional, for LLVM backend)
   - **Linux**: `sudo apt install llvm-dev libclang-dev`
   - **macOS**: `brew install llvm`
   - **Windows**: Download from LLVM releases

4. **Python** (optional, for Python interop)
   - Python 3.8 or later with development headers

### Clone and Build

```bash
# Clone the repository
git clone https://github.com/taurarolang/tauraro.git
cd tauraro

# Build with default features
cargo build

# Build with all features
cargo build --features "llvm,wasm,python-interop,ffi"

# Build release version
cargo build --release
```

### IDE Setup

#### Visual Studio Code
Recommended extensions:
- **rust-analyzer**: Rust language support
- **CodeLLDB**: Debugging support
- **Better TOML**: Cargo.toml syntax highlighting

#### IntelliJ IDEA / CLion
- Install the Rust plugin
- Configure Rust toolchain in settings

### Environment Variables

```bash
# Optional: Set LLVM paths if not in system PATH
export LLVM_SYS_170_PREFIX=/usr/lib/llvm-17

# Optional: Python development headers
export PYTHON_SYS_EXECUTABLE=/usr/bin/python3
```

## Build System

### Cargo Configuration

The project uses Cargo with feature flags for optional components:

```toml
[features]
default = ["interpreter", "async", "clang", "http"]
interpreter = []                    # VM-based execution
llvm = ["dep:llvm-sys", "dep:inkwell"]  # LLVM backend
wasm = ["dep:wasmer", "dep:object"]     # WebAssembly backend
c-backend = []                      # C code generation
ffi = ["dep:libffi", "dep:libloading"] # Foreign Function Interface
python-interop = ["dep:pyo3"]       # Python interoperability
async = ["dep:tokio", "dep:futures"] # Async/await support
http = ["dep:hyper", "dep:reqwest"]  # HTTP and WebSocket support
```

### Build Commands

```bash
# Development build
cargo build

# Release build
cargo build --release

# Build with specific features
cargo build --features llvm
cargo build --features "llvm,wasm,python-interop"

# Build documentation
cargo doc --open

# Run tests
cargo test

# Check code without building
cargo check

# Format code
cargo fmt

# Lint code
cargo clippy
```

### Cross-Compilation

```bash
# Add target
rustup target add x86_64-unknown-linux-musl

# Cross-compile
cargo build --target x86_64-unknown-linux-musl --release
```

## Development Workflow

### 1. Setting Up a Development Branch

```bash
# Create feature branch
git checkout -b feature/new-feature

# Make changes
# ... edit files ...

# Commit changes
git add .
git commit -m "Add new feature: description"

# Push branch
git push origin feature/new-feature
```

### 2. Development Cycle

1. **Write Code**: Implement new features or fix bugs
2. **Test Locally**: Run tests and manual testing
3. **Format and Lint**: Ensure code quality
4. **Document**: Update documentation if needed
5. **Commit**: Make atomic commits with clear messages

### 3. Code Quality Checks

```bash
# Format code
cargo fmt

# Check for common mistakes
cargo clippy

# Run tests
cargo test

# Check documentation
cargo doc --no-deps

# Benchmark (if applicable)
cargo bench
```

### 4. Testing Changes

```bash
# Test interpreter
cargo run -- run examples/hello.tr

# Test C backend
cargo run -- compile examples/hello.tr --backend c --output hello

# Test LLVM backend (if enabled)
cargo run -- compile examples/hello.tr --backend llvm --output hello

# Test REPL
cargo run -- repl
```

## Testing

### Test Structure

Currently, the project uses:
- **Unit Tests**: Embedded in source files with `#[cfg(test)]`
- **Integration Tests**: In the `tests/` directory (to be expanded)
- **Example Programs**: In the `examples/` directory
- **Benchmarks**: In the `benchmarks/` directory

### Running Tests

```bash
# Run all tests
cargo test

# Run specific test
cargo test test_name

# Run tests with output
cargo test -- --nocapture

# Run tests for specific feature
cargo test --features llvm
```

### Writing Tests

#### Unit Tests
```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_lexer_basic() {
        let source = "let x = 42";
        let tokens = lexer::Lexer::new(source).collect::<Result<Vec<_>, _>>().unwrap();
        assert_eq!(tokens.len(), 4);
    }
}
```

#### Integration Tests
```rust
// tests/integration_test.rs
use tauraro::vm::VM;

#[test]
fn test_basic_execution() {
    let mut vm = VM::new();
    let result = vm.run_code("print('Hello, World!')");
    assert!(result.is_ok());
}
```

### Benchmarking

```bash
# Run benchmarks
cd benchmarks
python run_benchmarks.py

# Compare with Python
python performance_test.py
```

## Code Style and Standards

### Rust Style Guidelines

Follow the official Rust style guide:

1. **Formatting**: Use `cargo fmt` (rustfmt)
2. **Naming Conventions**:
   - `snake_case` for functions, variables, modules
   - `PascalCase` for types, structs, enums
   - `SCREAMING_SNAKE_CASE` for constants
3. **Documentation**: Use `///` for public APIs
4. **Error Handling**: Use `Result<T, E>` and `?` operator

### Code Organization

1. **Module Structure**: Logical grouping of related functionality
2. **Public APIs**: Minimize public surface area
3. **Error Types**: Use `thiserror` for custom error types
4. **Async Code**: Use `tokio` for async runtime

### Documentation Standards

```rust
/// Parses a TauraroLang source file into an AST.
///
/// # Arguments
/// * `source` - The source code to parse
/// * `filename` - Optional filename for error reporting
///
/// # Returns
/// * `Ok(AST)` - Successfully parsed AST
/// * `Err(ParseError)` - Parse error with location information
///
/// # Examples
/// ```
/// let ast = parse_source("let x = 42", Some("test.tr"))?;
/// ```
pub fn parse_source(source: &str, filename: Option<&str>) -> Result<AST, ParseError> {
    // Implementation
}
```

## Contributing Guidelines

### Before Contributing

1. **Check Issues**: Look for existing issues or create a new one
2. **Discuss Changes**: For major changes, discuss in issues first
3. **Fork Repository**: Create a fork for your changes
4. **Create Branch**: Use descriptive branch names

### Contribution Process

1. **Fork and Clone**
   ```bash
   git clone https://github.com/yourusername/tauraro.git
   cd tauraro
   git remote add upstream https://github.com/taurarolang/tauraro.git
   ```

2. **Create Feature Branch**
   ```bash
   git checkout -b feature/descriptive-name
   ```

3. **Make Changes**
   - Follow code style guidelines
   - Add tests for new functionality
   - Update documentation

4. **Test Changes**
   ```bash
   cargo test
   cargo clippy
   cargo fmt --check
   ```

5. **Commit Changes**
   ```bash
   git add .
   git commit -m "feat: add new feature description"
   ```

6. **Push and Create PR**
   ```bash
   git push origin feature/descriptive-name
   # Create pull request on GitHub
   ```

### Commit Message Format

Use conventional commits:
- `feat:` - New features
- `fix:` - Bug fixes
- `docs:` - Documentation changes
- `style:` - Code style changes
- `refactor:` - Code refactoring
- `test:` - Test additions or changes
- `chore:` - Build system or auxiliary tool changes

### Pull Request Guidelines

1. **Clear Description**: Explain what and why
2. **Link Issues**: Reference related issues
3. **Small Changes**: Keep PRs focused and small
4. **Tests**: Include tests for new functionality
5. **Documentation**: Update docs if needed

## Architecture Overview

### Compilation Pipeline

```
Source Code
    ↓
Lexer (Tokenization)
    ↓
Parser (AST Generation)
    ↓
Semantic Analysis (Type Checking)
    ↓
IR Generation (Platform Independent)
    ↓
Backend Selection
    ↓
┌─────────────┬─────────────┬─────────────┬─────────────┐
│ Interpreter │ C Transpiler│ LLVM Backend│ WASM Backend│
└─────────────┴─────────────┴─────────────┴─────────────┘
    ↓               ↓               ↓               ↓
Direct Execution   C Code      LLVM IR/Binary   WASM Binary
```

### Key Abstractions

#### Value System
```rust
pub enum Value {
    Null,
    Bool(bool),
    Int(i64),
    Float(f64),
    String(String),
    List(Vec<Value>),
    Dict(HashMap<String, Value>),
    Function(Function),
    Object(Object),
    Module(Module),
}
```

#### IR Instructions
```rust
pub enum IRInstruction {
    LoadConst { dest: String, value: IRValue },
    LoadLocal { dest: String, name: String },
    StoreLocal { name: String, value: IRValue },
    Call { dest: String, func: IRValue, args: Vec<IRValue> },
    Return { value: Option<IRValue> },
    // ... more instructions
}
```

### Backend Interface
```rust
pub trait CodeGenerator {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>>;
    fn get_target(&self) -> Target;
    fn supports_optimization(&self) -> bool;
}
```

## Debugging and Profiling

### Debugging Tools

#### GDB/LLDB
```bash
# Build with debug info
cargo build --features debug

# Debug with GDB
gdb target/debug/tauraro
(gdb) run -- run examples/test.tr
```

#### Rust-specific Tools
```bash
# Use rust-gdb wrapper
rust-gdb target/debug/tauraro

# Use LLDB on macOS
rust-lldb target/debug/tauraro
```

### Profiling

#### CPU Profiling
```bash
# Install perf (Linux)
sudo apt install linux-perf

# Profile execution
perf record --call-graph=dwarf cargo run -- run examples/benchmark.tr
perf report
```

#### Memory Profiling
```bash
# Use valgrind
valgrind --tool=memcheck cargo run -- run examples/test.tr

# Use heaptrack
heaptrack cargo run -- run examples/test.tr
```

#### Rust-specific Profiling
```bash
# Install cargo-profiler
cargo install cargo-profiler

# Profile with flamegraph
cargo install flamegraph
cargo flamegraph -- run examples/benchmark.tr
```

### Logging and Tracing

```bash
# Enable debug logging
RUST_LOG=debug cargo run -- run examples/test.tr

# Enable trace logging for specific module
RUST_LOG=tauraro::vm=trace cargo run -- run examples/test.tr
```

## Release Process

### Version Management

1. **Semantic Versioning**: Follow semver (MAJOR.MINOR.PATCH)
2. **Changelog**: Maintain CHANGELOG.md
3. **Git Tags**: Tag releases with version numbers

### Release Steps

1. **Update Version**
   ```bash
   # Update Cargo.toml version
   vim Cargo.toml
   
   # Update documentation
   cargo doc
   ```

2. **Run Full Test Suite**
   ```bash
   cargo test --all-features
   cargo clippy --all-features
   cargo fmt --check
   ```

3. **Create Release**
   ```bash
   git add .
   git commit -m "chore: bump version to v0.2.0"
   git tag v0.2.0
   git push origin main --tags
   ```

4. **Publish to Crates.io** (when ready)
   ```bash
   cargo publish
   ```

### Release Checklist

- [ ] All tests pass
- [ ] Documentation updated
- [ ] Changelog updated
- [ ] Version bumped
- [ ] Git tag created
- [ ] Release notes written

## Troubleshooting

### Common Development Issues

#### Build Failures

**LLVM not found:**
```bash
# Install LLVM development packages
sudo apt install llvm-dev libclang-dev

# Or set LLVM path
export LLVM_SYS_170_PREFIX=/usr/lib/llvm-17
```

**Python interop issues:**
```bash
# Install Python development headers
sudo apt install python3-dev

# Or specify Python executable
export PYTHON_SYS_EXECUTABLE=/usr/bin/python3
```

#### Runtime Issues

**Stack overflow:**
- Check for infinite recursion
- Increase stack size if needed
- Use iterative algorithms where possible

**Memory leaks:**
- Use Valgrind or similar tools
- Check reference counting logic
- Ensure proper cleanup in FFI code

#### Performance Issues

**Slow compilation:**
- Use `cargo check` for syntax checking
- Enable incremental compilation
- Use `sccache` for caching

**Slow execution:**
- Profile with appropriate tools
- Check algorithm complexity
- Consider using release builds for testing

### Getting Help

1. **Documentation**: Check existing docs first
2. **Issues**: Search GitHub issues
3. **Discussions**: Use GitHub discussions for questions
4. **Community**: Join community channels (when available)

### Reporting Bugs

When reporting bugs, include:
1. **Environment**: OS, Rust version, TauraroLang version
2. **Reproduction**: Minimal example that reproduces the issue
3. **Expected vs Actual**: What you expected vs what happened
4. **Logs**: Relevant error messages or logs

### Contributing to Documentation

Documentation improvements are always welcome:
1. Fix typos and grammar
2. Add examples and clarifications
3. Update outdated information
4. Translate documentation (future)

---

This developer guide provides a comprehensive overview of the TauraroLang development process. For specific technical details, refer to the other documentation files in the `docs/` directory.

## Additional Resources

- [Language Reference](language-reference.md) - Complete language specification
- [API Reference](api-reference.md) - Built-in functions and APIs
- [Compilation Backends](compilation-backends.md) - Backend-specific documentation
- [FFI Guide](ffi-guide.md) - Foreign Function Interface usage
- [Best Practices](best-practices.md) - Coding best practices
- [Troubleshooting](troubleshooting.md) - Common issues and solutions

Happy coding! 🚀