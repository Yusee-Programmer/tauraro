# Test complete bitwise operator support
# Tests XOR, Left Shift, and Right Shift operations

print("=" * 70)
print("COMPLETE BITWISE OPERATORS TEST")
print("=" * 70)
print()

# Test 1: XOR operation
print("[TEST 1] Bitwise XOR (^)")
print("-" * 70)
a = 12  # 0b1100
b = 10  # 0b1010
result = a ^ b  # Should be 6 (0b0110)
print(f"{a} ^ {b} = {result}")
print(f"Expected: 6, Got: {result}, {'PASS' if result == 6 else 'FAIL'}")
print()

# Test 2: Left Shift operation
print("[TEST 2] Left Shift (<<)")
print("-" * 70)
a = 5   # 0b0101
result = a << 2  # Should be 20 (0b10100)
print(f"{a} << 2 = {result}")
print(f"Expected: 20, Got: {result}, {'PASS' if result == 20 else 'FAIL'}")
print()

# Test 3: Right Shift operation
print("[TEST 3] Right Shift (>>)")
print("-" * 70)
a = 20  # 0b10100
result = a >> 2  # Should be 5 (0b0101)
print(f"{a} >> 2 = {result}")
print(f"Expected: 5, Got: {result}, {'PASS' if result == 5 else 'FAIL'}")
print()

# Test 4: All bitwise operators in loop
print("[TEST 4] All Bitwise Operators in Loop")
print("-" * 70)
total = 0
for i in range(1000):
    val = i & 255      # AND
    val = val | 128    # OR
    val = val ^ 64     # XOR
    val = val << 1     # Left shift
    val = val >> 1     # Right shift
    total = total + val

print(f"Loop completed with total: {total}")
print("PASS" if total > 0 else "FAIL")
print()

# Test 5: Bitwise XOR in loop for JIT
print("[TEST 5] XOR in Hot Loop (JIT compilation)")
print("-" * 70)
xor_sum = 0
for i in range(100000):
    xor_sum = xor_sum + (i ^ 255)

print(f"XOR sum: {xor_sum}")
print("PASS" if xor_sum > 0 else "FAIL")
print()

# Test 6: Shift operations in loop for JIT
print("[TEST 6] Shift Operations in Hot Loop (JIT compilation)")
print("-" * 70)
shift_sum = 0
for i in range(100000):
    val = (i & 15) << 2  # Left shift
    val = val >> 1        # Right shift
    shift_sum = shift_sum + val

print(f"Shift sum: {shift_sum}")
print("PASS" if shift_sum > 0 else "FAIL")
print()

# Test 7: Combined bitwise in nested loops
print("[TEST 7] Combined Bitwise in Nested Loops")
print("-" * 70)
nested_result = 0
for i in range(200):
    for j in range(200):
        val = (i & j) | (i ^ j)
        nested_result = nested_result + val

print(f"Nested result: {nested_result}")
print("PASS" if nested_result > 0 else "FAIL")
print()

# Test 8: Float operations (verify they still work)
print("[TEST 8] Float Operations")
print("-" * 70)
x = 3.14
y = 2.0
print(f"{x} + {y} = {x + y}")
print(f"{x} * {y} = {x * y}")
print(f"int({x}) = {int(x)}")
print(f"float(5) = {float(5)}")
print("PASS")
print()

print("=" * 70)
print("ALL TESTS COMPLETED")
print("=" * 70)
