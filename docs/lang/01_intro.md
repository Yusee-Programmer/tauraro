# 01 — Introduction to Tauraro

---

## What Tauraro Is

Tauraro is a **compiled, statically-typed, systems programming language** with Python-inspired syntax. It transpiles to C, then uses GCC or Clang to produce a native binary. The result: Python-readable code that runs at C speed with no garbage collector, no virtual machine, and no runtime overhead.

> **Core promise:** Python syntax. C performance. The compiler handles the hard parts.

The name "Tauraro" (تارارو) is Hausa for "star." The language is bilingual: every keyword has both an English and a Hausa spelling, and either works anywhere in any program.

---

## Why Tauraro Exists

Systems languages force a trade-off:

| Language | Speed | Memory Safety | Ergonomics |
|----------|-------|---------------|------------|
| C | ✓ | ✗ manual | ✗ verbose |
| C++ | ✓ | ✗ manual | ✗ complex |
| Rust | ✓ | ✓ borrow checker | ✗ steep learning curve |
| Go | ~ | ✓ GC | ✓ easy |
| Python | ✗ GC | ✓ GC | ✓ easy |
| **Tauraro** | **✓** | **✓ inferred** | **✓ Python-like** |

Tauraro's answer to the trilemma: **move the burden to the compiler**. You write clean, readable code. The compiler analyzes ownership, injects `free()` calls at scope exit, enforces type safety, and emits optimized C. You don't write lifetime annotations. You don't fight a borrow checker. You don't write `free`.

---

## Core Design Principles

### 1. The Compiler Carries the Complexity
Every heap-allocated variable is tracked. Every `free()` is injected automatically at scope exit. Every type conversion is explicit. The user writes intent; the compiler handles mechanics.

### 2. Zero-Cost Abstractions
- Classes → C structs (no vtables unless you use interfaces)
- Methods → C functions with a `self*` parameter
- Interfaces → vtable structs (one pointer dereference per virtual call)
- Enums → tagged unions (no heap allocation)
- Generics → monomorphized at compile time (no boxing, no erasure)

There is no abstraction in Tauraro that costs more than its direct C equivalent would.

### 3. Safe by Default, Unsafe by Choice
All code is memory-safe unless you explicitly write `unsafe:`. Inside `unsafe:` you get raw pointers, pointer arithmetic, and inline assembly. Outside it, the compiler prevents dangling pointers, double-frees, and use-after-move. The unsafe surface is auditable — one keyword, clearly marked.

### 4. Compiled, Not Interpreted
There is no runtime, no REPL, no interpreter. Source goes through:
```
.tr → Lexer → Parser → AST → Sema → HIR → C → GCC/Clang → native binary
```
The binary runs standalone. No Tauraro runtime needs to be installed on the target machine.

### 5. Bilingual (English + Hausa)
Every keyword has a Hausa equivalent. Both are accepted anywhere. A program can mix them or use one exclusively. This is not cosmetic — it makes the language accessible to Hausa-speaking programmers who may think in Hausa more naturally.

---

## The Compiler Pipeline

```
source.tr
    │
    ├─ Lexer          (src/lexer.tr)
    │   Tokenizes the source into tokens with indentation tracking.
    │   Handles both English and Hausa keywords.
    │   Produces: tokens with Indent/Dedent tokens for blocks.
    │
    ├─ Parser          (src/parser.tr)
    │   Recursive descent parser. Converts tokens to an AST.
    │   Handles: classes, enums, interfaces, generics, decorators,
    │            async, match, try/except, GPU blocks, unsafe blocks.
    │   Produces: Program (list of declarations and statements)
    │
    ├─ Semantic Analysis  (src/sema.tr)
    │   Type checking, ownership inference, scope management.
    │   Injects free calls for every Own variable at scope exit.
    │   Checks: type rules (T-1 through T-4), memory rules (M-1 through M-7),
    │            function rules (F-1 through F-3), name rules (N-1).
    │   Produces: HIR (high-level intermediate representation)
    │
    ├─ C Code Generation  (src/codegen/c.tr)
    │   Walks the HIR and emits one .c file per module into build/:
    │     build/tauraro_rt.h          — runtime header (copied from runtime/)
    │     build/tauraro_types.h       — shared type definitions + all prototypes
    │     build/include/<path>.c      — one file per stdlib/core module
    │     build/module_<name>.c       — one file per user/third-party module
    │     build/main.c                — program entry point
    │   Classes → structs + ClassName_method() functions
    │   Interfaces → vtable structs + wrapper functions
    │   Enums → tagged unions
    │   Generics → monomorphized per type argument
    │
    └─ Compilation      (GCC or Clang, auto-detected)
        Invokes the C compiler once with every .c file in build/:
          gcc -O2 build/main.c build/module_foo.c build/include/std/io.c …
        The compiler links them directly (no separate linker invocation).
        With -o:   temporary .c files are deleted; only the exe survives.
        With --emit c: .c files are kept; no compilation happens.
        Produces: native executable (.exe on Windows, ELF/Mach-O elsewhere)
```

**Module resolution:** The compiler scans imports recursively from the entry file, loading each referenced `.tr` file or `mod.tr` directory. Each module produces its own `.c` file; the C compiler then links all of them in one pass, allowing GCC/Clang to inline across module boundaries at the object level.

---

## Getting Started

### Installation

Requirements:
- GCC or Clang (for compiling generated C)

Download the pre-built `tauraroc` binary (`tauraroc.exe` on Windows) from the GitHub releases page and place it anywhere on your `PATH`.

```bash
# Verify the installation:
tauraroc --version
```

### Your First Program

Create `hello.tr`:
```python
def main():
    print("Hello, Tauraro!")
```

Run it:
```bash
tauraroc --run hello.tr
```

Output:
```
Hello, Tauraro!
```

### A More Complete Example

```python
class Point:
    pub x: int
    pub y: int

extend Point:
    pub def init(x: int, y: int) -> Point:
        mut p = Point()
        p.x = x
        p.y = y
        return p

    pub def describe(self) -> void:
        print(f"Point({self.x}, {self.y})")

    pub def distance_sq(self) -> int:
        return self.x * self.x + self.y * self.y

def main():
    mut origin = Point.init(0, 0)
    mut p = Point.init(3, 4)
    origin.describe()
    p.describe()
    print(f"distance squared = {p.distance_sq()}")
```

Output:
```
Point(0, 0)
Point(3, 4)
distance squared = 25
```

---

## CLI Reference

```bash
# Print version
tauraroc --version

# Compile and run immediately
tauraroc --run program.tr

# Compile to an executable
tauraroc -o program.exe program.tr

# Compile and run with optimization level 3
tauraroc -O3 --run program.tr

# Write per-module C files to build/ without compiling (inspect what the compiler produces)
tauraroc --emit c program.tr

# Print AST and stop (inspect the parse tree)
tauraroc --emit ast program.tr

# Print MIR basic blocks
tauraroc --emit mir program.tr

# Run semantic analysis only — no code generation
tauraroc --check program.tr

# Show all pipeline phases (verbose output)
tauraroc --verbose program.tr

# Use the LLVM backend (experimental, not production-ready)
tauraroc --backend llvm program.tr
```

### CLI Flag Reference

| Flag | Description |
|------|-------------|
| `--version` | Print version and exit |
| `--run` | Compile and execute immediately |
| `-o <path>` | Set output executable path |
| `--emit c` | Write per-module `.c` files to `build/` (no compilation) |
| `--emit ast` | Print the AST and stop |
| `--emit mir` | Print MIR basic blocks and stop |
| `--check` | Semantic analysis only, no code generation |
| `--backend llvm` | Use LLVM IR backend (experimental) |
| `-O0` | No optimization |
| `-O1` | Basic optimization |
| `-O2` | Standard optimization (default) |
| `-O3` | Aggressive optimization (enables AVX2 on x86-64) |
| `--verbose` | Show all pipeline phases |
| `--static` | Link the output binary statically (no shared libs) |
| `--target <triple>` | Cross-compile for a different target (see below) |
| `--sysroot <path>` | Override the C compiler sysroot for cross-compilation |

### Environment Variables

| Variable | Description |
|----------|-------------|
| `TAURARO_PATH` | Extra module search paths, colon-separated on POSIX or semicolon-separated on Windows. Appended to the resolver's path list after all built-in paths. Equivalent to Python's `PYTHONPATH`. |

```bash
# Linux / macOS — add two extra library directories
export TAURARO_PATH=/opt/mylibs:/home/user/pkgs
tauraroc --run myapp.tr

# Windows (PowerShell)
$env:TAURARO_PATH = "C:\mylibs;C:\Users\user\pkgs"
tauraroc --run myapp.tr
```

### Cross-Compilation (`--target`)

`tauraroc` can cross-compile Tauraro programs to any target that the host C compiler supports.
Pass a shorthand alias or a raw LLVM triple:

| Shorthand | Triple | Notes |
|-----------|--------|-------|
| `android-arm64` | `aarch64-linux-android34` | Android 14+, ARM64 |
| `android-arm32` | `armv7a-linux-androideabi34` | Android 14+, 32-bit ARM |
| `android-x86` | `i686-linux-android34` | Android x86 emulator |
| `android-x64` | `x86_64-linux-android34` | Android x86-64 |
| `ios-arm64` | `aarch64-apple-ios` | iOS device |
| `ios-sim-x64` | `x86_64-apple-ios-simulator` | iOS Simulator, Intel |
| `ios-sim-arm64` | `aarch64-apple-ios-simulator` | iOS Simulator, Apple Silicon |
| `embedded-arm` | `arm-none-eabi` | Bare-metal ARM (adds `-nostdlib`) |
| `embedded-arm64` | `aarch64-none-elf` | Bare-metal AArch64 |
| `embedded-riscv` | `riscv32-unknown-elf` | Bare-metal RISC-V 32 |
| `wasm` | `wasm32-unknown-wasi` | WebAssembly (WASI) |
| `wasm-bare` | `wasm32-unknown-unknown` | Bare WebAssembly |
| `linux-arm64` | `aarch64-unknown-linux-gnu` | Linux AArch64 (glibc) |
| `linux-arm32` | `armv7-unknown-linux-gnueabihf` | Linux ARMv7 (glibc) |
| `linux-musl-arm64` | `aarch64-unknown-linux-musl` | Linux AArch64 (musl) |
| `windows-x64` | `x86_64-pc-windows-gnu` | Windows 64-bit (MinGW) |
| `windows-arm64` | `aarch64-pc-windows-gnu` | Windows ARM64 (MinGW) |
| `macos-arm64` | `aarch64-apple-darwin` | macOS Apple Silicon |
| `macos-x64` | `x86_64-apple-darwin` | macOS Intel |

For Android targets, `tauraroc` automatically searches for the Android NDK clang wrapper using
the `ANDROID_NDK_ROOT`, `ANDROID_NDK_HOME`, and `NDK_HOME` environment variables.

```bash
# Build for Android ARM64 (requires Android NDK in ANDROID_NDK_ROOT)
tauraroc --target android-arm64 --static -o hello-android hello.tr

# Build for bare-metal ARM embedded (no libc)
tauraroc --target embedded-arm -o firmware.elf firmware.tr

# Cross-compile with a custom sysroot
tauraroc --target linux-arm64 --sysroot /opt/aarch64-sysroot -o app app.tr

# Static Android binary (runs on Termux without modifications)
tauraroc --target android-arm64 --static --run hello.tr
```

---

## The Self-Hosted Compiler

Tauraro's compiler is **self-hosted** — written in Tauraro itself (`src/main.tr`). The distributed `tauraroc` binary is produced by compiling the compiler source with itself. This means the compiler is its own reference implementation: any feature that works in example programs also works in the compiler source.

---

Next: [Variables & Types →](02_variables_and_types.md)
