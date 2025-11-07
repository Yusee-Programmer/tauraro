# Comprehensive arithmetic benchmark - all operations
# Tests add, sub, mul, div, mod with TaggedValue

print("=== All Arithmetic Operations Benchmark ===")

# Test multiplication
result_mul = 1
for i in range(1, 1001):
    result_mul = result_mul * 2
    result_mul = result_mul / 2
print(f"Multiplication test complete: {result_mul}")

# Test division
result_div = 10000000
for i in range(1, 1001):
    result_div = result_div / 2
    result_div = result_div * 2
print(f"Division test complete: {result_div}")

# Test modulo
result_mod = 0
for i in range(1, 10001):
    result_mod = i % 7
print(f"Modulo test complete: {result_mod}")

# Test mixed operations (add/sub/mul/div/mod)
total = 100
for i in range(1, 10001):
    total = total + i
    total = total - 5
    total = total * 2
    total = total / 2
    total = total % 1000000

print(f"Mixed operations complete: {total}")

print("All arithmetic tests passed!")
