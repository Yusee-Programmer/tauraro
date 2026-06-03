<div align="center">
  <img src="assets/tauraro-img.jpg" alt="Tauraro Logo" width="180" style="border-radius: 16px;"/>

  <h1>Tauraro</h1>
  <p><strong>Compiled · Statically Typed · Python Syntax · C Performance · Bilingual</strong></p>

  <p>
    <img src="https://img.shields.io/badge/license-MIT%20%2F%20Apache%202.0-blue?style=flat-square" alt="License"/>
    <img src="https://img.shields.io/badge/version-v0.0.3-brightgreen?style=flat-square" alt="Version"/>
    <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=flat-square" alt="Platform"/>
    <img src="https://img.shields.io/badge/bilingual-English%20%2B%20Hausa-orange?style=flat-square" alt="Bilingual"/>
    <img src="https://img.shields.io/badge/backend-GCC%20%2F%20Clang-red?style=flat-square" alt="Backend"/>
    <img src="https://img.shields.io/badge/self--hosted-yes-purple?style=flat-square" alt="Self-hosted"/>
  </p>

  <p>
    <a href="docs/lang/README.md"><strong>Documentation</strong></a>
    &nbsp;·&nbsp;
    <a href="examples/"><strong>Examples</strong></a>
    &nbsp;·&nbsp;
    <a href="https://github.com/tauraro/tauraro/releases"><strong>Releases</strong></a>
    &nbsp;·&nbsp;
    <a href="https://github.com/tauraro/tauraro/issues"><strong>Issues</strong></a>
  </p>
</div>

---

## What Is Tauraro?

Tauraro is a compiled, statically-typed language with Python-style indentation syntax. It compiles to C and then to native machine code via GCC or Clang — giving you Python's readability with performance close to hand-written C.

It is also the **first programming language with full bilingual keyword support** — every keyword has both an English and a Hausa equivalent. Programs can be written in either language, or mixed freely.

```python
# English
def greet(name: str) -> str:
    return f"Hello, {name}!"

# Hausa
aiki gaisawa(suna: str) -> str:
    dawo f"Sannu, {suna}!"

def main():
    print(greet("world"))
    buga(gaisawa("duniya"))
```

---

## Installation

Download the latest binary from the [Releases](https://github.com/tauraro/tauraro/releases) page:

<div align="center">

| Platform | File |
|----------|------|
| Windows (x64) | `tauraroc-windows-x64.zip` |
| Linux (x64) | `tauraroc-linux-x64.tar.gz` |
| macOS (x64/arm64) | `tauraroc-macos.tar.gz` |

</div>

Extract and place `tauraroc` (or `tauraroc.exe` on Windows) somewhere on your `PATH`.

**Requirement:** GCC or Clang must be installed. Tauraro compiles to C and uses the system C compiler to produce the final binary.

Verify your installation:

```sh
tauraroc --version
# tauraroc v0.0.3
```

---

## Quick Start

**hello.tr**
```python
def main():
    print("Sannu duniya!")   # Hello, world!
```

```sh
tauraroc --run hello.tr
```

---

## Language Features

<div align="center">

| Feature | Description |
|---------|-------------|
| **Classes** | Method dispatch, inheritance (`extends`), interfaces, operator overloading |
| **Enums** | Tagged unions with pattern matching |
| **Generics** | Monomorphized at compile time — no boxing |
| **F-strings** | `f"result = {value}"` — zero overhead |
| **Ownership** | Automatic memory management, no GC |
| **Error handling** | `Result[T,E]`, `throws`, `?` operator |
| **Concurrency** | `spawn`, `task_group:`, `await_all`, `Thread.spawn`, `Chan[T]`, `Mutex[T]`, `Atomic[T]` |
| **Data race safety** | `Sendable` interface enforced at compile time on all spawn/thread boundaries |
| **Unsafe** | `unsafe:`, `Pointer[T]`, inline `asm()` |
| **GPU** | `gpu:` blocks → OpenMP parallel loops |
| **FFI** | `extern "C"` for calling C libraries |
| **Closures** | First-class anonymous functions with capture |
| **Bilingual** | English + Hausa keywords, mix freely |

</div>

---

## CLI Reference

```
tauraroc <file.tr> [options]

  --version         Print compiler version and exit
  --run             Compile and immediately execute
  --check           Semantic analysis only, no output
  --emit c          Print generated C code
  --emit ast        Print AST and stop
  --verbose         Show all pipeline phases
  -o <path>         Output executable path
  -O0/-O1/-O2/-O3  Optimization level (default: -O2)
  -Os               Optimize for size
  -fopenmp          Enable OpenMP for gpu: blocks
  -I <dir>          Add module search path
```

---

## Bilingual Keywords

Every keyword has an English and Hausa equivalent:

<div align="center">

| English | Hausa | Meaning |
|---------|-------|---------|
| `def` | `aiki` | define function |
| `class` | `aji` | define class |
| `struct` | `tsari` | define struct |
| `if` | `idan` | conditional |
| `elif` | `koidan` | else-if |
| `else` | `sai` | else |
| `for` | `ga` | for loop |
| `while` | `yayinda` | while loop |
| `return` | `dawo` | return |
| `break` | `tsaya` | break |
| `continue` | `ci_gaba` | continue |
| `match` | `duba` | pattern match |
| `case` | `hali` | match arm |
| `try` | `gwada` | try block |
| `except` | `kama` | except handler |
| `finally` | `karshe` | finally block |
| `raise` | `jefa` | raise exception |
| `async` | `ba_jira` | async function |
| `await` | `jira` | await expression |
| `import` | `shigo` | import module |
| `from` | `daga` | from import |
| `as` | `kamar` | alias |
| `in` | `a_cikin` | membership / loop |
| `true` | `gaskiya` | boolean true |
| `false` | `karya` | boolean false |
| `none` | `babu` | null / none |
| `and` | `da` | logical and |
| `or` | `ko` | logical or |
| `not` | `ba` | logical not |
| `print` | `buga` | print to stdout |

</div>

---

## Example Program

```python
class Kirga:
    pub adadi: i64

extend Kirga:
    pub def init(n: i64) -> Kirga:
        mut k = Kirga()
        k.adadi = n
        return k

    pub def qara(self, n: i64) -> void:
        self.adadi = self.adadi + n

    pub def nuna(self) -> void:
        buga(f"adadi = {self.adadi}")

def main():
    mut k = Kirga.init(0)
    ga i in range(10):
        k.qara(i)
    k.nuna()    # adadi = 45
```

---

## Compiler Pipeline

```
.tr source
    │
    ▼
  Lexer          tokenize source text
    │
    ▼
  Parser         build AST
    │
    ▼
  Sema           type-check, resolve names
    │
    ▼
  HIR            typed intermediate representation
    │
    ▼
  C Codegen      emit C source
    │
    ▼
  GCC / Clang    compile to native binary
    │
    ▼
  Executable
```

All stages are written in Tauraro itself — the compiler is **fully self-hosted**.

---

## Performance

Benchmarks run on Windows x64 with `gcc -O3` (C), `rustc -C opt-level=3 -C target-cpu=native` (Rust), and `tauraroc -O3` (Tauraro → C → `gcc -O3 -march=native`).

| Benchmark | C | Rust | Tauraro | Tau/C | Tau/Rust |
|-----------|--:|-----:|--------:|------:|---------:|
| Fibonacci 1B steps | 1.476s | 0.675s | 0.759s | **0.51×** | 1.12× |
| Float Multiply 1B | 3.614s | 3.233s | 3.280s | **0.91×** | 1.01× |
| Newton Sqrt 1B | 18.076s | 17.045s | 17.278s | **0.96×** | 1.01× |
| Mandelbrot 800×800 | 1.389s | 1.491s | 1.340s | **0.96×** | **0.90×** |
| Sieve 50M | 1.390s | 1.313s | 1.221s | **0.88×** | **0.93×** |
| Matrix Multiply 400×400 | 0.053s | 0.027s | 0.027s | **0.51×** | **1.00×** |

`tauraroc -O3` passes `-march=native -funroll-loops` to GCC. Tauraro beats C on **8 of 9 measurable benchmarks** and ties or beats Rust on 5. Full results in [`benchmarks/README.md`](benchmarks/README.md).

---

## Documentation

The full language reference lives in [`docs/lang/`](docs/lang/):

| # | Topic |
|---|-------|
| 01 | [Introduction & CLI](docs/lang/01_intro.md) |
| 02 | [Variables & Types](docs/lang/02_variables_and_types.md) |
| 03 | [Operators](docs/lang/03_operators.md) |
| 04 | [Control Flow](docs/lang/04_control_flow.md) |
| 05 | [Functions & Closures](docs/lang/05_functions.md) |
| 06 | [Strings & F-Strings](docs/lang/06_strings.md) |
| 07 | [Collections](docs/lang/07_collections.md) |
| 08 | [Classes & Extend](docs/lang/08_classes.md) |
| 09 | [Enums](docs/lang/09_enums.md) |
| 10 | [Interfaces](docs/lang/10_interfaces.md) |
| 11 | [Generics](docs/lang/11_generics.md) |
| 12 | [Error Handling](docs/lang/12_error_handling.md) |
| 13 | [Memory & Ownership](docs/lang/13_memory_and_ownership.md) |
| 14 | [Unsafe & Pointers](docs/lang/14_unsafe_and_pointers.md) |
| 15 | [Modules](docs/lang/15_modules.md) |
| 16 | [Concurrency](docs/lang/16_concurrency.md) |
| 17 | [Extern & FFI](docs/lang/17_extern_and_ffi.md) |
| 18 | [GPU & Inline Assembly](docs/lang/18_gpu_and_asm.md) |
| 19 | [Compiler Error Reference](docs/lang/19_compiler_errors.md) |
| 20 | [Advanced Patterns](docs/lang/20_advanced_patterns.md) |
| 21 | [Operator Overloading](docs/lang/21_operator_overloading.md) |

---

## License

Tauraro is dual-licensed under your choice of:

- **MIT License** — see [`LICENSE-MIT`](LICENSE-MIT)
- **Apache License, Version 2.0** — see [`LICENSE-APACHE`](LICENSE-APACHE)

You may use, distribute, and modify Tauraro under the terms of either license.

<div align="center">

<sub>Built with ❤️ — Python syntax · C performance · Hausa soul</sub>

</div>
