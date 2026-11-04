#!/usr/bin/env bash
# AddressSanitizer sweep over the .tr regression tests (C backend): each test is
# emitted to C, built with -fsanitize=address and run; any ASan report fails it.
# The normal suite cannot see heap/stack misuse that happens not to crash.
# Usage: [LEAKS=1] scripts/asan_check.sh [test.tr ...]   (default: tests/lang + tests/regression)
#        JOBS=4 to build/run in parallel.
set -u
# KEEP=dir saves the sanitizer output of every leaking test; tests/asan_leak_allow.list lists documented-limitation leaks (one test name per line).
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-$ROOT/tauraroc}"
[ -x "$TAURAROC" ] || { echo "ERROR: $TAURAROC not found"; exit 1; }
CC="${CC:-gcc}"
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
one() {
    local f="$1" n; n="$(basename "$f" .tr)"
    local d="$WORK/$n"; mkdir -p "$d"; cd "$d" || return
    # Build through tauraroc's own driver (--debug = -fsanitize=address,undefined -g) so std modules
    # that link a C shim (`# tauraro-cpp-shim:`) resolve exactly as in a real build.
    "$TAURAROC" "$ROOT/$f" --debug -o "$d/asan_bin" >"$d/emit.err" 2>&1 || { echo "BUILDFAIL $n"; return; }
    [ -x "$d/asan_bin" ] || { echo "BUILDFAIL $n (no executable)"; return; }
    # LEAKS=1 also reports leaks. use_stacks/use_registers=0: a leaked object still referenced from
    # a DEAD stack slot (main's locals after return) would otherwise count as reachable and hide.
    LSAN_OPTIONS=use_stacks=0:use_registers=0 ASAN_OPTIONS=detect_leaks=${LEAKS:-0} timeout 240 ./asan_bin >"$d/out.txt" 2>&1
    rc=$?
    if [ "$rc" = 124 ]; then echo "BAD $n (timeout: a killed run never reaches the leak report)"; cd "$WORK" && rm -rf "$d"; return; fi
    if grep -q 'ERROR: AddressSanitizer' "$d/out.txt"; then echo "BAD $n"; grep -m1 'ERROR: AddressSanitizer' "$d/out.txt"
    elif grep -q 'LeakSanitizer' "$d/out.txt"; then
        if grep -qx "$n" "$ROOT/tests/asan_leak_allow.list" 2>/dev/null; then echo "OK $n"; else echo "LEAK $n"; fi
        [ -n "${KEEP:-}" ] && { mkdir -p "$KEEP"; cp "$d/out.txt" "$KEEP/$n.txt"; }
    else echo "OK $n"; fi
    cd "$WORK" && rm -rf "$d"   # keep the sweep small: a --debug build tree is tens of MB
}
export -f one; export ROOT TAURAROC CC WORK KEEP LEAKS
if [ "$#" -gt 0 ]; then FILES=("$@"); else FILES=(tests/lang/*.tr tests/regression/*.tr); fi
printf '%s\n' "${FILES[@]}" | xargs -P "${JOBS:-3}" -I{} bash -c 'one {}' > "$WORK/results.txt"
cat "$WORK/results.txt" | grep -v '^OK' || true
bad=$(grep -c '^BAD\|^BUILDFAIL\|^LEAK' "$WORK/results.txt")
echo "ASan sweep: $(grep -c '^OK' "$WORK/results.txt") ok, $bad bad"
[ "$bad" = "0" ]
