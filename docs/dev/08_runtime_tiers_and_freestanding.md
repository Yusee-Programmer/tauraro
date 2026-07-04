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

- **Phase 0 — cross-compile validation — DONE ✅ (CI).** `scripts/cross_check.sh` +
  a CI job (Linux) do two things: **(A)** cross-compile a real heap program
  (`tests/cross/hello_std.tr`) to **aarch64 Linux** with `aarch64-linux-gnu-gcc
  -static` and **run it under qemu** — proving the full `std` runtime + ARC +
  collections cross-compile and execute on ARM (this is the *faithful* validator;
  MinGW can't do freestanding because its `stdatomic` pulls x86 intrinsics). This
  fully unlocks **edge/gateway Linux** and **mobile compute libraries** — no
  `--no-std` needed. **(B)** compile `tests/freestanding/vecsum.tr` freestanding for
  **arm-none-eabi** with a custom bump allocator (`tests/freestanding/platform.c`)
  and `-Werror=implicit-function-declaration`, **reporting** the remaining bare-metal
  libc seams (informational until the compiler flag lands). Validated locally: the
  programs produce the exact expected outputs; the ARM checks run in CI.

  **Key finding from B:** a fully-compiling *freestanding* program needs the
  **codegen** to stop emitting the top-level exception frame (`setjmp`/`longjmp`/
  `_tr_exc_push`) and the async-pool cleanup under `--no-std` — a compiler change,
  not runtime-only. That's the gating item that makes Phase 1b's `--no-std` flag a
  compiler flag (drop those emissions + emit `#define TAURARO_KERNEL`), after which
  the CI boundary check (B) goes from informational to green/blocking.
- **Phase 1 — close the libc boundary.**
  - **1a — DONE ✅ (allocation + trap boundary).** All raw `malloc`/`calloc`/
    `realloc`/`free` now route through `TAURARO_ALLOC/FREE/REALLOC/CALLOC`; all raw
    `fprintf(stderr,…)`/`abort()` route through new `_TR_DIAG(…)`/`_TR_TRAP()` hooks
    (hosted = fprintf/abort; kernel = pr_err/BUG or no-op/spin). **Validated:** hosted
    is byte-identical (suite 16/16, smoke correct); a `-DTAURARO_KERNEL` build of a
    `Vec[int]` program with a custom bump allocator compiles with **zero** libc
    `malloc/calloc/realloc/abort/fprintf` references — the alloc/trap boundary is
    proven clean.
  - **1b — IN PROGRESS.** Additive-under-`TAURARO_KERNEL` pieces done (default build
    cannot regress — guards only *remove* under kernel; verified suite 16/16):
    - **`_TR_WRITE` output hook ✅** — the kernel `print` path now routes through a
      user-providable `_TR_WRITE` (default no-op; redefine to UART/semihosting).
      Completes the hook trilogy: alloc / trap / write.
    - **libc-lite ✅** — freestanding `strlen`/`memcpy`/`memmove`/`memset`/`memcmp`/
      `strcmp`/`strncmp`/`strchr` under `TAURARO_KERNEL && !__KERNEL__`.
    - **REMAINING (needs a real cross-compiler to validate — see Phase 0):** gate the
      OS-service sections behind `#ifndef TAURARO_KERNEL` / `TAURARO_NO_STD` — stdin
      input (`fgets`/prompt/`_tr_checked_alloc` block), file I/O
      (`fopen`/`fclose`/`fflush`/`_fileno`), env (`getenv`/`_putenv_s`), and the
      always-emitted top-level exception frame (`_tr_exc_push` — this one needs the
      codegen to not emit the frame under `--no-std`, or a no-op stub). Then add the
      `--no-std` compiler flag that emits the define.

### Authoritative bare-metal seam map (from CI, arm-none-eabi)

Part A of the CI gate is **GREEN** — `hello_std.tr` cross-compiles to aarch64 and
runs under qemu (`CROSS 81 42 cross ok`): the **std runtime is proven to
cross-compile and execute on ARM**. Part B compiled the freestanding program and
reported the true seam list (no MinGW flood). Note gcc *parses* every `static
inline` in the header, so this is the **whole runtime's** libc surface, not just a
minimal program's — which is why gating whole subsystems is the big lever:

1. **Gate std-only subsystems under `#ifndef TAURARO_KERNEL`** (biggest reduction):
   file I/O (`fopen`/`fclose`/`fflush`/`fgets`/`fread`/`fwrite`/`fseek`/`ftell`/
   `rewind`/`fputs`/`remove`/`rename`), channels (`_tr_chan_*`), threads
   (`_tr_thread_*`), timers (`_tr_ticker_new`/`_tr_timer_new`/`_tr_sleep_ms`), env
   (`getenv`), `exit`, `printf`. These are whole subsystems a freestanding program
   never parses. Additive-under-kernel → hosted can't regress.
2. **Complete libc-lite** (`TAURARO_KERNEL && !__KERNEL__`): ctype
   (`isalnum`/`isalpha`/`isdigit`/`islower`/`isupper`/`isspace`/`tolower`/`toupper`),
   string (`strcpy`/`strstr`/`strtok`/`strdup`), `isinf`/`isnan` (bit checks), and
   the hard one — **`snprintf`/`sprintf`** (integer formatting for `to_str`; a
   freestanding integer `snprintf` is tractable, float is the stretch). `qsort`/
   `rand`/`strtod`/`strtoll` belong to gate-able stdlib.
3. **Investigate first — the suspicious core seams.** `_tr_str_new`,
   `_tr_checked_alloc`, `_tr_obj_release`, `_tr_str_*`, and even raw `malloc`/`free`
   appear implicit — those are *defined* in the header, so their showing up implies
   the header may be **failing early** under `TAURARO_KERNEL` on arm-none-eabi (a
   likely culprit: `<stdatomic.h>` without atomic support, or a libc-lite/builtin
   clash), making everything after it implicit. Reproduce by compiling with plain
   `-Wimplicit-function-declaration` (not `-Werror`) and reading the *first* real
   error. Fix that before chasing the long list — it may collapse most of it.
4. **Codegen: gate the top-level exception frame** (`setjmp`/`longjmp`/
   `_tr_exc_push`) under `--no-std` — the compiler change that Phase 1b/4 owns.

> **Sequencing correction (learned this session):** MinGW is **not** a faithful
> freestanding target — its `<stdatomic.h>` pulls x86 intrinsic headers, flooding a
> `TAURARO_KERNEL` compile with unrelated `__builtin_ia32_*` errors. So the local
> smoke can prove the *alloc/trap boundary* (it did — 0 libc alloc/abort) but not a
> full freestanding link. **Do Phase 0 (arm-none-eabi / riscv cross-compiler in CI)
> before finishing 1b's OS-service gating**, so that gating is validated on a real
> bare-metal target instead of gated blind.
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
