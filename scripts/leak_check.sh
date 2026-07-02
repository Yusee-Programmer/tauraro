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
TAURAROC="${TAURAROC:-./tauraroc}"
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc.exe" ]; then TAURAROC="./tauraroc.exe"; fi
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc" ]; then TAURAROC="./tauraroc"; fi
CC="${CC:-gcc}"
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value"
LIBS="-lm"
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; esac

# Build one gate under a given flag ("" = optimized/elided, "--no-elide" = pure ARC),
# echo its runtime output, or "" on emit/compile failure.
build_run() {
    local src="$1" name="$2" flag="$3"
    rm -rf build
    "$TAURAROC" "$src" $flag --emit c >/dev/null 2>&1 || { echo "__EMITFAIL__"; return; }
    "$CC" -O2 -DTAURARO_MEMCOUNT -DTAURARO_NO_RT_HELPERS $WARN -I build/include \
        -o "build/$name.exe" $(find build -name '*.c') $LIBS >/dev/null 2>&1 \
        || { echo "__COMPILEFAIL__"; return; }
    "build/$name.exe" 2>&1
}

fail=0
for src in tests/leak/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    # 1) Optimized (borrow elision on).
    out="$(build_run "$src" "$name" "")"
    echo "  [elided]   $out"
    case "$out" in
        *"LEAK-GATE PASS"*) ;;
        *) echo "FAIL  $name (elided)"; fail=1; continue ;;
    esac
    # 2) DIFFERENTIAL soundness oracle: pure ARC (elision OFF) is the ground truth.
    #    It must ALSO be leak-free AND produce byte-identical output — if the elision
    #    ever dropped/duplicated a retain/release, the net counts would diverge here.
    out2="$(build_run "$src" "$name" "--no-elide")"
    echo "  [pure-ARC] $out2"
    case "$out2" in
        *"LEAK-GATE PASS"*) ;;
        *) echo "FAIL  $name (pure-ARC leaked/failed)"; fail=1; continue ;;
    esac
    if [ "$out" = "$out2" ]; then
        echo "PASS  $name (elided == pure-ARC)"
    else
        echo "FAIL  $name (elision UNSOUND: elided != pure-ARC)"; fail=1
    fi
done
rm -rf build
echo "==============================="
if [ "$fail" -eq 0 ]; then echo "Leak gate: all clean (elided == pure-ARC)."; else echo "Leak gate: LEAKS / UNSOUND ELISION DETECTED."; fi
exit $fail
