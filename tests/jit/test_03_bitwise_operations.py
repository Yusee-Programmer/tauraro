"""
JIT Test Suite: Bitwise Operations
Tests all bitwise operation opcodes in JIT-compiled loops.

Opcodes tested:
- BinaryBitAndRR
- BinaryBitOrRR
- BinaryBitXorRR
- BinaryLShiftRR
- BinaryRShiftRR
- UnaryInvert
"""

print("=" * 60)
print("JIT Test Suite: Bitwise Operations")
print("=" * 60)

# Test 1: Bitwise AND in hot loop
def test_bitwise_and():
    total = 0
    for i in range(10000):
        total = total + (i & 255)  # Mask to low byte
    return total

result = test_bitwise_and()
expected = 0
for i in range(10000):
    expected = expected + (i & 255)
print(f"Test 1 - Bitwise AND: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 2: Bitwise OR in hot loop
def test_bitwise_or():
    total = 0
    for i in range(10000):
        total = total + (i | 128)  # Set bit 7
    return total

result = test_bitwise_or()
expected = 0
for i in range(10000):
    expected = expected + (i | 128)
print(f"Test 2 - Bitwise OR: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 3: Bitwise XOR in hot loop
def test_bitwise_xor():
    total = 0
    for i in range(10000):
        total = total + (i ^ 255)  # Flip low byte
    return total

result = test_bitwise_xor()
expected = 0
for i in range(10000):
    expected = expected + (i ^ 255)
print(f"Test 3 - Bitwise XOR: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 4: Left shift in hot loop
def test_left_shift():
    total = 0
    for i in range(1, 1000):
        total = total + (i << 2)  # Multiply by 4
    return total

result = test_left_shift()
expected = 0
for i in range(1, 1000):
    expected = expected + (i << 2)
print(f"Test 4 - Left Shift: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 5: Right shift in hot loop
def test_right_shift():
    total = 0
    for i in range(10000):
        total = total + (i >> 3)  # Divide by 8
    return total

result = test_right_shift()
expected = 0
for i in range(10000):
    expected = expected + (i >> 3)
print(f"Test 5 - Right Shift: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 6: Bitwise NOT (invert)
def test_bitwise_not():
    total = 0
    for i in range(256):
        x = ~i
        total = total + (x & 255)  # Mask to byte to avoid negative numbers
    return total

result = test_bitwise_not()
expected = 0
for i in range(256):
    x = ~i
    expected = expected + (x & 255)
print(f"Test 6 - Bitwise NOT: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 7: Combined bitwise operations
def test_combined_bitwise():
    total = 0
    for i in range(5000):
        x = (i & 255) | 128
        y = x ^ 64
        z = y << 1
        total = total + (z >> 1)
    return total

result = test_combined_bitwise()
expected = 0
for i in range(5000):
    x = (i & 255) | 128
    y = x ^ 64
    z = y << 1
    expected = expected + (z >> 1)
print(f"Test 7 - Combined Bitwise: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 8: Bit manipulation for flags
def test_bit_flags():
    total = 0
    for i in range(1000):
        flags = 0
        if i % 2 == 0:
            flags = flags | 1  # Set bit 0
        if i % 3 == 0:
            flags = flags | 2  # Set bit 1
        if i % 5 == 0:
            flags = flags | 4  # Set bit 2
        total = total + flags
    return total

result = test_bit_flags()
expected = 0
for i in range(1000):
    flags = 0
    if i % 2 == 0:
        flags = flags | 1
    if i % 3 == 0:
        flags = flags | 2
    if i % 5 == 0:
        flags = flags | 4
    expected = expected + flags
print(f"Test 8 - Bit Flags: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 9: Shift as multiplication/division
def test_shift_arithmetic():
    total = 0
    for i in range(1, 500):
        mul = i << 3  # i * 8
        div = i >> 2  # i / 4
        total = total + (mul + div)
    return total

result = test_shift_arithmetic()
expected = 0
for i in range(1, 500):
    mul = i << 3
    div = i >> 2
    expected = expected + (mul + div)
print(f"Test 9 - Shift Arithmetic: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 10: Mask extraction
def test_mask_extraction():
    total = 0
    for i in range(10000):
        # Extract different bit ranges
        low = i & 15       # Low nibble
        mid = (i >> 4) & 15  # Middle nibble
        high = (i >> 8) & 15 # High nibble
        total = total + (low + mid + high)
    return total

result = test_mask_extraction()
expected = 0
for i in range(10000):
    low = i & 15
    mid = (i >> 4) & 15
    high = (i >> 8) & 15
    expected = expected + (low + mid + high)
print(f"Test 10 - Mask Extraction: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

print("=" * 60)
print("Bitwise Operations Tests Complete")
print("=" * 60)
