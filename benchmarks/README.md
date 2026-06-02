# Tauraro Benchmarks

Performance comparison between **C** (`gcc -O3`), **Rust** (`rustc -C opt-level=3 -C target-cpu=native`), and **Tauraro** (self-hosted compiler, `tauraroc -O3`) across ten compute workloads.

## Results

| # | Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau / C | Tau / Rust |
|---|-----------|------:|---------:|------------:|--------:|-----------:|
| 1 | Integer Sum 1B | ~0 | ~0 | ~0 | — | — |
| 2 | Fibonacci 1B | 1.573 | 0.696 | 0.709 | **0.45×** | 1.02× |
| 3 | Float Multiply 1B | 3.124 | 3.014 | 3.021 | **0.97×** | 1.00× |
| 4 | XOR Shift PRNG 1B | 4.726 | 4.307 | 4.987 | 1.06× | 1.16× |
| 5 | Newton Sqrt 1B | 17.091 | 17.007 | 16.682 | **0.98×** | **0.98×** |
| 6 | Mandelbrot 800×800 | 1.695 | 1.581 | 1.474 | **0.87×** | **0.93×** |
| 7 | Sieve of Eratosthenes 50M | 1.024 | 1.308 | 1.295 | 1.26× | 0.99× |
| 8 | N-Body 3 bodies 10M steps | 0.843 | 0.790 | 0.905 | 1.07× | 1.15× |
| 9 | Collatz 1..10M | 8.819 | 7.236 | 10.833 | 1.23× | 1.50× |
| 10 | Matrix Multiply 400×400 | 0.072 | 0.062 | 0.106 | 1.47× | 1.71× |

> **Tau/C** = Tauraro time ÷ C time. Values **below 1.00×** mean Tauraro is faster than C.
> **Tau/Rust** = Tauraro time ÷ Rust time. **below 1.00×** means Tauraro is faster than Rust.

## Summary

**Tauraro vs C:**
- **2 clear wins:** Fibonacci (0.45×, **2.2× faster** than C) and Mandelbrot (0.87×, 15% faster)
- **2 near-ties** (within 3%): Float Multiply (0.97×) and Newton Sqrt (0.98×)
- **Slower on 5 benchmarks**: XOR Shift (1.06×), N-Body (1.07×), Sieve (1.26×), Collatz (1.23×), MatMul (1.47×)

**Tauraro vs Rust:**
- **2 wins:** Newton Sqrt (0.98×) and Mandelbrot (0.93×)
- **2 near-ties** (within 1%): Float Multiply (1.00×) and Sieve (0.99×)
- **Slower on 4 benchmarks**: XOR Shift (1.16×), N-Body (1.15×), Collatz (1.50×), MatMul (1.71×)

**Analysis:**
- Tauraro's biggest strength is sequential floating-point loops (Fibonacci, Mandelbrot, Newton Sqrt, Float Multiply) — the `#pragma GCC optimize("O3,unroll-loops")` emitted by `tauraroc` helps the C backend auto-vectorize these aggressively.
- Memory-access-heavy benchmarks (Sieve, Collatz, MatMul) show 20–47% overhead vs C. These are cache-miss-dominated workloads where the generated C's indirection overhead is most visible.
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
