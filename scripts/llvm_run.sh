#!/usr/bin/env bash
# LLVM backend (Path A) LINK + RUN proof. Compiles a Tauraro program with --backend llvm
# to textual LLVM IR (NO C source for the user code), lowers it to an object and links it
# with runtime.o (the SAME runtime the native backend uses), runs it, and asserts output.
# Proves the taumir(LIR) -> LLVM IR -> llc/clang -> link pipeline end to end.
#
# Unlike the native backend, LLVM targets the host, so this runs on any x86-64 host
# (including Windows/mingw) — not just Linux.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-cc}"; command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping LLVM run)"; exit 0; }

# Need an LLVM toolchain: clang (compiles .ll directly) OR llc (+ CC to link).
HAVE_CLANG=0; command -v clang >/dev/null 2>&1 && HAVE_CLANG=1
HAVE_LLC=0;   command -v llc   >/dev/null 2>&1 && HAVE_LLC=1
if [ "$HAVE_CLANG" = 0 ] && [ "$HAVE_LLC" = 0 ]; then
    echo "(no clang/llc on PATH — skipping LLVM run)"; exit 0
fi

# On mingw/msys the runtime.o + final exe must use the windows-gnu ABI; elsewhere the host
# default triple is correct.
TRIPLE=""
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) TRIPLE="x86_64-pc-windows-gnu";; esac

echo "=============================================================="
echo "  LLVM backend LINK + RUN — taumir LIR -> LLVM IR (no C)"
echo "=============================================================="
mkdir -p build
# Exercises the LLVM backend across the LIR subset: string vars+concat, recursion, params/
# returns, loops, if/else, arithmetic, List[int], f-strings, and a match statement.
cat > /tmp/llvm_p.tr <<'EOF'
def fib(n: int) -> int:
    if n < 2:
        return n
    return fib(n - 1) + fib(n - 2)

def kind(n: int) -> str:
    match n % 2:
        case 0:
            return "even"
        case _:
            return "odd"

def main():
    mut a = "hello"
    mut b = " llvm"
    print(a + b)              # hello llvm
    print(fib(10))           # 55
    mut xs = [10, 20, 30]
    xs.push(40)
    mut i = 0
    mut s = 0
    while i < xs.len:
        s = s + xs[i]
        i = i + 1
    print(s)                 # 100
    print(f"fib10={fib(10)} {kind(7)}")   # fib10=55 odd
    print("done")
EOF

bash scripts/build_runtime_o.sh build/runtime.o || { echo "FAIL: runtime.o"; exit 1; }

"$TAURAROC" /tmp/llvm_p.tr --backend llvm -o build/llvm_p.ll || { echo "FAIL: llvm emit"; exit 1; }
[ -f build/llvm_p.ll ] || { echo "FAIL: no IR emitted"; exit 1; }
echo "  emitted build/llvm_p.ll ($(wc -c < build/llvm_p.ll) bytes) — from .tr, no C"

# Build an executable: prefer clang (accepts .ll directly, applies -O2); else llc -> obj -> CC.
if [ "$HAVE_CLANG" = 1 ]; then
    CLANG_FLAGS="-O2"; [ -n "$TRIPLE" ] && CLANG_FLAGS="$CLANG_FLAGS -target $TRIPLE"
    clang $CLANG_FLAGS build/llvm_p.ll build/runtime.o -lm -o build/llvm_p 2>/tmp/llvm_ld.log \
        || { echo "FAIL: clang compile/link"; sed -n '1,20p' /tmp/llvm_ld.log; exit 1; }
else
    LLC_FLAGS="-O2 -filetype=obj"; [ -n "$TRIPLE" ] && LLC_FLAGS="$LLC_FLAGS -mtriple=$TRIPLE"
    llc $LLC_FLAGS build/llvm_p.ll -o build/llvm_p.o 2>/tmp/llvm_llc.log \
        || { echo "FAIL: llc"; sed -n '1,20p' /tmp/llvm_llc.log; exit 1; }
    "$CC" build/llvm_p.o build/runtime.o -lm -o build/llvm_p 2>/tmp/llvm_ld.log \
        || { echo "FAIL: link"; sed -n '1,20p' /tmp/llvm_ld.log; exit 1; }
fi

out="$(build/llvm_p 2>&1 | tr -d '\r')"
echo "--- output ---"; echo "$out" | sed 's/^/    /'
expected=$'hello llvm\n55\n100\nfib10=55 odd\ndone'
if [ "$out" = "$expected" ]; then
    echo "LLVM RUN OK ✅ — a Tauraro program (strings, recursion, loops, List, f-strings, match) went .tr -> LLVM IR -> native code and ran correctly"
    exit 0
else
    echo "FAIL: expected 'hello llvm/55/100/fib10=55 odd/done', got '$out'"; exit 1
fi
