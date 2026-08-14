# Tauraro v0.0.8 — Release Notes

**Focus of this release: Tauraro becomes a batteries-included, cross-platform toolchain — one download compiles for your machine *and* cross-compiles to any target, with nothing else to install.**

v0.0.8 turns Tauraro into a self-contained toolchain. The SDK now ships a full
LLVM + LLD + multi-target libc (via a bundled **zig**), so `tauraroc` compiles for
your host, cross-compiles to Linux/Windows/macOS/ARM/RISC-V/**WebAssembly**/bare-metal,
and drives the whole `--backend llvm` pipeline — **with no system C compiler, no
system LLVM, and no per-target sysroot to install.** On top of that: the LLVM backend
now self-hosts and is *faster than the C backend* on most benchmarks, and a compile-time
`--no-heap` mode makes zero-heap bare-metal programs a guarantee instead of a discipline.

> Pre-1.0 policy: until 1.0, any `0.x` bump may contain breaking changes with no
> deprecation period. Nothing user-facing was removed in this release.

---

## ✨ Highlights

- **One download = the whole toolchain.** The release SDK bundles **zig**, whose
  `zig cc` is a complete clang code generator + `lld` linker + a libc for *every*
  target. `tauraroc` auto-detects it beside the binary — so `--backend llvm` and
  `--target <anything>` work straight out of the unzip with **nothing else installed**.
- **Cross-compile to any target with `--target`.** One flag, both backends:
  `linux-arm64`, `linux-riscv64`, `windows-x64`, `macos-arm64`, `android-*`, `ios`,
  `wasm-wasi`, `embedded-arm`/`embedded-riscv*`, … or a raw LLVM triple. Because zig
  bundles the target libc, **hosted targets need no sysroot**.
- **WebAssembly is a first-class target.** `tauraroc app.tr --backend llvm --target
  wasm-wasi -o app.wasm` produces a real `.wasm` that runs under any WASI runtime
  (Node, Wasmtime, …).
- **Bare-metal works from the same one download.** `--freestanding` firmware cross-
  compiles through the bundled zig — no `arm-none-eabi-gcc` required — and `--no-heap`
  turns "stay off the heap" into an enforced, Zig-style compile-time wall.
- **The LLVM backend self-hosts and is faster than C.** It compiles the *entire*
  compiler to a binary that emits byte-identical C to the reference build, and a
  `getelementptr` codegen fix unblocked auto-vectorization: **MatMul ~4×, Collatz ~3×,
  Sieve and N-Body faster than C**; parity elsewhere.

---

## 🚀 Added

### Cross-compilation — `--target` on both backends
- `--target <name>` cross-compiles to a huge target matrix on **both** the default C
  backend and `--backend llvm`:
  `android-arm64/arm32/x86_64/x86`, `ios`, `ios-sim`,
  `linux-arm64/arm32/x86_64/riscv64`, `windows-x64/arm64`,
  `macos-arm64/x86_64`, `embedded-arm/arm64/riscv32/riscv64`, `wasm`, `wasm-wasi` —
  or pass a raw LLVM triple (e.g. `aarch64-linux-musl`).
- The compiler auto-configures the toolchain per target: bundled/`PATH` **`zig cc`**
  (bundled libc → **no sysroot**) → an installed `<triple>-gcc`/NDK → `clang`. Pass
  `--sysroot <path>` to override. `--static` links a fully static binary (via musl).
- The generated LLVM IR is target-neutral, so a single lowering cross-compiles to every
  supported architecture; the output filename gets the right extension automatically
  (`.exe` / `.wasm` / bare ELF).

### Bundled zig — a complete, self-contained toolchain
- Every `tauraroc-<platform>.zip` now ships **`zig/`** beside the compiler. `zig cc` is
  clang (it compiles the LLVM `.ll` directly), `lld` (the universal cross-linker), and a
  libc for every target — so it replaces a separate LLVM/`llc`/`lld` bundle *and* removes
  the need for a system C compiler or per-target sysroot.
- On startup `tauraroc` finds `<exe_dir>/zig/` and prepends it to `PATH`, so **every**
  toolchain lookup (host builds, `--backend llvm`, and all cross targets) picks it up
  automatically. Host builds still *prefer* a fast system `gcc`/`clang` when present and
  fall back to bundled `zig cc` — so Tauraro can compile with **absolutely nothing else
  installed**.
- Result: `unzip tauraroc-<platform>.zip` → `tauraroc app.tr` (host),
  `tauraroc app.tr --backend llvm` (LLVM), and `tauraroc app.tr --target <t>` (cross)
  all work immediately.

### WebAssembly (WASI)
- `--target wasm-wasi` with `--backend llvm` emits a runnable `.wasm` linked against
  wasi-libc. The runtime gained a **WASI profile** — single-threaded execution, no BSD
  sockets, no subprocess, and a wasm-safe exception path — so the full standard runtime
  compiles and runs in a sandbox.

### `--backend llvm` — self-hosting, fast
- **Full self-host:** the LLVM backend lowers the whole compiler; the LLVM-built binary
  reproduces the reference compiler's output exactly.
- **GEP addressing → vectorization:** element addresses are emitted as `getelementptr`
  (provenance-preserving) instead of `inttoptr(add)`, which the LLVM loop-vectorizer
  could not analyze. Numeric/array kernels now vectorize (`vfmadd`/`mulpd`) and beat C.
- **Sound `noalias`** on fresh-allocation runtime constructors plus a complete TBAA type
  hierarchy (list-header vs list-element vs object-field) for stronger alias analysis.
- **Optional in-process libLLVM (opt-in):** build with `TAURARO_LLVM_INPROC=1 bash
  scripts/build.sh` to emit objects through the `llvm-c` API with no subprocess. The
  default build has zero libLLVM dependency and uses the bundled zig.

### `--no-heap` — bare-metal zero-heap guarantee (new flag)
- A compile-time mode that **rejects any heap-allocating construct** with a clear `[H-1]`
  diagnostic (and a fix hint), forcing the value-type / fixed-array `[T; N]` /
  `Pointer[T]` / string-literal subset. Rejected: `List`/`Dict`/`Set`/`Vec`/`Map`
  literals *and* constructors, list comprehensions, f-strings, string concatenation (`+`)
  and repetition (`*`), and heap (`non-@value_type`) class construction. `@value_type`
  structs, fixed arrays, raw pointers, enums/tuples, scalars, and bit ops all pass.
- Pairs with the `--freestanding` / `--no-std` tiers:
  `tauraroc app.tr --no-heap --freestanding` gives provably-zero-heap firmware.
- **New examples** under `examples/freestanding/zero_heap/`: `firmware.tr` (exercises
  every zero-heap construct) and `violations.tr` (each heap construct rejected by `[H-1]`).

---

## 🐛 Fixed

### C backend — found via the LLVM ≡ C differential oracle
- **Interface values wrongly retained as fat-pointer structs** — `x: Iface = obj` emitted
  a bogus `_tr_obj_retain(<struct>)`; interface locals now borrow the object.
- **Undefined `_trdrop_<T>` for `free()`-owning classes** — a field of a self-managing
  class (e.g. std/async `Channel`) referenced a `_trdrop` never emitted; `is_heap_class_tn`
  now excludes them.
- **`_tr_rt_str_new` undefined in the C backend** — added a C-backend inline definition.
- Net effect: `09_interfaces`, `19_generic_interfaces`, `25_new_features` build again;
  **all examples build under both backends.**

### LLVM / native lowering
- **Wildcard-match over-release** — a `_` discard binder was treated as an owning variable
  (all `_` shared one slot), so a scope-exit drop could release a stale value. `_` is never
  dropped now — the final blocker to the full LLVM self-host.
- ARC balance fixes in the shared LIR lowering (field-set / index-set flushes,
  object-dict `.get` retain).

### Runtime — cross-compilation & the `TAURARO_BARE` split
- **`TAURARO_BARE` split into orthogonal capabilities** — `TAURARO_NO_LIBC` /
  `NO_THREADS` / `NO_NET` / `NO_SUBPROCESS`. `TAURARO_BARE` still implies all of them, so
  **bare-metal behaves exactly as before**, but WASI can now shed threads/sockets/subprocess
  while keeping its libc. (These are derived *after* every path that sets `TAURARO_BARE`,
  including the KERNEL/freestanding path.)
- **Host-vs-target link bugs fixed** — Winsock (`-lws2_32`) and the output extension were
  keyed on the *host* OS; a Windows→Linux cross wrongly demanded `ws2_32` / named the
  output `.exe`. Both now follow the *target*.
- **WASI entry glue** — emit `@__main_void = hidden alias @main` for wasm targets so the
  wasi crt reaches `main` (without it the module trapped at entry).

---

## 🔧 Tooling & CI

- **One-download SDK in CI:** each platform's build downloads zig (cached) into `zig/`,
  uses it for the LLVM and cross gates, and ships it in the artifact **and** the release
  zip — so the uploaded SDK is the complete toolchain.
- **New cross gates:** cross-compile `linux-arm64` with both backends and run the static
  build under **qemu**; compile/run WASI; freestanding compiles via zig. Existing
  Cortex-M / RISC-V bare-metal-under-qemu gates stay green.
- **`scripts/no_heap_check.sh`** (all platforms): asserts `firmware.tr` compiles + runs and
  `violations.tr` is rejected with `[H-1]`.
- Cross-backend equivalence stays enforced: **LLVM ≡ C differential**, native codegen
  byte-identical, object-leak net-zero, and the self-host **fixpoint (gen2 ≡ gen3)**.
- The old from-source minimal-LLVM build (~30–60 min) is **gone** — zig supersedes it.

---

## 📚 Notes & tips

- **No prerequisites for the release SDK.** `--backend llvm` and `--target` need nothing
  installed — the bundled zig is the code generator, linker, and libc. (A system
  `gcc`/`clang`, if present, is preferred for *host* builds because it avoids zig's one-
  time per-target libc compile.)
- **Multi-path `TAURARO_PATH`:** join external-library directories like `PYTHONPATH` —
  `:`-separated on Linux/macOS, `;` on Windows; additive to the built-in roots. On Windows,
  use native paths (`C:\...`).
- **First cross-compile per target is slower:** zig compiles that target's libc from source
  once, then caches it (subsequent builds are fast).

## ⚠️ Known limitations

- `--no-heap` is fully sound on the **C backend** (the bare-metal path). On the LLVM
  backend, enums/tuples still box, so a strict LLVM zero-heap build is a follow-up.
- The LLVM backend still has a feature subset (e.g. value-type methods + global fixed
  arrays + tuple returns together, as in `firmware.tr`) — use the C backend for those.
- **Bare-metal embedded** compiles via zig; a fully turnkey link+run for `--target
  embedded-*` (bundled picolibc-style libc) is a follow-up. `--freestanding` +
  `arm-none-eabi`/qemu is the proven end-to-end path today.

---

## ⬆️ Upgrading

- No source changes required. New capabilities are opt-in flags (`--target`, `--no-heap`)
  or automatic (the bundled toolchain).
- To use the full toolchain from a release:
  1. Download `tauraroc-<platform>.zip` and unzip it.
  2. That's it — `zig/` sits beside `tauraroc` and is auto-detected. Then:
     - `tauraroc app.tr -o app` (host),
     - `tauraroc app.tr --backend llvm -o app` (LLVM backend),
     - `tauraroc app.tr --target linux-arm64 -o app` (cross-compile),
     - `tauraroc app.tr --backend llvm --target wasm-wasi -o app.wasm` (WebAssembly).
- Optionally set `TAURARO_PATH=/path/to/unzipped-sdk` to import the bundled `std`/packages
  from anywhere.

_`tauraroc --version` reports **v0.0.8**._
