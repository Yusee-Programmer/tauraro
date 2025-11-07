"""
Quick JIT Validation Test
Tests core JIT functionality that is known to work
"""

print("=" * 60)
print("Quick JIT Validation Test")
print("=" * 60)

# Test 1: Integer addition
def test_int_add():
    total = 0
    for i in range(1000):
        total = total + i
    return total

result1 = test_int_add()
expected1 = 499500
status1 = "PASS"
if result1 != expected1:
    status1 = "FAIL"
print(f"1. Integer Addition: {result1} == {expected1} : {status1}")

# Test 2: Integer multiplication
def test_int_mul():
    total = 1
    for i in range(1, 11):
        total = total * i
    return total

result2 = test_int_mul()
expected2 = 3628800  # 10!
status2 = "PASS"
if result2 != expected2:
    status2 = "FAIL"
print(f"2. Integer Multiplication: {result2} == {expected2} : {status2}")

# Test 3: Integer subtraction
def test_int_sub():
    total = 10000
    for i in range(100):
        total = total - i
    return total

result3 = test_int_sub()
expected3 = 10000 - 4950
status3 = "PASS"
if result3 != expected3:
    status3 = "FAIL"
print(f"3. Integer Subtraction: {result3} == {expected3} : {status3}")

# Test 4: Comparisons
def test_comparisons():
    count = 0
    for i in range(1000):
        if i > 500:
            count = count + 1
    return count

result4 = test_comparisons()
expected4 = 499
status4 = "PASS"
if result4 != expected4:
    status4 = "FAIL"
print(f"4. Comparisons: {result4} == {expected4} : {status4}")

# Test 5: Mixed operations
def test_mixed():
    total = 0
    for i in range(100):
        x = i * 2
        y = x + 10
        total = total + y
    return total

result5 = test_mixed()
expected5 = 0
for i in range(100):
    x = i * 2
    y = x + 10
    expected5 = expected5 + y
status5 = "PASS"
if result5 != expected5:
    status5 = "FAIL"
print(f"5. Mixed Operations: {result5} == {expected5} : {status5}")

print("=" * 60)
total_tests = 5
passed = 0
if status1 == "PASS":
    passed = passed + 1
if status2 == "PASS":
    passed = passed + 1
if status3 == "PASS":
    passed = passed + 1
if status4 == "PASS":
    passed = passed + 1
if status5 == "PASS":
    passed = passed + 1

print(f"Results: {passed}/{total_tests} tests passed")
print("=" * 60)
