"""
JIT Test Suite: Integer Arithmetic Operations
Tests all integer arithmetic opcodes in JIT-compiled loops.

Opcodes tested:
- BinaryAddRR, FastIntAdd
- BinarySubRR, FastIntSub
- BinaryMulRR, FastIntMul
- BinaryDivRR, FastIntDiv
- BinaryModRR, FastIntMod
- BinaryPowRR
- BinaryFloorDivRR
"""

print("=" * 60)
print("JIT Test Suite: Integer Arithmetic Operations")
print("=" * 60)

# Test 1: Addition in hot loop (should trigger JIT)
def test_addition():
    total = 0
    for i in range(10000):
        total = total + i
    return total

result = test_addition()
expected = 49995000  # sum(0..9999) = 9999*10000/2
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 1 - Addition: {result} == {expected} : {status}")

# Test 2: Subtraction in hot loop
def test_subtraction():
    total = 100000
    for i in range(10000):
        total = total - i
    return total

result = test_subtraction()
expected = 100000 - 49995000
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 2 - Subtraction: {result} == {expected} : {status}")

# Test 3: Multiplication in hot loop
def test_multiplication():
    total = 1
    for i in range(1, 100):
        total = total * 2
    return total

result = test_multiplication()
expected = 2 ** 99
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 3 - Multiplication: {result} == {expected} : {status}")

# Test 4: Division in hot loop
def test_division():
    total = 1000000
    for i in range(10):
        total = total / 2
    return total

result = test_division()
expected = 1000000.0 / (2 ** 10)
status = "PASS"
diff = abs(result - expected)
if diff >= 0.01:
    status = "FAIL"
print(f"Test 4 - Division: {result} == {expected} : {status}")

# Test 5: Floor division in hot loop
def test_floor_division():
    total = 0
    for i in range(1, 1000):
        total = total + (i // 3)
    return total

result = test_floor_division()
# Calculate expected: sum of i//3 for i in 1..999
expected = 0
for i in range(1, 1000):
    expected = expected + (i // 3)
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 5 - Floor Division: {result} == {expected} : {status}")

# Test 6: Modulo in hot loop
def test_modulo():
    total = 0
    for i in range(10000):
        total = total + (i % 7)
    return total

result = test_modulo()
# Calculate expected
expected = 0
for i in range(10000):
    expected = expected + (i % 7)
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 6 - Modulo: {result} == {expected} : {status}")

# Test 7: Power in hot loop
def test_power():
    total = 0
    for i in range(1, 100):
        total = total + (2 ** 10)
    return total

result = test_power()
expected = 99 * (2 ** 10)
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 7 - Power: {result} == {expected} : {status}")

# Test 8: Mixed operations (tests register allocation)
def test_mixed():
    a = 0
    b = 0
    c = 0
    for i in range(5000):
        a = a + i
        b = b + (i * 2)
        c = c + (i % 3)
    return a + b + c

result = test_mixed()
expected_a = 12497500  # sum(0..4999)
expected_b = 24995000  # sum(i*2 for i in 0..4999)
expected_c = 0
for i in range(5000):
    expected_c = expected_c + (i % 3)
expected = expected_a + expected_b + expected_c
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 8 - Mixed Operations: {result} == {expected} : {status}")

# Test 9: Immediate operands (BinaryAddRI, BinaryAddIR)
def test_immediate_add():
    total = 0
    for i in range(10000):
        total = total + 5  # Immediate constant
    return total

result = test_immediate_add()
expected = 50000
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 9 - Add Immediate: {result} == {expected} : {status}")

# Test 10: Complex expression (multiple operations per iteration)
def test_complex_expression():
    total = 0
    for i in range(1, 1000):
        total = total + ((i * 3 + 5) % 7)
    return total

result = test_complex_expression()
expected = 0
for i in range(1, 1000):
    expected = expected + ((i * 3 + 5) % 7)
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 10 - Complex Expression: {result} == {expected} : {status}")

print("=" * 60)
print("Integer Arithmetic Tests Complete")
print("=" * 60)
