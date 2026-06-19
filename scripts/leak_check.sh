#!/usr/bin/env bash
# Leak gate (Phase 0): build tests/leak/*.tr with -DTAURARO_MEMCOUNT (so the
# runtime tracks net live allocations) and fail if any gate reports a leak.
#
# A gate program runs a leak-prone workload in a loop and prints
# "LEAK-GATE PASS" when net live allocations did not grow, or "LEAK-GATE FAIL".
# This is a regression detector: a real leak grows the counter (positive),
# which fails the gate. Run from the repo root: bash scripts/leak_check.sh
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc.exe}"
CC="${CC:-gcc}"
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value"
LIBS="-lm"
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; esac

fail=0
for src in tests/leak/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    rm -rf build
    "$TAURAROC" "$src" --emit c >/dev/null 2>&1 || { echo "FAIL  $name (emit)"; fail=1; continue; }
    "$CC" -O2 -DTAURARO_MEMCOUNT -DTAURARO_NO_RT_HELPERS $WARN -I build/include \
        -o "build/$name.exe" $(find build -name '*.c') $LIBS >/dev/null 2>&1 \
        || { echo "FAIL  $name (compile)"; fail=1; continue; }
    out="$("build/$name.exe" 2>&1)"
    echo "  $out"
    case "$out" in
        *"LEAK-GATE PASS"*) echo "PASS  $name" ;;
        *)                  echo "FAIL  $name"; fail=1 ;;
    esac
done
rm -rf build
echo "==============================="
if [ "$fail" -eq 0 ]; then echo "Leak gate: all clean."; else echo "Leak gate: LEAKS DETECTED."; fi
exit $fail
