#!/usr/bin/env bash
# Object-leak oracle for the LLVM backend (Path A). The C≡LLVM *output* differential
# CANNOT see a leaked or double-freed object — a mis-refcounted object still prints the
# right value — so object ARC needs its own memory oracle. This links the runtime with
# -DTAURARO_NMEM (live-object counter) and runs programs that allocate class/enum
# instances in loops; at exit the program prints _tr_rt_obj_live_count(), which must be
# 0 (every object that was allocated has been released). A non-zero count is a leak; a
# crash under this harness would be a double-free/UAF.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-gcc}"; command -v "$CC" >/dev/null 2>&1 || CC=cc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping object-leak oracle)"; exit 0; }

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
[ -z "$CLANGBIN" ] && [ -z "$LLCBIN" ] && { echo "(no clang/llc >= 15 — skipping object-leak oracle)"; exit 0; }

TRIPLE=""
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) TRIPLE="x86_64-pc-windows-gnu";; esac

echo "=============================================================="
echo "  LLVM backend object-leak oracle (-DTAURARO_NMEM)"
echo "=============================================================="
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
RT="$WORK/runtime_nmem.o"
"$CC" -O2 -c -DTAURARO_NMEM -Wno-attributes -Wno-unused-function -Wno-builtin-declaration-mismatch \
    -I runtime runtime/native_abi.c -o "$RT" 2>"$WORK/rt.log" \
    || { echo "FAIL: runtime.o (-DTAURARO_NMEM)"; sed -n '1,20p' "$WORK/rt.log"; exit 1; }

pass=0; fail=0
for T in tests/objleak/*.tr; do
    [ -e "$T" ] || { echo "(no tests/objleak/*.tr — nothing to check)"; exit 0; }
    name="$(basename "$T" .tr)"
    ll="$WORK/$name.ll"; bin="$WORK/$name.exe"
    if ! "$TAURAROC" "$T" --backend llvm -o "$ll" 2>"$WORK/$name.emit.log"; then
        echo "  $name: FAIL (llvm emit — fell back?)"; sed -n '1,8p' "$WORK/$name.emit.log"; fail=$((fail+1)); continue
    fi
    if [ -n "$CLANGBIN" ]; then
        F="-O2"; [ -n "$TRIPLE" ] && F="$F -target $TRIPLE"
        "$CLANGBIN" $F "$ll" "$RT" -lm -o "$bin" 2>"$WORK/$name.ld.log" \
            || { echo "  $name: FAIL (clang link)"; sed -n '1,8p' "$WORK/$name.ld.log"; fail=$((fail+1)); continue; }
    else
        F="-filetype=obj"; [ -n "$TRIPLE" ] && F="$F -mtriple=$TRIPLE"
        "$LLCBIN" $F "$ll" -o "$WORK/$name.o" 2>"$WORK/$name.llc.log" \
            || { echo "  $name: FAIL (llc)"; sed -n '1,8p' "$WORK/$name.llc.log"; fail=$((fail+1)); continue; }
        "$CC" "$WORK/$name.o" "$RT" -lm -o "$bin" 2>"$WORK/$name.ld.log" \
            || { echo "  $name: FAIL (link)"; sed -n '1,8p' "$WORK/$name.ld.log"; fail=$((fail+1)); continue; }
    fi
    out="$("$bin" 2>&1 | tr -d '\r')"; rc=$?
    live="$(printf '%s\n' "$out" | tail -1)"
    if [ "$rc" -ne 0 ]; then
        echo "  $name: FAIL (exit $rc — possible double-free/UAF)"; printf '%s\n' "$out" | sed 's/^/      /'; fail=$((fail+1)); continue
    fi
    if [ "$live" = "0" ]; then
        echo "  $name: OK (0 live objects at exit)"; pass=$((pass+1))
    else
        echo "  $name: LEAK ($live live objects at exit)"; fail=$((fail+1))
    fi
done

echo "--------------------------------------------------------------"
echo "object-leak oracle: pass=$pass leak/fail=$fail"
[ "$fail" -eq 0 ] && { echo "Object ARC: all clean (net-zero live objects) ✅"; exit 0; }
echo "Object ARC: LEAKS/failures present ❌"; exit 1
