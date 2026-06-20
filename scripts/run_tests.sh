#!/usr/bin/env bash
# Run the regression test suite under tests/lang/ and tests/regression/.
#
# Each test file uses std/test's TestRunner and is run with `tauraroc --run`.
# A test file passes iff:
#   - the compile+run exits 0, AND
#   - stdout does NOT contain the word "FAILED"
#
# Usage: scripts/run_tests.sh [path/to/single_test.tr]

set -uo pipefail

TAURAROC="${BOOTSTRAP_BIN:-./tauraroc}"
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc.exe" ]; then
    TAURAROC="./tauraroc.exe"
fi
if [ ! -x "$TAURAROC" ] && ! command -v "$TAURAROC" &>/dev/null; then
    echo "ERROR: tauraroc binary not found: $TAURAROC"
    echo "Set BOOTSTRAP_BIN, or build ./tauraroc / ./tauraroc.exe first."
    exit 1
fi

if [ "$#" -ge 1 ]; then
    FILES=("$@")
else
    FILES=()
    while IFS= read -r f; do FILES+=("$f"); done < <(find tests/lang tests/regression -name '*.tr' 2>/dev/null | sort)
fi

total=0
failed=0
failed_files=()

for f in "${FILES[@]}"; do
    total=$((total + 1))
    echo "==> $f"
    out=$("$TAURAROC" --run "$f" 2>&1)
    status=$?
    echo "$out"
    if [ $status -ne 0 ] || echo "$out" | grep -q "FAILED"; then
        failed=$((failed + 1))
        failed_files+=("$f")
    fi
done

# --- Formatter idempotency check ------------------------------------------
# `tauraroc fmt` must be idempotent: fmt(fmt(x)) == fmt(x). Verify on a couple
# of representative example files (skipped silently if examples are absent).
for FSAMPLE in examples/02_operators.tr examples/03_control_flow.tr; do
    [ -f "$FSAMPLE" ] || continue
    total=$((total + 1))
    echo "==> fmt idempotency: $FSAMPLE"
    f1=$("$TAURAROC" fmt "$FSAMPLE" 2>/dev/null)
    f1file=$(mktemp)
    printf '%s' "$f1" > "$f1file"
    f2=$("$TAURAROC" fmt "$f1file" 2>/dev/null)
    rm -f "$f1file"
    if [ "$f1" != "$f2" ]; then
        echo "  NOT IDEMPOTENT"
        failed=$((failed + 1))
        failed_files+=("fmt:$FSAMPLE")
    fi
done

# --- FFI / cdylib export check --------------------------------------------
# Build a shared library of `export def` functions and call them from a C
# program compiled against the generated header. Requires a C compiler (cc).
if command -v cc >/dev/null 2>&1 || command -v gcc >/dev/null 2>&1; then
    total=$((total + 1))
    echo "==> cdylib export"
    CCBIN=$(command -v cc || command -v gcc)
    libdir=$(mktemp -d)
    cat > "$libdir/lib.tr" <<'TREOF'
export def add(a: int, b: int) -> int:
    return a + b
export def multiply(a: int, b: int) -> int:
    return a * b
TREOF
    "$TAURAROC" "$libdir/lib.tr" -o "$libdir/lib" --lib >/dev/null 2>&1
    cat > "$libdir/consumer.c" <<'CEOF'
#include "lib.h"
#include <stdio.h>
int main(void){ printf("%lld %lld\n", add(3,4), multiply(5,6)); return 0; }
CEOF
    # The library extension is platform-dependent (.so / .dll / .dylib).
    libfile=""
    for cand in "$libdir/lib.so" "$libdir/lib.dll" "$libdir/lib.dylib"; do
        [ -f "$cand" ] && libfile="$cand" && break
    done
    cout=""
    if [ -n "$libfile" ]; then
        # Compile with -std=gnu11 on Linux for ucontext compatibility
        if [[ "$(uname -s)" == "Linux" ]]; then
            "$CCBIN" -std=gnu11 -D_GNU_SOURCE "$libdir/consumer.c" -I"$libdir" "$libfile" -o "$libdir/consumer" >/dev/null 2>&1
        else
            "$CCBIN" "$libdir/consumer.c" -I"$libdir" "$libfile" -o "$libdir/consumer" >/dev/null 2>&1
        fi
        # Run from the lib dir with it on the dynamic-loader search path so the
        # shared object is found at runtime (ELF: LD_LIBRARY_PATH, Mach-O:
        # DYLD_LIBRARY_PATH; Windows resolves the .dll from the cwd). Without
        # this the consumer fails to start on Linux/macOS -> empty output.
        if [ -f "$libdir/consumer" ]; then
            case "$(uname -s)" in
                Linux)
                    cout=$(cd "$libdir" && LD_LIBRARY_PATH="$libdir:${LD_LIBRARY_PATH:-}" ./consumer 2>/dev/null)
                    ;;
                Darwin)
                    cout=$(cd "$libdir" && DYLD_LIBRARY_PATH="$libdir:${DYLD_LIBRARY_PATH:-}" ./consumer 2>/dev/null)
                    ;;
                MINGW*|MSYS*|CYGWIN*)
                    cout=$(cd "$libdir" && ./consumer.exe 2>/dev/null)
                    ;;
                *)
                    cout=$(cd "$libdir" && LD_LIBRARY_PATH="$libdir:${LD_LIBRARY_PATH:-}" ./consumer 2>/dev/null)
                    ;;
            esac
        fi
        [ -z "$cout" ] && [ -f "$libdir/consumer.exe" ] && cout=$(cd "$libdir" && ./consumer.exe 2>/dev/null)
    fi
    if [ "$cout" != "7 30" ]; then
        echo "  FAILED (got: '$cout')"
        failed=$((failed + 1))
        failed_files+=("cdylib_export")
    fi
    rm -rf "$libdir"
fi

echo ""
echo "==================================="
echo "Test files: $total, failed: $failed"
if [ $failed -gt 0 ]; then
    echo "Failed files:"
    for f in "${failed_files[@]}"; do
        echo "  - $f"
    done
    exit 1
fi
echo "All test files passed."