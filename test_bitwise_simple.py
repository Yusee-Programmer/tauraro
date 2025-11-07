# Simple test for bitwise operators
print("Testing Bitwise Operators")
print("=" * 50)

# Test 1: XOR
a = 12  # 0b1100
b = 10  # 0b1010
result = a ^ b  # Should be 6 (0b0110)
print(f"TEST 1: XOR - {a} ^ {b} = {result}")
if result == 6:
    print("PASS")
else:
    print("FAIL")

# Test 2: Left Shift
a = 5
result = a << 2  # Should be 20
print(f"TEST 2: Left Shift - {a} << 2 = {result}")
if result == 20:
    print("PASS")
else:
    print("FAIL")

# Test 3: Right Shift
a = 20
result = a >> 2  # Should be 5
print(f"TEST 3: Right Shift - {a} >> 2 = {result}")
if result == 5:
    print("PASS")
else:
    print("FAIL")

# Test 4: Combined operators in loop
total = 0
for i in range(100):
    val = i & 255
    val = val | 128
    val = val ^ 64
    total = total + val

print(f"TEST 4: Combined bitwise in loop - total = {total}")
if total > 0:
    print("PASS")
else:
    print("FAIL")

print("=" * 50)
print("Bitwise operators test completed!")
