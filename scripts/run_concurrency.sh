#!/usr/bin/env bash
# Concurrency corpus — build + run each tests/concurrency/*.tr and require it to
# print "OK". On Linux with a sanitizer-capable toolchain (set ASAN=1 / TSAN=1)
# the programs run under AddressSanitizer (use-after-free / double-free of shared
# state, esp. across the spawn/Mutex/Shared boundary) or ThreadSanitizer (data
# races — e.g. a lost update on a Mutex-guarded counter, or a non-atomic refcount
# touched by two threads). This is the oracle for the "sound concurrency" work:
# any UAF/double-free/race is a HARD failure, not a lucky pass.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc.exe" ]; then TAURAROC="./tauraroc.exe"; fi
CC="${CC:-gcc}"
SAN=""
if [ "${TSAN:-0}" = "1" ]; then
    if echo 'int main(void){return 0;}' | "$CC" -fsanitize=thread -x c - -o /dev/null >/dev/null 2>&1; then
        SAN="-fsanitize=thread -g -fno-omit-frame-pointer"; echo "(ThreadSanitizer enabled)"
    else echo "(TSAN=1 but $CC can't link -fsanitize=thread; running without)"; fi
elif [ "${ASAN:-0}" = "1" ]; then
    if echo 'int main(void){return 0;}' | "$CC" -fsanitize=address,undefined -x c - -o /dev/null >/dev/null 2>&1; then
        SAN="-fsanitize=address,undefined -g -fno-omit-frame-pointer"
        # detect_leaks=0: this corpus targets USE-AFTER-FREE / DOUBLE-FREE / heap
        # corruption of state crossing the thread boundary (and UB via UBSan) — not
        # exit-time leaks, which are covered by the differential leak gate. Matches
        # run_soundness.sh so a benign fresh-arg print-concat isn't a false failure.
        export ASAN_OPTIONS="detect_leaks=0:abort_on_error=1:halt_on_error=1"; echo "(AddressSanitizer+UBSan enabled; leak detection off)"
    else echo "(ASAN=1 but $CC can't link sanitizers; running without)"; fi
fi
LIBS="-lm -lpthread"
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; esac
fail=0
for src in tests/concurrency/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    rm -rf build
    "$TAURAROC" "$src" --emit c >/dev/null 2>&1 || { echo "FAIL  $name (emit)"; fail=1; continue; }
    "$CC" -O1 $SAN -I build/include -o "build/$name.exe" $(find build -name '*.c') $LIBS >/dev/null 2>&1 \
        || { echo "FAIL  $name (compile)"; fail=1; continue; }
    out="$("build/$name.exe" 2>&1)"; rc=$?
    if [ $rc -eq 0 ] && echo "$out" | grep -q "OK"; then echo "PASS  $name  |  $out"
    else echo "FAIL  $name (rc=$rc): $out"; fail=1; fi
done
rm -rf build
echo "==============================="
if [ "$fail" -eq 0 ]; then echo "Concurrency corpus: all clean."; else echo "Concurrency corpus: FAILURES."; fi
exit $fail
