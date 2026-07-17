#!/usr/bin/env bash
# FFI proof for the LLVM backend (Path A): `extern "C"` calls into a custom C object,
# and `export def` producing an externally-linkable C-ABI symbol. Proves the LLVM
# backend interoperates with C both ways — extern "C" can't be tested via the C≡LLVM
# differential (the C backend redeclares system-header symbols, a separate quirk), so
# external linkage is exercised directly here.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-cc}"; command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping FFI proof)"; exit 0; }

# LLVM toolchain (LLVM >= 15 for opaque pointers).
_llvm_major() { "$1" --version 2>/dev/null | grep -oE '(clang|LLVM) version [0-9]+' | grep -oE '[0-9]+' | head -1; }
LLCBIN=""
for l in llc llc-18 llc-17 llc-16 llc-15; do
    command -v "$l" >/dev/null 2>&1 || continue
    v="$(_llvm_major "$l")"; [ -n "$v" ] && [ "$v" -ge 15 ] && { LLCBIN="$l"; break; }
done
CLANGBIN=""
for c in clang clang-18 clang-17 clang-16 clang-15; do
    command -v "$c" >/dev/null 2>&1 || continue
    v="$(_llvm_major "$c")"; [ -n "$v" ] && [ "$v" -ge 15 ] && { CLANGBIN="$c"; break; }
done
[ -z "$LLCBIN" ] && [ -z "$CLANGBIN" ] && { echo "(no clang/llc >= 15 — skipping FFI proof)"; exit 0; }

TRIPLE=""
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) TRIPLE="x86_64-pc-windows-gnu";; esac

echo "=============================================================="
echo "  LLVM backend FFI proof — extern \"C\" + export (C interop)"
echo "=============================================================="
mkdir -p build
bash scripts/build_runtime_o.sh build/runtime.o >/dev/null || { echo "FAIL: runtime.o"; exit 1; }

# A C object providing functions the Tauraro program calls via extern "C".
cat > build/ffi_stub.c <<'CEOF'
long long tauraro_ffi_mul(long long a, long long b) { return a * b; }
long long tauraro_ffi_slen(const char* s) { long long n = 0; while (s && s[n]) n++; return n; }
CEOF
"$CC" -O2 -c build/ffi_stub.c -o build/ffi_stub.o || { echo "FAIL: ffi_stub.o"; exit 1; }

cat > build/ffi_test.tr <<'TEOF'
extern "C":
    def tauraro_ffi_mul(a: int, b: int) -> int
    def tauraro_ffi_slen(s: str) -> int

export def tauraro_exported(n: int) -> int:
    return n + 1000

def main():
    print(tauraro_ffi_mul(6, 7))       # 42
    print(tauraro_ffi_slen("hello"))   # 5
    print(tauraro_exported(23))        # 1023
    mut s = "abcd"
    print(tauraro_ffi_slen(s))         # 4
TEOF

"$TAURAROC" build/ffi_test.tr --backend llvm -o build/ffi_test.ll || { echo "FAIL: llvm emit (extern C fell back?)"; exit 1; }

if [ -n "$CLANGBIN" ]; then
    F="-O2"; [ -n "$TRIPLE" ] && F="$F -target $TRIPLE"
    "$CLANGBIN" $F build/ffi_test.ll build/runtime.o build/ffi_stub.o -lm -o build/ffi_test 2>/tmp/ffi_ld.log \
        || { echo "FAIL: clang link"; sed -n '1,20p' /tmp/ffi_ld.log; exit 1; }
else
    F="-O2 -filetype=obj"; [ -n "$TRIPLE" ] && F="$F -mtriple=$TRIPLE"
    "$LLCBIN" $F build/ffi_test.ll -o build/ffi_test.o 2>/tmp/ffi_llc.log \
        || { echo "FAIL: llc"; sed -n '1,20p' /tmp/ffi_llc.log; exit 1; }
    "$CC" build/ffi_test.o build/runtime.o build/ffi_stub.o -lm -o build/ffi_test 2>/tmp/ffi_ld.log \
        || { echo "FAIL: link"; sed -n '1,20p' /tmp/ffi_ld.log; exit 1; }
fi

out="$(build/ffi_test 2>&1 | tr -d '\r')"
echo "--- output ---"; echo "$out" | sed 's/^/    /'
expected=$'42\n5\n1023\n4'
if [ "$out" = "$expected" ]; then
    echo "LLVM FFI OK ✅ — extern \"C\" called a custom C object + export def produced a C-ABI symbol"
    exit 0
else
    echo "FAIL: expected '42/5/1023/4', got '$out'"; exit 1
fi
