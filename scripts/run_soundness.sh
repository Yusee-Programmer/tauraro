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
# Binary resolution: PREFER the freshly-built ./tauraroc (current source) over the
# bootstrap stage0 seed (BOOTSTRAP_BIN). The soundness corpus validates the
# compiler being SHIPPED — testing the older seed would fail on any feature/check
# added since the seed was regenerated. Explicit TAURAROC= still wins.
TAURAROC="${TAURAROC:-}"
if [ -z "$TAURAROC" ]; then
    if   [ -x "./tauraroc" ];     then TAURAROC="./tauraroc"
    elif [ -x "./tauraroc.exe" ]; then TAURAROC="./tauraroc.exe"
    elif [ -n "${BOOTSTRAP_BIN:-}" ] && [ -x "$BOOTSTRAP_BIN" ]; then TAURAROC="$BOOTSTRAP_BIN"
    else TAURAROC="./tauraroc"
    fi
fi
if [ ! -x "$TAURAROC" ] && ! command -v "$TAURAROC" >/dev/null 2>&1; then
    echo "ERROR: tauraroc binary not found: $TAURAROC (build ./tauraroc[.exe] first)"
    exit 1
fi
echo "(compiler under test: $TAURAROC)"
CC="${CC:-gcc}"
WARN="-Wno-string-compare -Wno-comment -Wno-attributes -Wno-unused-value"
LIBS="-lm"
STD=""
case "$(uname -s 2>/dev/null)" in
    *NT*|*MINGW*|*MSYS*|*CYGWIN*) LIBS="-lm -lws2_32 -mconsole" ;;
    Linux)  LIBS="-lm -lpthread -lrt"; STD="-std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700" ;;
    Darwin) LIBS="-lm -lpthread";      STD="-std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700" ;;
esac
SAN=""
if [ "${ASAN:-0}" = "1" ]; then
    # Only enable sanitizers if the toolchain can actually link them (MinGW ships
    # no -lasan/-lubsan, so ASAN=1 there would be a false failure, not real UB).
    if echo 'int main(void){return 0;}' | "$CC" -fsanitize=address,undefined -x c - -o /dev/null >/dev/null 2>&1; then
        SAN="-fsanitize=address,undefined -g -fno-omit-frame-pointer"
        # The accept corpus checks for UB (use-after-free, double-free, OOB), NOT
        # leaks — a short program not freeing once is not a soundness bug (the leak
        # gate covers loop leaks). So turn LeakSanitizer off but hard-fail on real UB.
        export ASAN_OPTIONS="detect_leaks=0:abort_on_error=1:halt_on_error=1"
        export UBSAN_OPTIONS="halt_on_error=1:abort_on_error=1:print_stacktrace=1"
        echo "(sanitizers: address,undefined enabled; leak detection off)"
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

# Build one accept source into build/<exe> with the given extra tauraroc flag and
# extra C flag; echo its stdout, return its exit status. Caller checks both.
build_and_run() {
    local src="$1" exe="$2" tflag="$3" cflag="$4"
    rm -rf build
    "$TAURAROC" "$src" $tflag --emit c >/dev/null 2>&1 || return 90
    # shellcheck disable=SC2046
    "$CC" -O1 $STD $cflag -DTAURARO_NO_RT_HELPERS $WARN -I build/include \
        -o "$exe" $(find build -name '*.c') $LIBS >/dev/null 2>&1 || return 91
    "$exe" 2>/dev/null
}

echo
echo "== ACCEPT corpus (safe; --strict OK + DIFFERENTIAL: elided ≡ pure-ARC) =="
# For each safe program: it must pass --strict; then it is built TWO ways and run.
#   A) default (elision ON) + sanitizers  -> catches UB and yields the output
#   B) --no-elide (pure ARC, elision OFF) -> the soundness oracle baseline
# The borrow elision is sound iff A and B produce identical observable output;
# any divergence is an unsound zero-copy elision (e.g. a dangling alias).
for src in tests/soundness/accept/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"
    chk="$("$TAURAROC" "$src" --strict --check 2>&1)"; rc=$?
    if [ "$rc" -ne 0 ]; then echo "FAIL  $name (--strict rejected a SAFE program)"; echo "$chk" | grep -E '\[[A-Z]-[0-9]+\]' | head -2; acc_fail=$((acc_fail+1)); continue; fi
    out_def="$(build_and_run "$src" "build/${name}_def.exe" "" "$SAN")"; rc_a=$?
    if [ "$rc_a" -ne 0 ]; then
        case "$rc_a" in 90) echo "FAIL  $name (emit)";; 91) echo "FAIL  $name (C compile)";; *) echo "FAIL  $name (default build runtime exit $rc_a — possible UB under sanitizer)";; esac
        acc_fail=$((acc_fail+1)); continue
    fi
    out_ne="$(build_and_run "$src" "build/${name}_ne.exe" "--no-elide" "")"; rc_b=$?
    if [ "$rc_b" -ne 0 ]; then
        case "$rc_b" in 90) echo "FAIL  $name (--no-elide emit)";; 91) echo "FAIL  $name (--no-elide C compile)";; *) echo "FAIL  $name (--no-elide runtime exit $rc_b)";; esac
        acc_fail=$((acc_fail+1)); continue
    fi
    if [ "$out_def" = "$out_ne" ]; then
        echo "PASS  $name (elide ≡ arc)"; acc_pass=$((acc_pass+1))
    else
        echo "FAIL  $name (UNSOUND ELISION: elided output != pure-ARC output)"
        echo "      elided : $(echo "$out_def" | tr '\n' '|')"
        echo "      pure-arc: $(echo "$out_ne" | tr '\n' '|')"
        acc_fail=$((acc_fail+1))
    fi
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
