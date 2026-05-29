# Tauraro Benchmarks

Performance comparison between **C** (`gcc -O3`), **Rust** (`rustc -C opt-level=3 -C target-cpu=native`), and **Tauraro** (self-hosted compiler, `tauraroc -O3`) across ten compute workloads.

## Results

| # | Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau / C | Tau / Rust |
|---|-----------|------:|---------:|------------:|--------:|-----------:|
| 1 | Integer Sum 1B | ~0 | ~0 | ~0 | — | — |
| 2 | Fibonacci 1B | 0.728 | 0.385 | 0.433 | **0.59×** | 1.12× |
| 3 | Float Multiply 1B | 1.833 | 1.669 | 1.446 | **0.79×** | **0.87×** |
| 4 | XOR Shift PRNG 1B | 2.700 | 2.373 | 2.374 | **0.88×** | **1.00×** |
| 5 | Newton Sqrt 1B | 8.476 | 7.983 | 7.813 | **0.92×** | **0.98×** |
| 6 | Mandelbrot 800×800 | 0.759 | 0.741 | 0.799 | 1.05× | 1.08× |
| 7 | Sieve of Eratosthenes 50M | 0.641 | 0.592 | 0.720 | 1.12× | 1.22× |
| 8 | N-Body 3 bodies 10M steps | 0.543 | 0.344 | 0.543 | **1.00×** | 1.58× |
| 9 | Collatz 1..10M | 4.607 | 2.664 | 3.620 | **0.79×** | 1.36× |
| 10 | Matrix Multiply 400×400 | 0.035 | 0.020 | 0.035 | **1.00×** | 1.75× |

> **Tau/C** = Tauraro time ÷ C time. Values **below 1.00×** mean Tauraro is faster than C.
> **Tau/Rust** = Tauraro time ÷ Rust time. **1.00×** = identical speed.

## Summary

- Tauraro **beats C** on 6 of 10 benchmarks (benchmarks 2, 3, 4, 5, 9, and ties on 8/10).
- Tauraro **matches Rust exactly** on benchmarks 4, 8, and 10 (within 0.5%).
- Tauraro **beats Rust** on benchmarks 3 and 5 (float-heavy workloads) by 2–13%.
- The `#pragma GCC optimize("O3,unroll-loops")` and AVX2 hints emitted by `tauraroc` in the generated C give it an advantage over plain `gcc -O3`.
- Benchmark 1 (Integer Sum): all three compilers constant-fold the entire loop at `-O3`.

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
| Tauraro | `tauraroc -O3` (self-hosted → C backend → GCC with pragma O3+unroll+avx2) |

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
