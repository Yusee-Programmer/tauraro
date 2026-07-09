#!/usr/bin/env bash
# Bare-metal LINK + RUN proof (Linux CI) — 100% Tauraro, no C/.h/.ld shims.
# examples/freestanding/mps2_pure.tr supplies its allocator (@allocator), UART sink
# (@output), and boot entry (@entry) in pure Tauraro; the compiler generates the
# reset trampoline + .isr_vector table and the linker script (--emit-ld). We link
# the emitted C for a Cortex-M3 (no libc/OS) and run it under qemu-system-arm.
# print() → _TR_WRITE → CMSDK UART → qemu stdout. See docs/dev/08.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
ARM="$(command -v arm-none-eabi-gcc || true)"
QEMU="$(command -v qemu-system-arm || true)"
if [ -z "$ARM" ] || [ -z "$QEMU" ]; then
    echo "(arm-none-eabi-gcc / qemu-system-arm not installed — skipping bare-metal run)"; exit 0
fi
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value -Wno-builtin-declaration-mismatch -Wno-int-conversion"

echo "=============================================================="
echo "  Bare-metal LINK + RUN — Cortex-M3 (mps2-an385), 100% Tauraro"
echo "=============================================================="
rm -rf build
# --freestanding emits #define TAURARO_KERNEL + wires @allocator/@output/@entry;
# --emit-ld generates the matching linker script. No hand-written C/.h/.ld.
"$TAURAROC" examples/freestanding/mps2_pure.tr --freestanding --emit c --emit-ld build/app.ld >/dev/null 2>&1 \
    || { echo "FAIL: emit"; exit 1; }
[ -f build/app.ld ] || { echo "FAIL: linker script not generated"; exit 1; }

# Pure link: only the emitted C + the generated linker script + libgcc (ARMv7-M
# 64-bit arithmetic helpers). No startup.c / platform.c / hooks.h.
if ! "$ARM" -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib -fno-builtin \
      -T build/app.ld $WARN -I build/include \
      -o build/bare.elf $(find build -name '*.c') -lgcc 2>/tmp/bare_cc.log; then
    echo "FAIL: bare-metal link"; sed -n '1,20p' /tmp/bare_cc.log; exit 1
fi
echo "  bare.elf linked OK ($(stat -c%s build/bare.elf 2>/dev/null || echo '?') bytes) — from .tr only"

out="$(timeout 20 "$QEMU" -M mps2-an385 -cpu cortex-m3 -nographic -kernel build/bare.elf 2>&1 || true)"
echo "--- UART output ---"; echo "$out" | sed 's/^/    /'
if echo "$out" | grep -q "hello from pure-Tauraro bare metal" && echo "$out" | grep -q "sum 1..100 = 5050"; then
    echo "BARE-METAL RUN OK ✅ — a 100%-Tauraro binary executed on bare metal"
    exit 0
else
    echo "FAIL: expected UART output not seen"
    exit 1
fi
