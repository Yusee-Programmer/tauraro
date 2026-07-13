#!/usr/bin/env bash
# Native ARC leak check: compile a locals-only string program with the native backend,
# link it with runtime.o (built with the live-string counter) + a destructor probe, run
# it, and require the probe to report 0 leaked strings. Also implicitly catches double-
# frees/UAF (they crash -> non-zero exit). Linux/x86-64 only (native backend target).
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-cc}"; command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping native leak check)"; exit 0; }
case "$(uname -m 2>/dev/null || echo x)" in x86_64|amd64) : ;; *) echo "(non-x86_64 — skipping)"; exit 0 ;; esac

echo "=============================================================="
echo "  Native ARC leak check (refcounted strings, 0-leak assert)"
echo "=============================================================="
mkdir -p build
# runtime with the live-string counter active
"$CC" -O1 -DTAURARO_NMEM -I runtime -c runtime/native_abi.c -o build/runtime_nmem.o || { echo "FAIL: runtime.o"; exit 1; }

rc=0
for src in tests/native/arc_strings.tr; do
    name="$(basename "$src" .tr)"
    "$TAURAROC" "$src" --backend native -o "build/${name}_leak.o" || { echo "FAIL: native emit $name"; rc=1; continue; }
    if ! "$CC" "build/${name}_leak.o" build/runtime_nmem.o runtime/native_leak_probe.c -lm -o "build/${name}_leak" 2>/tmp/leak_ld.log; then
        echo "FAIL: link $name"; sed -n '1,10p' /tmp/leak_ld.log; rc=1; continue
    fi
    if "build/${name}_leak" >/dev/null; then
        echo "  ✓ $name: no leak, no crash"
    else
        echo "  ✗ $name: leak or crash (exit $?)"; rc=1
    fi
done

[ "$rc" -eq 0 ] && echo "NATIVE ARC LEAK CHECK OK ✅" || echo "NATIVE ARC LEAK CHECK FAILED"
exit $rc
