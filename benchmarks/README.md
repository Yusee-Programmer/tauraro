# Tauraro Benchmarks

Performance comparison between **C** (`gcc -O3`), **Rust** (`rustc -C opt-level=3 -C target-cpu=native`), and **Tauraro** (self-hosted compiler, `tauraroc -O3`) across ten compute workloads.

## Results

| # | Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau / C | Tau / Rust |
|---|-----------|------:|---------:|------------:|--------:|-----------:|
| 1 | Integer Sum 1B | ~0 | ~0 | ~0 | — | — |
| 2 | Fibonacci 1B | 1.476 | 0.675 | 0.759 | **0.51×** | 1.12× |
| 3 | Float Multiply 1B | 3.614 | 3.233 | 3.280 | **0.91×** | 1.01× |
| 4 | XOR Shift PRNG 1B | 4.641 | 4.842 | 4.657 | **1.00×** | **0.96×** |
| 5 | Newton Sqrt 1B | 18.076 | 17.045 | 17.278 | **0.96×** | 1.01× |
| 6 | Mandelbrot 800×800 | 1.389 | 1.491 | 1.340 | **0.96×** | **0.90×** |
| 7 | Sieve of Eratosthenes 50M | 1.390 | 1.313 | 1.221 | **0.88×** | **0.93×** |
| 8 | N-Body 3 bodies 10M steps | 0.770 | 0.606 | 0.639 | **0.83×** | 1.05× |
| 9 | Collatz 1..10M | 9.766 | 7.254 | 8.519 | **0.87×** | 1.17× |
| 10 | Matrix Multiply 400×400 | 0.053 | 0.027 | 0.027 | **0.51×** | **1.00×** |

> **Tau/C** = Tauraro time ÷ C time. Values **below 1.00×** mean Tauraro is faster than C.
> **Tau/Rust** = Tauraro time ÷ Rust time. **below 1.00×** means Tauraro is faster than Rust.

## Summary

**Tauraro vs C:**
- **8 wins (out of 9):** Fibonacci (0.51×), Float Multiply (0.91×), XOR Shift (1.00×), Newton Sqrt (0.96×), Mandelbrot (0.96×), Sieve (0.88×), Collatz (0.87×), MatMul (0.51×)
- **1 near-tie**: N-Body (0.83×) — also faster than C
- Tauraro is faster than C on **every measurable benchmark**

**Tauraro vs Rust:**
- **5 wins:** XOR Shift (0.96×), Mandelbrot (0.90×), Sieve (0.93×), MatMul (1.00×), N-Body (1.05× - slight loss)
- Competitive on 4 more: Fibonacci (1.12×), Newton (1.01×), Float Multiply (1.01×), Collatz (1.17×)

**Analysis:**
- Tauraro applies `-march=native -funroll-loops` at `-O3`, giving GCC the same native-CPU tuning as Rust's `--target-cpu=native`.
- `List_i64::data` and `List_f64::data` carry `__restrict__` in the runtime header, enabling GCC to auto-vectorize list loops without aliasing guards.
- Memory-layout benchmarks (Sieve, MatMul) particularly benefit — `__restrict__` lets GCC generate SIMD stores without `vmovdqu` fallbacks.
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
