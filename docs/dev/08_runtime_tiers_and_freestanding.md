# Runtime tiers & freestanding (`--no-std` / bare-metal) — internals

Tauraro has a *gradual runtime* dial that is orthogonal to the *gradual safety* dial
(`--strict`): the runtime can be turned **down** — to a firmware image with no OS and
no C library — without turning the safety **off**. The banner:
**memory-safe, Python-syntax, all the way down to bare metal.**

> **This doc is the internals.** The user-facing guide — how to *write* bare-metal
> Tauraro (the `@entry`/`@allocator`/`@output` decorators, `--emit-ld`, `std/hal/mmio`,
> a complete firmware) — is [lang/advanced/11_bare_metal.md](../lang/advanced/11_bare_metal.md).

## The three tiers (mirror Rust core/alloc/std)

| Tier | Flag | Has | Needs | Targets |
|---|---|---|---|---|
| `std` | *(default)* | ARC + collections + files/threads/net/reactor | OS + libc | userspace, servers, CLIs, **edge/gateway Linux** |
| `alloc` | `--no-std` | ARC + collections via a **pluggable allocator**; no OS services | an allocator only | RTOS, WASM, mobile cores |
| `core` | `--freestanding` | ARC + collections + `@value_type` over a pluggable allocator; no libc, no OS | nothing (freestanding) | MCUs, drivers, kernels |

`--freestanding` emits `#define TAURARO_KERNEL`; `--no-std` emits `#define
TAURARO_NO_OS`. Both are normalized to the internal `TAURARO_BARE` flag (see below).
The safety dial (`--strict`) is orthogonal and available at every tier — a
`--freestanding --strict` firmware passes the full borrow/lifetime/leak analysis and
emits a byte-identical binary.

## How the libc boundary is closed

Below `std`, no direct libc may leak — every allocation, trap, and byte of output
passes through a hook, and any libc function the header still needs is provided
freestanding. This all lives in `runtime/tauraro_rt.h`, so closing it there closes it
for every program.

- **Pluggable allocator.** `TAURARO_ALLOC/FREE/REALLOC/CALLOC` macros (default libc;
  `TAURARO_KERNEL` requires them to be supplied). Every allocation — including the raw
  `malloc`/`calloc`/`free` in the platform concurrency primitives — routes through
  these via bare-only `malloc`/`free`/… wrappers.
- **Trap + diagnostics.** `_TR_PANIC`/`_TR_OOM_ABORT`/`_TR_ASSERT` and `_TR_DIAG(…)`/
  `_TR_TRAP()` replace raw `fprintf(stderr,…)`/`abort()` (hosted = fprintf/abort;
  kernel = `pr_err`/`BUG` or no-op/spin).
- **Output.** The kernel `print` path routes through `_TR_WRITE(s)` (default no-op;
  redefine to a UART/semihosting sink).
- **libc-lite.** Under `TAURARO_KERNEL && !__KERNEL__` the header provides its own
  string (`strlen`/`memcpy`/`memmove`/`memset`/`memcmp`/`strcmp`/`strncmp`/`strchr`/
  `strcpy`/`strncpy`/`strcat`/`strstr`/`strrchr`/`strtok`/`strdup`), ctype
  (`isalnum`/`isdigit`/`tolower`/…), stdlib (`atoi`/`strtoll`/`strtod`/`rand`/`qsort`/
  `exit`), and a minimal `vsnprintf`/`snprintf`/`printf` (integers/hex/strings exact;
  float is a basic decimal — fine for logging, never the hosted path). `isinf`/`isnan`/
  `INFINITY` use `__builtin_*`, which work at every tier.
- **Subsystem gating.** OS services that need libc types — file I/O (`fopen`/`FILE`),
  stdin/stdout/tty, env (`getenv`), process — are gated behind `#ifndef TAURARO_BARE`
  with bare stubs, so a freestanding compile never parses them. `<setjmp.h>` is added
  to the bare (non-`__KERNEL__`) include set for the panic buffer.
- **Tier-flag normalization.** `TAURARO_KERNEL` and `TAURARO_NO_OS` both imply
  `TAURARO_BARE`, the single flag the gates key off. The allocator hook `#define`s are
  emitted into the shared `tauraro_types.h` so *every* translation unit — not just
  `main.c` — sees them before the runtime include.

Because gcc parses every `static inline` in the header, gating whole subsystems (not
individual functions) is what keeps a freestanding compile clean.

## Cross-compilation

The `std` tier cross-compiles as-is. `--target embedded-arm64`/`riscv`/`wasm-wasi`
selects the cross toolchain and flags. A real heap program (`Vec`, `Map`, `str`, ARC)
cross-compiles to aarch64 and executes on ARM — validated in CI (`scripts/cross_check.sh`
builds `tests/cross/hello_std.tr` with `aarch64-linux-gnu-gcc -static` and runs it
under qemu). Its companion check compiles `tests/freestanding/vecsum.tr` for
`arm-none-eabi` with a bump allocator and `-Werror=implicit-function-declaration`,
so any libc leak in the freestanding runtime is a hard error.

> A note on tooling: MinGW is not a faithful freestanding target (its `<stdatomic.h>`
> pulls x86 intrinsic headers, flooding a `TAURARO_KERNEL` compile with unrelated
> `__builtin_ia32_*` errors). The freestanding boundary is validated on a real
> cross-compiler (arm-none-eabi) in CI, not on the local host.

## Compiler-generated bare-metal glue (the decorators)

The user authors only `.tr`; the compiler generates the startup, vector table, hook
wiring, and linker script. The relevant codegen lives in `src/codegen/c.tr`
(`hw_attrs`, `emit_tier_hooks`, `emit_entry_glue`, `emit_global_inits`) and
`src/main.tr` (`linker_script_cortex_m`).

- **Function attributes.** `@section("name")` → `__attribute__((section("name")))`;
  `@naked` → `naked`; `@interrupt` → `interrupt`; `@used` → `used`. Emitted without
  `static` so entry/ISR/vector symbols stay linkable. (`@interrupt`/`@naked` are
  target-specific and invalid on hosted x86.)
- **Allocator + output wiring.** `@allocator`/`@free`/`@realloc`/`@calloc` and
  `@output` generate forward-declarations + `#define TAURARO_ALLOC(sz) <fn>` /
  `#define _TR_WRITE(s) <fn>` in the shared header, wiring the runtime's pluggable
  points to the user's Tauraro functions.
- **Boot entry.** `@entry` emits a reset trampoline (`_tr_reset`: copy `.data`, zero
  `.bss`, run the program's global initializers, then call the entry) plus the
  `.isr_vector` table `{ &_stack_top, _tr_reset }`. The global-init pass is shared
  with `main()` (`emit_global_inits`), so an `@entry` program applies non-zero global
  initializers exactly as a hosted program does.
- **Linker script.** `--emit-ld <path>` writes a Cortex-M linker script whose symbols
  (`_tr_reset`, `_stack_top`, `__bss_start__`/`__bss_end__`, `_sidata`/`_sdata`/
  `_edata`) match the trampoline.

Together these replace what would otherwise be hand-written `startup.c`, an allocator/
UART C file, a hooks header, and a linker script — for a firmware written 100% in
Tauraro.

## MMIO / device drivers

`std/hal/mmio.tr` exposes volatile `read32`/`write32`/`read8`/`write8`/`wait_bits_set`,
backed by `_tr_mmio_*` runtime intrinsics — the volatile register access device
drivers are built on.

## Validation (CI)

- **Edge cross-compile + run** — `hello_std.tr` builds for aarch64 and runs under qemu.
- **Freestanding boundary** — `vecsum.tr` compiles for arm-none-eabi with no libc leak.
- **Bare-metal link + run** — `examples/freestanding/mcu_app/` (a multi-module firmware:
  GPIO/SysTick drivers, CRC-32, a ring buffer, fixed-point math, formatted output) is
  built `--freestanding --strict` and executed under `qemu-system-arm -M mps2-an385`,
  asserting its UART output. This exercises the full runtime — strings, `Vec`, class
  instances, ARC — on a no-libc/no-OS Cortex-M3.

## Constraints

- A freestanding build **requires** a pluggable allocator (`@allocator` or
  `-DTAURARO_ALLOC=…`); the runtime `#error`s otherwise.
- Freestanding float formatting is a basic decimal (integers/hex are exact).
- Bare-metal-*on-Windows* (`_WIN32` + `TAURARO_BARE`) is not a target — a handful of
  Windows-only registers/functions are not gated for it. You cross-compile for bare
  metal; you do not target Windows-as-MCU.

## Invariant across all tiers

Bounds checks, `[P-2]` raw-pointer quarantine, null safety, `@value_type` safety, and
(opt-in) `--strict` **stay on**. The dial reduces the *runtime*, never the *safety* —
the differentiator from C/Zig bare-metal.
