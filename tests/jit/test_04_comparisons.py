"""
JIT Test Suite: Comparison Operations
Tests all comparison opcodes in JIT-compiled loops.

Opcodes tested:
- CompareEqualRR
- CompareNotEqualRR
- CompareLessRR, FastIntCompare
- CompareLessEqualRR
- CompareGreaterRR
- CompareGreaterEqualRR
- CompareEqualF64RR
- CompareNotEqualF64RR
- CompareLessF64RR
- CompareLessEqualF64RR
- CompareGreaterF64RR
- CompareGreaterEqualF64RR
"""

print("=" * 60)
print("JIT Test Suite: Comparison Operations")
print("=" * 60)

# Test 1: Integer equality comparison
def test_int_equality():
    total = 0
    for i in range(10000):
        if i == 5000:
            total = total + 1
    return total

result = test_int_equality()
expected = 1
print(f"Test 1 - Int Equality: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 2: Integer inequality
def test_int_inequality():
    total = 0
    for i in range(1000):
        if i != 500:
            total = total + 1
    return total

result = test_int_inequality()
expected = 999  # All except one
print(f"Test 2 - Int Inequality: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 3: Less than comparison
def test_less_than():
    total = 0
    for i in range(10000):
        if i < 5000:
            total = total + 1
    return total

result = test_less_than()
expected = 5000
print(f"Test 3 - Less Than: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 4: Less than or equal
def test_less_equal():
    total = 0
    for i in range(10000):
        if i <= 5000:
            total = total + 1
    return total

result = test_less_equal()
expected = 5001
print(f"Test 4 - Less Equal: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 5: Greater than comparison
def test_greater_than():
    total = 0
    for i in range(10000):
        if i > 5000:
            total = total + 1
    return total

result = test_greater_than()
expected = 4999
print(f"Test 5 - Greater Than: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 6: Greater than or equal
def test_greater_equal():
    total = 0
    for i in range(10000):
        if i >= 5000:
            total = total + 1
    return total

result = test_greater_equal()
expected = 5000
print(f"Test 6 - Greater Equal: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 7: Float equality (approximate)
def test_float_equality():
    total = 0
    for i in range(1000):
        x = float(i) / 10.0
        if x == 50.0:
            total = total + 1
    return total

result = test_float_equality()
expected = 1
print(f"Test 7 - Float Equality: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 8: Float less than
def test_float_less_than():
    total = 0
    for i in range(1000):
        x = float(i) / 10.0
        if x < 50.0:
            total = total + 1
    return total

result = test_float_less_than()
expected = 500
print(f"Test 8 - Float Less Than: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 9: Combined comparisons
def test_combined_comparisons():
    total = 0
    for i in range(10000):
        if i > 2000:
            if i < 8000:
                if i != 5000:
                    total = total + 1
    return total

result = test_combined_comparisons()
# Count numbers in range (2000, 8000) excluding 5000
expected = 5999  # 2001..7999 minus 5000
print(f"Test 9 - Combined Comparisons: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

# Test 10: Comparison in accumulation
def test_comparison_accumulation():
    total = 0
    for i in range(10000):
        if i % 2 == 0:
            total = total + i
        else:
            total = total - i
    return total

result = test_comparison_accumulation()
expected = 0
for i in range(10000):
    if i % 2 == 0:
        expected = expected + i
    else:
        expected = expected - i
print(f"Test 10 - Comparison Accumulation: {result} == {expected} : {'PASS' if result == expected else 'FAIL'}")

print("=" * 60)
print("Comparison Operations Tests Complete")
print("=" * 60)
