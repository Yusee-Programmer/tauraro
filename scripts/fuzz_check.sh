#!/usr/bin/env bash
# Ownership fuzz oracle. For each seed, generate a deterministic, UB-free-by-
# construction program (tests/fuzz/gen.py) that stresses the drop / escape /
# borrow analysis, then subject it to three independent checks. ANY discrepancy
# is a COMPILER bug (not a program bug), reproducible by re-running with SEED=<n>:
#
#   1. Differential elision — the elided build and the --no-elide (pure ARC) build
#      must print byte-identical output. A dropped/duplicated retain/release makes
#      them diverge (this is the leak_check oracle, applied to random programs).
#   2. Leak — net live allocations after a warm-up must be <= 0 ("LIVE" line).
#   3. Memory safety — on Linux with a sanitizer toolchain (ASAN=1), the program
#      must run clean under AddressSanitizer (catches double-free / use-after-free
#      that the counters alone might miss — e.g. the class-with-free double-free
#      and the Mutex.get() UAF this oracle is built to catch).
#
# Usage:  bash scripts/fuzz_check.sh [COUNT]        # default 50 seeds
#         SEED=1234 bash scripts/fuzz_check.sh 1    # reproduce one seed
#         ASAN=1 bash scripts/fuzz_check.sh 200     # Linux CI: + AddressSanitizer
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc.exe" ]; then TAURAROC="./tauraroc.exe"; fi
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc" ]; then TAURAROC="./tauraroc"; fi
CC="${CC:-gcc}"
PY="$(command -v python3 || command -v python)"
COUNT="${1:-50}"
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value"
LIBS="-lm"
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; esac

ASAN_FLAGS=""
if [ "${ASAN:-0}" = "1" ]; then
    if echo 'int main(void){return 0;}' | "$CC" -fsanitize=address,undefined -x c - -o /dev/null >/dev/null 2>&1; then
        ASAN_FLAGS="-fsanitize=address,undefined -g -fno-omit-frame-pointer"
        export ASAN_OPTIONS="detect_leaks=0:abort_on_error=1:halt_on_error=1"
        echo "(AddressSanitizer+UBSan enabled)"
    else echo "(ASAN=1 but $CC can't link sanitizers; running without)"; fi
fi

# Compile build/*.c with the given extra flags -> $1 exe; echo "OK" or "FAIL".
compile() { # <exe> <extra-cflags>
    "$CC" -O1 $2 -DTAURARO_NO_RT_HELPERS $WARN -I build/include -o "$1" $(find build -name '*.c') $LIBS >/dev/null 2>&1 \
        && echo OK || echo FAIL
}

fail=0; ran=0
i=0
while [ "$i" -lt "$COUNT" ]; do
    seed="${SEED:-$i}"
    src="$(mktemp).tr"
    "$PY" tests/fuzz/gen.py "$seed" > "$src"

    # --- elided (memcount) ---
    rm -rf build
    if ! "$TAURAROC" "$src" --emit c >/dev/null 2>&1; then echo "FAIL seed=$seed (elided emit)"; fail=1; rm -f "$src"; i=$((i+1)); continue; fi
    [ "$(compile build/e.exe "-O2 -DTAURARO_MEMCOUNT")" = OK ] || { echo "FAIL seed=$seed (elided cc)"; fail=1; rm -f "$src"; i=$((i+1)); continue; }
    out1="$(build/e.exe 2>&1)"; rc1=$?

    # --- pure ARC (--no-elide, memcount) ---
    rm -rf build
    "$TAURAROC" "$src" --no-elide --emit c >/dev/null 2>&1
    [ "$(compile build/a.exe "-O2 -DTAURARO_MEMCOUNT")" = OK ] || { echo "FAIL seed=$seed (pure-ARC cc)"; fail=1; rm -f "$src"; i=$((i+1)); continue; }
    out2="$(build/a.exe 2>&1)"; rc2=$?

    chk1="$(printf '%s\n' "$out1" | grep '^CHK ')"; live1="$(printf '%s\n' "$out1" | grep '^LIVE ' | awk '{print $2}')"
    chk2="$(printf '%s\n' "$out2" | grep '^CHK ')"; live2="$(printf '%s\n' "$out2" | grep '^LIVE ' | awk '{print $2}')"

    bad=""
    [ "$rc1" -eq 0 ] || bad="elided crashed (rc=$rc1)"
    [ "$rc2" -eq 0 ] || bad="pure-ARC crashed (rc=$rc2)"
    [ "$chk1" = "$chk2" ] && [ -n "$chk1" ] || bad="checksum diverged ('$chk1' vs '$chk2')"
    [ -n "$live1" ] && [ "$live1" -le 0 ] 2>/dev/null || bad="elided leaked (LIVE=$live1)"
    [ -n "$live2" ] && [ "$live2" -le 0 ] 2>/dev/null || bad="pure-ARC leaked (LIVE=$live2)"

    # --- AddressSanitizer (Linux) ---
    if [ -z "$bad" ] && [ -n "$ASAN_FLAGS" ]; then
        rm -rf build; "$TAURAROC" "$src" --emit c >/dev/null 2>&1
        if [ "$(compile build/s.exe "$ASAN_FLAGS")" = OK ]; then
            build/s.exe >/dev/null 2>&1 || bad="ASan/UBSan error (double-free / UAF / UB)"
        fi
    fi

    if [ -n "$bad" ]; then
        echo "FAIL seed=$seed: $bad"
        echo "  reproduce: python3 tests/fuzz/gen.py $seed  (or SEED=$seed bash scripts/fuzz_check.sh 1)"
        cp "$src" "fuzz_fail_${seed}.tr"; echo "  saved: fuzz_fail_${seed}.tr"
        fail=1
    fi
    ran=$((ran+1))
    rm -f "$src"
    [ -n "${SEED:-}" ] && break
    i=$((i+1))
done
rm -rf build
echo "==============================="
if [ "$fail" -eq 0 ]; then echo "Fuzz oracle: $ran seeds clean."; else echo "Fuzz oracle: FAILURES (see fuzz_fail_*.tr)."; fi
exit $fail
