# Compiling, Backends & Cross-Compilation

**What it is** → How Tauraro turns your `.tr` source into a runnable binary: the code
generators (backends), the bundled toolchain, and how to cross-compile to any target —
Linux, Windows, macOS, ARM/RISC-V, WebAssembly, and bare metal — from a single download.

**When to use it** → Any time you build something that isn't "run on my machine": shipping
a binary for another OS/CPU, targeting WebAssembly, or writing firmware.

---

## TL;DR

```sh
tauraroc app.tr                              # compile for THIS machine (C backend)
tauraroc app.tr --backend llvm               # ... via the optimizing LLVM backend
tauraroc app.tr --target linux-arm64         # cross-compile for ARM64 Linux
tauraroc app.tr --backend llvm --target wasm-wasi -o app.wasm   # WebAssembly
tauraroc app.tr --freestanding --emit c      # bare-metal firmware (no OS, no libc)
tauraroc app.tr --run                        # compile and run in one step
```

**One download = the whole toolchain.** The release SDK bundles **zig** beside the
compiler. `zig cc` is a complete C/LLVM code generator + `lld` linker + a libc for
*every* target, so `--backend llvm` and `--target <anything>` work straight out of the
unzip with **no system C compiler, no LLVM, and no per-target sysroot to install.**

---

## Backends — `--backend <b>`

Tauraro lowers your program to a shared intermediate representation, then a *backend*
turns that into a binary. Pick one with `--backend`:

| Backend | Flag | What it does | Use it when |
|---|---|---|---|
| **C** *(default)* | *(none)* / `--backend c` | Emits portable C, compiled by `gcc`/`clang`/`zig cc`. Widest feature coverage. | Almost always. The reference backend; every language feature works here. |
| **LLVM** | `--backend llvm` | Lowers to LLVM IR, compiled by `clang`/`llc`/`zig cc`. Auto-vectorizes; often faster than C. | Numeric/array-heavy code, or when you want the LLVM optimizer. |
| **native** | `--backend native` | Direct x86-64 → ELF (no C). | Under construction — use C or LLVM for now. |

The **C backend is the default and the most complete** — the compiler self-hosts on it.
The **LLVM backend** shares the same lowering and is *faster on most benchmarks* (a
`getelementptr` codegen fix unblocked LLVM's auto-vectorizer: MatMul ~4×, Collatz ~3×);
it covers a large feature subset — including fixed-size arrays `[T; N]`, which lower to an
inline stack region (`alloca [N x i64]` with `getelementptr` access, **no heap**), so an
array-heavy `--no-heap` firmware build stays heap-free on the LLVM backend too. If the LLVM
backend can't lower something yet it tells you clearly — fall back to `--backend c`.

Both backends produce identical observable output — a differential oracle (`LLVM ≡ C`)
enforces this in CI.

---

## The bundled toolchain (zig) — nothing else to install

Every release `tauraroc-<platform>.zip` ships a `zig/` folder next to the binary. On
startup the compiler detects `<exe_dir>/zig/` and puts it on `PATH`, so **all** compiler
and linker invocations can use it:

- **`zig cc`** is `clang` (it even compiles the LLVM `.ll` directly) + `lld` (a universal
  cross-linker) + a libc for every target.
- Because it bundles the target libc, **hosted cross targets need no sysroot.**

**Toolchain selection is automatic**, per build:

- **Host builds** prefer a fast system `gcc`/`clang` when present, and fall back to the
  bundled `zig cc` — so Tauraro can compile with **absolutely nothing else installed**.
- **Cross builds** prefer `zig cc` (its bundled per-target libc is the whole point), then
  an installed `<triple>-gcc` / Android NDK, then `clang`. Override the libc/headers with
  `--sysroot <path>`.

> First cross-compile to a given target is slower: zig compiles that target's libc from
> source once, then caches it. Subsequent builds are fast.

If you build from source instead of a release, install any C compiler (or drop a `zig/`
next to `tauraroc` yourself) — the compiler finds whatever is available.

---

## Cross-compilation — `--target <name>`

One flag, **both** the C and LLVM backends. The generated code is target-neutral, so a
single program cross-compiles to every supported architecture; the output gets the right
extension automatically (`.exe` / `.wasm` / bare ELF).

```sh
tauraroc app.tr --target linux-arm64                 # ARM64 Linux (C backend)
tauraroc app.tr --backend llvm --target linux-riscv64  # RISC-V 64 Linux (LLVM backend)
tauraroc app.tr --target windows-x64 -o app.exe      # Windows from Linux/macOS
tauraroc app.tr --target aarch64-linux-musl --static # fully static ARM64 binary
```

### Supported target shorthands

| Category | Names |
|---|---|
| **Linux** | `linux-x86_64`, `linux-arm64`, `linux-arm32`, `linux-riscv64` |
| **Windows** | `windows-x64`, `windows-arm64` |
| **macOS** | `macos-arm64`, `macos-x86_64` |
| **Android** | `android-arm64`, `android-arm32`, `android-x86_64`, `android-x86` |
| **Apple mobile** | `ios`, `ios-sim` |
| **WebAssembly** | `wasm`, `wasm-wasi` |
| **Bare-metal** | `embedded-arm`, `embedded-arm64`, `embedded-riscv32`, `embedded-riscv64` |

Or pass a **raw LLVM triple**: `--target aarch64-linux-gnu`, `--target
x86_64-linux-musl`, `--target aarch64-linux-android34`, etc.

### Static binaries

`--static` links a fully static binary (no shared-library dependencies at runtime). On
Linux this uses **musl** — pass a `*-musl` triple or a `linux-*` shorthand with `--static`:

```sh
tauraroc app.tr --target aarch64-linux-musl --static   # runs anywhere, incl. bare qemu-user
```

*(zig cannot static-link glibc; musl static-links cleanly.)*

### Notes

- **Android** additionally uses the NDK's clang wrapper if `ANDROID_NDK_ROOT` /
  `ANDROID_NDK_HOME` is set.
- **macOS/iOS** cross targets link Apple frameworks, which need Apple's SDK on the build
  machine; the compiled objects themselves are produced by the bundled toolchain.
- The **LLVM backend** needs a clang-family compiler for the target (bundled `zig cc` or
  a system `clang`); a plain GNU `gcc` cannot consume LLVM IR (the C backend can use gcc).

---

## WebAssembly

`--target wasm-wasi` with the LLVM backend produces a real `.wasm` linked against
wasi-libc:

```sh
tauraroc app.tr --backend llvm --target wasm-wasi -o app.wasm
wasmtime app.wasm        # or: node --experimental-wasi-unstable-preview1 …
```

The runtime ships a **WASI profile** — single-threaded execution, no BSD sockets, no
subprocess, and a wasm-safe exception path — so the full standard runtime compiles and
runs inside the sandbox. (Threading, networking, and process APIs are unavailable on
this target and degrade to no-ops/errors, as WASI itself has none.)

---

## Bare-metal & freestanding

Tauraro compiles all the way down to firmware with **no OS and no C library**. Three
runtime tiers (mirroring Rust `std`/`alloc`/`core`):

| Tier | Flag | Has | Needs |
|---|---|---|---|
| `std` | *(default)* | ARC + collections + files/threads/net | an OS + libc |
| `alloc` | `--no-std` | collections via a pluggable allocator; no OS services | an allocator |
| `core` | `--freestanding` | no OS, **no libc** — you supply allocator + I/O | nothing |

```sh
# Cortex-M firmware, 100% Tauraro — emits C + a linker script:
tauraroc firmware.tr --freestanding --emit c --emit-ld build/app.ld
# then compile the emitted C with any freestanding toolchain (the bundled zig works):
#   zig cc -target thumb-freestanding-eabi -mcpu=cortex_m3 -ffreestanding ... build/*.c
# or a system arm-none-eabi-gcc.
```

`--freestanding` emits `#define TAURARO_KERNEL`; `--no-std` emits `#define
TAURARO_NO_OS` — you never hand-pass those. The freestanding runtime is **memory-safe**:
bounds checks, null safety, the `[P-2]` raw-pointer quarantine, and `@value_type` safety
all stay on at every tier. See **[Advanced — Bare-Metal & Freestanding](advanced/11_bare_metal.md)**
for the full guide (reset vector, allocator wiring, `@entry`, RISC-V, qemu).

### `--no-heap` — a compile-time zero-heap guarantee

For the strictest firmware, `--no-heap` makes the compiler **reject every heap-allocating
construct** with a clear `[H-1]` diagnostic, turning "stay off the heap" from a discipline
into an enforced wall:

```sh
tauraroc firmware.tr --no-heap --freestanding --emit c
```

Rejected: `List`/`Dict`/`Set`/`Vec`/`Map` literals *and* constructors, list comprehensions,
f-strings, string `+`/`*`, and heap (`non-@value_type`) class construction. Allowed:
`@value_type` structs, fixed arrays `[T; N]` (local **and** global), raw `Pointer[T]`,
`StrView`, scalars, and bit ops. See `examples/freestanding/zero_heap/`.

**Tuples and enums under `--no-heap`.** On the **C backend** (the recommended freestanding
path) tuples and enums are stack value structs — a `TrTuple` compound literal and a stack
tagged union — so `--no-heap` accepts them. The **LLVM and native backends** currently
heap-box them (via the object allocator), so under `--backend llvm --no-heap` a tuple or
enum construction is rejected with `[H-1]`, keeping the zero-heap guarantee honest. Use the
C backend for a zero-heap build that needs tuples/enums, or return values via out-params /
`@value_type` structs on the LLVM path. (Fixed arrays `[T; N]` are stack-allocated on **all**
backends — see the note above — so array-heavy firmware stays heap-free everywhere.)

---

## CLI reference

### Output & mode
| Flag | Meaning |
|---|---|
| `-o <path>` | Exact output path (its directory is honored; overrides `-d`). |
| `-d <dir>` | Output directory (default: the current working directory). |
| `--run` | Compile then immediately execute. |
| `--check` | Semantic analysis only — no codegen. |
| `--lib` | Build a shared library (`.so`/`.dll`/`.dylib`) of `export def`s + a C header. |
| `--emit c\|ast\|mir` | Emit generated C (to `build/`), the AST, or MIR, and stop. |
| `--emit-ld <path>` | Also write a linker script (bare-metal `@entry` builds). |
| `--verbose` | Print every pipeline phase (and the chosen toolchain). |

### Backend & target
| Flag | Meaning |
|---|---|
| `--backend c\|llvm\|native` | Choose the code generator (default `c`). |
| `--target <name\|triple>` | Cross-compile (see the matrix above). |
| `--sysroot <path>` | Override the cross toolchain's sysroot. |
| `--static` | Statically link (musl on Linux). |

### Optimization
| Flag | Meaning |
|---|---|
| `-O0`/`-O1`/`-O2`/`-O3` | Optimization level (default `-O2`). |
| `-Os` | Optimize for size. |
| `--debug` | AddressSanitizer + bounds-check assertions. |

### Linking
| Flag | Meaning |
|---|---|
| `--link <path>` | Link a file by path (`.c .o .a .dll .lib .so`). |
| `-l<name>` / `-l <name>` | Link a library by name (e.g. `-luser32`). |

### Safety & tiers
| Flag | Meaning |
|---|---|
| `--strict` | Alloc/dealloc outside `unsafe:` is a hard error (`[U-1]`). |
| `--no-heap` | Reject all heap-allocating constructs (`[H-1]`). |
| `--no-std` | `alloc` tier — pluggable allocator, no OS services. |
| `--freestanding` | `core` tier — no OS, no libc. |

### Subcommands
| Command | Meaning |
|---|---|
| `tauraroc fmt [-w] <file>` | Format source (to stdout, or in place with `-w`). |
| `tauraroc lint <file>` | Analyze and report warnings/errors (like `--check`). |
| `tauraroc --version` | Print the compiler version. |

---

## How output paths work

Given no `-o`/`-d`, the executable lands in the **current working directory** with the
source's base name (all backends). `-d <dir>` redirects the directory; `-o <path>` sets
the exact path (its directory is created if missing). The output extension follows the
**target** (`.exe` for Windows, `.wasm` for WebAssembly, bare for ELF/Mach-O).

```sh
cd ~/project
tauraroc src/app.tr                 # → ~/project/app  (or app.exe on Windows)
tauraroc src/app.tr -d dist         # → ~/project/dist/app
tauraroc src/app.tr -o out/myprog   # → ~/project/out/myprog
```

---

## Common recipes

```sh
# Run a script instantly
tauraroc script.tr --run

# Release build, this machine
tauraroc app.tr -O3 -o app

# Ship a Linux ARM64 binary that runs anywhere (static)
tauraroc app.tr --target aarch64-linux-musl --static -o app-arm64

# Windows .exe from Linux/macOS
tauraroc app.tr --target windows-x64 -o app.exe

# WebAssembly module
tauraroc app.tr --backend llvm --target wasm-wasi -o app.wasm

# A shared library for C/Rust/Python to load
tauraroc lib.tr --lib -o mylib          # → mylib.so + mylib.h

# Provably zero-heap firmware
tauraroc fw.tr --no-heap --freestanding --emit c --emit-ld build/fw.ld

# Inspect what the compiler generates
tauraroc app.tr --emit c                 # C sources in build/
tauraroc app.tr --backend llvm -o app.ll # textual LLVM IR
```

---

## Troubleshooting

- **"the LLVM backend can't lower this program yet"** — that feature isn't in the LLVM
  subset. Use `--backend c` (the default) for that program.
- **"target … needs its libc headers/sysroot"** — a hosted cross target has no libc
  available. Install `zig` (or use the bundled one), or pass `--sysroot <path>`.
- **"could not compile the LLVM IR"** — no clang-family compiler was found. Use the
  release SDK (bundled zig), install `clang`/`llc`, or install `zig`.
- **Cross build is slow the first time** — zig is compiling that target's libc; it's
  cached after the first build.

---

## See also

- [Introduction](01_intro.md) — the pipeline and quick start.
- [Advanced — Bare-Metal & Freestanding](advanced/11_bare_metal.md) — full firmware guide.
- [Extern & FFI](17_extern_and_ffi.md) — linking against C libraries.
- [Modules](15_modules.md) — `TAURARO_PATH` and importing external libraries.
