#!/bin/bash
# Tauraro vs Python Benchmark Runner
# Run this script from the tauraro root directory
# Usage: ./benchmarks/run_benchmarks.sh

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

echo -e "\n${CYAN}========================================================"
echo "   TAURARO vs PYTHON BENCHMARK SUITE (1 Billion Iterations)"
echo -e "========================================================${NC}\n"

# Detect OS
OS="unknown"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    OS="windows"
fi
echo -e "Detected OS: ${YELLOW}$OS${NC}"

# Check if tauraro compiler exists
TAURARO="./target/release/tauraro"
if [[ "$OS" == "windows" ]]; then
    TAURARO="./target/release/tauraro.exe"
fi

if [ ! -f "$TAURARO" ]; then
    echo -e "${YELLOW}Building Tauraro compiler...${NC}"
    cargo build --release
fi

# Benchmark list
declare -a BENCHMARKS=(
    "Simple Loop:bench_loop"
    "Arithmetic:bench_arithmetic"
    "Function Calls:bench_function_calls"
    "Conditionals:bench_conditionals"
    "Nested Loops:bench_nested_loops"
    "Bitwise Ops:bench_bitwise"
)

# Results arrays
declare -a NAMES
declare -a TAU_TIMES
declare -a PY_TIMES
declare -a SPEEDUPS

# Time function that works on both Linux and macOS
get_time() {
    if [[ "$OS" == "macos" ]]; then
        # macOS: use perl for millisecond precision
        perl -MTime::HiRes=time -e 'print time'
    else
        # Linux: use date with nanoseconds
        date +%s.%N
    fi
}

idx=0
for bench in "${BENCHMARKS[@]}"; do
    IFS=':' read -r name file <<< "$bench"
    
    TR_FILE="benchmarks/${file}.tr"
    PY_FILE="benchmarks/${file}.py"
    C_FILE="benchmarks/${file}.c"
    EXE_FILE="benchmarks/${file}"
    
    echo -e "\n${GREEN}--- Benchmark: $name ---${NC}"
    
    # Compile Tauraro to C
    echo "  Compiling ${file}.tr to C..."
    $TAURARO compile "$TR_FILE" --backend c -o "$C_FILE" 2>/dev/null
    
    # Compile C with GCC/Clang -O3
    echo "  Compiling C with -O3..."
    if command -v gcc &> /dev/null; then
        gcc -O3 -o "$EXE_FILE" "$C_FILE" -lm 2>/dev/null
    elif command -v clang &> /dev/null; then
        clang -O3 -o "$EXE_FILE" "$C_FILE" -lm 2>/dev/null
    else
        echo -e "${RED}Error: No C compiler found${NC}"
        exit 1
    fi
    
    # Run Tauraro benchmark
    echo "  Running Tauraro..."
    START=$(get_time)
    ./"$EXE_FILE" > /dev/null 2>&1
    END=$(get_time)
    TAU_TIME=$(echo "$END - $START" | bc)
    echo -e "    Tauraro: ${CYAN}${TAU_TIME}s${NC}"
    
    # Run Python benchmark
    echo "  Running Python..."
    START=$(get_time)
    python3 "$PY_FILE" > /dev/null 2>&1
    END=$(get_time)
    PY_TIME=$(echo "$END - $START" | bc)
    echo -e "    Python:  ${YELLOW}${PY_TIME}s${NC}"
    
    # Calculate speedup
    SPEEDUP=$(echo "scale=1; $PY_TIME / $TAU_TIME" | bc)
    echo -e "    Speedup: ${MAGENTA}${SPEEDUP}x${NC}"
    
    # Store results
    NAMES[$idx]="$name"
    TAU_TIMES[$idx]="$TAU_TIME"
    PY_TIMES[$idx]="$PY_TIME"
    SPEEDUPS[$idx]="$SPEEDUP"
    idx=$((idx + 1))
    
    # Cleanup
    rm -f "$C_FILE" "$EXE_FILE"
done

# Print results table
echo -e "\n\n${CYAN}========================================================"
echo "                    RESULTS TABLE"
echo -e "========================================================${NC}"
printf "| %-17s | %11s | %10s | %7s |\n" "Benchmark" "Tauraro (s)" "Python (s)" "Speedup"
echo "|-------------------|-------------|------------|---------|"

TOTAL_TAU=0
TOTAL_PY=0

for i in "${!NAMES[@]}"; do
    printf "| %-17s | %11.2f | %10.2f | %6.1fx |\n" "${NAMES[$i]}" "${TAU_TIMES[$i]}" "${PY_TIMES[$i]}" "${SPEEDUPS[$i]}"
    TOTAL_TAU=$(echo "$TOTAL_TAU + ${TAU_TIMES[$i]}" | bc)
    TOTAL_PY=$(echo "$TOTAL_PY + ${PY_TIMES[$i]}" | bc)
done

echo "|-------------------|-------------|------------|---------|"

# Calculate averages
NUM=${#NAMES[@]}
AVG_TAU=$(echo "scale=2; $TOTAL_TAU / $NUM" | bc)
AVG_PY=$(echo "scale=2; $TOTAL_PY / $NUM" | bc)
AVG_SPEEDUP=$(echo "scale=1; $AVG_PY / $AVG_TAU" | bc)

printf "${GREEN}| %-17s | %11.2f | %10.2f | %6.1fx |${NC}\n" "AVERAGE" "$AVG_TAU" "$AVG_PY" "$AVG_SPEEDUP"
echo -e "${CYAN}========================================================${NC}\n"

echo -e "${GREEN}Tauraro compiled code is approximately ${AVG_SPEEDUP}x faster than Python!${NC}"
echo ""
