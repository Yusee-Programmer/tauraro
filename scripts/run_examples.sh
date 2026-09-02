#!/usr/bin/env bash
# Example harness — the regression net for docs/examples drift (Track 3).
#
# For every deterministic, terminating program under examples/, this:
#   1. BUILDS it with tauraroc  (a build failure = the example uses a
#      removed/renamed API — exactly the "stale example" drift class), and
#   2. RUNS it (bounded), asserting exit 0, and — if a golden output exists at
#      examples/golden/<name>.out — asserting stdout matches it byte-for-byte.
#
# Modes:
#   bash scripts/run_examples.sh            # check: build + run (+ golden diff if present)
#   bash scripts/run_examples.sh --record   # (re)generate examples/golden/<name>.out
#
# An example opts OUT of running (still build-checked) with a header line:
#   # EXAMPLE: no-run        -> build only (servers, interactive, non-deterministic)
#   # EXAMPLE: skip          -> skip entirely (needs special toolchain/targets)
#
# Binary: prefers ./tauraroc[.exe]; override with TAURAROC=/path.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

TAURAROC="${TAURAROC:-}"
if [ -z "$TAURAROC" ]; then
    if   [ -x "./tauraroc" ];     then TAURAROC="./tauraroc"
    elif [ -x "./tauraroc.exe" ]; then TAURAROC="./tauraroc.exe"
    else TAURAROC="tauraroc"
    fi
fi
command -v "$TAURAROC" >/dev/null 2>&1 || [ -x "$TAURAROC" ] || { echo "ERROR: tauraroc not found ($TAURAROC)"; exit 1; }

RECORD=0
[ "${1:-}" = "--record" ] && RECORD=1
GOLD="examples/golden"
mkdir -p "$GOLD"

# Build-only (non-deterministic output: thread interleaving / panic timing). These
# are still compiled — the drift-catching half — but not run/golden-checked. A file
# may also opt in per-file with a `# EXAMPLE: no-run` header.
# ffi_callbacks declares `extern "C" run_events(...)` but ships no C implementation
# (it demonstrates the callback ABI); it needs a companion C object to link+run, which
# CI doesn't provide — so build-check only (a bad extern signature still fails the build).
NO_RUN=" 15_concurrency 22_thread_atomic 23_thread_safety 24_advanced_concurrency 25_new_features 27_async_and_panic 01_channels 02_thread_pool 03_producer_consumer 04_select 05_green_threads 06_scoped_threads 07_combined_pipeline ffi_callbacks "
# Skipped entirely (need a special toolchain/target the default CI job lacks).
SKIP_LIST=" "

_TMO=""; command -v timeout >/dev/null 2>&1 && _TMO="timeout -k 5 30"
EXE_EXT=""; case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) EXE_EXT=".exe";; esac

pass=0; fail=0; norun=0; skip=0

# Only top-level tutorial examples + the concurrency set are golden-checkable;
# freestanding/ needs embedded targets and is covered by its own build.
# NB: no `mapfile` — macOS ships Bash 3.2, which lacks it. Read into the array the
# portable way (process substitution + read loop works on Bash 3.2+).
FILES=()
while IFS= read -r _f; do FILES+=("$_f"); done < <(find examples -maxdepth 2 -name '*.tr' | grep -vE '/freestanding/' | sort)

for src in "${FILES[@]}"; do
    name="$(basename "$src" .tr)"
    hdr="$(head -20 "$src")"
    case "$hdr" in
        *"EXAMPLE: skip"*) echo "SKIP  $name"; skip=$((skip+1)); continue;;
    esac
    case "$SKIP_LIST" in *" $name "*) echo "SKIP  $name"; skip=$((skip+1)); continue;; esac

    rm -rf build; mkdir -p build
    exe="build/${name}${EXE_EXT}"
    if ! "$TAURAROC" "$src" -o "$exe" >.ex_buildlog 2>&1; then
        echo "FAIL  $name (build)"; sed -n '1,4p' .ex_buildlog 2>/dev/null | sed 's/^/        /'
        fail=$((fail+1)); continue
    fi

    case "$hdr" in
        *"EXAMPLE: no-run"*) echo "BUILT $name (no-run)"; norun=$((norun+1)); continue;;
    esac
    case "$NO_RUN" in *" $name "*) echo "BUILT $name (no-run)"; norun=$((norun+1)); continue;; esac

    out="$($_TMO "./$exe" 2>/dev/null)"; rc=$?
    if [ "$rc" -ne 0 ]; then
        echo "FAIL  $name (exit $rc)"; fail=$((fail+1)); continue
    fi

    gf="$GOLD/${name}.out"
    if [ "$RECORD" -eq 1 ]; then
        printf '%s\n' "$out" > "$gf"; echo "REC   $name"; pass=$((pass+1)); continue
    fi
    if [ -f "$gf" ]; then
        if [ "$out" = "$(cat "$gf")" ]; then echo "PASS  $name (golden)"; pass=$((pass+1))
        else echo "FAIL  $name (golden mismatch)"; fail=$((fail+1)); fi
    else
        echo "PASS  $name (smoke; no golden)"; pass=$((pass+1))
    fi
done
rm -rf build; rm -f .ex_buildlog

echo "======================================================"
echo "examples: $pass ok, $fail failed, $norun build-only, $skip skipped"
[ "$fail" -eq 0 ] && { echo "EXAMPLES: all clean."; exit 0; } || { echo "EXAMPLES: FAILURES."; exit 1; }
