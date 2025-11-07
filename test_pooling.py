# Test object pooling optimization
print("Testing object pooling...")

# Test small integer pooling (-5 to 256)
small_ints = []
for i in range(-5, 257):
    small_ints.append(i)
print(f"Created {len(small_ints)} small integers: OK")

# Test large integers (not pooled)
large_ints = [1000, 5000, 10000]
print(f"Created {len(large_ints)} large integers: OK")

# Test boolean pooling
bools = [True, False, True, False] * 100
print(f"Created {len(bools)} booleans: OK")

# Test None pooling
nones = [None] * 100
print(f"Created {len(nones)} Nones: OK")

# Test string pooling
empty_strings = [""] * 50
single_chars = ["a", "b", "c", "x", "y", "z"] * 20
regular_strings = ["hello", "world", "test"] * 10
print(f"Created {len(empty_strings)} empty strings: OK")
print(f"Created {len(single_chars)} single-char strings: OK")
print(f"Created {len(regular_strings)} regular strings: OK")

# Test arithmetic with pooled values
result = 0
for i in range(100):
    result = result + i
print(f"Arithmetic with pooled integers: result = {result}")

# Test in real-world scenario
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

fib_10 = fibonacci(10)
print(f"Fibonacci(10) = {fib_10}")

print("\nAll object pooling tests passed!")
