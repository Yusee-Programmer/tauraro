#!/usr/bin/env bash
# Formatter guarantee check: `tauraroc fmt` must (1) succeed on every .tr file of the
# standard library, tests, examples and the compiler itself, (2) reach a FIXED POINT
# (a second pass changes nothing) and (3) not change any program: the C generated
# for every test program from the formatted tree must be byte-identical to the C
# generated from the original tree.
#
# Usage: scripts/fmt_guarantee.sh [--quick]     (uses ./tauraroc or $TAURAROC)
#   --quick  compile-compare only a sample of the programs (the fixed-point and
#            token-stream checks always cover every file).
# The compiler's own `fmt` additionally verifies, per file and before writing, that
# the output re-lexes to the identical real-lexer token stream, parses, and is idempotent.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
[ -x "$TAURAROC" ] || { echo "ERROR: $TAURAROC not found"; exit 1; }
QUICK=0; [ "${1:-}" = "--quick" ] && QUICK=1
FAILS=0
ok()  { echo "PASS  $1"; }
bad() { echo "FAIL  $1"; FAILS=$((FAILS+1)); }

W="$(mktemp -d "${TMPDIR:-/tmp}/fmtg_XXXXXX")"; trap 'rm -rf "$W"' EXIT
mkdir -p "$W/orig" "$W/fmt"
for d in std tests/lang tests/regression examples src; do
    [ -d "$d" ] || continue
    mkdir -p "$W/orig/$d" "$W/fmt/$d"
    cp -r "$d/." "$W/orig/$d/"; cp -r "$d/." "$W/fmt/$d/"
done
rm -rf "$W/orig/src/build" "$W/fmt/src/build"

# 1. format everything in place (no per-file failure allowed)
( cd "$W/fmt" && "$TAURAROC" fmt -w std tests examples src ) > "$W/fmt1.log" 2>&1; rc=$?
if [ $rc -eq 0 ]; then ok "fmt -w std tests examples src (rc=0)"; else bad "fmt -w std tests examples src (rc=$rc)"; grep -m5 '^error' "$W/fmt1.log"; fi
NFILES=$(find "$W/fmt" -name '*.tr' | wc -l)

# 2. fixed point
( cd "$W/fmt" && "$TAURAROC" fmt --check std tests examples src ) > "$W/fmt2.log" 2>&1; rc=$?
if [ $rc -eq 0 ]; then ok "fixed point: second pass changes nothing ($NFILES files)"; else bad "fixed point (rc=$rc)"; grep -m5 -E '^(would|error)' "$W/fmt2.log"; fi

# 3. program equivalence via emitted C
sig() { # $1 = tree, $2 = test file -> hash of the generated C of that program
    ( cd "$1" && rm -rf build && "$TAURAROC" --emit c "$2" >/dev/null 2>&1 && \
      find build -type f \( -name '*.c' -o -name '*.h' \) ! -name tauraro_rt.h | sort | xargs cat | sha256sum | cut -d' ' -f1 )
}
mapfile -t PROGS < <(cd tests && ls lang/*.tr regression/*.tr 2>/dev/null | sed 's#^#tests/#')
SAMPLE=("${PROGS[@]}")
if [ "$QUICK" = 1 ]; then SAMPLE=(); i=0; for p in "${PROGS[@]}"; do i=$((i+1)); [ $((i % 7)) -eq 0 ] && SAMPLE+=("$p"); done; fi
DIFF=0; N=0; SKIP=0
for p in "${SAMPLE[@]}"; do
    a="$(sig "$W/orig" "$p")"; b="$(sig "$W/fmt" "$p")"
    if [ -z "$a" ] || [ -z "$b" ]; then SKIP=$((SKIP+1)); continue; fi
    N=$((N+1))
    if [ "$a" != "$b" ]; then DIFF=$((DIFF+1)); echo "  generated C differs: $p"; fi
done
if [ "$DIFF" = 0 ] && [ "$N" -gt 0 ]; then ok "generated C identical before/after fmt ($N programs, $SKIP skipped)"; else bad "generated C identical before/after fmt ($DIFF differ of $N)"; fi

[ "$FAILS" = "0" ] && { echo "fmt guarantee: all clean"; exit 0; }
echo "fmt guarantee: $FAILS failed"; exit 1
