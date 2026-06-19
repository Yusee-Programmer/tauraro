#!/usr/bin/env bash
# run_all.sh -- Tauraro Benchmark Suite: C vs Rust vs Tauraro (Linux/macOS)
# Reports BOTH wall time (from each program's TIME_MS:) and peak resident
# memory (via /usr/bin/time). Writes a Markdown report to benchmarks/results.md.
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BENCH="$SCRIPT_DIR"
RESULTS_MD="$BENCH/results.md"

# Self-hosted compiler — look next to this script's build dir, then in PATH
if   [ -x "$ROOT/tauraro/src/build/tauraroc" ]; then
    TAU_EXE="$ROOT/tauraro/src/build/tauraroc"
elif [ -x "$ROOT/tauraro/tauraroc" ]; then
    TAU_EXE="$ROOT/tauraro/tauraroc"
elif [ -x "$BENCH/../tauraroc" ]; then
    TAU_EXE="$BENCH/../tauraroc"
elif command -v tauraroc &>/dev/null; then
    TAU_EXE="tauraroc"
else
    echo "ERROR: tauraroc not found. Build it first or add it to PATH." >&2
    exit 1
fi

# ── Colors ────────────────────────────────────────────────────────────────────
RED='\033[0;31m'; GRN='\033[0;32m'; YLW='\033[0;33m'
CYN='\033[0;36m'; WHT='\033[1;37m'; GRY='\033[0;37m'; RST='\033[0m'

# ── Memory-measuring `time` detection ──────────────────────────────────────────
# GNU /usr/bin/time -v gives "Maximum resident set size (kbytes)"; BSD (macOS)
# /usr/bin/time -l gives "maximum resident set size" in bytes. If neither is
# present, memory columns show "n/a" but timing still works.
HAVE_TIME=0
IS_MACOS=0
[ "$(uname)" = "Darwin" ] && IS_MACOS=1
if [ -x /usr/bin/time ]; then HAVE_TIME=1; fi

# ── Helpers ───────────────────────────────────────────────────────────────────

# measure <exe> -> echoes "TIME_S|MEM_KB" (either field may be empty on failure)
measure() {
    local exe="$1"
    local timefile out ms time_s rss_kb
    timefile="$(mktemp)"
    if [ "$HAVE_TIME" -eq 1 ] && [ "$IS_MACOS" -eq 1 ]; then
        out="$(/usr/bin/time -l "$exe" 2>"$timefile" || true)"
        local rss_b
        rss_b="$(grep -i 'maximum resident set size' "$timefile" | grep -oE '[0-9]+' | head -1)"
        [ -n "$rss_b" ] && rss_kb="$(echo "scale=1; $rss_b / 1024" | bc)"
    elif [ "$HAVE_TIME" -eq 1 ]; then
        out="$(/usr/bin/time -v "$exe" 2>"$timefile" || true)"
        rss_kb="$(grep -i 'Maximum resident set size' "$timefile" | grep -oE '[0-9]+' | head -1)"
    else
        out="$("$exe" 2>"$timefile" || true)"
    fi
    rm -f "$timefile"
    ms="$(echo "$out" | grep -oE 'TIME_MS:[0-9]+' | grep -oE '[0-9]+' | head -1)"
    [ -n "$ms" ] && time_s="$(echo "scale=3; $ms / 1000" | bc)"
    echo "${time_s:-}|${rss_kb:-}"
}

compile_c() {
    local src="$1" out="$2" extra="${3:-}"
    gcc -O3 $extra -o "$out" "$src" -lm 2>&1
}

compile_rust() {
    local src="$1" out="$2"
    rustc -C opt-level=3 -C target-cpu=native -o "$out" "$src" 2>&1
}

compile_tauraro() {
    local src="$1"
    "$TAU_EXE" -O3 "$src" 2>&1
}

# Format a maybe-empty number with a unit, else "FAIL".
fmt() { if [ -n "$1" ]; then echo "$1$2"; else echo "FAIL"; fi; }

# ── Benchmark list ────────────────────────────────────────────────────────────

benchmarks=(
    "1 - Integer Sum|1_sum"
    "2 - Fibonacci|2_fibonacci"
    "3 - Float Multiply|3_float_mul"
    "4 - XOR Shift PRNG|4_xorshift"
    "5 - Newton Sqrt|5_newton"
    "6 - Mandelbrot|6_mandelbrot"
    "7 - Sieve 50M|7_sieve"
    "8 - N-Body 3b|8_nbody"
    "9 - Collatz 10M|9_collatz"
    "10 - MatMul 400x400|10_matmul"
)

echo ""
printf "${CYN}=================================================================${RST}\n"
printf "${CYN}   Tauraro Benchmark Suite  --  C vs Rust vs Tauraro${RST}\n"
printf "${CYN}   Compiler: %s${RST}\n" "$TAU_EXE"
if [ "$HAVE_TIME" -eq 0 ]; then
    printf "${YLW}   (note: /usr/bin/time absent -- memory columns will be n/a)${RST}\n"
fi
printf "${CYN}=================================================================${RST}\n\n"

declare -a results=()

for entry in "${benchmarks[@]}"; do
    name="${entry%%|*}"
    dir="$BENCH/${entry##*|}"

    printf "${YLW}Compiling %-22s...${RST}\n" "$name"

    c_ok=0; rs_ok=0; tr_ok=0

    if compile_c  "$dir/bench.c"  "$dir/bench_c"  2>/dev/null; then c_ok=1; fi
    if compile_rust "$dir/bench.rs" "$dir/bench_rs" 2>/dev/null; then rs_ok=1; fi
    if compile_tauraro "$dir/bench.tr"              2>/dev/null; then tr_ok=1; fi

    printf "  ${GRY}Running...${RST}\n"

    c_time=""; c_mem=""
    rs_time=""; rs_mem=""
    tr_time=""; tr_mem=""

    if [ $c_ok -eq 1 ]; then IFS='|' read -r c_time c_mem <<< "$(measure "$dir/bench_c")"; fi
    if [ $rs_ok -eq 1 ]; then IFS='|' read -r rs_time rs_mem <<< "$(measure "$dir/bench_rs")"; fi
    # Self-hosted tauraroc places the exe in build/bench(.exe)
    tr_exe=""
    if   [ $tr_ok -eq 1 ] && [ -x "$dir/build/bench.exe" ];   then tr_exe="$dir/build/bench.exe"
    elif [ $tr_ok -eq 1 ] && [ -x "$dir/build/bench" ];       then tr_exe="$dir/build/bench"
    elif [ $tr_ok -eq 1 ] && [ -x "$BENCH/build/bench.exe" ]; then tr_exe="$BENCH/build/bench.exe"
    elif [ $tr_ok -eq 1 ] && [ -x "$BENCH/build/bench" ];     then tr_exe="$BENCH/build/bench"
    elif [ $tr_ok -eq 1 ] && [ -x "$dir/bench" ];             then tr_exe="$dir/bench"
    fi
    if [ -n "$tr_exe" ]; then IFS='|' read -r tr_time tr_mem <<< "$(measure "$tr_exe")"; fi

    tau_c=""; tau_rs=""
    if [ -n "$c_time"  ] && [ -n "$tr_time" ] && [ "$c_time"  != "0" ]; then
        tau_c="$(echo "scale=2; $tr_time / $c_time"  | bc)x"
    fi
    if [ -n "$rs_time" ] && [ -n "$tr_time" ] && [ "$rs_time" != "0" ]; then
        tau_rs="$(echo "scale=2; $tr_time / $rs_time" | bc)x"
    fi

    results+=("$name|${c_time:-}|${rs_time:-}|${tr_time:-}|${tau_c:---}|${tau_rs:---}|${c_mem:-}|${rs_mem:-}|${tr_mem:-}")
    printf "  Done: C=$(fmt "$c_time" s)/$(fmt "$c_mem" KB)  Rust=$(fmt "$rs_time" s)/$(fmt "$rs_mem" KB)  Tauraro=$(fmt "$tr_time" s)/$(fmt "$tr_mem" KB)\n\n"
done

# ── Console: timing table ───────────────────────────────────────────────────────

printf "${CYN}=================================================================${RST}\n"
printf "${CYN}  RESULTS  (seconds -- lower is faster)${RST}\n"
printf "${CYN}=================================================================${RST}\n\n"

printf "${WHT}%-24s %8s %8s %10s %9s %9s${RST}\n" \
    "Benchmark" "C(s)" "Rust(s)" "Tauraro(s)" "Tau/C" "Tau/Rust"
printf "${GRY}%-24s %8s %8s %10s %9s %9s${RST}\n" \
    "------------------------" "-------" "-------" "---------" "--------" "--------"

for row in "${results[@]}"; do
    IFS='|' read -r rname c_t rs_t tr_t tc trs c_m rs_m tr_m <<< "$row"
    ratio="${tc%%x*}"
    color="$WHT"
    if [ -n "$ratio" ] && [ "$ratio" != "-" ] && (( $(echo "$ratio <= 1.05" | bc -l) )); then
        color="$GRN"
    elif [ -n "$ratio" ] && [ "$ratio" != "-" ] && (( $(echo "$ratio <= 1.20" | bc -l) )); then
        color="$YLW"
    fi
    printf "${color}%-24s %8s %8s %10s %9s %9s${RST}\n" \
        "$rname" "$(fmt "$c_t" s)" "$(fmt "$rs_t" s)" "$(fmt "$tr_t" s)" "$tc" "$trs"
done

# ── Console: memory table ───────────────────────────────────────────────────────

printf "\n${CYN}=================================================================${RST}\n"
printf "${CYN}  PEAK MEMORY  (KB -- lower is more efficient)${RST}\n"
printf "${CYN}=================================================================${RST}\n\n"

printf "${WHT}%-24s %10s %10s %10s %9s %9s${RST}\n" \
    "Benchmark" "C(KB)" "Rust(KB)" "Tau(KB)" "Tau/C" "Tau/Rust"
printf "${GRY}%-24s %10s %10s %10s %9s %9s${RST}\n" \
    "------------------------" "---------" "---------" "---------" "--------" "--------"

for row in "${results[@]}"; do
    IFS='|' read -r rname c_t rs_t tr_t tc trs c_m rs_m tr_m <<< "$row"
    tcm="--"; trsm="--"
    if [ -n "$c_m"  ] && [ -n "$tr_m" ] && [ "$c_m"  != "0" ]; then tcm="$(echo "scale=2; $tr_m / $c_m"  | bc)x"; fi
    if [ -n "$rs_m" ] && [ -n "$tr_m" ] && [ "$rs_m" != "0" ]; then trsm="$(echo "scale=2; $tr_m / $rs_m" | bc)x"; fi
    printf "${WHT}%-24s %10s %10s %10s %9s %9s${RST}\n" \
        "$rname" "$(fmt "$c_m" '')" "$(fmt "$rs_m" '')" "$(fmt "$tr_m" '')" "$tcm" "$trsm"
done

printf "\n"
printf "${GRY}  Tau/C    = Tauraro / C    (< 1.00x = Tauraro better)${RST}\n"
printf "${GRY}  Tau/Rust = Tauraro / Rust (< 1.00x = Tauraro better)${RST}\n\n"

# ── Markdown report -> benchmarks/results.md ────────────────────────────────────

{
    echo "# Tauraro Benchmark Results"
    echo ""
    echo "Auto-generated by \`benchmarks/run_all.sh\`. Lower is better in every column."
    echo ""
    echo "- **OS:** $(uname -s) $(uname -m)"
    echo "- **Date (UTC):** $(date -u '+%Y-%m-%d %H:%M:%S')"
    echo "- **Compiler:** \`$TAU_EXE\`"
    if command -v gcc &>/dev/null;   then echo "- **C:** $(gcc --version | head -1)"; fi
    if command -v rustc &>/dev/null; then echo "- **Rust:** $(rustc --version)"; fi
    echo ""
    echo "## Wall time (seconds)"
    echo ""
    echo "| Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau/C | Tau/Rust |"
    echo "|-----------|------:|---------:|------------:|------:|---------:|"
    for row in "${results[@]}"; do
        IFS='|' read -r rname c_t rs_t tr_t tc trs c_m rs_m tr_m <<< "$row"
        echo "| $rname | $(fmt "$c_t" '') | $(fmt "$rs_t" '') | $(fmt "$tr_t" '') | $tc | $trs |"
    done
    echo ""
    echo "## Peak resident memory (KB)"
    echo ""
    echo "| Benchmark | C (KB) | Rust (KB) | Tauraro (KB) | Tau/C | Tau/Rust |"
    echo "|-----------|-------:|----------:|-------------:|------:|---------:|"
    for row in "${results[@]}"; do
        IFS='|' read -r rname c_t rs_t tr_t tc trs c_m rs_m tr_m <<< "$row"
        tcm="--"; trsm="--"
        if [ -n "$c_m"  ] && [ -n "$tr_m" ] && [ "$c_m"  != "0" ]; then tcm="$(echo "scale=2; $tr_m / $c_m"  | bc)x"; fi
        if [ -n "$rs_m" ] && [ -n "$tr_m" ] && [ "$rs_m" != "0" ]; then trsm="$(echo "scale=2; $tr_m / $rs_m" | bc)x"; fi
        echo "| $rname | $(fmt "$c_m" '') | $(fmt "$rs_m" '') | $(fmt "$tr_m" '') | $tcm | $trsm |"
    done
    echo ""
    echo "_\`Tau/C\` and \`Tau/Rust\` are ratios: < 1.00x means Tauraro is faster / leaner._"
    if [ "$HAVE_TIME" -eq 0 ]; then
        echo ""
        echo "> ⚠️ \`/usr/bin/time\` was unavailable on this host, so memory figures are missing."
    fi
} > "$RESULTS_MD"

printf "${GRN}Wrote Markdown report: %s${RST}\n\n" "$RESULTS_MD"
