#!/usr/bin/env bash
# Runtime sanitizer corpus (tests/runtime_san): programs whose RUN-TIME behaviour must be defined.
# Each file carries header lines:
#   # EXPECT-EXIT: 0 | nonzero
#   # EXPECT-STDOUT: <exact line>      (repeatable, checked in order)
#   # EXPECT-STDERR: <substring>       (optional)
#   # EXPECT-NOOPT: 1                  (build at -O0; optimizers may legally remove runaway recursion)
# Every program is built twice: plain, and with --debug (ASan + UBSan). Any sanitizer report on a
# program that is expected to exit 0 is a failure. Run from the repo root.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
# stock macOS has no `timeout`: use coreutils' gtimeout if present, else run unguarded
if command -v timeout >/dev/null 2>&1; then TMO="timeout 30"
elif command -v gtimeout >/dev/null 2>&1; then TMO="gtimeout 30"
else TMO=""; fi
fail=0; pass=0
for f in tests/runtime_san/*.tr; do
  n=$(basename "$f" .tr)
  opt="-O2"; grep -q '^# EXPECT-NOOPT' "$f" && opt="-O0"
  for mode in plain san; do
    if [ $mode = plain ] && grep -q "^# SAN-ONLY" "$f"; then continue; fi
    flags="$opt"; [ $mode = san ] && flags="$opt --debug"
    if ! "$TAURAROC" "$f" -o "$TMP/$n.$mode" $flags >"$TMP/build.log" 2>&1; then
      echo "FAIL  $n [$mode] (build): $(grep -m1 error "$TMP/build.log")"; fail=$((fail+1)); continue
    fi
    $TMO "$TMP/$n.$mode" >"$TMP/out" 2>"$TMP/err"; rc=$?
    ok=1; why=""
    want=$(sed -n 's/^# EXPECT-EXIT: //p' "$f" | head -1)
    if [ "$want" = 0 ] && [ $rc -ne 0 ]; then ok=0; why="exit $rc (wanted 0): $(head -c 200 "$TMP/err")"; fi
    if [ "$want" = nonzero ] && [ $rc -eq 0 ]; then ok=0; why="exit 0 (wanted a trap)"; fi
    exp="$(sed -n 's/^# EXPECT-STDOUT: //p' "$f")"
    if [ $mode = san ] && grep -q "^# SAN-SKIP-STDOUT" "$f"; then exp=""; fi
    if [ -n "$exp" ] && [ "$(head -n $(echo "$exp" | wc -l) "$TMP/out")" != "$exp" ]; then ok=0; why="stdout mismatch: $(head -c 120 "$TMP/out" | tr '\n' '|')"; fi
    es="$(sed -n 's/^# EXPECT-STDERR: //p' "$f" | head -1)"
    if [ -n "$es" ]; then
      # under ASan a stack overflow is reported by the sanitizer as "stack-overflow"
      if ! grep -qiE "$es|$(echo "$es" | tr ' ' '-')" "$TMP/err"; then ok=0; why="stderr lacks '$es': $(head -c 160 "$TMP/err")"; fi
    fi
    if [ "$want" = 0 ] && grep -qE 'runtime error|AddressSanitizer' "$TMP/err"; then ok=0; why="sanitizer report: $(grep -m1 -E 'runtime error|AddressSanitizer' "$TMP/err")"; fi
    if [ $ok = 1 ]; then pass=$((pass+1)); echo "PASS  $n [$mode]"; else fail=$((fail+1)); echo "FAIL  $n [$mode] $why"; fi
  done
done
echo "runtime_san: $pass passed, $fail failed"
[ $fail = 0 ]
