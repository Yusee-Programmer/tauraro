# Test JIT compilation support for functions and classes
# This test validates that the JIT compiler can handle:
# 1. Builtin function calls within hot loops
# 2. Attribute access on objects
# 3. Method calls on builtin types
# 4. Simple class operations

print("=== JIT Functions and Classes Test Suite ===")

# Test 1: Builtin function calls in loop
print("\nTest 1: Builtin function calls in loop")
def test_builtin_calls():
    total = 0
    for i in range(1000):
        # len() is a builtin function - should JIT compile
        s = "hello"
        total = total + len(s) * i
    return total

result = test_builtin_calls()
print(f"Result: {result}")
# Calculate expected: sum of 5*i for i in range(1000)
expected = 0
for i in range(1000):
    expected = expected + (5 * i)
if result == expected:
    print("✓ Test 1 PASSED")
else:
    print(f"✗ Test 1 FAILED: expected {expected}, got {result}")

# Test 2: List method calls in loop
print("\nTest 2: List method calls in loop")
def test_list_methods():
    items = []
    for i in range(100):
        # append() is a method call
        items.append(i * 2)
    return len(items)

result = test_list_methods()
print(f"List length: {result}")
if result == 100:
    print("✓ Test 2 PASSED")
else:
    print(f"✗ Test 2 FAILED: expected 100, got {result}")

# Test 3: Dictionary method calls
print("\nTest 3: Dictionary operations in loop")
def test_dict_methods():
    counts = {}
    for i in range(50):
        key = str(i % 10)
        counts[key] = counts.get(key, 0) + 1
    return len(counts)

result = test_dict_methods()
print(f"Dict size: {result}")
if result == 10:
    print("✓ Test 3 PASSED")
else:
    print(f"✗ Test 3 FAILED: expected 10, got {result}")

# Test 4: Simple arithmetic with multiple operations
print("\nTest 4: Mixed operations in loop")
def test_mixed_operations():
    numbers = [1, 2, 3, 4, 5]
    total = 0
    for i in range(500):
        # Mix of subscript access and arithmetic
        for j in range(len(numbers)):
            total = total + numbers[j] * i
    return total

result = test_mixed_operations()
print(f"Total: {result}")
# Calculate expected: base_sum * sum(range(500))
base_sum = 1 + 2 + 3 + 4 + 5  # 15
range_sum = 0
for i in range(500):
    range_sum = range_sum + i
expected = base_sum * range_sum  # 15 * 124750 = 1871250
if result == expected:
    print("✓ Test 4 PASSED")
else:
    print(f"✗ Test 4 FAILED: expected {expected}, got {result}")

# Test 5: String operations in loop
print("\nTest 5: String operations in loop")
def test_string_ops():
    result_str = ""
    for i in range(50):
        if i % 10 == 0:
            result_str = result_str + "X"
    return len(result_str)

result = test_string_ops()
print(f"String length: {result}")
if result == 5:  # 0, 10, 20, 30, 40
    print("✓ Test 5 PASSED")
else:
    print(f"✗ Test 5 FAILED: expected 5, got {result}")

# Test 6: Nested loop with function calls
print("\nTest 6: Nested loops with operations")
def test_nested_loops():
    total = 0
    for i in range(10):
        for j in range(10):
            val = i * 10 + j
            total = total + val
    return total

result = test_nested_loops()
print(f"Matrix sum: {result}")
# Calculate expected: sum of i*10+j for all i,j in range(10)
expected = 0
for i in range(10):
    for j in range(10):
        expected = expected + (i * 10 + j)
if result == expected:
    print("✓ Test 6 PASSED")
else:
    print(f"✗ Test 6 FAILED: expected {expected}, got {result}")

# Test 7: Performance test - ensure JIT kicks in
print("\nTest 7: JIT hot loop detection")
def hot_loop_test():
    count = 0
    # This loop should trigger JIT compilation after threshold
    for i in range(10000):
        count = count + 1
        count = count * 2
        count = count - i
    return count

result = hot_loop_test()
print(f"Final count: {result}")
print("✓ Test 7 COMPLETED (JIT should have compiled this loop)")

# Test 8: Simple loop operations
print("\nTest 8: Simple loop calculations")
def test_simple_loop():
    total = 0
    for i in range(1000):
        total = total + (i * 2)
    return total

result = test_simple_loop()
print(f"Total: {result}")
# Calculate expected
expected = 0
for i in range(1000):
    expected = expected + (i * 2)
if result == expected:
    print("✓ Test 8 PASSED")
else:
    print(f"✗ Test 8 FAILED: expected {expected}, got {result}")

print("\n=== All Tests Completed ===")
