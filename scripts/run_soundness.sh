#!/usr/bin/env bash
# Soundness corpus runner (§1b/§1c of the safety roadmap).
#
# Two suites under tests/soundness/:
#   reject/  - programs that MUST be rejected under --strict. Each file carries a
#              `# EXPECT: [CODE]` header naming the diagnostic that must fire. The
#              runner asserts the compile fails AND that exact code is emitted, so
#              a safety check silently regressing to non-fatal is caught.
#   accept/  - safe programs that MUST compile under --strict, build, and run
#              (exit 0). Guards the "safe patterns keep working" half of the claim.
#
# Set ASAN=1 to build the accept corpus with -fsanitize=address,undefined (Linux/
# macOS CI) so latent UB in generated C becomes a hard failure.
#
# Run from the repo root:  bash scripts/run_soundness.sh
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc.exe}"
CC="${CC:-gcc}"
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value"
LIBS="-lm"
case "$(uname -s 2>/dev/null)" in *NT*|*MINGW*|*MSYS*|*CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; esac
SAN=""
if [ "${ASAN:-0}" = "1" ]; then
    # Only enable sanitizers if the toolchain can actually link them (MinGW ships
    # no -lasan/-lubsan, so ASAN=1 there would be a false failure, not real UB).
    if echo 'int main(void){return 0;}' | "$CC" -fsanitize=address,undefined -x c - -o /dev/null >/dev/null 2>&1; then
        SAN="-fsanitize=address,undefined -g -fno-omit-frame-pointer"
        echo "(sanitizers: address,undefined enabled)"
    else
        echo "(ASAN=1 requested but $CC cannot link sanitizers; running without)"
    fi
fi

rej_pass=0; rej_fail=0; acc_pass=0; acc_fail=0

echo "== REJECT corpus (must fail under --strict with the expected code) =="
for src in tests/soundness/reject/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    want="$(grep -oE '\[[A-Z]-[0-9]+\]' "$src" | head -1)"
    if [ -z "$want" ]; then echo "FAIL  $name (missing '# EXPECT: [CODE]' header)"; rej_fail=$((rej_fail+1)); continue; fi
    out="$("$TAURAROC" "$src" --strict --check 2>&1)"; rc=$?
    if [ "$rc" -eq 0 ]; then echo "FAIL  $name (compiled clean; expected $want)"; rej_fail=$((rej_fail+1)); continue; fi
    case "$out" in
        *"$want"*) echo "PASS  $name ($want)"; rej_pass=$((rej_pass+1)) ;;
        *)         echo "FAIL  $name (failed, but not with $want)"; rej_fail=$((rej_fail+1)) ;;
    esac
done

echo
echo "== ACCEPT corpus (safe; must compile under --strict, build, run, exit 0) =="
for src in tests/soundness/accept/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    chk="$("$TAURAROC" "$src" --strict --check 2>&1)"; rc=$?
    if [ "$rc" -ne 0 ]; then echo "FAIL  $name (--strict rejected a SAFE program)"; echo "$chk" | grep -E '\[[A-Z]-[0-9]+\]' | head -2; acc_fail=$((acc_fail+1)); continue; fi
    rm -rf build
    "$TAURAROC" "$src" --strict --emit c >/dev/null 2>&1 || { echo "FAIL  $name (emit)"; acc_fail=$((acc_fail+1)); continue; }
    "$CC" -O1 $SAN -DTAURARO_NO_RT_HELPERS $WARN -I build/include \
        -o "build/$name.exe" $(find build -name '*.c') $LIBS >/dev/null 2>&1 \
        || { echo "FAIL  $name (C compile)"; acc_fail=$((acc_fail+1)); continue; }
    if "build/$name.exe" >/dev/null 2>&1; then echo "PASS  $name"; acc_pass=$((acc_pass+1)); else echo "FAIL  $name (runtime exit != 0)"; acc_fail=$((acc_fail+1)); fi
done
rm -rf build

echo
echo "================================================"
echo "reject: $rej_pass passed, $rej_fail failed    accept: $acc_pass passed, $acc_fail failed"
if [ $((rej_fail + acc_fail)) -eq 0 ]; then
    echo "SOUNDNESS CORPUS: all clean."
    exit 0
else
    echo "SOUNDNESS CORPUS: FAILURES DETECTED."
    exit 1
fi
