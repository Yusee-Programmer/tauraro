#!/usr/bin/env bash
# AddressSanitizer sweep over the .tr regression tests (C backend): each test is
# emitted to C, built with -fsanitize=address and run; any ASan report fails it.
# The normal suite cannot see heap/stack misuse that happens not to crash.
# Usage: scripts/asan_check.sh [test.tr ...]   (default: tests/lang + tests/regression)
#        JOBS=4 to build/run in parallel.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-$ROOT/tauraroc}"
[ -x "$TAURAROC" ] || { echo "ERROR: $TAURAROC not found"; exit 1; }
CC="${CC:-gcc}"
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
one() {
    local f="$1" n; n="$(basename "$f" .tr)"
    local d="$WORK/$n"; mkdir -p "$d"; cd "$d" || return
    "$TAURAROC" "$ROOT/$f" --emit c >/dev/null 2>"$d/emit.err" || { echo "EMITFAIL $n"; return; }
    $CC -g -O1 -fsanitize=address -w -I build/include -o a.exe $(find build -name '*.c') -lm -lpthread 2>"$d/cc.err" || { echo "CCFAIL $n"; return; }
    ASAN_OPTIONS=detect_leaks=0 timeout 120 ./a.exe >"$d/out.txt" 2>&1
    if grep -q 'AddressSanitizer' "$d/out.txt"; then echo "BAD $n"; grep -m1 'ERROR: AddressSanitizer' "$d/out.txt"; else echo "OK $n"; fi
}
export -f one; export ROOT TAURAROC CC WORK
if [ "$#" -gt 0 ]; then FILES=("$@"); else FILES=(tests/lang/*.tr tests/regression/*.tr); fi
printf '%s\n' "${FILES[@]}" | xargs -P "${JOBS:-3}" -I{} bash -c 'one {}' > "$WORK/results.txt"
cat "$WORK/results.txt" | grep -v '^OK' || true
bad=$(grep -c '^BAD\|^EMITFAIL\|^CCFAIL' "$WORK/results.txt")
echo "ASan sweep: $(grep -c '^OK' "$WORK/results.txt") ok, $bad bad"
[ "$bad" = "0" ]
