#!/usr/bin/env bash
# run_all.sh -- Tauraro Benchmark Suite: C vs Rust vs Tauraro (Linux/macOS)
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BENCH="$SCRIPT_DIR"

# Self-hosted compiler — look next to this script's build dir, then in PATH
if   [ -x "$ROOT/tauraro/src/build/tauraroc" ]; then
    TAU_EXE="$ROOT/tauraro/src/build/tauraroc"
elif command -v tauraroc &>/dev/null; then
    TAU_EXE="tauraroc"
else
    echo "ERROR: tauraroc not found. Build it first or add it to PATH." >&2
    exit 1
fi

# ── Colors ────────────────────────────────────────────────────────────────────
RED='\033[0;31m'; GRN='\033[0;32m'; YLW='\033[0;33m'
CYN='\033[0;36m'; WHT='\033[1;37m'; GRY='\033[0;37m'; RST='\033[0m'

# ── Helpers ───────────────────────────────────────────────────────────────────

run_bench() {
    local exe="$1"
    local out
    out="$("$exe" 2>&1)"
    local ms
    ms="$(echo "$out" | grep -oP '(?<=TIME_MS:)\d+')"
    if [ -n "$ms" ]; then
        # convert ms to seconds with 3 decimal places
        echo "scale=3; $ms / 1000" | bc
    fi
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

    c_time=""
    rs_time=""
    tr_time=""

    [ $c_ok  -eq 1 ] && c_time="$(run_bench  "$dir/bench_c"  2>/dev/null || echo '')"
    [ $rs_ok -eq 1 ] && rs_time="$(run_bench "$dir/bench_rs" 2>/dev/null || echo '')"
    # Self-hosted tauraroc places the exe in build/bench(.exe)
    if   [ $tr_ok -eq 1 ] && [ -x "$dir/build/bench.exe" ]; then
        tr_time="$(run_bench "$dir/build/bench.exe" 2>/dev/null || echo '')"
    elif [ $tr_ok -eq 1 ] && [ -x "$dir/build/bench" ]; then
        tr_time="$(run_bench "$dir/build/bench" 2>/dev/null || echo '')"
    elif [ $tr_ok -eq 1 ] && [ -x "$dir/bench" ]; then
        tr_time="$(run_bench "$dir/bench" 2>/dev/null || echo '')"
    fi

    tau_c=""
    tau_rs=""
    if [ -n "$c_time"  ] && [ -n "$tr_time" ] && [ "$c_time"  != "0" ]; then
        tau_c="$(echo "scale=2; $tr_time / $c_time"  | bc)x"
    fi
    if [ -n "$rs_time" ] && [ -n "$tr_time" ] && [ "$rs_time" != "0" ]; then
        tau_rs="$(echo "scale=2; $tr_time / $rs_time" | bc)x"
    fi

    results+=("$name|${c_time:-FAIL}|${rs_time:-FAIL}|${tr_time:-FAIL}|${tau_c:---}|${tau_rs:---}")
    printf "  Done: C=${c_time:-FAIL}s  Rust=${rs_time:-FAIL}s  Tauraro=${tr_time:-FAIL}s\n\n"
done

# ── Results table ──────────────────────────────────────────────────────────────

printf "${CYN}=================================================================${RST}\n"
printf "${CYN}  RESULTS  (seconds -- lower is faster)${RST}\n"
printf "${CYN}=================================================================${RST}\n\n"

printf "${WHT}%-24s %8s %8s %10s %9s %9s${RST}\n" \
    "Benchmark" "C(s)" "Rust(s)" "Tauraro(s)" "Tau/C" "Tau/Rust"
printf "${GRY}%-24s %8s %8s %10s %9s %9s${RST}\n" \
    "------------------------" "-------" "-------" "---------" "--------" "--------"

for row in "${results[@]}"; do
    IFS='|' read -r rname c_t rs_t tr_t tc trs <<< "$row"
    ratio="${tc%%x*}"
    color="$WHT"
    if [ -n "$ratio" ] && (( $(echo "$ratio <= 1.05" | bc -l) )); then
        color="$GRN"
    elif [ -n "$ratio" ] && (( $(echo "$ratio <= 1.20" | bc -l) )); then
        color="$YLW"
    fi
    printf "${color}%-24s %8s %8s %10s %9s %9s${RST}\n" \
        "$rname" "${c_t}s" "${rs_t}s" "${tr_t}s" "$tc" "$trs"
done

printf "\n"
printf "${GRY}  Tau/C    = Tauraro / C time    (< 1.00x = Tauraro faster than C)${RST}\n"
printf "${GRY}  Tau/Rust = Tauraro / Rust time (< 1.00x = Tauraro faster than Rust)${RST}\n\n"
