# Advanced — Bare-Metal & Freestanding

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

Tauraro has a *gradual runtime* dial that is orthogonal to the *gradual safety* dial (`--strict`). You can turn the **runtime** down — all the way to a firmware image with no operating system and no C library — **without turning the safety off**. Bounds checks, the `[P-2]` raw-pointer quarantine, null safety, and `@value_type` safety stay on at every tier.

The banner: **memory-safe, Python-syntax, all the way down to bare metal.** A Tauraro program can be a web server *and* a Cortex-M firmware image, and the firmware is written 100% in Tauraro — the compiler generates the reset vector, the startup trampoline, the allocator wiring, and the linker script.

### The three tiers (mirrors Rust `std` / `alloc` / `core`)

| Tier | Flag | Has | Needs | Targets |
|---|---|---|---|---|
| `std` | *(default)* | ARC + collections + files/threads/net/reactor | an OS + libc | servers, CLIs, desktop, **edge/gateway Linux** |
| `alloc` | `--no-std` | ARC + collections via a **pluggable allocator**; no OS services | an allocator only | RTOS, WASM, mobile compute cores |
| `core` | `--freestanding` | no OS, **no libc** — pluggable allocator + your own I/O | nothing (freestanding) | **MCUs, drivers, kernels** |

`--freestanding` emits `#define TAURARO_KERNEL`; `--no-std` emits `#define TAURARO_NO_OS`. You do not hand-pass those defines — the flag does it.

---

## When You Need This

- Cross-compiling to **ARM/RISC-V Linux** (edge nodes, gateways, mobile native libraries) — this is the `std` tier, no `--freestanding` required.
- Writing **microcontroller firmware** (Cortex-M, RISC-V MCUs) with no OS.
- Writing **device drivers** — memory-mapped I/O over hardware registers.
- Bringing up a **toy kernel** or a freestanding component.

If you are writing an application, a service, or a CLI, you do not need any of this — the default `std` tier is what you want.

---

## Cross-compilation (the `std` tier on another target)

The `std` tier already cross-compiles. To build for another Linux target, pass `--target`:

```bash
tauraroc app.tr --target embedded-arm64 -o app       # aarch64 Linux
tauraroc app.tr --target wasm-wasi       -o app.wasm  # WASI
```

`--target` selects the cross toolchain and the right flags automatically. A real heap program (`Vec`, `Map`, `str`, ARC) cross-compiles to aarch64 and runs unchanged — this path is regression-gated in CI (built + executed under qemu). This is the tier most "embedded Linux" work lives in.

---

## Freestanding: the bare-metal decorators

Below `std`, there is no OS to call `main()`, no libc `malloc`, and no `stdout`. Rather than make you hand-write C startup files, Tauraro lets you supply those pieces **as Tauraro functions**, marked with decorators. *The compiler carries the burden* — it generates all the glue.

### `@entry` — the boot entry point

Marks the function the hardware boots into. The compiler emits a **reset trampoline** (copy `.data`, zero `.bss`, run global initializers, then call your function) **and** the interrupt vector table (`{ &_stack_top, _tr_reset }`) in the `.isr_vector` section. You write no `startup.c` and no assembly.

```python
@entry
def kernel_main():
    print("hello from bare metal")
```

Global initializers run in the trampoline, so a non-zero module global (`mut base: usize = 0x40000000`) is applied exactly as it would be in a hosted program.

### `@allocator` / `@free` / `@realloc` / `@calloc` — the allocator

Bare-metal has no libc `malloc`. Mark your allocator functions and the compiler wires the runtime's `TAURARO_ALLOC`/`FREE`/`REALLOC`/`CALLOC` to them (`#define` + forward-declaration before the runtime include, in the shared header so every module sees it). Your functions must use basic C types (`usize`, `Pointer[u8]`).

```python
mut _heap_next: usize = 0x20080000 as usize

@allocator
def heap_alloc(n: usize) -> Pointer[u8]:
    unsafe:
        mut p = _heap_next as Pointer[u8]
        _heap_next = _heap_next + ((n + 7) / 8) * 8   # 8-byte aligned bump
        return p
@free
def heap_free(p: Pointer[u8]):
    pass                                              # bump allocator: no per-object free
@realloc
def heap_realloc(p: Pointer[u8], n: usize) -> Pointer[u8]:
    return heap_alloc(n)
@calloc
def heap_calloc(n: usize, sz: usize) -> Pointer[u8]:
    return heap_alloc(n * sz)
```

With an allocator in place, the full runtime — `str`, `Vec`, `Map`, classes, ARC — works on bare metal.

### `@output` — where `print()` goes

There is no `stdout`. Mark one function as the byte sink and the compiler routes the runtime's `_TR_WRITE` (which `print()` flows through) to it. It receives a NUL-terminated `Pointer[char]`.

```python
@output
def uart_write(s: Pointer[char]):
    unsafe:
        mut i = 0
        while s.offset(i).read() != (0 as char):
            write32(0x40004000 as usize, s.offset(i).read() as u32)   # UART data register
            i = i + 1
```

### Low-level attributes: `@section`, `@naked`, `@interrupt`, `@used`

These map straight to C/GCC attributes (emitted without `static`, so entry/ISR/vector symbols stay linkable):

| Decorator | Emits | Use |
|---|---|---|
| `@section("name")` | `__attribute__((section("name")))` | Place a function in a named linker section |
| `@naked` | `__attribute__((naked))` | A function with no prologue/epilogue (expert / inline-asm) |
| `@interrupt` | `__attribute__((interrupt))` | An interrupt service routine (target-specific) |
| `@used` | `__attribute__((used))` | Keep a symbol the linker would otherwise drop |

> `@interrupt` and `@naked` are target-specific and are **invalid on hosted x86** — they are cross-compile/bare-metal tools.

---

## Linker script generation (`--emit-ld`)

The linker script is a linker *input*, not code — but you should not hand-write it. `--emit-ld <path>` makes the compiler generate a Cortex-M linker script whose symbols (`_tr_reset`, `_stack_top`, `__bss_start__`/`__bss_end__`, `_sidata`/`_sdata`/`_edata`) exactly match the `@entry` trampoline:

```bash
tauraroc firmware.tr --freestanding --emit c --emit-ld build/app.ld
```

---

## MMIO — writing device drivers

`std/hal/mmio` gives **volatile** memory-mapped I/O — reads and writes the compiler never elides or reorders. It is how you write drivers in Tauraro.

```python
from std.hal.mmio import read32, write32, wait_bits_set

# A GPIO driver over hardware registers.
def led_on():
    write32(0x40010000 as usize, 1 as u32)
def led_read() -> u32:
    return read32(0x40010000 as usize)
```

`read8`/`write8`/`wait_bits_set` are also provided. The addresses are raw device registers the caller vouches for — this is the safe surface over what would otherwise be raw `unsafe` pointer arithmetic.

---

## A complete firmware, 100% Tauraro

```python
# firmware.tr — builds with:
#   tauraroc firmware.tr --freestanding --emit c --emit-ld build/app.ld
#   arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -nostdlib -fno-builtin \
#       -T build/app.ld -I build/include -o app.elf $(find build -name '*.c') -lgcc
#   qemu-system-arm -M mps2-an385 -nographic -kernel app.elf
from std.hal.mmio import write32

mut _heap_next: usize = 0x20080000 as usize
@allocator
def heap_alloc(n: usize) -> Pointer[u8]:
    unsafe:
        mut p = _heap_next as Pointer[u8]
        _heap_next = _heap_next + ((n + 7) / 8) * 8
        return p
@free
def heap_free(p: Pointer[u8]): pass
@realloc
def heap_realloc(p: Pointer[u8], n: usize) -> Pointer[u8]: return heap_alloc(n)
@calloc
def heap_calloc(n: usize, sz: usize) -> Pointer[u8]: return heap_alloc(n * sz)

@output
def uart_write(s: Pointer[char]):
    unsafe:
        write32(0x40004008 as usize, 1 as u32)     # TX enable
        mut i = 0
        while s.offset(i).read() != (0 as char):
            write32(0x40004000 as usize, s.offset(i).read() as u32)
            i = i + 1

@entry
def boot():
    mut total = 0
    mut i = 1
    while i <= 100:
        total = total + i
        i = i + 1
    print("sum 1..100 = " + total.to_str())   # allocator + to_str + concat + UART
```

For a **comprehensive multi-module** version — a GPIO driver, a SysTick timer, CRC-32, a ring buffer, fixed-point math, and formatted output split across imported sub-modules — see [`examples/freestanding/mcu_app/`](../../../examples/freestanding/mcu_app). Both examples run on `qemu-system-arm -M mps2-an385`, regression-gated in CI.

---

## Gotchas

- **Use a pluggable allocator, not libc.** Under `--freestanding` the runtime `#error`s if `TAURARO_ALLOC/…` are undefined — supply them with `@allocator`/`@free`/`@realloc`/`@calloc`.
- **Float formatting is basic** in the freestanding `snprintf`-lite (integers/hex/strings are exact). Fine for logging; not IEEE-precise. Hosted uses the real libc.
- **`@interrupt`/`@naked` don't compile on hosted x86** — they are only valid on the cross target.
- **Link with `-lgcc`** on Cortex-M for the 64-bit arithmetic helpers the CPU lacks.

---

## The safety dial is orthogonal — and it works on bare metal

`--strict`, bounds checks, `[P-2]` raw-pointer quarantine, null safety, and `@value_type` safety **stay on** at every tier. The dial reduces the *runtime*, never the *safety* — the differentiator from C/Zig bare-metal.

This is not aspirational — it is CI-proven. The comprehensive `mcu_app` firmware **compiles under `--freestanding --strict`**:

```bash
tauraroc main.tr --freestanding --strict --emit c --emit-ld build/app.ld
```

It passes the full safety analysis — the borrow-checker (`[B-*]`), lifetimes (`[L-*]`), the `[P-2]` raw-pointer quarantine (the allocator's and UART's raw accesses are correctly confined to `unsafe:`), and `[S-2]` leak-freedom — and emits a **byte-identical binary** to the non-strict build (the checks are compile-time only). So you get **Rust-level compile-time safety guarantees on firmware with no OS and no libc** — a combination no mainstream systems language offers. The `bare_run.sh` CI gate builds `mcu_app` this way.

For the internals (the tier defines, the libc boundary, how the seams were closed), see [`docs/dev/08_runtime_tiers_and_freestanding.md`](../../dev/08_runtime_tiers_and_freestanding.md).

---

← [Macros](10_macros.md) | [Advanced Docs Index](README.md)
