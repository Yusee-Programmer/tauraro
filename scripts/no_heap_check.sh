#!/usr/bin/env bash
# no_heap_check.sh — CI oracle for the `--no-heap` compile-time zero-heap guarantee.
#
#   1. examples/freestanding/zero_heap/firmware.tr  MUST compile under --no-heap (and run).
#   2. examples/freestanding/zero_heap/violations.tr MUST be REJECTED with [H-1] diagnostics.
#
# Mirrors the other oracles:  TAURAROC=./tauraroc bash scripts/no_heap_check.sh
set -euo pipefail

TAURAROC="${TAURAROC:-./tauraroc}"
D="examples/freestanding/zero_heap"
export TAURARO_PATH="$(pwd)"          # the examples import no stdlib; the repo root suffices

pick_bin() { [ -f "$1" ] && echo "$1" || { [ -f "$1.exe" ] && echo "$1.exe"; }; }

echo "== [1/2] zero-heap firmware MUST compile under --no-heap =="
rm -rf build
"$TAURAROC" "$D/firmware.tr" --no-heap -o nh_firmware_check
BIN="$(pick_bin nh_firmware_check)"
[ -n "$BIN" ] || { echo "FAIL: firmware.tr did not build under --no-heap"; exit 1; }
echo "   compiled OK; run tail: $("./$BIN" | tail -1)"

echo "== [2/2] heap violations MUST be rejected under --no-heap =="
rm -rf build
if "$TAURAROC" "$D/violations.tr" --no-heap -o nh_violations_check >nh_err.txt 2>&1; then
    echo "FAIL: --no-heap did NOT reject violations.tr"; cat nh_err.txt; exit 1
fi
N=$(grep -c '\[H-1\]' nh_err.txt || true)
[ "${N:-0}" -ge 12 ] || { echo "FAIL: expected >= 12 [H-1] diagnostics, got ${N:-0}"; cat nh_err.txt; exit 1; }
echo "   correctly rejected with ${N}x [H-1]"

rm -f nh_firmware_check nh_firmware_check.exe nh_violations_check nh_violations_check.exe nh_err.txt
echo "no-heap oracle: PASS"
