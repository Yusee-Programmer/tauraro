#!/bin/bash

# Benchmark Runner - Compares Tauraro vs Python Performance

echo "=========================================="
echo "  Tauraro vs Python Performance Benchmark"
echo "=========================================="
echo ""

# Ensure we have a release build
if [ ! -f "../target/release/tauraro" ]; then
    echo "Building Tauraro in release mode..."
    cd .. && cargo build --release && cd benchmarks
fi

TAURARO="../target/release/tauraro"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

run_benchmark() {
    local name=$1
    local python_file=$2
    local tauraro_file=$3

    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "  Benchmark: $name"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

    # Run Python
    echo -e "${YELLOW}Running Python...${NC}"
    python_time=$(python3 $python_file 2>&1)

    # Run Tauraro
    echo -e "${YELLOW}Running Tauraro...${NC}"
    tauraro_time=$($TAURARO run $tauraro_file 2>&1)

    echo ""
    echo "Results:"
    echo "$python_time"
    echo ""
    echo "$tauraro_time"
    echo ""
}

# Run all benchmarks
run_benchmark "Arithmetic Operations" "01_arithmetic.py" "01_arithmetic.tr"
run_benchmark "Loop Performance" "02_loops.py" "02_loops.tr"
run_benchmark "Function Calls" "03_functions.py" "03_functions.tr"
run_benchmark "List Operations" "04_lists.py" "04_lists.tr"
run_benchmark "String Operations" "05_strings.py" "05_strings.tr"

echo ""
echo "=========================================="
echo "  Benchmark Complete!"
echo "=========================================="
