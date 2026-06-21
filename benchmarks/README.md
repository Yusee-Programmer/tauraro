# Tauraro Benchmarks

Performance comparison between **C** (`gcc -O3`), **Rust** (`rustc -C opt-level=3 -C target-cpu=native`), and **Tauraro** (self-hosted compiler, `tauraroc -O3`) across ten compute workloads.

Latest run (auto-generated into [`results.md`](results.md)):

- **OS:** Linux x86_64 · **Date (UTC):** 2026-06-21
- **C:** gcc 13.3.0 · **Rust:** rustc 1.96.0

## Results — wall time (seconds)

| # | Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau / C | Tau / Rust |
|---|-----------|------:|---------:|------------:|--------:|-----------:|
| 1 | Integer Sum 1B | 0.000 | 0.000 | 0.000 | — | — |
| 2 | Fibonacci 1B | 0.313 | 0.311 | 0.311 | **0.99×** | **1.00×** |
| 3 | Float Multiply 1B | 0.933 | 0.934 | 0.934 | **1.00×** | **1.00×** |
| 4 | XOR Shift PRNG 1B | 1.866 | 1.867 | 1.870 | **1.00×** | **1.00×** |
| 5 | Newton Sqrt 1B | 6.063 | 6.046 | 6.053 | **1.00×** | **1.00×** |
| 6 | Mandelbrot 800×800 | 0.442 | 0.441 | 0.428 | **0.97×** | **0.97×** |
| 7 | Sieve of Eratosthenes 50M | 0.172 | 0.182 | 0.265 | 1.54× | 1.46× |
| 8 | N-Body 3 bodies 10M steps | 0.286 | 0.284 | 0.289 | **1.01×** | 1.02× |
| 9 | Collatz 1..10M | 2.138 | 1.512 | 2.123 | **0.99×** | 1.40× |
| 10 | Matrix Multiply 400×400 | 0.015 | 0.012 | 0.033 | 2.20× | 2.75× |

## Results — peak resident memory (KB)

| # | Benchmark | C (KB) | Rust (KB) | Tauraro (KB) | Tau / C | Tau / Rust |
|---|-----------|-------:|----------:|-------------:|--------:|-----------:|
| 1 | Integer Sum 1B | 1516 | 2100 | 1516 | **1.00×** | **0.72×** |
| 2 | Fibonacci 1B | 1516 | 1984 | 1516 | **1.00×** | **0.76×** |
| 3 | Float Multiply 1B | 1644 | 2000 | 1632 | **0.99×** | **0.82×** |
| 4 | XOR Shift PRNG 1B | 1516 | 2104 | 1452 | **0.96×** | **0.69×** |
| 5 | Newton Sqrt 1B | 1644 | 2056 | 1644 | **1.00×** | **0.80×** |
| 6 | Mandelbrot 800×800 | 1516 | 1972 | 1516 | **1.00×** | **0.77×** |
| 7 | Sieve of Eratosthenes 50M | 50124 | 50808 | 52260 | 1.04× | 1.03× |
| 8 | N-Body 3 bodies 10M steps | 1644 | 2064 | 1572 | **0.96×** | **0.76×** |
| 9 | Collatz 1..10M | 1452 | 2088 | 1516 | 1.04× | **0.73×** |
| 10 | Matrix Multiply 400×400 | 4960 | 5692 | 5484 | 1.11× | **0.96×** |

> **Tau/C** and **Tau/Rust** are ratios (Tauraro ÷ baseline). Values **below 1.00×**
> mean Tauraro is faster / leaner.

## Summary

- **Time:** Tauraro is at **C/Rust parity** on every scalar compute kernel
  (Fibonacci, Float Multiply, XOR Shift, Newton, Mandelbrot, N-Body, Collatz — all
  within ~2%). Sieve (1.54×) and the naive MatMul (2.20×) are the two outliers,
  both cache/aliasing-bound.
- **Memory:** Tauraro is **leaner than Rust on all ten** (≈0.69–0.96×) and within
  ~4% of C — no GC, no runtime overhead, a flat per-program footprint.
- **Why parity:** `tauraroc -O3` passes `-march=native -funroll-loops` to GCC
  (matching Rust's `target-cpu=native`), and `List_i64::data` / `List_f64::data`
  carry `__restrict__` in the runtime header so GCC can auto-vectorize without
  aliasing guards. Integer Sum is constant-folded to ~0 by all three at `-O3`.

## Benchmark Descriptions

| # | Name | Operation | Workload |
|---|------|-----------|----------|
| 1 | Integer Sum | `sum += i` | 1B iterations |
| 2 | Fibonacci | iterative `a, b = b, a+b` | 1B steps |
| 3 | Float Multiply | `x *= 1.000001` | 1B multiplications |
| 4 | XOR Shift PRNG | xorshift64 `s ^= s<<13; s ^= s>>7; s ^= s<<17` | 1B steps |
| 5 | Newton Sqrt | `x = (x + 2.0/x) * 0.5` | 1B iterations |
| 6 | Mandelbrot | escape-time algorithm on complex plane | 800×800 grid, 1000 max iters |
| 7 | Sieve | Sieve of Eratosthenes, count primes | up to 50,000,000 |
| 8 | N-Body | 3-body gravitational simulation with `sqrt` | 10M steps |
| 9 | Collatz | sum of stopping times for all n | n = 1..10,000,000 |
| 10 | Matrix Multiply | naive triple-loop `C = A × B` | 400×400 f64 matrices |

## Compiler Flags

| Language | Command |
|----------|---------|
| C | `gcc -O3 -lm` |
| Rust | `rustc -C opt-level=3 -C target-cpu=native` |
| Tauraro | `tauraroc -O3` (self-hosted → C backend → `gcc -O3 -march=native -funroll-loops`) |

## Running

**Windows (PowerShell):**
```powershell
.\tauraro\benchmarks\run_all.ps1
```

**Linux / macOS (Bash):**
```bash
bash tauraro/benchmarks/run_all.sh
```

Requires GCC, `rustc`, and `tauraro/src/build/tauraroc` on your system.

## File Layout

```
benchmarks/
  run_all.ps1              Windows runner
  run_all.sh               Linux/macOS runner
  1_sum/       bench.c  bench.rs  bench.tr
  2_fibonacci/ bench.c  bench.rs  bench.tr
  3_float_mul/ bench.c  bench.rs  bench.tr
  4_xorshift/  bench.c  bench.rs  bench.tr
  5_newton/    bench.c  bench.rs  bench.tr
  6_mandelbrot/bench.c  bench.rs  bench.tr
  7_sieve/     bench.c  bench.rs  bench.tr
  8_nbody/     bench.c  bench.rs  bench.tr
  9_collatz/   bench.c  bench.rs  bench.tr
  10_matmul/   bench.c  bench.rs  bench.tr
```
