#!/usr/bin/env bash
# run.sh -- Rust vs Tauraro, zero-copy hot paths (Linux/macOS).
# str_view (Rust &str slice vs Tauraro StrView), enum_payload (Rust enum<'a> vs
# Tauraro `enum E from r`). Each self-times (TIME_MS:<n>); peak RSS via /usr/bin/time.
set -u
BENCH="$(cd "$(dirname "$0")" && pwd)"
TAU="$BENCH/../../tauraroc.exe"; [ -x "$TAU" ] || TAU="$BENCH/../../tauraroc"
BDIR="$BENCH/build"
WARN="-DTAURARO_NO_RT_HELPERS -Wno-attributes -Wno-unused-value -Wno-string-compare -Wno-unknown-attributes"
case "$(uname -s)" in MINGW*|MSYS*|CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; *) LIBS="-lm -lpthread";; esac
CASES="str_view enum_payload"

peak_kb() {
  if /usr/bin/time -v true >/dev/null 2>&1; then
    /usr/bin/time -v "$@" 2>_t.txt >/dev/null; grep "Maximum resident" _t.txt | grep -oE '[0-9]+'; rm -f _t.txt
  elif /usr/bin/time -l true >/dev/null 2>&1; then
    /usr/bin/time -l "$@" 2>_t.txt >/dev/null; awk '/maximum resident/{print int($1/1024)}' _t.txt; rm -f _t.txt
  else "$@" >/dev/null 2>&1; echo 0; fi
}
besttime() { local exe="$1" best=999999999 ms; for r in 1 2 3; do ms=$("$exe" | grep -oE 'TIME_MS:[0-9]+' | grep -oE '[0-9]+'); [ -n "$ms" ] && [ "$ms" -lt "$best" ] && best=$ms; done; echo "$best"; }

declare -A RT RM TT TM
for c in $CASES; do
  echo "Building $c (rust)..."
  rustc -O -C panic=abort "$BENCH/$c.rs" -o "$BENCH/${c}_rs" 2>/dev/null
  echo "Building $c (tauraro)..."
  rm -rf "$BDIR"; ( cd "$BENCH" && "$TAU" "$BENCH/$c.tr" --strict --emit c >/dev/null 2>&1 )
  if [ -d "$BDIR/include" ]; then gcc -O2 $WARN -I"$BDIR/include" -o "$BENCH/${c}_tr" $(find "$BDIR" -name '*.c') $LIBS 2>/dev/null; fi
  if [ -x "$BENCH/${c}_rs" ]; then RT[$c]=$(besttime "$BENCH/${c}_rs"); RM[$c]=$(peak_kb "$BENCH/${c}_rs"); else RT[$c]=FAIL; RM[$c]=FAIL; fi
  if [ -x "$BENCH/${c}_tr" ]; then TT[$c]=$(besttime "$BENCH/${c}_tr"); TM[$c]=$(peak_kb "$BENCH/${c}_tr"); else TT[$c]=FAIL; TM[$c]=FAIL; fi
  rm -f "$BENCH/${c}_rs" "$BENCH/${c}_tr"
done
rm -rf "$BDIR"

RES="$BENCH/results.md"
{
  echo "# Rust vs Tauraro -- zero-copy hot paths"; echo
  echo "Both do genuine zero-copy. Tauraro is built --strict. Lower is better."; echo
  echo "| Case | Rust time (ms) | Rust peak (KB) | Tauraro time (ms) | Tauraro peak (KB) |"
  echo "|------|---------------:|---------------:|------------------:|------------------:|"
  for c in $CASES; do printf '| %s | %s | %s | %s | %s |\n' "$c" "${RT[$c]}" "${RM[$c]}" "${TT[$c]}" "${TM[$c]}"; done
} | tee "$RES"
echo; echo "Wrote $RES"
