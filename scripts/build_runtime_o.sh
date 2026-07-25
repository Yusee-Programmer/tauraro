#!/usr/bin/env bash
# Materialize the header-only Tauraro runtime into runtime.o with extern entry points,
# for the NATIVE (x86-64/ELF) and LLVM backends to link against. The C backend doesn't
# need this (it #includes the header); native/LLVM code calls the symbols instead.
# Run once; re-run when runtime/native_abi.c or the runtime header changes.
#
#   scripts/build_runtime_o.sh [OUT.o]      (default: build/runtime.o)
set -eu
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
CC="${CC:-cc}"; command -v "$CC" >/dev/null 2>&1 || CC=gcc
OUT="${1:-build/runtime.o}"
mkdir -p "$(dirname "$OUT")"
WARN="-Wno-attributes -Wno-unused-function -Wno-builtin-declaration-mismatch"
# coro_stub.c provides EXPORTED no-op coroutine entry points (_tr_co_*_h) for the
# synchronous-async model, in a SEPARATE TU so they don't collide with the header's
# file-local `static` versions. Merge both objects into the single runtime.o via `ld -r`.
if [ -f runtime/coro_stub.c ] && command -v ld >/dev/null 2>&1; then
    _tmpd="$(mktemp -d)"
    "$CC" -O2 -c $WARN -I runtime runtime/native_abi.c -o "$_tmpd/na.o"
    "$CC" -O2 -c $WARN runtime/coro_stub.c -o "$_tmpd/cs.o"
    ld -r "$_tmpd/na.o" "$_tmpd/cs.o" -o "$OUT"
    rm -rf "$_tmpd"
else
    "$CC" -O2 -c $WARN -I runtime runtime/native_abi.c -o "$OUT"
fi
echo "runtime.o -> $OUT ($(stat -c%s "$OUT" 2>/dev/null || echo '?') bytes)"
