#!/usr/bin/env bash
# Native backend LINK + RUN proof (Linux x86-64). Compiles a trivial Tauraro program with
# --backend native to an ELF64 object (NO C source, NO gcc for the user code), links it
# with runtime.o + crt + libc via cc, runs it, and asserts the output. Proves the
# taumir(LIR) -> x86-64 -> ELF -> link pipeline end to end. See project_native_backend.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-cc}"
command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping native run)"; exit 0; }

echo "=============================================================="
echo "  Native backend LINK + RUN — x86-64/ELF, 100% Tauraro (no C)"
echo "=============================================================="
mkdir -p build
printf 'def main():\n    print(42)\n' > /tmp/native_p42.tr

# 1) runtime.o — extern entry points to the header-only runtime (compiled once).
bash scripts/build_runtime_o.sh build/runtime.o || { echo "FAIL: runtime.o"; exit 1; }

# 2) native object emitted by Tauraro itself — no C, no gcc-of-user-code.
"$TAURAROC" /tmp/native_p42.tr --backend native -o build/native_p42.o || { echo "FAIL: native emit"; exit 1; }
[ -f build/native_p42.o ] || { echo "FAIL: no object emitted"; exit 1; }
echo "  emitted build/native_p42.o ($(stat -c%s build/native_p42.o 2>/dev/null || echo '?') bytes) — from .tr, no C"
echo "--- readelf ---"; readelf -hSr build/native_p42.o 2>/dev/null | grep -E 'Class:|Machine:|Type:|\.text|\.rela|\.symtab|R_X86_64|_tr_rt_print_i64' | sed 's/^/    /'

# 3) link with the system linker (crt + libc + runtime.o).
if ! "$CC" build/native_p42.o build/runtime.o -o build/native_p42 2>/tmp/native_ld.log; then
    echo "FAIL: link"; sed -n '1,20p' /tmp/native_ld.log; exit 1
fi

# 4) run.
out="$(build/native_p42 2>&1)"
echo "--- output ---"; echo "$out" | sed 's/^/    /'
if [ "$out" = "42" ]; then
    echo "NATIVE RUN OK ✅ — a Tauraro program compiled straight to x86-64/ELF (no C) printed 42"
    exit 0
else
    echo "FAIL: expected 42, got '$out'"; exit 1
fi
