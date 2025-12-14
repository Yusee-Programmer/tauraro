#!/bin/bash
# Comprehensive Tauraro vs Python Benchmark Suite
# Automatically compiles and benchmarks all test files

echo "=========================================="
echo "TAURARO vs PYTHON - COMPREHENSIVE BENCHMARKS"
echo "=========================================="
echo ""

# Array of benchmark files
BENCHMARKS=(
    "benchmark_01_basic_types"
    "benchmark_02_arithmetic"
    "benchmark_03_control_flow"
    "benchmark_04_functions"
)

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Results array
declare -A python_times
declare -A tauraro_times
declare -A speedups
declare -A statuses

total_speedup=0
successful_count=0

for bench in "${BENCHMARKS[@]}"; do
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo -e "${BLUE}Testing: ${bench}${NC}"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

    py_file="${bench}.py"
    c_file="${bench}.c"
    exe_file="${bench}.exe"

    # Step 1: Compile to C
    echo -e "${YELLOW}[1/4] Compiling to C...${NC}"
    timeout 60 ./target/release/tauraro.exe compile "$py_file" \
        --use-native-transpiler --backend c -o "$bench" 2>&1 | grep -E "(Error|success)" || true

    if [ ! -f "${bench}.exe.c" ]; then
        echo -e "${RED}✗ C code generation failed${NC}"
        statuses[$bench]="FAILED_COMPILE"
        continue
    fi

    # Step 2: Patch generated C code (temporary fix)
    echo -e "${YELLOW}[2/4] Patching generated C code...${NC}"
    # Add len function if missing
    if ! grep -q "int tauraro_len_string" "${bench}.exe.c"; then
        sed -i '71a int tauraro_len_string(const char* str) {\n    return str ? strlen(str) : 0;\n}' "${bench}.exe.c"
    fi
    # Fix main() recursive call
    sed -i 's/^    main();$/    \/\/ main() call removed/' "${bench}.exe.c"

    # Step 3: Compile C to executable
    echo -e "${YELLOW}[3/4] Compiling C to native executable...${NC}"
    if gcc "${bench}.exe.c" -o "$exe_file" -lm -O3 2>&1 | grep -v "warning"; then
        echo -e "${GREEN}✓ Compilation successful${NC}"
    else
        echo -e "${RED}✗ GCC compilation failed${NC}"
        statuses[$bench]="FAILED_GCC"
        continue
    fi

    # Step 4: Run benchmarks
    echo -e "${YELLOW}[4/4] Running benchmarks...${NC}"
    echo ""

    # Run Python
    echo -e "  ${BLUE}Running Python...${NC}"
    py_start=$(date +%s.%N)
    py_output=$(timeout 120 python "$py_file" 2>&1)
    py_end=$(date +%s.%N)
    py_time=$(echo "$py_end - $py_start" | bc)
    python_times[$bench]=$py_time

    # Run Tauraro
    echo -e "  ${BLUE}Running Tauraro (compiled)...${NC}"
    tau_start=$(date +%s.%N)
    tau_output=$(timeout 120 "./$exe_file" 2>&1)
    tau_end=$(date +%s.%N)
    tau_time=$(echo "$tau_end - $tau_start" | bc)
    tauraro_times[$bench]=$tau_time

    # Calculate speedup
    speedup=$(echo "scale=2; $py_time / $tau_time" | bc)
    speedups[$bench]=$speedup
    statuses[$bench]="SUCCESS"

    # Verify output matches
    if [ "$py_output" == "$tau_output" ]; then
        output_match="${GREEN}✓ Outputs match${NC}"
    else
        output_match="${YELLOW}⚠  Outputs differ${NC}"
    fi

    # Display results
    echo ""
    echo -e "${GREEN}Results:${NC}"
    printf "  Python:  %.3fs\n" $py_time
    printf "  Tauraro: %.3fs\n" $tau_time
    printf "  Speedup: ${GREEN}%.2fx${NC}\n" $speedup
    echo -e "  $output_match"

    total_speedup=$(echo "$total_speedup + $speedup" | bc)
    ((successful_count++))
done

# Final Summary
echo ""
echo ""
echo "=========================================="
echo "FINAL SUMMARY"
echo "=========================================="
echo ""
printf "%-35s %12s %12s %10s %s\n" "Benchmark" "Python (s)" "Tauraro (s)" "Speedup" "Status"
echo "──────────────────────────────────────────────────────────────────────────────"

for bench in "${BENCHMARKS[@]}"; do
    status="${statuses[$bench]}"
    if [ "$status" == "SUCCESS" ]; then
        printf "%-35s %12.3f %12.3f ${GREEN}%9.2fx${NC} ${GREEN}✓${NC}\n" \
            "$bench" "${python_times[$bench]}" "${tauraro_times[$bench]}" "${speedups[$bench]}"
    else
        printf "%-35s %12s %12s %10s ${RED}✗ $status${NC}\n" "$bench" "N/A" "N/A" "N/A"
    fi
done

echo "══════════════════════════════════════════════════════════════════════════════"

if [ $successful_count -gt 0 ]; then
    avg_speedup=$(echo "scale=2; $total_speedup / $successful_count" | bc)
    echo ""
    echo -e "${GREEN}Average Speedup: ${avg_speedup}x${NC}"
    echo -e "${GREEN}Successful: $successful_count/${#BENCHMARKS[@]}${NC}"
else
    echo -e "${RED}No successful benchmarks${NC}"
fi

echo ""
