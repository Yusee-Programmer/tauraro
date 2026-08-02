#!/usr/bin/env bash
# run_all_llvm.sh -- Tauraro Benchmark Suite, LLVM BACKEND (Linux/macOS/Windows-MSYS)
# Compiles + runs every benchmark with `tauraroc --backend llvm` and compares against
# gcc -O3 C, rustc -O3, and the DEFAULT Tauraro C backend, so the LLVM speedup is visible.
# Reports wall time (from each program's TIME_MS:) and peak resident memory. Writes a
# Markdown report to benchmarks/results_llvm.md.
#
# The LLVM driver produces a running executable in one shot (it builds runtime.o + invokes
# clang/llc and links the right platform libs itself), so `--backend llvm <src> -o <exe>`
# is all we need. Requires clang or llc (>= 15, for opaque pointers) on PATH.
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BENCH="$SCRIPT_DIR"
RESULTS_MD="$BENCH/results_llvm.md"

# Self-hosted compiler -- prefer a freshly-built binary at the repo root/build dir, then
# PATH. Both bare and .exe names so it works on Linux/macOS and Windows (MSYS/Git-Bash).
TAU_EXE=""
for cand in \
    "$BENCH/../tauraroc" "$BENCH/../tauraroc.exe" \
    "$ROOT/tauraro/src/build/tauraroc" "$ROOT/tauraro/tauraroc" "$ROOT/tauraro/tauraroc.exe"; do
    if [ -x "$cand" ]; then TAU_EXE="$cand"; break; fi
done
if [ -z "$TAU_EXE" ] && command -v tauraroc &>/dev/null; then TAU_EXE="tauraroc"; fi
if [ -z "$TAU_EXE" ]; then
    echo "ERROR: tauraroc not found. Build it first or add it to PATH." >&2
    exit 1
fi

# The LLVM backend needs clang or llc (>= 15) to turn the emitted .ll into a binary.
if ! command -v clang &>/dev/null && ! command -v llc &>/dev/null; then
    echo "ERROR: neither clang nor llc found -- the LLVM backend cannot link. Install LLVM >= 15." >&2
    exit 1
fi

# ── Colors ────────────────────────────────────────────────────────────────────
RED='\033[0;31m'; GRN='\033[0;32m'; YLW='\033[0;33m'
CYN='\033[0;36m'; WHT='\033[1;37m'; GRY='\033[0;37m'; RST='\033[0m'

# ── Portable arithmetic (awk; no `bc`) ──────────────────────────────────────────
div() { [ -n "$1" ] && [ -n "$2" ] && awk "BEGIN{d=$2; if(d==0) exit 1; printf \"%.${3:-2}f\", $1/d}"; }
ratio() { local q; q="$(div "$1" "$2" 2)"; [ -n "$q" ] && echo "${q}x"; }
le()  { awk "BEGIN{exit !($1<=$2)}"; }

# ── Memory-measuring `time` detection ──────────────────────────────────────────
HAVE_TIME=0; IS_MACOS=0
[ "$(uname)" = "Darwin" ] && IS_MACOS=1
[ -x /usr/bin/time ] && HAVE_TIME=1

# measure <exe> -> echoes "TIME_S|MEM_KB"
measure() {
    local exe="$1"; local timefile out ms time_s rss_kb
    timefile="$(mktemp)"
    if [ "$HAVE_TIME" -eq 1 ] && [ "$IS_MACOS" -eq 1 ]; then
        out="$(/usr/bin/time -l "$exe" 2>"$timefile" || true)"
        local rss_b; rss_b="$(grep -i 'maximum resident set size' "$timefile" | grep -oE '[0-9]+' | head -1)"
        [ -n "$rss_b" ] && rss_kb="$(div "$rss_b" 1024 1)"
    elif [ "$HAVE_TIME" -eq 1 ]; then
        out="$(/usr/bin/time -v "$exe" 2>"$timefile" || true)"
        rss_kb="$(grep -i 'Maximum resident set size' "$timefile" | grep -oE '[0-9]+' | head -1)"
    else
        out="$("$exe" 2>"$timefile" || true)"
    fi
    rm -f "$timefile"
    ms="$(echo "$out" | grep -oE 'TIME_MS:[0-9]+' | grep -oE '[0-9]+' | head -1)"
    [ -n "$ms" ] && time_s="$(div "$ms" 1000 3)"
    echo "${time_s:-}|${rss_kb:-}"
}

# The C and LLVM backends BOTH write to a CWD-relative build/ dir; on Windows a just-run
# bench(.exe) or the LLVM driver's log can hold a handle, so the next compile fails with
# "Permission denied" / "resource busy". Wipe build/ (retrying past the transient lock)
# before every Tauraro compile so each starts from a clean, unlocked directory.
clean_build() {
    local n=0
    rm -rf "$BENCH/build" 2>/dev/null
    while [ -d "$BENCH/build" ] && [ "$n" -lt 50 ]; do sleep 0.1; rm -rf "$BENCH/build" 2>/dev/null; n=$((n+1)); done
}

compile_c()    { gcc -O3 -o "$2" "$1" -lm 2>&1; }
compile_rust() { rustc -C opt-level=3 -C target-cpu=native -o "$2" "$1" 2>&1; }
# Default Tauraro C backend (writes build/bench(.exe)).
compile_tauraro_c()    { "$TAU_EXE" -O3 "$1" 2>&1; }
# LLVM backend -- produces the exe directly at $2 (the driver links platform libs itself).
compile_tauraro_llvm() { "$TAU_EXE" --backend llvm -O3 "$1" -o "$2" 2>&1; }

fmt() { if [ -n "$1" ]; then echo "$1$2"; else echo "FAIL"; fi; }

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
printf "${CYN}   Tauraro Benchmark Suite  --  LLVM backend vs C / Rust / Tau-C${RST}\n"
printf "${CYN}   Compiler: %s${RST}\n" "$TAU_EXE"
if command -v clang &>/dev/null; then printf "${CYN}   LLVM: %s${RST}\n" "$(clang --version | head -1)"; fi
[ "$HAVE_TIME" -eq 0 ] && printf "${YLW}   (note: /usr/bin/time absent -- memory columns will be n/a)${RST}\n"
printf "${CYN}=================================================================${RST}\n\n"

declare -a results=()

# Anchor CWD to $BENCH so the C-backend's CWD-relative build/ dir is $BENCH/build.
cd "$BENCH" || exit 1

for entry in "${benchmarks[@]}"; do
    name="${entry%%|*}"
    dir="$BENCH/${entry##*|}"

    printf "${YLW}Compiling %-22s...${RST}\n" "$name"

    c_ok=0; rs_ok=0; tc_ok=0; ll_ok=0
    compile_c    "$dir/bench.c"  "$dir/bench_c"  &>/dev/null && c_ok=1
    compile_rust "$dir/bench.rs" "$dir/bench_rs" &>/dev/null && rs_ok=1
    clean_build
    compile_tauraro_c "$dir/bench.tr"            &>/dev/null && tc_ok=1
    # Resolve the C-backend exe (build/bench(.exe)) BEFORE the LLVM compile reuses build/.
    tc_exe=""
    for cand in "$dir/build/bench.exe" "$dir/build/bench" "$BENCH/build/bench.exe" "$BENCH/build/bench"; do
        [ -x "$cand" ] && { tc_exe="$cand"; break; }
    done
    tc_time=""; tc_mem=""
    [ $tc_ok -eq 1 ] && [ -n "$tc_exe" ] && IFS='|' read -r tc_time tc_mem <<< "$(measure "$tc_exe")"

    # LLVM backend -> its own exe path (avoids clobbering the C-backend build/).
    clean_build
    ll_exe="$dir/bench_llvm"; [ -f "$ll_exe.exe" ] && ll_exe="$ll_exe.exe"
    ll_out="$dir/bench_llvm"
    if compile_tauraro_llvm "$dir/bench.tr" "$ll_out" &>/dev/null; then ll_ok=1; fi
    # The driver may append .exe on Windows.
    ll_exe=""
    for cand in "$ll_out" "$ll_out.exe"; do [ -x "$cand" ] && { ll_exe="$cand"; break; }; done

    printf "  ${GRY}Running...${RST}\n"

    c_time=""; c_mem=""; rs_time=""; rs_mem=""; ll_time=""; ll_mem=""
    [ $c_ok  -eq 1 ] && IFS='|' read -r c_time  c_mem  <<< "$(measure "$dir/bench_c")"
    [ $rs_ok -eq 1 ] && IFS='|' read -r rs_time rs_mem <<< "$(measure "$dir/bench_rs")"
    [ $ll_ok -eq 1 ] && [ -n "$ll_exe" ] && IFS='|' read -r ll_time ll_mem <<< "$(measure "$ll_exe")"

    # Ratios are all relative to the LLVM backend (the subject of this script).
    ll_c=""; ll_rs=""; ll_tc=""
    [ -n "$c_time"  ] && [ -n "$ll_time" ] && [ "$c_time"  != "0" ] && ll_c="$(ratio "$ll_time" "$c_time")"
    [ -n "$rs_time" ] && [ -n "$ll_time" ] && [ "$rs_time" != "0" ] && ll_rs="$(ratio "$ll_time" "$rs_time")"
    [ -n "$tc_time" ] && [ -n "$ll_time" ] && [ "$tc_time" != "0" ] && ll_tc="$(ratio "$ll_time" "$tc_time")"

    results+=("$name|${c_time:-}|${rs_time:-}|${tc_time:-}|${ll_time:-}|${ll_c:---}|${ll_rs:---}|${ll_tc:---}|${c_mem:-}|${rs_mem:-}|${tc_mem:-}|${ll_mem:-}")
    printf "  Done: C=$(fmt "$c_time" s)  Rust=$(fmt "$rs_time" s)  Tau-C=$(fmt "$tc_time" s)  ${WHT}Tau-LLVM=$(fmt "$ll_time" s)${RST}\n\n"
done

# ── Console: timing table ───────────────────────────────────────────────────────
printf "${CYN}=================================================================${RST}\n"
printf "${CYN}  RESULTS  (seconds -- lower is faster; ratios vs the LLVM backend)${RST}\n"
printf "${CYN}=================================================================${RST}\n\n"

printf "${WHT}%-22s %7s %7s %7s %9s %7s %8s %8s${RST}\n" \
    "Benchmark" "C" "Rust" "Tau-C" "Tau-LLVM" "LL/C" "LL/Rust" "LL/TauC"
printf "${GRY}%-22s %7s %7s %7s %9s %7s %8s %8s${RST}\n" \
    "----------------------" "------" "------" "------" "--------" "------" "-------" "-------"

for row in "${results[@]}"; do
    IFS='|' read -r rname c_t rs_t tc_t ll_t lc lrs ltc c_m rs_m tc_m ll_m <<< "$row"
    ratio="${lc%%x*}"; color="$WHT"
    if [ -n "$ratio" ] && [ "$ratio" != "-" ] && le "$ratio" 1.05; then color="$GRN"
    elif [ -n "$ratio" ] && [ "$ratio" != "-" ] && le "$ratio" 1.20; then color="$YLW"; fi
    printf "${color}%-22s %7s %7s %7s %9s %7s %8s %8s${RST}\n" \
        "$rname" "$(fmt "$c_t" '')" "$(fmt "$rs_t" '')" "$(fmt "$tc_t" '')" "$(fmt "$ll_t" '')" "$lc" "$lrs" "$ltc"
done

# ── Console: memory table ───────────────────────────────────────────────────────
printf "\n${CYN}=================================================================${RST}\n"
printf "${CYN}  PEAK MEMORY  (KB -- lower is leaner)${RST}\n"
printf "${CYN}=================================================================${RST}\n\n"

printf "${WHT}%-22s %9s %9s %9s %11s${RST}\n" "Benchmark" "C(KB)" "Rust(KB)" "TauC(KB)" "TauLLVM(KB)"
printf "${GRY}%-22s %9s %9s %9s %11s${RST}\n" "----------------------" "--------" "--------" "--------" "----------"
for row in "${results[@]}"; do
    IFS='|' read -r rname c_t rs_t tc_t ll_t lc lrs ltc c_m rs_m tc_m ll_m <<< "$row"
    printf "${WHT}%-22s %9s %9s %9s %11s${RST}\n" \
        "$rname" "$(fmt "$c_m" '')" "$(fmt "$rs_m" '')" "$(fmt "$tc_m" '')" "$(fmt "$ll_m" '')"
done

printf "\n${GRY}  LL/C = Tauraro-LLVM / C, LL/Rust vs rustc, LL/TauC vs the C backend (< 1.00x = faster)${RST}\n\n"

# ── Markdown report -> benchmarks/results_llvm.md ───────────────────────────────
{
    echo "# Tauraro Benchmark Results -- LLVM backend"
    echo ""
    echo "Auto-generated by \`benchmarks/run_all_llvm.sh\`. Lower is better in every column."
    echo "Ratios (\`LL/*\`) are relative to the **LLVM backend** (< 1.00x = LLVM faster/leaner)."
    echo ""
    echo "- **OS:** $(uname -s) $(uname -m)"
    echo "- **Date (UTC):** $(date -u '+%Y-%m-%d %H:%M:%S')"
    echo "- **Compiler:** \`$TAU_EXE\`"
    command -v clang &>/dev/null && echo "- **LLVM:** $(clang --version | head -1)"
    command -v gcc   &>/dev/null && echo "- **C:** $(gcc --version | head -1)"
    command -v rustc &>/dev/null && echo "- **Rust:** $(rustc --version)"
    echo ""
    echo "## Wall time (seconds)"
    echo ""
    echo "| Benchmark | C (s) | Rust (s) | Tau-C (s) | Tau-LLVM (s) | LL/C | LL/Rust | LL/TauC |"
    echo "|-----------|------:|---------:|----------:|-------------:|-----:|--------:|--------:|"
    for row in "${results[@]}"; do
        IFS='|' read -r rname c_t rs_t tc_t ll_t lc lrs ltc c_m rs_m tc_m ll_m <<< "$row"
        echo "| $rname | $(fmt "$c_t" '') | $(fmt "$rs_t" '') | $(fmt "$tc_t" '') | $(fmt "$ll_t" '') | $lc | $lrs | $ltc |"
    done
    echo ""
    echo "## Peak resident memory (KB)"
    echo ""
    echo "| Benchmark | C (KB) | Rust (KB) | Tau-C (KB) | Tau-LLVM (KB) |"
    echo "|-----------|-------:|----------:|-----------:|--------------:|"
    for row in "${results[@]}"; do
        IFS='|' read -r rname c_t rs_t tc_t ll_t lc lrs ltc c_m rs_m tc_m ll_m <<< "$row"
        echo "| $rname | $(fmt "$c_m" '') | $(fmt "$rs_m" '') | $(fmt "$tc_m" '') | $(fmt "$ll_m" '') |"
    done
    echo ""
    echo "_\`Tau-C\` is the default C backend; \`Tau-LLVM\` is \`--backend llvm\`. \`LL/TauC\` < 1.00x means the LLVM backend is faster than the C backend._"
    [ "$HAVE_TIME" -eq 0 ] && { echo ""; echo "> ⚠️ \`/usr/bin/time\` was unavailable, so memory figures are missing."; }
} > "$RESULTS_MD"

printf "${GRN}Wrote Markdown report: %s${RST}\n\n" "$RESULTS_MD"
