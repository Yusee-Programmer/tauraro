# Tauraro v0.0.8 — Release Notes

**Focus of this release: the LLVM backend grows up, and bare-metal gets a real guarantee.**

v0.0.8 turns the LLVM backend into a complete, self-hosting, self-contained code
generator that is now *faster than the C backend* on most benchmarks — and adds a
compile-time `--no-heap` mode that makes zero-heap bare-metal programs a guarantee,
not a discipline.

> Pre-1.0 policy: until 1.0, any `0.x` bump may contain breaking changes with no
> deprecation period. Nothing user-facing was removed in this release.

---

## ✨ Highlights

- **The LLVM backend self-hosts.** `tauraroc --backend llvm` now compiles the *entire*
  compiler, and the LLVM-built compiler emits **byte-identical C** to the reference
  C-built compiler — a strong end-to-end equivalence proof.
- **LLVM backend is now faster than the C backend on most benchmarks.** A codegen fix
  (proper `getelementptr` addressing) unblocked LLVM's auto-vectorizer: **MatMul ~4×,
  Collatz ~3×, Sieve and N-Body faster than C**; parity elsewhere.
- **`--backend llvm` is self-contained out of the box.** Release archives now bundle a
  minimal `llc`, so you no longer need to install LLVM — just a C compiler (used purely
  as the linker).
- **Optional in-process LLVM.** An opt-in build links libLLVM directly (`llvm-c` API) and
  emits objects with **no subprocess** at all.
- **`--no-heap`: a compile-time zero-heap guarantee for bare metal.** The compiler now
  *rejects* every heap-allocating construct, turning the "stay off the heap" discipline
  into an enforced wall — Zig-style — for MCUs, kernels, and freestanding targets.

---

## 🚀 Added

### `--backend llvm` — self-hosting, fast, self-contained
- **Full self-host:** the LLVM backend can lower the whole compiler; the resulting binary
  reproduces the reference compiler's output exactly.
- **GEP addressing → vectorization:** list/array element addresses are now emitted as
  `getelementptr` (provenance-preserving) instead of `inttoptr(add)`, which the LLVM
  loop-vectorizer could not analyze. Numeric/array kernels now vectorize (`vfmadd`/`mulpd`)
  and beat the C backend.
- **Sound `noalias`** on fresh-allocation runtime constructors and a complete TBAA type
  hierarchy (list-header vs list-element vs object-field) for better alias analysis.

### Bundled LLVM toolchain (no system LLVM required)
- **Stage 1 — bundled `llc`:** `--backend llvm` looks for a bundled `<exe_dir>/llvm/llc`
  first, then falls back to a `PATH` `clang`/`llc`. It drives the full pipeline for you
  (emit `.ll` → object → link), reusing the C compiler you already have as the linker.
  The whole flow is automatic: `tauraroc prog.tr --backend llvm -o prog`.
- **Stage 2 — in-process libLLVM (opt-in):** build the compiler with
  `TAURARO_LLVM_INPROC=1 bash scripts/build.sh` (needs libLLVM dev files) to emit objects
  through the `llvm-c` C API in-process — no `llc` subprocess. The default build stays a
  tiny stub with **zero libLLVM dependency** and uses the bundled `llc`.

### Complete SDK release archive
- Each `tauraroc-<platform>.zip` is now a **self-contained SDK**: `tauraroc` +
  `llvm/llc` + `runtime/` + `std/`. After unzip, set `TAURARO_PATH` to the folder and
  `--backend llvm` works immediately (only prerequisite: a C compiler on `PATH`).

### `--no-heap` — bare-metal zero-heap guarantee (new flag)
- New compile-time mode that **rejects any heap-allocating construct** with a clear
  `[H-1]` diagnostic (and a fix hint), forcing the value-type / fixed-array `[T; N]` /
  `Pointer[T]` / string-literal subset. Rejected: `List`/`Dict`/`Set`/`Vec`/`Map`
  literals *and* constructors, list comprehensions, f-strings, string concatenation
  (`+`) and repetition (`*`), and heap (`non-@value_type`) class construction.
  `@value_type` structs, fixed arrays, raw pointers, enums/tuples (value on the C
  backend), scalars, and bit ops all pass.
- Pairs with the existing `--freestanding`/`--no-std` tiers: `tauraroc app.tr --no-heap
  --freestanding --emit c` gives you provably-zero-heap C to cross-compile with your MCU
  toolchain (`arm-none-eabi-gcc`, RISC-V, …).
- **New examples** under `examples/freestanding/zero_heap/`:
  - `firmware.tr` — a complete MCU-style firmware exercising *every* zero-heap construct
    (value types, 1D/2D fixed arrays, MMIO via raw pointers, bit ops, an enum state
    machine, tuples, fixed-point math, global fixed buffers).
  - `violations.tr` — the counter-example: every heap construct, each rejected by `[H-1]`.

---

## 🐛 Fixed (C backend — found via the LLVM differential oracle)

- **Interface values wrongly retained as fat-pointer structs.** `x: Iface = obj` emitted
  `_tr_obj_retain(<struct>)`, a C type error, breaking interface examples. Now interface
  locals borrow the underlying object (no bogus retain).
- **Undefined `_trdrop_<T>` for `free()`-owning classes.** A field of a class with an
  explicit `free()` (e.g. std/async `Channel`) referenced a `_trdrop` that is never
  emitted for such self-managing classes. `is_heap_class_tn` now excludes them.
- **`_tr_rt_str_new` undefined in the C backend.** std/async helper wrappers referenced a
  native-only runtime symbol; added a C-backend inline definition.
- Net effect: three programs (`09_interfaces`, `19_generic_interfaces`, `25_new_features`)
  that failed to build under the C backend now build and run; **all 35 examples build
  under both backends.**

## 🐛 Fixed (LLVM/native lowering)

- **Wildcard-match over-release.** A `_` discard binder was treated as an owning variable
  (all `_` shared one slot), so a scope-exit drop could release a stale value. `_` is now
  never dropped — this was the final blocker to the full LLVM self-host.
- Several ARC balance fixes in the shared LIR lowering (field-set / index-set statement
  flushes, object-dict `.get` retain).

---

## 🔧 Tooling & CI

- **New oracle `scripts/no_heap_check.sh`** (wired into CI on every platform): asserts the
  zero-heap `firmware.tr` compiles + runs and that `violations.tr` is rejected with
  `[H-1]` — keeping the guarantee and the examples honest.
- CI builds and caches a **minimal `llc`** per platform (host target only) and bundles it
  into the release SDK.
- Cross-backend equivalence stays enforced: LLVM≡C differential (94/94), native codegen
  byte-identical, object-leak net-zero, self-host fixpoint (gen2 ≡ gen3).

---

## 📚 Notes & tips

- **Multi-path `TAURARO_PATH`** (already supported, now documented): join arbitrary
  external-library directories like `PYTHONPATH` — `:`-separated on Linux/macOS,
  `;`-separated on Windows. It is additive to the built-in roots (`std`, `packages`, …).
  On Windows, use native paths (`C:\...`).
- **Prerequisite for `--backend llvm`:** only a C compiler on `PATH` (used as the linker).
  The LLVM code generator itself is bundled (`llc`) or in-process (libLLVM).

## ⚠️ Known limitations

- `--no-heap` is fully sound on the **C backend** (the bare-metal path). On the LLVM
  backend, enums/tuples still box, so a strict LLVM zero-heap build is a follow-up.
- The LLVM backend still has a feature subset (e.g. value-type methods + global fixed
  arrays + tuple returns together, as in `firmware.tr`, aren't lowered yet) — use the C
  backend for those, which is the natural cross-compile path for embedded targets.

---

## ⬆️ Upgrading

- No source changes required. New capabilities are opt-in flags (`--no-heap`) or automatic
  (bundled `--backend llvm`).
- To use the LLVM backend from a release: download `tauraroc-<platform>.zip`, unzip,
  `export TAURARO_PATH=/path/to/unzipped-sdk`, then
  `tauraroc prog.tr --backend llvm -o prog` (a C compiler must be on `PATH`).

_`tauraroc --version` now reports **v0.0.8**._
