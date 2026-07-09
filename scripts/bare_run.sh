#!/usr/bin/env bash
# Bare-metal LINK + RUN proof (Linux CI). Build examples/freestanding/hello_bare.tr
# for a Cortex-M3 (QEMU mps2-an385) — no libc, no OS, our own startup + linker +
# UART sink — and run it under qemu-system-arm. print() → _TR_WRITE → CMSDK UART →
# qemu stdout. This turns "the runtime compiles freestanding" into "a Tauraro
# binary runs on bare metal". See docs/dev/08.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
ARM="$(command -v arm-none-eabi-gcc || true)"
QEMU="$(command -v qemu-system-arm || true)"
if [ -z "$ARM" ] || [ -z "$QEMU" ]; then
    echo "(arm-none-eabi-gcc / qemu-system-arm not installed — skipping bare-metal run)"; exit 0
fi
MCU=tests/freestanding/mcu
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value -Wno-builtin-declaration-mismatch -Wno-int-conversion"

echo "=============================================================="
echo "  Bare-metal LINK + RUN — Cortex-M3 (mps2-an385), no libc/OS"
echo "=============================================================="
rm -rf build
# --freestanding auto-emits #define TAURARO_KERNEL into the generated C.
"$TAURAROC" examples/freestanding/hello_bare.tr --freestanding --emit c >/dev/null 2>&1 || { echo "FAIL: emit"; exit 1; }

# -nostdlib (no crt0/newlib — pure core tier) + our startup/platform; -lgcc supplies
# the ARMv7-M 64-bit arithmetic helpers (__aeabi_ldivmod etc.) the CPU lacks.
if ! "$ARM" -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib -fno-builtin \
      -T "$MCU/mps2.ld" -include "$MCU/mps2_hooks.h" $WARN -I build/include \
      -o build/bare.elf $(find build -name '*.c') "$MCU/startup_cm3.c" "$MCU/platform_mps2.c" -lgcc \
      2>/tmp/bare_cc.log; then
    echo "FAIL: bare-metal link"; sed -n '1,20p' /tmp/bare_cc.log; exit 1
fi
echo "  bare.elf linked OK ($(stat -c%s build/bare.elf 2>/dev/null || echo '?') bytes)"

# Run; the program spins after main() so bound it with a timeout and read the UART.
out="$(timeout 20 "$QEMU" -M mps2-an385 -cpu cortex-m3 -nographic -kernel build/bare.elf 2>&1 || true)"
echo "--- UART output ---"; echo "$out" | sed 's/^/    /'
if echo "$out" | grep -q "hello from bare metal" && echo "$out" | grep -q "sum 1..100 = 5050"; then
    echo "BARE-METAL RUN OK ✅ — a Tauraro binary executed on bare metal"
    exit 0
else
    echo "FAIL: expected UART output not seen"
    exit 1
fi
