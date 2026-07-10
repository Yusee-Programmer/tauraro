#!/usr/bin/env bash
# Materialize the header-only Tauraro runtime into runtime.o with extern entry points,
# for the NATIVE (x86-64/ELF) and LLVM backends to link against. The C backend doesn't
# need this (it #includes the header); native/LLVM code calls the symbols instead.
# Run once; re-run when runtime/native_abi.c or the runtime header changes.
#
#   scripts/build_runtime_o.sh [OUT.o]      (default: build/runtime.o)
set -eu
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
CC="${CC:-cc}"
OUT="${1:-build/runtime.o}"
mkdir -p "$(dirname "$OUT")"
WARN="-Wno-attributes -Wno-unused-function -Wno-builtin-declaration-mismatch"
"$CC" -O2 -c $WARN -I runtime runtime/native_abi.c -o "$OUT"
echo "runtime.o -> $OUT ($(stat -c%s "$OUT" 2>/dev/null || echo '?') bytes)"
