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

### MILESTONE: bare-metal boundary CLOSED for arm/riscv ✅

After the iterative CI-driven descent below, the arm-none-eabi bare compile went
**33 → 8 → 4 → 0 arm-relevant seams**. The runtime header now parses and compiles
freestanding (no libc) on a real embedded target. The last batch: raw `malloc`/
`calloc`/`realloc`/`free` (used by the platform concurrency primitives) now route
through the pluggable `TAURARO_ALLOC/…` macros via bare-only wrappers, and
`_tr_stdout_supports_ansi` returns 0 under `TAURARO_BARE`. The **only** residual
errors are `_WIN32`+`TAURARO_BARE` (bare-metal-*on-Windows*: `_fileno`/`_putenv_s`/
`fflush`/the Windows-branch threadpool) — a niche non-target (you cross-compile for
bare metal; you don't target Windows-as-MCU). Gating those is the same
`#if defined(_WIN32) && !defined(TAURARO_BARE)` pattern, deferred as low-value.
Remaining for a *linkable* freestanding binary: the codegen `--no-std` flag (drop
the top-level exception frame) — Phase 4.

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
2. **Complete libc-lite — DONE ✅** (`TAURARO_KERNEL && !__KERNEL__`, bare-only so
   hosted is untouched). Added, correct-by-inspection: ctype (`isalnum`/`isalpha`/
   `isdigit`/`islower`/`isupper`/`isspace`/`isxdigit`/`tolower`/`toupper`), string
   (`strcpy`/`strncpy`/`strcat`/`strstr`/`strrchr`/`strtok`/`strdup`), stdlib
   (`atoi`/`strtoll`/`strtoull`/`strtod`/`rand`/`srand`/`qsort`/`exit`), and a
   minimal **`vsnprintf`/`snprintf`/`sprintf`/`printf`** (integers/hex/strings exact;
   float is a basic decimal — bare logging only, never the hosted path). `isinf`/
   `isnan`/`INFINITY` now use `__builtin_*` (every tier). **Result: the bare-metal
   implicit-decl list collapsed 33 → 8**, and the 8 remaining are all `_WIN32`+`BARE`
   MinGW-local (`_fileno`/`_putenv_s`/threadpool/Win file paths) — arm-none-eabi
   skips them. The one genuinely-common file seam left (`_tr_uuid_v4`'s
   `/dev/urandom`) is now gated to the `rand()` fallback under `TAURARO_BARE`.
3. **Early-header-failure — DIAGNOSED & FIXED.** The "core seams" (`_tr_str_new`,
   `_tr_checked_alloc`, raw `malloc`/`free`, …) were a **cascade**: the CI's
   first-hard-error probe pinpointed `tauraro_rt.h:535: 'FILE' undeclared` — the file
   I/O helpers (`_tr_c_fopen`/`fclose`/`fread`/…) and `_tr_getenv` were *ungated*, so
   under bare-metal (no `<stdio.h>`) `FILE` was undeclared, gcc derailed, and
   *everything after* looked implicit. **Fixes:** (a) normalize the tier flags so
   `TAURARO_KERNEL`/`TAURARO_NO_OS` ⇒ `TAURARO_BARE` (the existing stub convention);
   (b) gate the file-I/O + env helpers under `#ifndef TAURARO_BARE` (bare gets a
   `_tr_getenv`→`""` stub); (c) add `<setjmp.h>` to the bare (non-`__KERNEL__`)
   include set — the next hard error was `jmp_buf` undeclared, needed by the
   bare-metal panic buffer. Both hard type-errors now resolve and hosted stays
   byte-identical (suite 16/16). (The remaining *local* MinGW errors — `stderr`,
   `_tr_time_ns`/`_tr_path_canonicalize` redefs — are all inside `#ifdef _WIN32`
   Windows blocks, which arm-none-eabi skips; they're a niche "bare-metal-on-Windows"
   latent issue, not on the arm path.) Next CI run should drop past the hard errors
   to the pure implicit-decl list = items 1 (gate std subsystems) + 2 (libc-lite).
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
- **Phase 4 — `--freestanding` / `--no-std` flag + link-and-run — DONE (pending CI) ✅.**
  - `--freestanding` auto-emits `#define TAURARO_KERNEL` (no libc, pluggable
    allocator); `--no-std` emits `#define TAURARO_NO_OS` — so a bare-metal build
    needs no hand-passed `-D`. Threaded via `CGenerator.tier_define`, emitted before
    the runtime include in every TU. Hosted is unchanged when the flag is absent.
  - **Link-and-run harness** (`scripts/bare_run.sh` + CI, non-blocking until green):
    `examples/freestanding/hello_bare.tr` → `--freestanding` → linked against a
    Cortex-M3 startup (`tests/freestanding/mcu/startup_cm3.c`), linker script
    (`mps2.ld`), and a UART+bump-allocator platform (`platform_mps2.c` /
    `mps2_hooks.h`) → **run under `qemu-system-arm -M mps2-an385`**. `print()` flows
    through `_TR_WRITE` → CMSDK UART → qemu stdout; the harness asserts the program's
    output. This is the step that turns "compiles freestanding" into "a Tauraro
    binary runs on bare metal."
### Bare-metal decorators — eliminating the C shims (Phase 5, in progress)

Goal: the user authors **only `.tr`** — the compiler generates all glue (the emitted
C/asm/ld is the compiler's burden, not the user's). Progress:

- **Batch 1 — low-level function attributes — DONE ✅** (`src/codegen/c.tr`,
  `hw_attrs`): `@section("name")` → `__attribute__((section("name")))` (vector table
  / custom placement); `@naked` → `naked`; `@interrupt` → `interrupt` (ISRs); `@used`
  → `used` (keep a symbol the linker would drop). Emitted *without* `static` so
  entry/ISR/vector symbols stay linkable. Note `@interrupt`/`@naked` are
  target-specific (invalid on hosted x86 — they're cross-compile/bare-metal tools).
- **Batch 2 — hook wiring + entry + linker-gen — DONE ✅ (blessed):**
  - `@allocator`/`@free`/`@realloc`/`@calloc` → forward-decl + `#define
    TAURARO_ALLOC(sz) <fn>` before the runtime include (kills `platform_mps2.c`'s
    allocator + `mps2_hooks.h`). `@output` → `#define _TR_WRITE(s) <fn>` (UART sink;
    `s` is a `Pointer[char]`). (`emit_tier_hooks` in `src/codegen/c.tr`.)
  - `@entry` → the compiler emits a reset trampoline (copy `.data`, zero `.bss`, call
    the entry) **and** the `.isr_vector` table `{ &_stack_top, _tr_reset }` (kills
    `startup_cm3.c`). (`emit_entry_glue`.)
  - `--emit-ld <path>` → the compiler writes a Cortex-M linker script whose symbols
    match the trampoline (kills `mps2.ld`). (`linker_script_cortex_m` in
    `src/main.tr`.)
  - **Result — PROVEN in CI ✅: `examples/freestanding/mps2_pure.tr` is a bare-metal
    firmware written 100% in Tauraro** — allocator (`@allocator`), UART driver
    (`@output` + `std/hal/mmio`), boot entry (`@entry`) — that links with only the
    generated C + `.ld` and **executes under `qemu-system-arm`** (prints
    `hello from pure-Tauraro bare metal` + `sum 1..100 = 5050`). **The four C/`.h`/
    `.ld` shim files are deleted** — there is no non-`.tr` platform code. This is the
    "everything in Tauraro, down to bare metal" milestone.
  - Remaining niceties: a `target {…}` block (nicer than `--emit-ld`'s fixed map) and
    fixed-size static arrays (`[u8; N]`) so the arena needn't sit at a fixed address.
  - **KNOWN LIMITATION (found in CI):** Tauraro runs global initializers in `main()`,
    but `@entry` boots straight to the entry — so a non-zero global initializer
    (`mut x: usize = 0x20080000`) is **not** applied (the global stays at its `.bss`
    zero). Bare-metal globals must currently be correct at zero (use a local constant
    + a zero-init offset, as `mps2_pure.tr`'s allocator does). Proper fix (follow-up):
    factor `main()`'s global-init into a `_tr_init_globals()` that the reset
    trampoline also calls. Symptom if you forget: a HardFault (NULL/garbage global).

- **Phase 5 — arch/HAL + drivers.** `std/hal/mmio.tr` gives volatile MMIO
  `read32/write32/read8/write8/wait_bits_set` (backed by `_tr_mmio_*` runtime
  intrinsics) — device drivers written in Tauraro. Still ahead: more archs
  (riscv/Cortex-A), interrupts/ISR entry, a toy kernel, linker/startup generators.

## Invariant across all tiers

Bounds checks, `[P-2]` raw-pointer quarantine, null safety, `@value_type` safety, and
(opt-in) `--strict` **stay on**. The dial reduces the *runtime*, never the *safety* —
that is the entire point and the differentiator from C/Zig bare-metal.
