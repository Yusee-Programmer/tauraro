# Runtime tiers & freestanding (`--no-std` / bare-metal)

Extend Tauraro's *gradual safety* dial with an orthogonal *gradual runtime* dial —
turn the runtime **down** without turning the safety **off**. The banner:
**memory-safe, Python-syntax, all the way down to bare metal.**

## The three tiers (mirrors Rust core/alloc/std)

| Tier | Flag | Has | Needs | Targets |
|---|---|---|---|---|
| `std` | *(default)* | ARC + collections + files/threads/net/reactor | OS + libc | today's userspace |
| `alloc` | `--no-std` | ARC + collections via a **pluggable allocator**; no OS services | an allocator only | RTOS, WASM, mobile cores, edge-with-heap |
| `core` | `--freestanding` | no heap, no ARC — `@value_type`, slices, `unsafe:`, raw ptrs, `extern C` | nothing (freestanding) | MCUs, drivers, kernels |

`@value_type` is already the seed of `core`; the safety dial (`--strict`) is orthogonal
and stays available at every tier.

## What already exists (runtime/tauraro_rt.h)

The header is **already substantially freestanding-aware** — this is a coverage
completion, not a green field:

- **Pluggable allocator**: `TAURARO_ALLOC/FREE/REALLOC/CALLOC` macros (default libc,
  overridable; `TAURARO_KERNEL` *requires* them). Wrappers `_tr_c_malloc`/`_tr_c_calloc`/
  `_tr_free` route through them.
- **Panic/OOM hooks**: `_TR_PANIC(msg)`, `_TR_OOM_ABORT()`, `_TR_ASSERT[_MSG]` — with
  kernel (`BUG()`/spin) and hosted (`fprintf`+`abort`) variants.
- **Freestanding includes**: guarded on `TAURARO_KERNEL`/`__KERNEL__`; needs only
  `stddef/stdbool/stdint/stdatomic`.
- OS services (threads/net/epoll/kqueue/WSAPoll/files/ucontext) are already in
  platform `#ifdef` blocks — the seams for `--no-std` to compile them out.

## The libc boundary (what Phase 1 closes)

For a tier below `std`, **no direct libc may leak** — every allocation, panic, and
byte of output must pass through a hook. Audit (this is the whole gap):

- **~10 raw `malloc`/`calloc`/`realloc`** sites bypass the macros (List/Dict/Chan/poll
  internals) → route to `TAURARO_ALLOC/CALLOC/REALLOC`.
- **~28 raw `abort()`** (Option/Result unwrap, null-list access, OOM) → route to
  `_TR_PANIC(msg)` / `_TR_OOM_ABORT()`.
- **Output** (`_tr_print`, `fprintf` diagnostics) → a `_TR_WRITE(str,len)` hook
  (default `fwrite(stdout)`; `--no-std` = user-provided, e.g. UART/semihosting).

Codegen already routes user `print`/allocation through runtime functions
(`_tr_print`, `_tr_obj_alloc`, `_tr_str_new`, …), so the libc boundary lives in the
runtime header — closing it there closes it for all programs.

## Phased plan

- **Phase 0 — cross-compile polish.** musl-static + ARM/RISC-V. Fully unlocks
  **edge/gateway Linux** and **mobile compute libraries** with no `--no-std` needed.
- **Phase 1 — close the libc boundary.**
  - **1a — DONE ✅ (allocation + trap boundary).** All raw `malloc`/`calloc`/
    `realloc`/`free` now route through `TAURARO_ALLOC/FREE/REALLOC/CALLOC`; all raw
    `fprintf(stderr,…)`/`abort()` route through new `_TR_DIAG(…)`/`_TR_TRAP()` hooks
    (hosted = fprintf/abort; kernel = pr_err/BUG or no-op/spin). **Validated:** hosted
    is byte-identical (suite 16/16, smoke correct); a `-DTAURARO_KERNEL` build of a
    `Vec[int]` program with a custom bump allocator compiles with **zero** libc
    `malloc/calloc/realloc/abort/fprintf` references — the alloc/trap boundary is
    proven clean.
  - **1b — NEXT.** `--no-std` compiler flag → defines `TAURARO_NO_STD`, `#ifdef`-out
    the OS services. The freestanding smoke pins the exact seam list to gate: file I/O
    (`fclose`/`fflush`/`fgets`/`fopen`/`_fileno`), the threadpool
    (`_tr_threadpool_*`), exceptions (`_tr_exc_push`), env (`_putenv_s`), plus the
    net/thread/reactor blocks (already in platform `#ifdef`s). Keep ARC on the
    allocator hook. Add `_TR_WRITE` so `print`/`_tr_print` routes to a user sink.
  - **1c — libc-lite.** The pervasive freestanding needs `strlen`/`memcpy`/`memset`/
    `memmove`/`strcmp`/`snprintf`. Provide minimal freestanding impls behind
    `TAURARO_KERNEL && !__KERNEL__` (kernel gets `<linux/string.h>`), or require the
    target to supply them.
- **Phase 2 — tier-tagging in sema.** Each stdlib module/feature carries a minimum
  tier; using a `std` feature under `--no-std` is a clear `[R-1]` diagnostic ("needs
  the std runtime"). *The compiler carries the burden* — Tauraro's identity applied
  to runtimes.
- **Phase 3 — `core` stdlib.** Value-type primitives: fixed arrays, slices, StrView,
  math, bit/volatile/MMIO helpers — no allocator.
- **Phase 4 — `--freestanding` (`core`) + entry control.** ARC off; reuse the
  `@export`/cdylib machinery for named entries (`_start`, an ISR) instead of a hosted
  `main`. Ship a blink + a toy kernel as proof.
- **Phase 5 — arch/HAL ecosystem.** startup, linker scripts, interrupt/volatile,
  `hal` layer. Deep, ongoing; where real MCU support lives.

## Invariant across all tiers

Bounds checks, `[P-2]` raw-pointer quarantine, null safety, `@value_type` safety, and
(opt-in) `--strict` **stay on**. The dial reduces the *runtime*, never the *safety* —
that is the entire point and the differentiator from C/Zig bare-metal.
