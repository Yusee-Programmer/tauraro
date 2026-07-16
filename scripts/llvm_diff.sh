#!/usr/bin/env bash
# LLVM backend differential oracle: every tests/native/*.tr compiled by BOTH the C backend
# and the LLVM backend must produce byte-identical stdout. The LLVM backend consumes the
# SAME lowered IR (src/taumir) as the native backend, so the native corpus is exactly its
# feature surface. A divergence means the LLVM IR emitter mistranslated an LIR op.
#
# Pipeline per test: .tr --backend llvm-> .ll --(clang | llc+cc)--> exe -> run.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-cc}"; command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping LLVM diff)"; exit 0; }
# Opaque pointers (`ptr`) require LLVM >= 15: pick the newest usable clang/llc, or
# skip gracefully (e.g. ubuntu-22.04-arm ships clang 14, which rejects `ptr` syntax).
_llvm_major() { "$1" --version 2>/dev/null | grep -oE '(clang|LLVM) version [0-9]+' | grep -oE '[0-9]+' | head -1; }
CLANGBIN=""
for c in clang clang-18 clang-17 clang-16 clang-15; do
    command -v "$c" >/dev/null 2>&1 || continue
    v="$(_llvm_major "$c")"; [ -n "$v" ] && [ "$v" -ge 15 ] && { CLANGBIN="$c"; break; }
done
LLCBIN=""
for l in llc llc-18 llc-17 llc-16 llc-15; do
    command -v "$l" >/dev/null 2>&1 || continue
    v="$(_llvm_major "$l")"; [ -n "$v" ] && [ "$v" -ge 15 ] && { LLCBIN="$l"; break; }
done
HAVE_CLANG=0; [ -n "$CLANGBIN" ] && HAVE_CLANG=1
HAVE_LLC=0;   [ -n "$LLCBIN" ]   && HAVE_LLC=1
if [ "$HAVE_CLANG" = 0 ] && [ "$HAVE_LLC" = 0 ]; then
    echo "(no clang/llc >= 15 on PATH — opaque pointers need LLVM 15+; skipping LLVM diff)"; exit 0
fi
TRIPLE=""
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) TRIPLE="x86_64-pc-windows-gnu";; esac

# runtime.o lives OUTSIDE build/ — the per-test `rm -rf build` (stale-state hygiene)
# must not delete it (that broke every link on CI with __CLANGFAIL__).
RTDIR="$(mktemp -d)"
RT="$RTDIR/runtime.o"
bash scripts/build_runtime_o.sh "$RT" >/dev/null || { echo "FAIL: runtime.o"; exit 1; }
trap 'rm -rf "$RTDIR"' EXIT

build_llvm_exe() {  # $1=.ll  $2=out-exe ; echoes "" on success or an error tag
    if [ "$HAVE_CLANG" = 1 ]; then
        local f="-O2"; [ -n "$TRIPLE" ] && f="$f -target $TRIPLE"
        "$CLANGBIN" $f "$1" "$RT" -lm -o "$2" >/tmp/ldiff.log 2>&1 || { echo "__CLANGFAIL__"; return; }
    else
        local f="-O2 -filetype=obj"; [ -n "$TRIPLE" ] && f="$f -mtriple=$TRIPLE"
        "$LLCBIN" $f "$1" -o "$2.o" >/tmp/ldiff.log 2>&1 || { echo "__LLCFAIL__"; return; }
        "$CC" "$2.o" "$RT" -lm -o "$2" >/tmp/ldiff.log 2>&1 || { echo "__LINKFAIL__"; return; }
    fi
    echo ""
}

pass=0; fail=0; skip=0
for src in tests/native/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    # C backend reference output (clean build/ first: stale-state hygiene).
    rm -rf build; "$TAURAROC" "$src" -o "/tmp/${name}_c" >/dev/null 2>&1 \
        || { echo "  C-FAIL   $name"; fail=1; continue; }
    c_out="$("/tmp/${name}_c" 2>&1)"
    # LLVM backend output (skip cleanly if the program is outside the LIR subset).
    rm -rf build
    if ! "$TAURAROC" "$src" --backend llvm -o "/tmp/${name}.ll" >/dev/null 2>&1; then
        echo "  skip     $name (LIR fallback)"; skip=$((skip+1)); continue
    fi
    err="$(build_llvm_exe "/tmp/${name}.ll" "/tmp/${name}_ll")"
    if [ -n "$err" ]; then echo "  BUILDERR $name ($err)"; sed -n '1,8p' /tmp/ldiff.log; fail=1; continue; fi
    l_out="$("/tmp/${name}_ll" 2>&1)"
    if [ "$c_out" = "$l_out" ]; then
        pass=$((pass+1))
    else
        echo "  MISMATCH $name"; echo "    C   : $(echo "$c_out" | head -3 | tr '\n' '|')"
        echo "    LLVM: $(echo "$l_out" | head -3 | tr '\n' '|')"; fail=1
    fi
done
echo "=============================================================="
echo "  LLVM diff: pass=$pass skip=$skip"
if [ "$fail" != 0 ]; then echo "  LLVM DIFFERENTIAL FAILED ❌"; exit 1; fi
echo "  LLVM ≡ C across the native corpus ✅"
