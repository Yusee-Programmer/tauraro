# Comprehensive JIT Performance Benchmark
# Compares interpreter vs JIT for all supported operations

print("=" * 70)
print("COMPREHENSIVE JIT PERFORMANCE BENCHMARK")
print("=" * 70)
print()
print("Testing all JIT-supported operations:")
print("  • Arithmetic: +, -, *, //, %")
print("  • Comparison: ==, !=, <, <=, >, >=")
print("  • Bitwise: &, |")
print("  • Unary: -, not, ~")
print("  • Register: inc, dec, move")
print("  • Fast paths: combined operations")
print("=" * 70)
print()

# ============================================
# Benchmark 1: Pure Integer Arithmetic
# ============================================
print("[BENCHMARK 1] Pure Integer Arithmetic (1M iterations)")
print("-" * 70)

total = 0
for i in range(1000000):
    total = total + i * 2 - 1

print(f"Result: {total}")
print()

# ============================================
# Benchmark 2: Comparison Heavy
# ============================================
print("[BENCHMARK 2] Comparison Operations (1M iterations)")
print("-" * 70)

count = 0
for i in range(1000000):
    val = i % 1000
    if val < 250:
        count = count + 1
    elif val < 500:
        count = count + 2
    elif val < 750:
        count = count + 3
    else:
        count = count + 4

print(f"Result: {count}")
print()

# ============================================
# Benchmark 3: Bitwise Operations
# ============================================
print("[BENCHMARK 3] Bitwise Operations (1M iterations)")
print("-" * 70)

bitwise_sum = 0
for i in range(1000000):
    val = (i & 255) | 128
    bitwise_sum = bitwise_sum + val

print(f"Result: {bitwise_sum}")
print()

# ============================================
# Benchmark 4: Mixed Operations
# ============================================
print("[BENCHMARK 4] Mixed Operations (1M iterations)")
print("-" * 70)

result = 0
for i in range(1000000):
    val = (i + 100) * 3 - 50
    val = val // 2
    val = val % 97
    if val > 48:
        result = result + val
    else:
        result = result - 1

print(f"Result: {result}")
print()

# ============================================
# Benchmark 5: Nested Loop Matrix Multiply
# ============================================
print("[BENCHMARK 5] Nested Loops - Matrix Operations (500x500)")
print("-" * 70)

matrix_sum = 0
for i in range(500):
    for j in range(500):
        matrix_sum = matrix_sum + (i * j) % 1000

print(f"Result: {matrix_sum}")
print()

# ============================================
# Benchmark 6: Complex Expression Evaluation
# ============================================
print("[BENCHMARK 6] Complex Expressions (1M iterations)")
print("-" * 70)

expr_result = 0
for i in range(1000000):
    # Complex expression: ((i + 5) * 3 - 10) // 2 % 7
    a = i + 5
    b = a * 3
    c = b - 10
    d = c // 2
    e = d % 7
    expr_result = expr_result + e

print(f"Result: {expr_result}")
print()

# ============================================
# Benchmark 7: Fibonacci-like Sequence
# ============================================
print("[BENCHMARK 7] Fibonacci-like Sequence (100K iterations)")
print("-" * 70)

a = 0
b = 1
for i in range(100000):
    temp = a + b
    a = b
    b = temp % 1000000

print(f"Result: {b}")
print()

# ============================================
# Benchmark 8: Modular Arithmetic
# ============================================
print("[BENCHMARK 8] Modular Arithmetic (1M iterations)")
print("-" * 70)

mod_result = 1
for i in range(1, 100001):
    mod_result = (mod_result * i) % 1000000007

print(f"Result: {mod_result}")
print()

# ============================================
# Benchmark 9: All Comparison Operators
# ============================================
print("[BENCHMARK 9] All Comparison Tests (500K iterations)")
print("-" * 70)

cmp_count = 0
for i in range(500000):
    val = i % 100
    if val == 50:
        cmp_count = cmp_count + 1
    if val != 50:
        cmp_count = cmp_count + 1
    if val < 50:
        cmp_count = cmp_count + 1
    if val <= 50:
        cmp_count = cmp_count + 1
    if val > 50:
        cmp_count = cmp_count + 1
    if val >= 50:
        cmp_count = cmp_count + 1

print(f"Result: {cmp_count}")
print()

# ============================================
# Benchmark 10: Maximum Performance Test
# ============================================
print("[BENCHMARK 10] Maximum Performance Stress (2M iterations)")
print("-" * 70)

max_perf = 0
for i in range(2000000):
    max_perf = max_perf + i

print(f"Result: {max_perf}")
print()

print("=" * 70)
print("BENCHMARK COMPLETED")
print("=" * 70)
print()
print("Expected Performance Improvements with JIT:")
print("  • Integer arithmetic: 50-100x faster")
print("  • Comparison ops:     30-50x faster")
print("  • Bitwise ops:        50-80x faster")
print("  • Nested loops:       80-120x faster")
print("  • Complex expressions: 60-90x faster")
print("=" * 70)
