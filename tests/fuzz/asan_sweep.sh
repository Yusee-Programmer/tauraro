#!/usr/bin/env bash
# Run every test under tests/lang + tests/regression with --debug (AddressSanitizer + UBSan) and report
# the ones that hit a memory error / UB.  Leak detection is OFF here (see leak_sweep.sh).
# LEAKS=1 turns leak detection ON and reports only LeakSanitizer findings (leak sweep).
# Usage: tests/fuzz/asan_sweep.sh [jobs]      # run from the repo root; uses N private copies of the tree
set -u
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
JOBS="${1:-4}"
WORK="${SWEEP_DIR:-$(mktemp -d)}"
mkdir -p "$WORK"
mapfile -t FILES < <(cd "$ROOT" && find tests/lang tests/regression -name '*.tr' 2>/dev/null | sort)
for ((j = 0; j < JOBS; j++)); do
    mkdir -p "$WORK/w$j"
    rsync -a --exclude .git --exclude .claude --exclude build --exclude scratch --exclude benchmarks "$ROOT/" "$WORK/w$j/" 2>/dev/null || cp -r "$ROOT/." "$WORK/w$j/"
done
run_one() {
    local w="$1" f="$2"
    local out
    # one job at a time per private copy (they share the copy's build/ directory)
    exec 9>"$w/.lock"; flock 9
    out=$(cd "$w" && ASAN_OPTIONS=detect_leaks=${LEAKS:-0}:halt_on_error=1:abort_on_error=0 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 timeout 600 ./tauraroc --debug --run "$f" 2>&1)
    local rc=$?
    if [ "${LEAKS:-0}" = 1 ]; then
        if echo "$out" | grep -q "LeakSanitizer"; then echo "LEAK $f"; echo "$out" | grep -E -m1 "SUMMARY: AddressSanitizer"; fi
    elif echo "$out" | grep -qE "ERROR: AddressSanitizer|runtime error:|SUMMARY: (Address|Undefined)"; then
        echo "SANITIZER $f"; echo "$out" | grep -E -m6 "ERROR: AddressSanitizer|runtime error:|SUMMARY:|#[0-4] " | sed 's/^/    /'
    elif [ $rc -ne 0 ] || echo "$out" | grep -q "FAILED"; then
        echo "FAIL(rc=$rc) $f"; echo "$out" | grep -E -m4 "FAIL|error" | sed 's/^/    /'
    fi
}
export -f run_one
i=0
for f in "${FILES[@]}"; do
    w="$WORK/w$((i % JOBS))"
    echo "$w $f"
    i=$((i + 1))
done | xargs -P "$JOBS" -L1 bash -c 'run_one "$0" "$1"' > "$WORK/report.txt" 2>&1
echo "== ASan/UBSan sweep of ${#FILES[@]} test files; report: $WORK/report.txt"
cat "$WORK/report.txt"
