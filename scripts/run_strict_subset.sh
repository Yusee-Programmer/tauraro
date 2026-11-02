#!/usr/bin/env bash
# Keeps `--strict` honest on real programs: every file listed in tests/strict_subset.list
# (examples/, tests/concurrency/ and a curated slice of tests/lang/) must pass
# `tauraroc --strict --check` - borrow checker, lifetimes, Sendable, cycles AND the
# consuming-call move rules ([M-1]/[M-5], docs/lang/13_memory_and_ownership.md).
# A new rejection means either a genuine use-after-move that should be fixed in the program
# (declare the parameter `ref`/`mut ref`, or `.clone()` at the call), or a checker false positive.
# Run from the repo root:  bash scripts/run_strict_subset.sh
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
[ -x "$TAURAROC" ] || command -v "$TAURAROC" >/dev/null 2>&1 || { echo "ERROR: tauraroc not found: $TAURAROC"; exit 1; }
pass=0; fail=0
while IFS= read -r f; do
    case "$f" in ''|'#'*) continue ;; esac
    if [ ! -f "$f" ]; then echo "FAIL  $f (listed but missing)"; fail=$((fail+1)); continue; fi
    out="$("$TAURAROC" "$f" --strict --check 2>&1)"; rc=$?
    if [ "$rc" -eq 0 ]; then pass=$((pass+1)); else
        echo "FAIL  $f"; echo "$out" | grep -E '^error' | head -3; fail=$((fail+1))
    fi
done < tests/strict_subset.list
echo "strict subset: $pass passed, $fail failed"
[ "$fail" -eq 0 ]
