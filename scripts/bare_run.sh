#!/usr/bin/env bash
# Bare-metal LINK + RUN proof (Linux CI) — a COMPREHENSIVE multi-module program in
# 100% Tauraro. examples/freestanding/mcu_app/ is a main module (platform:
# @allocator/@output/@entry) that imports three sub-modules (console, mathx, a
# sysinfo MMIO driver). The compiler generates the reset trampoline, .isr_vector
# table, allocator/UART wiring, and the linker script. We link the emitted C for a
# Cortex-M3 (no libc/OS, no C/.h/.ld shims) and run it under qemu-system-arm. See
# docs/dev/08.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
# absolute-ize so it still resolves after we cd into the app dir
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
ARM="$(command -v arm-none-eabi-gcc || true)"
QEMU="$(command -v qemu-system-arm || true)"
if [ -z "$ARM" ] || [ -z "$QEMU" ]; then
    echo "(arm-none-eabi-gcc / qemu-system-arm not installed — skipping bare-metal run)"; exit 0
fi
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value -Wno-builtin-declaration-mismatch -Wno-int-conversion"
APP="examples/freestanding/mcu_app"

echo "=============================================================="
echo "  Bare-metal LINK + RUN — Cortex-M3, multi-module, 100% Tauraro"
echo "=============================================================="
rm -rf "$APP/build"
# Compiled from the app dir so `from console import …` (local siblings) resolve.
( cd "$APP" && "$TAURAROC" main.tr --freestanding --emit c --emit-ld build/app.ld >/dev/null 2>&1 ) \
    || { echo "FAIL: emit"; exit 1; }
[ -f "$APP/build/app.ld" ] || { echo "FAIL: linker script not generated"; exit 1; }
echo "  emitted $(find "$APP/build" -name '*.c' | wc -l) C files (main + sub-modules), all from .tr"

if ! "$ARM" -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib -fno-builtin \
      -T "$APP/build/app.ld" $WARN -I "$APP/build/include" -I "$APP/build" \
      -o "$APP/build/bare.elf" $(find "$APP/build" -name '*.c') -lgcc 2>/tmp/bare_cc.log; then
    echo "FAIL: bare-metal link"; sed -n '1,20p' /tmp/bare_cc.log; exit 1
fi
echo "  bare.elf linked OK ($(stat -c%s "$APP/build/bare.elf" 2>/dev/null || echo '?') bytes) — from .tr only"

out="$(timeout 20 "$QEMU" -M mps2-an385 -cpu cortex-m3 -nographic -kernel "$APP/build/bare.elf" 2>&1 || true)"
echo "--- UART output ---"; echo "$out" | sed 's/^/    /'
if echo "$out" | grep -q "fib(20) = 6765" && echo "$out" | grep -q "sum 1..100 = 5050" \
   && echo "$out" | grep -q "is_prime(97) = 1" && echo "$out" | grep -q "mcu_app: done"; then
    echo "BARE-METAL RUN OK ✅ — a multi-module 100%-Tauraro binary executed on bare metal"
    exit 0
else
    echo "FAIL: expected UART output not seen"
    exit 1
fi
