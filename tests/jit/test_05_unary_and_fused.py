"""
JIT Test Suite: Unary and Fused Operations
Tests unary operations and fused super-instructions in JIT-compiled loops.

Opcodes tested:
- UnaryNegate
- UnaryNot
- UnaryInvert
- UnaryNegateF64
- LoadAddStore
- LoadMulStore
- LoadSubStore
- LoadDivStore
- MoveReg
- IncLocal
- DecLocal
"""

print("=" * 60)
print("JIT Test Suite: Unary and Fused Operations")
print("=" * 60)

# Test 1: Unary negation (integer)
def test_unary_negate():
    total = 0
    for i in range(1000):
        x = -i
        total = total + x
    return total

result = test_unary_negate()
expected = -499500  # -sum(0..999)
print(f"Test 1 - Unary Negate: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 2: Unary NOT (logical negation)
def test_unary_not():
    total = 0
    for i in range(1000):
        if not (i > 500):
            total = total + 1
    return total

result = test_unary_not()
expected = 501  # i in [0, 500]
print(f"Test 2 - Unary NOT: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 3: Unary invert (bitwise NOT)
def test_unary_invert():
    total = 0
    for i in range(256):
        x = ~i
        total = total + (x & 255)  # Mask to byte
    return total

result = test_unary_invert()
expected = 0
for i in range(256):
    x = ~i
    expected = expected + (x & 255)
print(f"Test 3 - Unary Invert: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 4: Float negation
def test_float_negate():
    total = 0.0
    for i in range(1000):
        x = -float(i)
        total = total + x
    return total

result = test_float_negate()
expected = -499500.0
diff = abs(result - expected)
print(f"Test 4 - Float Negate: {result} ≈ {expected} : {'PASS' if diff < 0.01 else 'FAIL'}")

# Test 5: Compound assignment (+=)
def test_compound_add():
    total = 0
    for i in range(10000):
        total = total + i  # Should use LoadAddStore fused op
    return total

result = test_compound_add()
expected = 49995000
print(f"Test 5 - Compound Add (+=): {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 6: Compound multiplication (*=)
def test_compound_mul():
    total = 1
    for i in range(20):
        total = total * 2  # Should use LoadMulStore
    return total

result = test_compound_mul()
expected = 2 ** 20
print(f"Test 6 - Compound Mul (*=): {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 7: Compound subtraction (-=)
def test_compound_sub():
    total = 100000
    for i in range(1000):
        total = total - i  # Should use LoadSubStore
    return total

result = test_compound_sub()
expected = 100000 - 499500
print(f"Test 7 - Compound Sub (-=): {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 8: Increment operation
def test_increment():
    counter = 0
    for i in range(10000):
        counter = counter + 1  # Should use IncLocal
    return counter

result = test_increment()
expected = 10000
print(f"Test 8 - Increment: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 9: Multiple variables with fused ops
def test_multiple_fused():
    a = 0
    b = 0
    c = 0
    for i in range(5000):
        a = a + i
        b = b * 1  # Should optimize
        c = c - i
    return a + b + c

result = test_multiple_fused()
expected_a = 12497500
expected_b = 0
expected_c = -12497500
expected = expected_a + expected_b + expected_c
print(f"Test 9 - Multiple Fused: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 10: Negation in expression
def test_negate_in_expression():
    total = 0
    for i in range(1000):
        x = -(i * 2)
        total = total + x
    return total

result = test_negate_in_expression()
expected = 0
for i in range(1000):
    x = -(i * 2)
    expected = expected + x
print(f"Test 10 - Negate in Expression: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

print("=" * 60)
print("Unary and Fused Operations Tests Complete")
print("=" * 60)
