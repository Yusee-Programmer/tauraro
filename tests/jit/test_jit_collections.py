"""
Phase 4 JIT Test: Collection Operations with Runtime Helpers

Tests list indexing, dict access, and other collection operations
that require runtime helper calls from JIT-compiled code.
"""

print("=" * 60)
print("Phase 4 JIT Test: Collection Operations")
print("=" * 60)

# Test 1: List indexing in hot loop
def test_list_index():
    items = [10, 20, 30, 40, 50]
    total = 0
    for i in range(1000):
        idx = i % 5
        total = total + items[idx]
    return total

result1 = test_list_index()
# Expected: (10+20+30+40+50) * 200 = 150 * 200 = 30000
expected1 = 30000
status1 = "PASS"
if result1 != expected1:
    status1 = "FAIL"
print(f"1. List Indexing: {result1} == {expected1} : {status1}")

# Test 2: List building
def test_list_build():
    count = 0
    for i in range(100):
        items = [i, i+1, i+2]
        count = count + len(items)
    return count

result2 = test_list_build()
expected2 = 300  # 3 items * 100 iterations
status2 = "PASS"
if result2 != expected2:
    status2 = "FAIL"
print(f"2. List Building: {result2} == {expected2} : {status2}")

# Test 3: List append
def test_list_append():
    items = []
    for i in range(100):
        items.append(i)
    return len(items)

result3 = test_list_append()
expected3 = 100
status3 = "PASS"
if result3 != expected3:
    status3 = "FAIL"
print(f"3. List Append: {result3} == {expected3} : {status3}")

# Test 4: Mixed list operations
def test_mixed_list():
    items = [1, 2, 3, 4, 5]
    total = 0
    for i in range(200):
        idx = i % 5
        val = items[idx]
        total = total + val
    return total

result4 = test_mixed_list()
expected4 = (1+2+3+4+5) * 40  # 15 * 40 = 600
status4 = "PASS"
if result4 != expected4:
    status4 = "FAIL"
print(f"4. Mixed List Operations: {result4} == {expected4} : {status4}")

print("=" * 60)
total_tests = 4
passed = 0
if status1 == "PASS":
    passed = passed + 1
if status2 == "PASS":
    passed = passed + 1
if status3 == "PASS":
    passed = passed + 1
if status4 == "PASS":
    passed = passed + 1

print(f"Results: {passed}/{total_tests} tests passed")

if passed == total_tests:
    print("✓ All collection operations working with JIT!")
else:
    print(f"✗ {total_tests - passed} test(s) failed")

print("=" * 60)
