# Tauraro Self-Hosted Compiler

A complete, production-ready **Tauraro compiler written entirely in Tauraro**. This is the **Phase 1.5** milestone: self-hosting the compiler so it can compile itself.

## Overview

The Tauraro compiler is a modern systems programming language compiler that transpiles to C, then leverages GCC/Clang for native code generation. It features:

- **Self-hosted**: Written entirely in Tauraro, the compiler can compile itself
- **Multi-backend**: Primary C transpiler with LLVM IR stub for Phase 2
- **Modular compilation**: Per-module C output with separate type headers
- **Bilingual**: Full support for English and Hausa keywords
- **Production-ready C codegen**: Optimized, brutally efficient C output
- **Complete pipeline**: Lexer → Parser → Semantic Analysis → Code Generation

## Quick Start

### Bootstrap from Rust (initial compilation)

```bash
# Clone and build the Rust bootstrap compiler
cargo build --release

# Use it to compile the self-hosted Tauraro compiler
cargo run --release -- src/main.tr --emit c -o bootstrap.c

# Compile the generated bootstrap C code
gcc -O3 bootstrap.c runtime/tauraro_rt.h -o tauraroc

# Now you have a self-hosted compiler!
./tauraroc examples/01_variables.tr --run
```

### Using the self-hosted compiler

```bash
# Compile and run
./tauraroc examples/hello.tr --run

# Compile to modular C output
./tauraroc src/main.tr --emit c

# Type-check only
./tauraroc examples/01_variables.tr --check

# Compile with optimization
./tauraroc program.tr -O3 -o program

# Link external C libraries
./tauraroc program.tr --link /path/to/lib.a -luser32 -o program.exe
```

## CLI Reference

```
Usage: tauraroc <file.tr> [options]

Options:
  --emit c           Emit modular C code to build/ directory
  --emit ast         Print AST representation and stop
  --emit mir         Print MIR basic blocks and stop
  --run              Compile and immediately execute the binary
  --check            Run semantic analysis only (no codegen)
  --verbose          Show all pipeline phases with timing
  --backend llvm     Use LLVM IR backend instead of C (stub for Phase 2)
  
  -o <path>          Output executable name (default: source filename)
  -O0/-O1/-O2/-O3    Optimization level (default: -O2)
  -Os                Optimize for size
  
  --link <path>      Link a file by path (.c .o .a .dll .lib .so)
  -l<name>           Link a library by name (e.g., -luser32, -lgdi32)
  -l <name>          Same as -l<name> with a space separator
```

## Project Structure

```
.
├── README.md                 ← This file
├── docs/                     ← Documentation
│   └── lang/                 ← Language feature guides
├── examples/                 ← Example programs
│   ├── 01_variables.tr       ← Variable declarations and types
│   ├── 02_operators.tr       ← Arithmetic and logical operators
│   ├── 03_control_flow.tr    ← If/elif/else, for, while
│   ├── 04_functions.tr       ← Function definitions and calls
│   ├── 05_strings.tr         ← String literals and f-strings
│   ├── 06_classes.tr         ← Class definitions with methods
│   ├── 07_enums.tr           ← Enum types with pattern matching
│   ├── 08_pattern_matching.tr ← Match expressions and destructuring
│   ├── 09_interfaces.tr      ← Interface definitions and vtables
│   ├── 10_error_handling.tr  ← Result types and error handling
│   ├── 11_closures.tr        ← Closure captures and lambdas
│   ├── 12_collections.tr     ← Vec, Map, deque, etc.
│   ├── 13_ownership_pointers.tr ← Ownership, borrow, move semantics
│   ├── 14_system_programming.tr ← Unsafe blocks, raw pointers
│   ├── 15_dynamic_linking.tr ← Loading .dll/.so at runtime
│   ├── 16_win32_window.tr    ← Win32 API window creation
│   ├── 19_async_spawn.tr     ← Async/await with spawned tasks
│   ├── 20_shared_unsafe.tr   ← Shared mutable state with unsafe
│   └── [more examples...]
│
├── runtime/                  ← C runtime headers
│   ├── tauraro_rt.h          ← Memory allocation, system calls
│   └── tauraro_types.h       ← Generated type definitions
│
├── src/                      ← Compiler source code
│   ├── main.tr               ← CLI driver, module resolver, linker
│   ├── token.tr              ← Token enum and keyword tables (bilingual)
│   ├── lexer.tr              ← Hand-written FSM lexer with indent/dedent
│   ├── parser.tr             ← Recursive descent parser (~1400 lines)
│   ├── ast.tr                ← AST type definitions (Program, Decl, Stmt, Expr)
│   ├── sema.tr               ← Semantic analysis & type checking
│   ├── hir.tr                ← HIR types and AST→HIR lowering
│   ├── resolver.tr           ← Module resolver (unity-build)
│   └── codegen/              ← Code generators
│       ├── c.tr              ← C transpiler (PRIMARY, production-ready)
│       ├── llvm.tr           ← LLVM IR backend (stub)
│       └── mod.tr            ← Module declarations
│
└── std/                      ← Standard library modules
    ├── core/                 ← Core allocators, Vec, Map, string
    ├── collections/          ← Deque, Queue, Stack, Dict, Set, Tuple
    ├── async/                ← Async runtime, channels, task spawning
    ├── io/                   ← Console I/O, file I/O
    ├── iter/                 ← Iterator protocols, range
    ├── math/                 ← Math functions, bitwise ops
    ├── net/                  ← TCP, HTTP, URL parsing
    ├── string/               ← String formatting, f-strings
    └── sys/                  ← Environment, process, time
```

## Compiler Pipeline

```
source.tr
    │
    ├─→ [1] Module Resolver (resolver.tr)
    │   Recursively loads and merges all imported modules
    │   Builds unified program tree
    │
    ├─→ [2] Lexer (lexer.tr)
    │   Hand-written finite state machine
    │   Tracks indentation levels → INDENT/DEDENT tokens
    │   Returns flat token stream
    │
    ├─→ [3] Parser (parser.tr)
    │   Recursive descent parser
    │   Builds Abstract Syntax Tree (AST)
    │
    ├─→ [4] Semantic Analysis (sema.tr)
    │   Type checking & inference
    │   Ownership analysis (Own/Borrow/Move/Shared/Stack)
    │   Escape analysis
    │   Automatic free() injection
    │   Produces annotated Higher Intermediate Representation (HIR)
    │
    ├─→ [5] Code Generation
    │   │
    │   ├─ [C Backend] codegen/c.tr (PRIMARY)
    │   │   Monomorphizes generics
    │   │   Generates modular C output:
    │   │   ├─ tauraro_types.h  (shared type defs + prototypes)
    │   │   ├─ tauraro_rt.h     (runtime headers)
    │   │   ├─ main.c           (program entry point)
    │   │   └─ module_*.c       (per-module implementation)
    │   │
    │   └─ [LLVM Backend] codegen/llvm.tr (stub for Phase 2)
    │       Generates LLVM IR
    │
    ├─→ [6] C Compilation
    │   Detected compiler: gcc, clang, or cc
    │   Compiles all .c files with -O{0,1,2,3,s}
    │   Links external libraries via -l<name> or --link <path>
    │   Injects runtime math library (-lm)
    │
    └─→ executable
```

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

## C Code Generation Examples

### Classes

**Tauraro:**
```tauraro
class Point:
    x: int
    y: int
    
    pub def distance(self) -> float:
        return sqrt(self.x * self.x + self.y * self.y)
```

**Generated C:**
```c
typedef struct {
    long long x;
    long long y;
} Point;

double Point_distance(Point* self) {
    return sqrt((double)self->x * self->x + self->y * self->y);
}
```

### Enums

**Tauraro:**
```tauraro
enum Result[T, E]:
    case Ok(T)
    case Err(E)
```

**Generated C:**
```c
typedef struct {
    int tag;  // 0 = Ok, 1 = Err
    union {
        T ok_value;
        E err_value;
    } data;
} Result;
```

### F-strings

**Tauraro:**
```tauraro
def format_point(x: int, y: int):
    return f"Point({x}, {y})"
```

**Generated C:**
```c
char* format_point(long long x, long long y) {
    char* buf = malloc(64);
    snprintf(buf, 64, "Point(%lld, %lld)", x, y);
    return buf;
}
```

## Compilation Modes

### 1. Full Compilation (Default)
Produces optimized executable via C:
```bash
tauraroc program.tr -O3 -o program
```

### 2. C Code Emission
Generates modular C source to `build/`:
```bash
tauraroc program.tr --emit c
# Output: build/tauraro_types.h, build/main.c, build/module_*.c
```

### 3. AST Emission
Prints AST and stops (no code generation):
```bash
tauraroc program.tr --emit ast
```

### 4. MIR Emission
Prints intermediate representation stats:
```bash
tauraroc program.tr --emit mir
```

### 5. Type-Check Only
Runs semantic analysis, reports errors:
```bash
tauraroc program.tr --check
```

## Building from Source

### Requirements
- GCC, Clang, or MSVC (for C compilation)
- Rust toolchain (for bootstrap compiler only)
- CMake 3.20+ (optional, for LLVM Phase 2)

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/user/tauraro.git
cd tauraro

# 2. Build the Rust bootstrap compiler
cargo build --release

# 3. Compile the self-hosted compiler
cargo run --release -- src/main.tr -O3 -o bootstrap.c

# 4. Compile bootstrap C to native executable
gcc -O3 bootstrap.c runtime/tauraro_rt.h -o tauraroc

# 5. Verify self-hosting
./tauraroc src/main.tr -O3 -o tauraroc2
./tauraroc2 examples/01_variables.tr --run
```

## Examples

See `examples/` directory:

- **01_variables.tr** - Variable declarations, type inference
- **04_functions.tr** - Function definitions, parameters, return types
- **06_classes.tr** - Class definitions with methods
- **07_enums.tr** - Enums with pattern matching
- **09_interfaces.tr** - Interface vtable dispatch
- **12_collections.tr** - Vec, Map, collections usage
- **13_ownership_pointers.tr** - Ownership, borrowing, pointers
- **19_async_spawn.tr** - Async functions, task spawning

## Development Status

- ✅ **Phase 1.5**: Self-hosted compiler (complete)
  - ✅ Lexer with indent/dedent
  - ✅ Parser (recursive descent, ~1400 lines)
  - ✅ Semantic analysis & ownership inference
  - ✅ C code generation (production-ready)
  - ✅ Module resolution (unity-build)
  - ✅ Bilingual support (English + Hausa)

- 🔄 **Phase 2**: LLVM backend & optimizations
  - ⏳ LLVM IR generation
  - ⏳ Advanced optimizations
  - ⏳ Cross-platform support

## License

[License information here]

## Contributing

Contributions welcome! Please see CONTRIBUTING.md for guidelines.

## Contact

For questions or issues, open an issue on GitHub or contact the maintainers.
