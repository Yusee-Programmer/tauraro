#!/usr/bin/env bash
# run_zerocopy.sh -- ARC vs Zero-copy (borrows / lifetimes / StrView) benchmark.
# Linux/macOS port of run_zerocopy.ps1.
#
# For each case we compile two source variants:
#   <case>_arc.tr  - idiomatic ARC (by-value, copying slices)
#   <case>_zc.tr   - zero-copy (ref borrows / StrView / borrowed payloads), built --strict
# and measure, for each:
#   TIME_MS  - self-timed workload (optimized build, no counters)
#   PeakKB   - peak resident memory (/usr/bin/time)
#   ALLOCS   - net live heap blocks held at peak  (-DTAURARO_MEMCOUNT build)
#   STRS     - net live TrStr string objects held (-DTAURARO_MEMCOUNT build)
#
# --strict only ENFORCES the borrows; it does not change codegen, so ARC and
# Zero-copy differ only by the source.
set -u
BENCH="$(cd "$(dirname "$0")" && pwd)"
TAU="$BENCH/../../tauraroc.exe"
[ -x "$TAU" ] || TAU="$BENCH/../../tauraroc"
BDIR="$BENCH/build"
WARN="-DTAURARO_NO_RT_HELPERS -Wno-attributes -Wno-unused-value -Wno-string-compare -Wno-unknown-attributes"
# Platform link libs: math everywhere, winsock on Windows-toolchains, pthread on Unix.
case "$(uname -s)" in
  MINGW*|MSYS*|CYGWIN*) LIBS="-lm -lws2_32 -mconsole" ;;
  *)                    LIBS="-lm -lpthread" ;;
esac
CASES="str_view str_pass list_iter class_pass enum_payload dict_pass interface"

# Peak RSS in KB for a command, cross-platform.
peak_kb() {
  if /usr/bin/time -v true >/dev/null 2>&1; then            # GNU time (Linux)
    /usr/bin/time -v "$@" 2>_t.txt >/dev/null; grep "Maximum resident" _t.txt | grep -oE '[0-9]+'; rm -f _t.txt
  elif /usr/bin/time -l true >/dev/null 2>&1; then          # BSD time (macOS)
    /usr/bin/time -l "$@" 2>_t.txt >/dev/null; awk '/maximum resident/{print int($1/1024)}' _t.txt; rm -f _t.txt
  else
    "$@" >/dev/null 2>&1; echo 0
  fi
}

declare -A T P A S
for c in $CASES; do
  for v in arc zc; do
    src="$BENCH/${c}_${v}.tr"; [ -f "$src" ] || continue
    echo "Building $c ($v)..."
    rm -rf "$BDIR"; ( cd "$BENCH" && [ "$v" = zc ] && "$TAU" "$src" --strict --emit c >/dev/null 2>&1 || "$TAU" "$src" --emit c >/dev/null 2>&1 )
    [ -d "$BDIR/include" ] || { echo "  build FAILED (emit)"; continue; }
    cfiles=$(find "$BDIR" -name '*.c')
    gcc -O2 $WARN -I"$BDIR/include" -o "$BENCH/${c}_${v}_opt" $cfiles $LIBS 2>/dev/null
    gcc -O2 -DTAURARO_MEMCOUNT $WARN -I"$BDIR/include" -o "$BENCH/${c}_${v}_mc" $cfiles $LIBS 2>/dev/null
    [ -x "$BENCH/${c}_${v}_opt" ] || { echo "  build FAILED (gcc)"; continue; }
    # time: best TIME_MS over 3 runs
    best=999999999
    for r in 1 2 3; do
      out=$("$BENCH/${c}_${v}_opt"); ms=$(echo "$out" | grep -oE 'TIME_MS:[0-9]+' | grep -oE '[0-9]+')
      [ -n "$ms" ] && [ "$ms" -lt "$best" ] && best=$ms
    done
    T["$c.$v"]=$best
    P["$c.$v"]=$(peak_kb "$BENCH/${c}_${v}_opt")
    mout=$("$BENCH/${c}_${v}_mc")
    A["$c.$v"]=$(echo "$mout" | grep -oE 'ALLOCS:-?[0-9]+' | grep -oE '\-?[0-9]+')
    S["$c.$v"]=$(echo "$mout" | grep -oE 'STRS:-?[0-9]+' | grep -oE '\-?[0-9]+')
    rm -f "$BENCH/${c}_${v}_opt" "$BENCH/${c}_${v}_mc"
  done
done
rm -rf "$BDIR"

RES="$BENCH/results.md"
{
  echo "# Zero-copy vs ARC -- benchmark results"
  echo
  echo "ARC variant = by-value / copying. Zero-copy variant = ref borrows / StrView /"
  echo "borrowed payloads, compiled with --strict. Lower is better in every column."
  echo
  echo "| Case | Variant | Time (ms) | Peak (KB) | Live allocs | Live strs |"
  echo "|------|---------|----------:|----------:|------------:|----------:|"
  for c in $CASES; do for v in arc zc; do
    [ -n "${T[$c.$v]:-}" ] && printf '| %s | %s | %s | %s | %s | %s |\n' "$c" "$v" "${T[$c.$v]}" "${P[$c.$v]}" "${A[$c.$v]}" "${S[$c.$v]}"
  done; done
  echo
  echo "## Ratio (ARC / Zero-copy) -- higher means zero-copy wins more"
  echo
  echo "| Case | Speedup | Mem ratio | Strs ratio |"
  echo "|------|--------:|----------:|-----------:|"
  for c in $CASES; do
    ta=${T[$c.arc]:-}; tz=${T[$c.zc]:-}; pa=${P[$c.arc]:-}; pz=${P[$c.zc]:-}; sa=${S[$c.arc]:-}; sz=${S[$c.zc]:-}
    [ -n "$ta" ] && [ -n "$tz" ] || continue
    sp=$(awk -v a="$ta" -v b="$tz" 'BEGIN{printf (b>0)?"%.2f":"-",a/b}')
    mr=$(awk -v a="$pa" -v b="$pz" 'BEGIN{printf (b>0)?"%.2f":"-",a/b}')
    sr=$(awk -v a="$sa" -v b="$sz" 'BEGIN{printf (b>0)?"%.2f":"-",a/b}')
    printf '| %s | %sx | %sx | %sx |\n' "$c" "$sp" "$mr" "$sr"
  done
} | tee "$RES"
echo
echo "Wrote $RES"
