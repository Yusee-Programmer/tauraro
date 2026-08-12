#!/usr/bin/env bash
# ARC hardening: AddressSanitizer over the LLVM backend. clang instruments the emitted
# LLVM IR with -fsanitize=address, so a use-after-free / double-free introduced by object
# refcounting ABORTS at runtime — catching the one failure mode the memcount oracle (leaks
# only) and the C≡LLVM output differential (a UAF can still print the right value) both
# miss. Runs the objleak corpus + the native differential corpus. On platforms without a
# working ASan (e.g. some mingw clangs) it skips cleanly. LeakSanitizer (leak reports) is
# NOT relied on here — that is the memcount oracle's job; this pass is about UAF/DF.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac

_llvm_major() { "$1" --version 2>/dev/null | grep -oE '(clang|LLVM) version [0-9]+' | grep -oE '[0-9]+' | head -1; }
CLANGBIN=""
for c in clang clang-18 clang-17 clang-16 clang-15; do
    command -v "$c" >/dev/null 2>&1 || continue
    v="$(_llvm_major "$c")"; [ -n "$v" ] && [ "$v" -ge 15 ] && { CLANGBIN="$c"; break; }
done
[ -z "$CLANGBIN" ] && { echo "(no clang >= 15 — skipping ASan hardening)"; exit 0; }

TRIPLE=""
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) TRIPLE="x86_64-pc-windows-gnu";; esac

WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
# ASan-instrumented runtime (LeakSanitizer off — we only assert UAF/DF here).
RT="$WORK/runtime_asan.o"
AF="-O1 -g -fsanitize=address"; [ -n "$TRIPLE" ] && AF="$AF -target $TRIPLE"
if ! "$CLANGBIN" $AF -DTAURARO_NMEM -Wno-attributes -Wno-unused-function \
        -Wno-builtin-declaration-mismatch -I runtime -c runtime/native_abi.c -o "$RT" 2>"$WORK/rt.log"; then
    echo "(clang -fsanitize=address unavailable on this toolchain — skipping ASan hardening)"
    sed -n '1,4p' "$WORK/rt.log"; exit 0
fi

# Probe: can we actually LINK + RUN an ASan binary here? If not, skip cleanly.
echo 'int main(void){return 0;}' > "$WORK/probe.c"
if ! "$CLANGBIN" $AF "$WORK/probe.c" -o "$WORK/probe" 2>"$WORK/probe.log" || ! "$WORK/probe" >/dev/null 2>&1; then
    echo "(ASan runtime can't link/run on this toolchain — skipping ASan hardening)"; exit 0
fi

echo "=============================================================="
echo "  ARC hardening: AddressSanitizer over the LLVM backend"
echo "=============================================================="
export ASAN_OPTIONS="detect_leaks=0:abort_on_error=1:exitcode=99"
pass=0; fail=0
run_one() {  # $1=.tr  $2=label
    local src="$1" name="$2"
    local ll="$WORK/$name.ll" bin="$WORK/$name.exe"
    "$TAURAROC" "$src" --backend llvm -o "$ll" >/dev/null 2>&1 || { echo "  $name: skip (LIR fallback)"; return; }
    "$CLANGBIN" $AF "$ll" "$RT" -lm -o "$bin" 2>"$WORK/$name.ld" || { echo "  $name: skip (asan link)"; return; }
    if "$bin" >/dev/null 2>"$WORK/$name.run"; then
        echo "  $name: OK"; pass=$((pass+1))
    else
        echo "  $name: ASAN FAULT (UAF/double-free)"; sed -n '1,12p' "$WORK/$name.run"; fail=$((fail+1))
    fi
}
for T in tests/objleak/*.tr; do [ -e "$T" ] && run_one "$T" "obj_$(basename "$T" .tr)"; done
for T in tests/native/*.tr; do [ -e "$T" ] && run_one "$T" "nat_$(basename "$T" .tr)"; done

echo "--------------------------------------------------------------"
echo "ASan hardening: pass=$pass fault=$fail"
[ "$fail" -eq 0 ] && { echo "ARC ASan-clean (no UAF/double-free) ✅"; exit 0; }
echo "ARC ASan FAULTS present ❌"; exit 1
