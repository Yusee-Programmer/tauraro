#!/usr/bin/env bash
# CI-friendly formatting gate: fails (exit 1) when any checked .tr file is not in
# `tauraroc fmt` form. Not wired into .github/workflows yet - the existing tree has
# not been reformatted, so running it over std/ would fail today.
#
# Usage:
#   scripts/check_fmt.sh                 # .tr files changed vs origin/master (or HEAD~1)
#   scripts/check_fmt.sh --all           # every .tr file under std tests examples src
#   scripts/check_fmt.sh path [path...]  # explicit files / directories
#   WIDTH=100 TAURAROC=./tauraroc scripts/check_fmt.sh ...
# To fix what it reports:  tauraroc fmt -w <file>
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
[ -x "$TAURAROC" ] || [ -x "$TAURAROC.exe" ] || command -v "$TAURAROC" >/dev/null 2>&1 || { echo "ERROR: $TAURAROC not found"; exit 2; }
WIDTH="${WIDTH:-100}"

if [ "$#" -eq 0 ]; then
    BASE=origin/master; git rev-parse -q --verify "$BASE" >/dev/null || BASE=HEAD~1
    mapfile -t TARGETS < <({ git diff --name-only --diff-filter=ACMR "$BASE" -- '*.tr'; git ls-files --others --exclude-standard -- '*.tr'; } 2>/dev/null | sort -u)
elif [ "$1" = "--all" ]; then
    TARGETS=(std tests examples src)
else
    TARGETS=("$@")
fi
if [ "${#TARGETS[@]}" -eq 0 ]; then echo "check_fmt: no .tr files to check"; exit 0; fi

"$TAURAROC" fmt --check --width "$WIDTH" "${TARGETS[@]}"; rc=$?
case "$rc" in
    0) echo "check_fmt: ok" ;;
    1) echo "check_fmt: files above are not formatted (run: tauraroc fmt -w <file>)" ;;
    *) echo "check_fmt: tauraroc fmt failed (rc=$rc)" ;;
esac
exit "$rc"
