# Tauraro vs Python Benchmarks

This directory contains benchmarks comparing the performance of Tauraro and Python across various computational tasks.

## Benchmark Categories

1. **Arithmetic Operations** - Basic mathematical operations
2. **String Operations** - String manipulation and processing
3. **Loop Performance** - Various loop constructs
4. **Function Calls** - Function definition and calling
5. **Sorting Algorithms** - Bubble sort implementation
6. **Mathematical Computations** - Complex mathematical operations

## Directory Structure

```
benchmarks/
├── run_benchmarks.py      # Main benchmark runner script
├── verify_installation.py # Installation verification script
├── README.md             # This file
├── TAURARO_VS_PYTHON_BENCHMARK_REPORT.md # Detailed report
├── tauraro/              # Tauraro benchmark implementations
│   ├── arithmetic_benchmark.tr
│   ├── string_benchmark.tr
│   ├── loop_benchmark.tr
│   ├── function_benchmark.tr
│   ├── sorting_benchmark.tr
│   └── math_benchmark.tr
├── python/               # Python benchmark implementations
│   ├── arithmetic_benchmark.py
│   ├── string_benchmark.py
│   ├── loop_benchmark.py
│   ├── function_benchmark.py
│   ├── sorting_benchmark.py
│   └── math_benchmark.py
├── results/              # Benchmark results (generated)
└── performance_results.json # Historical performance data
```

## Prerequisites

1. **Tauraro Installation**: Tauraro must be compiled and available
   ```bash
   cd ..  # Go to project root
   cargo build --release
   ```

2. **Python 3.8+**: For running Python benchmarks and the benchmark runner

## Running Benchmarks

### Quick Verification

First, verify that everything is set up correctly:

```bash
python verify_installation.py
```

### Full Benchmark Suite

Run all benchmarks and generate a comparison report:

```bash
python run_benchmarks.py
```

This will:
1. Execute each benchmark in both Tauraro and Python
2. Measure execution times
3. Compare performance
4. Generate a detailed report in `results/benchmark_results.json`

### Individual Benchmarks

You can also run individual benchmarks manually:

```bash
# Run Tauraro benchmark
../../target/release/tauraro.exe run tauraro/arithmetic_benchmark.tr

# Run Python benchmark
python python/arithmetic_benchmark.py
```

## Benchmark Details

### Iteration Counts

- Basic operations: 5,000 iterations
- Recursive functions: 1,000 iterations (to prevent stack overflow)
- Sorting: Arrays of 100, 500, and 1,000 elements
- Mathematical computations: 1,000 iterations

### Timing Methodology

Execution time is measured from benchmark start to completion using high-resolution timers. The benchmark runner captures:

1. **Start Time**: Immediately before process execution
2. **End Time**: Immediately after process completion
3. **Execution Time**: Difference between end and start times

## Results

Results are saved in JSON format in `results/benchmark_results.json` with the following structure:

```json
{
  "timestamp": "2023-10-15 14:30:45",
  "benchmarks": {
    "arithmetic": {
      "tauraro": 0.1234,
      "python": 0.2345
    },
    "string": {
      "tauraro": 0.4567,
      "python": 0.5678
    }
  }
}
```

## Performance Report

A detailed analysis is available in [TAURARO_VS_PYTHON_BENCHMARK_REPORT.md](TAURARO_VS_PYTHON_BENCHMARK_REPORT.md).

## Adding New Benchmarks

To add a new benchmark category:

1. Create Tauraro implementation in `tauraro/{name}_benchmark.tr`
2. Create Python implementation in `python/{name}_benchmark.py`
3. Add the benchmark name to the `benchmarks` list in `run_benchmarks.py`

## Troubleshooting

### Tauraro Not Found

If you get "tauraro.exe not found" error:
```bash
cd ..  # Go to project root
cargo build --release
cd benchmarks
```

### Benchmark Timeout

If benchmarks timeout, you can increase the timeout in `run_benchmarks.py` by modifying the `timeout` parameter in `subprocess.run()` calls.

### Permission Errors

On some systems, you may need to grant execute permissions:
```bash
chmod +x ../target/release/tauraro.exe
```

## Contributing

Feel free to add new benchmarks or improve existing ones. When contributing:

1. Ensure both Tauraro and Python implementations perform equivalent work
2. Use consistent iteration counts
3. Include appropriate progress messages
4. Follow the existing code style