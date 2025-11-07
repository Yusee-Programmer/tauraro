"""
JIT Test Suite: Collection Types (Tuple, Dict, Set)
Tests JIT runtime helpers for tuple, dictionary, and set operations.

Runtime helpers tested:
- tauraro_jit_build_tuple
- tauraro_jit_tuple_index
- tauraro_jit_dict_get
- tauraro_jit_dict_set
- tauraro_jit_build_dict
- tauraro_jit_build_set
- tauraro_jit_set_add
"""

print("=" * 60)
print("JIT Test Suite: Collection Types")
print("=" * 60)

# ============================================================================
# TUPLE TESTS
# ============================================================================

# Test 1: Tuple building
def test_tuple_build():
    total = 0
    for i in range(100):
        t = (i, i+1, i+2)
        total = total + len(t)
    return total

expected = 300
result = test_tuple_build()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 1 - Tuple Build: {result} == {expected} : {status}")

# Test 2: Tuple indexing
def test_tuple_index():
    t = (10, 20, 30, 40, 50)
    total = 0
    for i in range(100):
        total = total + t[2]  # Should be 30
    return total

expected = 3000
result = test_tuple_index()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 2 - Tuple Index: {result} == {expected} : {status}")

# Test 3: Tuple iteration
def test_tuple_iteration():
    t = (1, 2, 3, 4, 5)
    total = 0
    for i in range(len(t)):
        total = total + t[i]
    return total

expected = 15  # sum(1..5)
result = test_tuple_iteration()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 3 - Tuple Iteration: {result} == {expected} : {status}")

# Test 4: Tuple unpacking pattern
def test_tuple_unpacking():
    total = 0
    for i in range(50):
        pair = (i, i * 2)
        first = pair[0]
        second = pair[1]
        total = total + first + second
    return total

expected = 0
for i in range(50):
    expected = expected + i + (i * 2)
result = test_tuple_unpacking()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 4 - Tuple Unpacking: {result} == {expected} : {status}")

# ============================================================================
# DICTIONARY TESTS
# ============================================================================

# Test 5: Dictionary get
def test_dict_get():
    config = {"timeout": 30, "retries": 3, "port": 8080}
    total = 0
    for i in range(100):
        total = total + config["timeout"]
    return total

expected = 3000
result = test_dict_get()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 5 - Dict Get: {result} == {expected} : {status}")

# Test 6: Dictionary set
def test_dict_set():
    cache = {}
    for i in range(50):
        cache[str(i)] = i * 2

    total = 0
    for key in range(50):
        total = total + cache[str(key)]

    return total

expected = 0
for i in range(50):
    expected = expected + (i * 2)
result = test_dict_set()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 6 - Dict Set: {result} == {expected} : {status}")

# Test 7: Dictionary building
def test_dict_build():
    total = 0
    for i in range(50):
        d = {"x": i, "y": i * 2}
        total = total + d["x"] + d["y"]
    return total

expected = 0
for i in range(50):
    expected = expected + i + (i * 2)
result = test_dict_build()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 7 - Dict Build: {result} == {expected} : {status}")

# Test 8: Dictionary accumulation
def test_dict_accumulation():
    counts = {}
    items = ["a", "b", "a", "c", "b", "a", "d"]

    for item in items:
        if item in counts:
            current = counts[item]
            counts[item] = current + 1
        else:
            counts[item] = 1

    total = 0
    for key in ["a", "b", "c", "d"]:
        if key in counts:
            total = total + counts[key]

    return total

expected = 7  # Total number of items
result = test_dict_accumulation()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 8 - Dict Accumulation: {result} == {expected} : {status}")

# ============================================================================
# SET TESTS
# ============================================================================

# Test 9: Set building
def test_set_build():
    total = 0
    for i in range(50):
        s = {i, i+1, i+2}
        total = total + len(s)
    return total

expected = 150  # 50 * 3
result = test_set_build()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 9 - Set Build: {result} == {expected} : {status}")

# Test 10: Set add (deduplication)
def test_set_add():
    seen = set()
    for i in range(100):
        seen.add(i % 10)  # Only 0-9 unique values

    return len(seen)

expected = 10
result = test_set_add()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 10 - Set Add: {result} == {expected} : {status}")

print("=" * 60)
print("Collection Types Tests Complete")
print("=" * 60)
