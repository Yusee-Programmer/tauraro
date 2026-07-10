#!/usr/bin/env bash
# Bare-metal RISC-V LINK + RUN proof (Linux CI) — a no-OS program in 100% Tauraro.
# examples/freestanding/riscv_hello/ is a --freestanding @entry program (platform:
# @allocator over a [u8; N] arena + NS16550 @output). The compiler generates the
# RISC-V _start boot path (hart parking, stack setup, .bss clear), the allocator/UART
# wiring, and the linker script. We link the emitted C for rv64 (no libc/OS, no
# C/.h/.ld shims) and run it under qemu-system-riscv64 'virt'. See docs/dev/08.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
GCC="$(command -v riscv64-unknown-elf-gcc || command -v riscv64-linux-gnu-gcc || true)"
QEMU="$(command -v qemu-system-riscv64 || true)"
if [ -z "$GCC" ] || [ -z "$QEMU" ]; then
    echo "(riscv64 gcc / qemu-system-riscv64 not installed — skipping RISC-V bare-metal run)"; exit 0
fi
# A Linux-targeted riscv gcc (riscv64-linux-gnu-gcc) can still build freestanding.
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value -Wno-builtin-declaration-mismatch -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-discarded-qualifiers"
APP="examples/freestanding/riscv_hello"

echo "=============================================================="
echo "  Bare-metal LINK + RUN — RISC-V (qemu virt, rv64), 100% Tauraro"
echo "=============================================================="
rm -rf "$APP/build"
( cd "$APP" && "$TAURAROC" main.tr --freestanding --target embedded-riscv64 --emit c --emit-ld build/app.ld >/dev/null 2>&1 ) \
    || { echo "FAIL: emit (--freestanding --target embedded-riscv64)"; exit 1; }
[ -f "$APP/build/app.ld" ] || { echo "FAIL: linker script not generated"; exit 1; }
echo "  emitted $(find "$APP/build" -name '*.c' | wc -l) C files, all from .tr"

# -fno-stack-protector: Ubuntu's gcc enables -fstack-protector-strong by default, which
# emits __stack_chk_fail/__stack_chk_guard refs unresolvable under -nostdlib. -fno-pic /
# -fno-pie avoids GOT relocations that need a dynamic loader.
# rv64imac_zicsr: the boot code uses `csrr … mhartid`; modern binutils split CSR
# instructions into the zicsr extension, so it must be named explicitly in -march.
if ! "$GCC" -O2 -march=rv64imac_zicsr -mabi=lp64 -mcmodel=medany -ffreestanding -nostdlib -fno-builtin \
      -fno-stack-protector -fno-pic -fno-pie \
      -T "$APP/build/app.ld" $WARN -I "$APP/build/include" -I "$APP/build" \
      -o "$APP/build/bare.elf" $(find "$APP/build" -name '*.c') -lgcc 2>/tmp/bare_rv_cc.log; then
    echo "FAIL: bare-metal link"
    echo "--- errors (compile + link) ---"
    grep -nE 'error:|undefined reference|ld returned|fatal error|collect2' /tmp/bare_rv_cc.log | head -40
    echo "--- log tail ---"
    tail -n 25 /tmp/bare_rv_cc.log
    exit 1
fi
echo "  bare.elf linked OK ($(stat -c%s "$APP/build/bare.elf" 2>/dev/null || echo '?') bytes) — from .tr only"

out="$(timeout 20 "$QEMU" -machine virt -bios none -nographic -kernel "$APP/build/bare.elf" 2>&1 || true)"
echo "--- UART output ---"; echo "$out" | sed 's/^/    /'
if echo "$out" | grep -q "fib(20) = 6765" && echo "$out" | grep -q "sum 1..100 = 5050" \
   && echo "$out" | grep -q "riscv_app: done"; then
    echo "BARE-METAL RUN OK ✅ — a 100%-Tauraro binary executed on bare-metal RISC-V"
    exit 0
else
    echo "FAIL: expected UART output not seen"
    exit 1
fi
