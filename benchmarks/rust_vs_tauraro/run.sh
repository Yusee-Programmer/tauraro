#!/usr/bin/env bash
# run.sh -- Rust vs Tauraro, zero-copy / borrow hot paths (Linux/macOS).
# Tauraro uses explicit borrow/own/lifetime annotations (ref / @value_type /
# `enum E from r`) built --strict, so every borrow is COMPILE-TIME PROVEN (proven
# borrows elide ARC retain/release) -- apples-to-apples with Rust's &/lifetimes:
#   str_view (StrView vs &str) . enum_payload (`enum from r` vs enum<'a>) .
#   class_field (`ref Point` vs &Point) . list_sum (`ref List` vs &Vec) .
#   dict_borrow (`ref str = d.get(k)` vs &HashMap+&str) .
#   value_dict (@value_type in dict vs HashMap<_,Point>) . iface_call (`ref Shape` vs &dyn).
# Each self-times (TIME_MS:<n>); peak RSS via /usr/bin/time.
set -u
BENCH="$(cd "$(dirname "$0")" && pwd)"
TAU="$BENCH/../../tauraroc.exe"; [ -x "$TAU" ] || TAU="$BENCH/../../tauraroc"
BDIR="$BENCH/build"
WARN="-DTAURARO_NO_RT_HELPERS -Wno-attributes -Wno-unused-value -Wno-string-compare -Wno-unknown-attributes"
case "$(uname -s)" in MINGW*|MSYS*|CYGWIN*) LIBS="-lm -lws2_32 -mconsole";; *) LIBS="-lm -lpthread";; esac
CASES="str_view enum_payload class_field list_sum dict_borrow value_dict iface_call"

peak_kb() {
  if /usr/bin/time -v true >/dev/null 2>&1; then
    /usr/bin/time -v "$@" 2>_t.txt >/dev/null; grep "Maximum resident" _t.txt | grep -oE '[0-9]+'; rm -f _t.txt
  elif /usr/bin/time -l true >/dev/null 2>&1; then
    /usr/bin/time -l "$@" 2>_t.txt >/dev/null; awk '/maximum resident/{print int($1/1024)}' _t.txt; rm -f _t.txt
  else "$@" >/dev/null 2>&1; echo 0; fi
}
besttime() { local exe="$1" best=999999999 ms; for r in 1 2 3; do ms=$("$exe" | grep -oE 'TIME_MS:[0-9]+' | grep -oE '[0-9]+'); [ -n "$ms" ] && [ "$ms" -lt "$best" ] && best=$ms; done; echo "$best"; }

# Build + run from $BENCH so all build paths are RELATIVE (`build`, not the
# absolute project path which may contain spaces — an unquoted $(find ...) over a
# space-containing absolute path word-splits and breaks the gcc invocation).
cd "$BENCH" || exit 1
declare -A RT RM TT TM
for c in $CASES; do
  echo "Building $c (rust)..."
  rustc -O -C panic=abort "$c.rs" -o "${c}_rs" 2>/dev/null
  echo "Building $c (tauraro)..."
  rm -rf build; "$TAU" "$c.tr" --strict --emit c >/dev/null 2>&1
  if [ -d build/include ]; then gcc -O2 $WARN -Ibuild/include -o "${c}_tr" $(find build -name '*.c') $LIBS 2>/dev/null; fi
  if [ -x "./${c}_rs" ]; then RT[$c]=$(besttime "./${c}_rs"); RM[$c]=$(peak_kb "./${c}_rs"); else RT[$c]=FAIL; RM[$c]=FAIL; fi
  if [ -x "./${c}_tr" ]; then TT[$c]=$(besttime "./${c}_tr"); TM[$c]=$(peak_kb "./${c}_tr"); else TT[$c]=FAIL; TM[$c]=FAIL; fi
  rm -f "${c}_rs" "${c}_tr" "${c}_rs.pdb"
done
rm -rf build

RES="$BENCH/results.md"
{
  echo "# Rust vs Tauraro -- borrow / zero-copy hot paths"; echo
  echo "Tauraro uses explicit borrow/own/lifetime annotations, built --strict (borrows"
  echo "compile-time proven; proven borrows elide ARC). Lower is better. See README.md"
  echo "for the case descriptions and honest interpretation."; echo
  echo "| Case | Rust time (ms) | Rust peak (KB) | Tauraro time (ms) | Tauraro peak (KB) |"
  echo "|------|---------------:|---------------:|------------------:|------------------:|"
  for c in $CASES; do printf '| %s | %s | %s | %s | %s |\n' "$c" "${RT[$c]}" "${RM[$c]}" "${TT[$c]}" "${TM[$c]}"; done
} | tee "$RES"
echo; echo "Wrote $RES"
