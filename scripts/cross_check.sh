#!/usr/bin/env bash
# Cross-compilation gate (Linux CI). Two independent checks:
#
#   A) EDGE / gateway (aarch64 Linux, full `std` tier) — cross-compile a real
#      heap-using program and RUN it under qemu. Proves the std runtime + ARC +
#      collections cross-compile and execute on ARM Linux. Blocking.
#
#   B) BARE-METAL boundary (arm-none-eabi, `core`/`alloc` tier) — compile a no-OS
#      program freestanding with a bump allocator and NO libc, using
#      -Werror=implicit-function-declaration so ANY libc leak is a hard error.
#      Reports the remaining libc seams. Informational until the compiler grows
#      `--no-std` (drop the exception frame / async cleanup); see
#      docs/dev/08_runtime_tiers_and_freestanding.md.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc.exe" ]; then TAURAROC="./tauraroc.exe"; fi
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value"
fail=0

echo "=============================================================="
echo "  A. Edge cross-compile — aarch64 Linux (std tier) + qemu run"
echo "=============================================================="
AA="$(command -v aarch64-linux-gnu-gcc || true)"
QEMU="$(command -v qemu-aarch64-static || command -v qemu-aarch64 || true)"
if [ -z "$AA" ]; then
    echo "  (aarch64-linux-gnu-gcc not installed — skipping)"
else
    rm -rf build
    "$TAURAROC" tests/cross/hello_std.tr --emit c >/dev/null 2>&1 || { echo "FAIL emit"; fail=1; }
    if "$AA" -O2 -static $WARN -I build/include -o build/hello_aa $(find build -name '*.c') -lm -lpthread 2>/tmp/aa_cc.log; then
        echo "  aarch64 static build: OK"
        if [ -n "$QEMU" ]; then
            out="$("$QEMU" build/hello_aa 2>&1)"; echo "  qemu run: $out"
            echo "$out" | grep -q "CROSS 81 42 cross ok" || { echo "FAIL: unexpected aarch64 output"; fail=1; }
        else
            echo "  (qemu-aarch64 not installed — built but not run)"
        fi
    else
        echo "FAIL: aarch64 cross-compile"; sed -n '1,8p' /tmp/aa_cc.log; fail=1
    fi
fi

echo "=============================================================="
echo "  B. Bare-metal boundary — arm-none-eabi (core tier), no libc"
echo "=============================================================="
ARM="$(command -v arm-none-eabi-gcc || true)"
if [ -z "$ARM" ]; then
    echo "  (arm-none-eabi-gcc not installed — skipping)"
else
    rm -rf build
    "$TAURAROC" tests/freestanding/vecsum.tr --emit c >/dev/null 2>&1 || echo "  emit issue"
    # Compile-only, freestanding, custom allocator, treat any implicit libc call as error.
    if "$ARM" -O2 -ffreestanding -DTAURARO_KERNEL -include tests/freestanding/fs_hooks.h \
         -Werror=implicit-function-declaration $WARN -I build/include \
         -c build/main.c -o /tmp/fs_main.o 2>/tmp/arm_cc.log; then
        echo "  BOUNDARY CLEAN — main.c compiles freestanding with no libc leak"
    else
        echo "  boundary report — remaining libc seams the compiler's --no-std must gate:"
        grep -oiE "implicit declaration of function '[a-z_0-9]+'" /tmp/arm_cc.log \
            | grep -viE "__builtin_ia32|_x(abort|test|begin|end)" | sort -u | sed 's/^/    /'
        echo "  (informational — not failing the build; tracked in docs/dev/08)"
    fi
fi

echo "=============================================================="
if [ "$fail" -eq 0 ]; then echo "Cross gate: edge (aarch64) OK."; else echo "Cross gate: FAILURES."; fi
exit $fail
