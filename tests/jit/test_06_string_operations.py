"""
JIT Test Suite: String Operations
Tests JIT runtime helpers for string operations.

Runtime helpers tested:
- tauraro_jit_string_concat
- tauraro_jit_string_index
- tauraro_jit_string_slice
- tauraro_jit_string_len
"""

print("=" * 60)
print("JIT Test Suite: String Operations")
print("=" * 60)

# Test 1: String concatenation in loop
def test_string_concat():
    result = ""
    for i in range(100):
        result = result + "x"
    return len(result)

expected = 100
result = test_string_concat()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 1 - String Concat: {result} == {expected} : {status}")

# Test 2: String indexing
def test_string_index():
    s = "hello world"
    chars = ""
    for i in range(len(s)):
        chars = chars + s[i]
    return chars

expected = "hello world"
result = test_string_index()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 2 - String Index: {result} == {expected} : {status}")

# Test 3: String slicing
def test_string_slice():
    s = "hello world"
    total = 0
    for i in range(100):
        sub = s[0:5]
        total = total + len(sub)
    return total

expected = 500
result = test_string_slice()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 3 - String Slice: {result} == {expected} : {status}")

# Test 4: String length
def test_string_len():
    strings = ["", "a", "ab", "abc", "abcd"]
    total = 0
    for s in strings:
        for i in range(100):
            total = total + len(s)
    return total

expected = 1000  # (0+1+2+3+4) * 100
result = test_string_len()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 4 - String Len: {result} == {expected} : {status}")

# Test 5: String building pattern
def test_string_building():
    parts = []
    for i in range(50):
        parts.append("item")

    result = ""
    for part in parts:
        result = result + part

    return len(result)

expected = 200  # 50 * 4
result = test_string_building()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 5 - String Building: {result} == {expected} : {status}")

# Test 6: Character extraction
def test_char_extraction():
    s = "abcdefghij"
    chars_sum = 0
    for i in range(len(s)):
        char = s[i]
        # Sum ASCII values (simplified)
        chars_sum = chars_sum + i
    return chars_sum

expected = 45  # sum(0..9)
result = test_char_extraction()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 6 - Char Extraction: {result} == {expected} : {status}")

# Test 7: Negative indexing
def test_negative_index():
    s = "hello"
    last_char = ""
    for i in range(10):
        last_char = s[-1]
    return last_char

expected = "o"
result = test_negative_index()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 7 - Negative Index: '{result}' == '{expected}' : {status}")

# Test 8: Empty string handling
def test_empty_string():
    s = ""
    count = 0
    for i in range(100):
        count = count + len(s)
    return count

expected = 0
result = test_empty_string()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 8 - Empty String: {result} == {expected} : {status}")

# Test 9: String comparison pattern
def test_string_pattern():
    target = "test"
    matches = 0
    strings = ["test", "other", "test", "different", "test"]

    for s in strings:
        if s == target:
            matches = matches + 1

    return matches

expected = 3
result = test_string_pattern()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 9 - String Pattern: {result} == {expected} : {status}")

# Test 10: Complex string operations
def test_complex_string_ops():
    total = 0
    for i in range(50):
        s = "item" + str(i)
        total = total + len(s)
    return total

# Length varies: "item0" (5), "item1" (5), ... "item9" (5), "item10" (6), etc.
expected = 0
for i in range(50):
    expected = expected + (4 + len(str(i)))

result = test_complex_string_ops()
status = "PASS"
if result != expected:
    status = "FAIL"
print(f"Test 10 - Complex String Ops: {result} == {expected} : {status}")

print("=" * 60)
print("String Operations Tests Complete")
print("=" * 60)
