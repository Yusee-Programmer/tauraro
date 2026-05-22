# Tauraro Programming Language

A modern systems programming language designed for performance, safety, and expressiveness. **Tauraro compiles to C**, then leverages GCC/Clang for native code generation. The compiler itself is now **self-hosted—written entirely in Tauraro** (initially bootstrapped from Rust).

## Overview

Tauraro is a statically-typed, compiled language that combines the best of modern language design with practical systems programming capabilities:

- **Fast compilation & execution**: Transpiles to optimized C then native code
- **Type safety**: Strong static typing with type inference
- **Ownership semantics**: Automatic memory management with optional manual control
- **Bilingual**: Every keyword has English and Hausa equivalents
- **Multi-paradigm**: Object-oriented, functional, and procedural programming styles
- **System access**: Raw pointers, unsafe blocks, and FFI for systems programming
- **Async/await**: Lightweight concurrency with channels and task spawning

## Quick Start

### Installation

Download or build the `tauraroc` compiler from releases, or [build from source](docs/building.md).

### Your First Program

Create `hello.tr`:
```tauraro
pub def main():
    print("Hello, Tauraro!")
```

Compile and run:
```bash
tauraroc hello.tr --run
```

### Basic Examples

**Variables and Types**:
```tauraro
def main():
    mut x: int = 42
    let name: str = "Tauraro"
    print(f"{name}: {x}")
```

**Functions**:
```tauraro
pub def add(a: int, b: int) -> int:
    return a + b

def main():
    print(add(5, 3))
```

**Classes**:
```tauraro
class Point:
    x: int
    y: int
    
    pub def distance(self) -> float:
        return sqrt(self.x * self.x + self.y * self.y)

def main():
    mut p = Point { x: 3, y: 4 }
    print(p.distance())
```

**Pattern Matching**:
```tauraro
enum Result[T, E]:
    case Ok(T)
    case Err(E)

def main():
    let result: Result[int, str] = Ok(42)
    match result:
        case Ok(value):
            print(f"Success: {value}")
        case Err(err):
            print(f"Error: {err}")
```

## Learning Tauraro

**Getting Started:**
- Read [docs/lang/01_intro.md](docs/lang/01_intro.md) for a language overview
- Check [examples/](examples/) for practical code samples
- Study [docs/lang/](docs/lang/) for detailed feature guides

**Language Features by Category:**

| Category | Examples |
|----------|----------|
| **Basics** | Variables, operators, control flow |
| **Functions** | Parameters, return types, default args |
| **OOP** | Classes, methods, inheritance |
| **Functional** | Pattern matching, closures, higher-order functions |
| **Type System** | Generics, interfaces, enums, error handling |
| **Systems** | Raw pointers, unsafe blocks, FFI, inline asm |
| **Async** | async/await, channels, task spawning |
| **Collections** | Vec, Map, Deque, Queue, Stack, Set |

## Project Structure

```
.
├── README.md                          ← This file
├── examples/                          ← Example programs (start here!)
│   ├── 01_variables.tr                ← Variables, types, type inference
│   ├── 02_operators.tr                ← Arithmetic, logical, comparison
│   ├── 03_control_flow.tr             ← if/elif/else, for, while, break
│   ├── 04_functions.tr                ← Function definitions and calls
│   ├── 05_strings.tr                  ← String literals and f-strings
│   ├── 06_classes.tr                  ← Classes, fields, methods
│   ├── 07_enums.tr                    ← Enums and pattern matching
│   ├── 08_pattern_matching.tr         ← Advanced pattern matching
│   ├── 09_interfaces.tr               ← Interfaces and vtable dispatch
│   ├── 10_error_handling.tr           ← Result types, error propagation
│   ├── 11_closures.tr                 ← Closures and lambda expressions
│   ├── 12_collections.tr              ← Vec, Map, Deque, etc.
│   ├── 13_ownership_pointers.tr       ← Memory management, pointers
│   ├── 14_system_programming.tr       ← Unsafe, raw pointers, FFI
│   ├── 15_dynamic_linking.tr          ← Load libraries at runtime
│   ├── 16_win32_window.tr             ← Platform-specific examples
│   ├── 19_async_spawn.tr              ← Async/await, task spawning
│   ├── 20_shared_unsafe.tr            ← Shared state with unsafe
│   └── [more examples...]
│
├── docs/                              ← Documentation
│   ├── lang/                          ← Language reference
│   │   ├── 01_intro.md
│   │   ├── 02_variables_and_types.md
│   │   ├── 03_operators.md
│   │   ├── 04_control_flow.md
│   │   └── 05_functions.md
│   └── README.md
│
├── std/                               ← Standard Library
│   ├── core/                          ← Core: Vec, Map, string, allocators
│   ├── collections/                   ← Deque, Queue, Stack, Set, Dict
│   ├── async/                         ← Async runtime, channels, tasks
│   ├── io/                            ← Console I/O, file I/O
│   ├── iter/                          ← Iterators, ranges, transformations
│   ├── math/                          ← Math functions, bitwise ops
│   ├── net/                           ← TCP, HTTP, URL parsing
│   ├── string/                        ← String utilities, formatting
│   └── sys/                           ← Environment, processes, time
│
├── runtime/                           ← C Runtime (internal)
│   ├── tauraro_rt.h                   ← Memory allocation, system calls
│   └── tauraro_types.h                ← Generated type definitions
│
├── src/                               ← Compiler Source (written in Tauraro)
│   ├── main.tr                        ← CLI driver
│   ├── lexer.tr                       ← Lexer
│   ├── parser.tr                      ← Parser
│   ├── sema.tr                        ← Type checking & analysis
│   ├── codegen/                       ← Code generators
│   └── [other compiler modules]
```

## How Tauraro Works

Tauraro is a **statically-typed compiled language** with a modern compilation pipeline:

```
your_program.tr
    │
    ├─→ Lexer
    │   Tokenization with indentation tracking
    │
    ├─→ Parser
    │   Abstract Syntax Tree (AST) generation
    │
    ├─→ Semantic Analysis
    │   Type checking, inference, ownership analysis
    │
    ├─→ C Code Generation
    │   Transpiles to optimized C code
    │
    ├─→ C Compilation (GCC/Clang)
    │   Compiles to native machine code
    │
    └─→ Executable
        Ready to run!
```

**Key Design Principles:**

1. **Transpile to C**: Leverage proven C compilers and optimize for all platforms
2. **Type safety**: Strong static typing catches errors at compile time
3. **Zero-cost abstractions**: High-level features compile to efficient code
4. **Predictable performance**: What you see is what you get (no hidden GC)
5. **Explicit is better**: Memory management and unsafe code are opt-in

## Language Features Supported

### Object-Oriented Programming
- **Classes**: Compiled to C structs with methods as static functions
- **Inheritance**: Via method overriding and composition patterns
- **Interfaces**: Compiled to vtable dispatch structures
- **Enums**: Tagged unions with pattern matching

### Functional Features
- **Pattern matching**: Destructuring with `match`/`case` expressions
- **Closures**: Capture by value or reference with automatic free injection
- **Higher-order functions**: First-class function types
- **Iterators**: Protocol-based with lazy evaluation

### Systems Programming
- **Raw pointers**: `Pointer[T]` for low-level memory access
- **Unsafe blocks**: Explicit opt-in for unsafe operations
- **Ownership semantics**: Automatic vs. manual free() management
- **Escape analysis**: Stack vs. heap allocation decisions

### Modern Conveniences
- **F-strings**: `f"Value: {x}"` → snprintf synthesis
- **Generics**: Monomorphized at compile time
- **Error handling**: `Result[T, E]` as plain structs
- **For loops**: `for i in range(start, end)` → optimized C for loops
- **Bilingual keywords**: English + Hausa equally supported

### Async/Concurrency
- **Async/await**: `async fn` with `.await` syntax
- **Channels**: Multi-producer channels for task communication
- **Task spawning**: `spawn()` for lightweight green threads
- **Task groups**: Coordinated cancellation via task groups

## Standard Library

Organized by module:

| Module | Purpose |
|--------|---------|
| `core` | Allocators, `Vec[T]`, `Map[K,V]`, strings |
| `collections` | `Deque`, `Queue`, `Stack`, `Dict`, `Set`, `Tuple` |
| `async` | Channels, task spawning, task groups |
| `io` | Console I/O, file operations, buffering |
| `iter` | Iterator protocols, ranges, transformations |
| `math` | Arithmetic functions, bitwise operations |
| `net` | TCP sockets, HTTP client, URL parsing |
| `string` | String formatting, f-string compilation |
| `sys` | Environment variables, process spawning, timers |

## Bilingual Support (English + Hausa)

Every keyword has both English and Hausa equivalents:

| English | Hausa | Meaning |
|---------|-------|---------|
| `pub` | `dabbare` | public visibility |
| `def` | `aiki` | function definition |
| `class` | `aji` | class definition |
| `enum` | `baida` | enumeration |
| `interface` | `ilimi` | interface definition |
| `extend` | `tsara` | extend/impl block |
| `if` | `idan` | conditional |
| `elif` | `koidan` | else-if |
| `else` | `sai` | else |
| `for` | `ga` | for loop |
| `while` | `yayinda` | while loop |
| `return` | `dawo` | return statement |
| `match` | `duba` | pattern match |
| `case` | `hali` | match arm |
| `break` | `katse` | break from loop |
| `continue` | `ci` | continue loop |
| `true` | `gaskiya` | boolean true |
| `false` | `karya` | boolean false |
| `none` | `babu` | null/none value |
| `and` | `da` | logical AND |
| `or` | `ko` | logical OR |
| `not` | `ba` | logical NOT |
| `in` | `a` | membership/iteration |
| `as` | `kamar` | type cast |
| `mut` | `maye` | mutable binding |
| `unsafe` | `rashin tsaro` | unsafe block |
| `print` | `buga` | print to stdout |
| `async` | `asynk` | async function |
| `await` | `jira` | await expression |

Example bilingual code:
```tauraro
aiki greet(name: str):
    idan name == "":
        buga("Hello, World!")
    sai:
        buga(f"Hello, {name}!")

def hello(n: int):
    ga i a range(0, n):
        print(i)
```



## Compilation Options

The `tauraroc` compiler provides flexible compilation modes:

```bash
# Compile and run immediately
tauraroc program.tr --run

# Compile to executable (default)
tauraroc program.tr -o program

# Compile with optimization level
tauraroc program.tr -O3 -o program

# Type-check only (don't generate code)
tauraroc program.tr --check

# Emit C code to build/ (for inspection)
tauraroc program.tr --emit c

# Show AST representation
tauraroc program.tr --emit ast

# Link external C libraries
tauraroc program.tr --link /path/to/lib.a -luser32 -o program

# Verbose output
tauraroc program.tr --verbose
```

**Optimization levels:**
- `-O0` — No optimization (fastest compile)
- `-O1`, `-O2`, `-O3` — Increasing optimization (slower compile, faster runtime)
- `-Os` — Optimize for binary size

## Installation & Building

### Using Pre-built Binaries

Download `tauraroc` from [releases](https://github.com/your/repo/releases).

### Building from Source

**Requirements:**
- GCC, Clang, or MSVC (for C compilation)
- A working Tauraro compiler (bootstrap)

The Tauraro compiler is written in Tauraro. It was initially written in Rust to bootstrap the language, but now the compiler is self-hosted and can compile itself.

To get started, use a pre-built binary or refer to [building documentation](docs/building.md).

## Community & Support

- **Issues**: Report bugs or suggest features on [GitHub Issues](https://github.com/your/repo/issues)
- **Discussions**: Join our [community discussions](https://github.com/your/repo/discussions)
- **Documentation**: See [docs/](docs/) for comprehensive guides

## Status & Roadmap

**Current Status: Alpha** — Tauraro is under active development. Core language features are stable and production-ready for systems programming tasks.

**Completed:**
- ✅ Core language features (functions, classes, enums, interfaces)
- ✅ Pattern matching & destructuring
- ✅ Generics with monomorphization
- ✅ Ownership & memory safety analysis
- ✅ Async/await with lightweight concurrency
- ✅ Comprehensive standard library
- ✅ Bilingual support (English + Hausa)
- ✅ Self-hosted compiler

**Planned (Phase 2):**
- 🔄 LLVM backend for additional optimization
- ⏳ Package manager & dependency resolution
- ⏳ Cross-platform support improvements
- ⏳ Language server protocol (LSP) support
- ⏳ Interactive REPL

## License

[License information here]

## Contributing

Contributions welcome! Please see CONTRIBUTING.md for guidelines.

## Contact

For questions or issues, open an issue on GitHub or contact the maintainers.
