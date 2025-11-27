# Tauraro Benchmarks

This directory contains comprehensive benchmarks comparing **Tauraro compiled code** vs **Python** for compute-intensive operations with **1 billion iterations** each.

## Benchmark Results Summary

| Benchmark | Tauraro (s) | Python (s) | Speedup |
|-----------|-------------|------------|---------|
| Simple Loop | ~1.0 | ~120 | **~118x** |
| Arithmetic | ~1.0 | ~310 | **~297x** |
| Function Calls | ~1.0 | ~249 | **~238x** |
| Conditionals | ~1.5 | ~190 | **~125x** |
| Nested Loops | ~1.1 | ~124 | **~116x** |
| Bitwise Ops | ~1.0 | ~350 | **~339x** |
| **AVERAGE** | **~1.1s** | **~224s** | **~205x** |

> **Tauraro compiled code is approximately 200x faster than Python!**

## Benchmarks Description

### 1. Simple Loop (`bench_loop`)
Tests basic loop iteration performance with a simple counter increment.
- **Iterations**: 1,000,000,000
- **Operations**: Loop control, integer increment

### 2. Arithmetic (`bench_arithmetic`)
Tests arithmetic computation with multiple operations per iteration.
- **Iterations**: 1,000,000,000
- **Operations**: Multiplication, division, modulo, addition, subtraction

### 3. Function Calls (`bench_function_calls`)
Tests function call overhead with a simple function called in a tight loop.
- **Iterations**: 1,000,000,000
- **Operations**: Function calls, return values

### 4. Conditionals (`bench_conditionals`)
Tests conditional branching performance with if/else statements.
- **Iterations**: 1,000,000,000
- **Operations**: Modulo, comparison, branching

### 5. Nested Loops (`bench_nested_loops`)
Tests nested loop performance with O(n²) complexity.
- **Iterations**: 31,623 × 31,623 ≈ 1,000,000,000
- **Operations**: Nested loop control, integer increment

### 6. Bitwise Operations (`bench_bitwise`)
Tests bitwise/shift operations performance.
- **Iterations**: 1,000,000,000
- **Operations**: Left shift, right shift, bitwise AND

## Requirements

### All Platforms
- **Tauraro**: Built from source (`cargo build --release`)
- **Python 3**: Any recent version (3.8+)
- **C Compiler**: GCC or Clang with `-O3` optimization support

### Platform-Specific

#### Windows
- PowerShell 5.1+ (comes with Windows)
- GCC (MinGW-w64) or install via `choco install mingw`

#### Linux
- Bash shell
- GCC: `sudo apt install gcc` (Debian/Ubuntu) or `sudo dnf install gcc` (Fedora)
- bc calculator: `sudo apt install bc`

#### macOS
- Bash or Zsh shell
- Clang (comes with Xcode Command Line Tools): `xcode-select --install`
- bc calculator (comes with macOS)
- Perl (comes with macOS, used for timing)

## How to Run

### Windows (PowerShell)

```powershell
# From the tauraro root directory
.\benchmarks\run_benchmarks.ps1
```

### Linux / macOS (Bash)

```bash
# From the tauraro root directory
chmod +x benchmarks/run_benchmarks.sh
./benchmarks/run_benchmarks.sh
```

## Running Individual Benchmarks

### Compile and Run Tauraro Version

```bash
# Compile Tauraro to C
./target/release/tauraro compile benchmarks/bench_loop.tr --backend c -o benchmarks/bench_loop.c

# Compile C with optimizations
gcc -O3 -o benchmarks/bench_loop benchmarks/bench_loop.c -lm

# Run
./benchmarks/bench_loop
```

### Run Python Version

```bash
python3 benchmarks/bench_loop.py
```

## File Structure

```
benchmarks/
├── README.md                    # This file
├── run_benchmarks.ps1           # Windows PowerShell benchmark runner
├── run_benchmarks.sh            # Linux/macOS Bash benchmark runner
├── bench_loop.tr                # Simple loop (Tauraro)
├── bench_loop.py                # Simple loop (Python)
├── bench_arithmetic.tr          # Arithmetic ops (Tauraro)
├── bench_arithmetic.py          # Arithmetic ops (Python)
├── bench_function_calls.tr      # Function calls (Tauraro)
├── bench_function_calls.py      # Function calls (Python)
├── bench_conditionals.tr        # Conditionals (Tauraro)
├── bench_conditionals.py        # Conditionals (Python)
├── bench_nested_loops.tr        # Nested loops (Tauraro)
├── bench_nested_loops.py        # Nested loops (Python)
├── bench_bitwise.tr             # Bitwise ops (Tauraro)
└── bench_bitwise.py             # Bitwise ops (Python)
```

## Understanding the Results

### Why is Tauraro So Fast?

1. **Native Compilation**: Tauraro compiles to optimized C code, then to native machine code via GCC/Clang with `-O3` optimizations.

2. **Type Inference**: Typed Tauraro code uses native C types (`long long`, `double`) instead of boxed Python objects.

3. **No Interpreter Overhead**: Python interprets bytecode at runtime; Tauraro runs as native CPU instructions.

4. **No GIL**: Unlike Python, compiled Tauraro code has no Global Interpreter Lock overhead.

5. **Compiler Optimizations**: GCC/Clang apply aggressive optimizations like loop unrolling, vectorization, and constant folding.

### When to Use Typed Tauraro

- CPU-intensive numerical computations
- Tight loops with millions/billions of iterations
- Mathematical algorithms (fibonacci, primes, etc.)
- Data processing pipelines
- Performance-critical code paths

### Benchmark Methodology

- Each benchmark performs exactly 1 billion operations (or equivalent)
- Tauraro code is compiled with `--backend c` and GCC `-O3`
- Python code uses standard CPython (no Cython, PyPy, or Numba)
- Times measured using PowerShell `Measure-Command` or Bash time utilities
- Multiple runs recommended for consistent results

## Troubleshooting

### Windows: "gcc not found"
Install MinGW-w64:
```powershell
choco install mingw
# or download from https://www.mingw-w64.org/
```

### Linux: "bc: command not found"
```bash
sudo apt install bc  # Debian/Ubuntu
sudo dnf install bc  # Fedora
```

### macOS: "Permission denied"
```bash
chmod +x benchmarks/run_benchmarks.sh
```

### Build Tauraro First
```bash
cargo build --release
```

## Contributing

To add a new benchmark:
1. Create `bench_<name>.tr` (Tauraro version)
2. Create `bench_<name>.py` (Python equivalent)
3. Add the benchmark to the `$benchmarks` array in `run_benchmarks.ps1`
4. Add the benchmark to the `BENCHMARKS` array in `run_benchmarks.sh`
5. Update this README with the benchmark description

## License

Same as the main Tauraro project.
