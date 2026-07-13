#!/usr/bin/env bash
# Security regression suite. Compiles the C-level runtime security tests and runs them,
# under AddressSanitizer + UBSan where supported (Linux), so memory-safety regressions in
# runtime/tauraro_rt.h surface as hard failures. See tests/security/*.c.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
CC="${CC:-cc}"
command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping security tests)"; exit 0; }
mkdir -p build

SAN=""
if [ "$(uname 2>/dev/null)" = "Linux" ]; then SAN="-fsanitize=address,undefined -fno-sanitize-recover=all"; fi

rc=0
for t in tests/security/*.c; do
    [ -f "$t" ] || continue
    name="$(basename "$t" .c)"
    # Tests that reference the native ARC counter must be linked with native_abi.c and
    # built with the live-string counter enabled; others are self-contained.
    if grep -q '_tr_rt_str_live_count' "$t"; then
        BUILD=("$CC" $SAN -O1 -g -DTAURARO_NMEM -I runtime runtime/native_abi.c "$t" -o "build/$name")
    else
        BUILD=("$CC" $SAN -O1 -g -I runtime "$t" -o "build/$name")
    fi
    if ! "${BUILD[@]}" 2>/tmp/sec_build.log; then
        echo "  ✗ $name: build failed"; sed -n '1,15p' /tmp/sec_build.log; rc=1; continue
    fi
    if "build/$name"; then
        echo "  ✓ $name passed"
    else
        echo "  ✗ $name FAILED (exit $?)"; rc=1
    fi
done

[ "$rc" -eq 0 ] && echo "SECURITY TESTS OK ✅" || echo "SECURITY TESTS FAILED"
exit $rc
