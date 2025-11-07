# Simple JIT compilation test
# This test should trigger JIT compilation after 10,000 iterations

print("=== Testing JIT Compilation ===")
print()

# Test 1: Simple loop that should trigger JIT
print("Test 1: Simple integer loop (should JIT compile after 10k iterations)")
total = 0
for i in range(15000):
    total = total + i

print(f"Total after 15,000 iterations: {total}")
expected = 112492500  # sum(range(15000))
print(f"Expected: {expected}")
assert total == expected, f"Expected {expected}, got {total}"
print("✓ Test passed")
print()

# Test 2: Loop with multiplication
print("Test 2: Loop with multiplication")
result = 0
for i in range(12000):
    result = result + i * 2

print(f"Result: {result}")
expected_result = 143988000  # sum of i*2 for i in range(12000)
print(f"Expected: {expected_result}")
assert result == expected_result, f"Expected {expected_result}, got {result}"
print("✓ Test passed")
print()

# Test 3: Complex arithmetic
print("Test 3: Complex arithmetic")
value = 1
for i in range(11000):
    temp = i * 3
    value = value + temp - i

print(f"Value: {value}")
expected_value = 120989001  # 1 + sum of (i*3 - i) = 1 + sum of (i*2) for i in range(11000)
print(f"Expected: {expected_value}")
assert value == expected_value, f"Expected {expected_value}, got {value}"
print("✓ Test passed")
print()

print("=== All JIT Tests Passed! ===")
print()
print("Note: Check console output for JIT compilation messages")
print("You should see messages like: 'JIT: Compiled loop in <main> at PC X'")
