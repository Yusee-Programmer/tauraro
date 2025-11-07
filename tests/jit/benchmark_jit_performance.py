"""
JIT Performance Benchmark Suite
Measures performance of JIT-compiled vs interpreted code.

This benchmark establishes baseline performance for all JIT-supported operations.
"""

import time

print("=" * 70)
print(" " * 15 + "TAURARO JIT PERFORMANCE BENCHMARK")
print("=" * 70)
print()

# Benchmark configuration
ITERATIONS = 100000

print(f"Configuration: {ITERATIONS:,} iterations per benchmark")
print()

# Benchmark 1: Integer Addition
print("Benchmark 1: Integer Addition")
print("-" * 70)
start = time.time()
total = 0
for i in range(ITERATIONS):
    total = total + i
end = time.time()
time_add = end - start
print(f"Result: {total}")
print(f"Time: {time_add:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_add / 1000000:.2f} million ops/sec")
print()

# Benchmark 2: Integer Multiplication
print("Benchmark 2: Integer Multiplication")
print("-" * 70)
start = time.time()
total = 0
for i in range(ITERATIONS):
    total = total + (i * 3)
end = time.time()
time_mul = end - start
print(f"Result: {total}")
print(f"Time: {time_mul:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_mul / 1000000:.2f} million ops/sec")
print()

# Benchmark 3: Integer Division and Modulo
print("Benchmark 3: Integer Division and Modulo")
print("-" * 70)
start = time.time()
total = 0
for i in range(1, ITERATIONS + 1):
    total = total + (i // 7) + (i % 7)
end = time.time()
time_divmod = end - start
print(f"Result: {total}")
print(f"Time: {time_divmod:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_divmod / 1000000:.2f} million ops/sec")
print()

# Benchmark 4: Bitwise Operations
print("Benchmark 4: Bitwise Operations")
print("-" * 70)
start = time.time()
total = 0
for i in range(ITERATIONS):
    x = (i & 255) | 128
    y = x ^ 64
    z = y << 2
    total = total + (z >> 1)
end = time.time()
time_bitwise = end - start
print(f"Result: {total}")
print(f"Time: {time_bitwise:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_bitwise / 1000000:.2f} million ops/sec")
print()

# Benchmark 5: Float Operations
print("Benchmark 5: Float Operations")
print("-" * 70)
start = time.time()
total = 0.0
for i in range(ITERATIONS):
    x = float(i)
    total = total + (x * 1.5 + 2.0) / 3.0
end = time.time()
time_float = end - start
print(f"Result: {total:.2f}")
print(f"Time: {time_float:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_float / 1000000:.2f} million ops/sec")
print()

# Benchmark 6: Comparisons with Branching
print("Benchmark 6: Comparisons with Branching")
print("-" * 70)
start = time.time()
total = 0
for i in range(ITERATIONS):
    if i < ITERATIONS / 2:
        total = total + i
    else:
        total = total - i
end = time.time()
time_cmp = end - start
print(f"Result: {total}")
print(f"Time: {time_cmp:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_cmp / 1000000:.2f} million ops/sec")
print()

# Benchmark 7: Complex Expression
print("Benchmark 7: Complex Expression (Multiple Operations)")
print("-" * 70)
start = time.time()
total = 0
for i in range(1, ITERATIONS + 1):
    total = total + ((i * 3 + 5) % 7 - (i // 2))
end = time.time()
time_complex = end - start
print(f"Result: {total}")
print(f"Time: {time_complex:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_complex / 1000000:.2f} million ops/sec")
print()

# Benchmark 8: Multiple Accumulators
print("Benchmark 8: Multiple Accumulators")
print("-" * 70)
start = time.time()
a = 0
b = 0
c = 0
for i in range(ITERATIONS):
    a = a + i
    b = b + (i * 2)
    c = c + (i % 3)
end = time.time()
time_multi = end - start
total = a + b + c
print(f"Result: {total}")
print(f"Time: {time_multi:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_multi / 1000000:.2f} million ops/sec")
print()

# Benchmark 9: Power Operations
print("Benchmark 9: Power Operations")
print("-" * 70)
start = time.time()
total = 0
for i in range(ITERATIONS):
    total = total + (2 ** 10)
end = time.time()
time_pow = end - start
print(f"Result: {total}")
print(f"Time: {time_pow:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_pow / 1000000:.2f} million ops/sec")
print()

# Benchmark 10: Float Comparisons
print("Benchmark 10: Float Comparisons")
print("-" * 70)
start = time.time()
total = 0
for i in range(ITERATIONS):
    x = float(i) / 1000.0
    if x < 50.0:
        total = total + 1
end = time.time()
time_float_cmp = end - start
print(f"Result: {total}")
print(f"Time: {time_float_cmp:.4f} seconds")
print(f"Throughput: {ITERATIONS / time_float_cmp / 1000000:.2f} million ops/sec")
print()

# Summary
print("=" * 70)
print("BENCHMARK SUMMARY")
print("=" * 70)
print(f"{'Benchmark':<40} {'Time (s)':<12} {'Throughput (Mops/s)':<20}")
print("-" * 70)
benchmarks = [
    ("Integer Addition", time_add),
    ("Integer Multiplication", time_mul),
    ("Integer Division/Modulo", time_divmod),
    ("Bitwise Operations", time_bitwise),
    ("Float Operations", time_float),
    ("Comparisons with Branching", time_cmp),
    ("Complex Expression", time_complex),
    ("Multiple Accumulators", time_multi),
    ("Power Operations", time_pow),
    ("Float Comparisons", time_float_cmp),
]

for name, bench_time in benchmarks:
    throughput = ITERATIONS / bench_time / 1000000
    print(f"{name:<40} {bench_time:<12.4f} {throughput:<20.2f}")

avg_time = sum(t for _, t in benchmarks) / len(benchmarks)
avg_throughput = ITERATIONS / avg_time / 1000000

print("-" * 70)
print(f"{'Average':<40} {avg_time:<12.4f} {avg_throughput:<20.2f}")
print("=" * 70)
print()
print("NOTE: These benchmarks establish baseline performance for JIT optimization.")
print("      Run this benchmark before and after JIT improvements to measure impact.")
print("=" * 70)
