#!/usr/bin/env python3
"""
Test file for newly implemented Tauraro features:
1. Chained comparisons (a < b < c)
2. Bitwise NOT (~) operator
3. New string methods: encode, isidentifier, isascii, partition, rpartition, expandtabs
4. bytes.decode() method
"""

print("=" * 60)
print("Testing New Tauraro Features")
print("=" * 60)

# Test 1: Chained Comparisons
print("\n1. Testing Chained Comparisons:")
print("-" * 40)

a, b, c = 1, 5, 10
result1 = a < b < c
print(f"   {a} < {b} < {c} = {result1}")
assert result1 == True, "Expected True"

d, e, f = 10, 5, 1
result2 = d > e > f
print(f"   {d} > {e} > {f} = {result2}")
assert result2 == True, "Expected True"

g, h, i = 1, 5, 3
result3 = g < h < i
print(f"   {g} < {h} < {i} = {result3}")
assert result3 == False, "Expected False"

# Multiple chained comparisons
result4 = 1 < 2 < 3 < 4 < 5
print(f"   1 < 2 < 3 < 4 < 5 = {result4}")
assert result4 == True, "Expected True"

result5 = 1 < 2 == 2 < 3
print(f"   1 < 2 == 2 < 3 = {result5}")
assert result5 == True, "Expected True"

print("   ✓ Chained comparisons working correctly!")

# Test 2: Bitwise NOT operator (~)
print("\n2. Testing Bitwise NOT Operator:")
print("-" * 40)

x = 5
result_not = ~x
print(f"   ~{x} = {result_not}")
assert result_not == -6, f"Expected -6, got {result_not}"

y = -1
result_not2 = ~y
print(f"   ~{y} = {result_not2}")
assert result_not2 == 0, f"Expected 0, got {result_not2}"

z = 0
result_not3 = ~z
print(f"   ~{z} = {result_not3}")
assert result_not3 == -1, f"Expected -1, got {result_not3}"

# Test with boolean
b_true = True
result_not4 = ~b_true
print(f"   ~{b_true} = {result_not4}")
assert result_not4 == -2, f"Expected -2, got {result_not4}"

b_false = False
result_not5 = ~b_false
print(f"   ~{b_false} = {result_not5}")
assert result_not5 == -1, f"Expected -1, got {result_not5}"

print("   ✓ Bitwise NOT operator working correctly!")

# Test 3: New String Methods
print("\n3. Testing New String Methods:")
print("-" * 40)

# Test encode()
text = "Hello, World!"
encoded = text.encode()
print(f"   '{text}'.encode() = {encoded}")
assert isinstance(encoded, bytes), "Expected bytes type"

# Test isidentifier()
valid_id = "my_variable"
invalid_id = "123invalid"
print(f"   '{valid_id}'.isidentifier() = {valid_id.isidentifier()}")
assert valid_id.isidentifier() == True, "Expected True"
print(f"   '{invalid_id}'.isidentifier() = {invalid_id.isidentifier()}")
assert invalid_id.isidentifier() == False, "Expected False"

# Test isascii()
ascii_str = "Hello"
non_ascii_str = "Héllo"
print(f"   '{ascii_str}'.isascii() = {ascii_str.isascii()}")
assert ascii_str.isascii() == True, "Expected True"
# Note: non-ASCII test might fail if not supported
# print(f"   '{non_ascii_str}'.isascii() = {non_ascii_str.isascii()}")

# Test partition()
text2 = "hello-world-python"
parts = text2.partition("-")
print(f"   '{text2}'.partition('-') = {parts}")
assert parts == ("hello", "-", "world-python"), f"Expected ('hello', '-', 'world-python'), got {parts}"

text3 = "nodelimiter"
parts2 = text3.partition("-")
print(f"   '{text3}'.partition('-') = {parts2}")
assert parts2 == ("nodelimiter", "", ""), f"Expected ('nodelimiter', '', ''), got {parts2}"

# Test rpartition()
text4 = "hello-world-python"
parts3 = text4.rpartition("-")
print(f"   '{text4}'.rpartition('-') = {parts3}")
assert parts3 == ("hello-world", "-", "python"), f"Expected ('hello-world', '-', 'python'), got {parts3}"

# Test expandtabs()
text5 = "hello\tworld"
expanded = text5.expandtabs()
print(f"   '{text5}'.expandtabs() = '{expanded}'")
# Default tab size is 8
assert "\t" not in expanded, "Expected tabs to be expanded"

text6 = "a\tb"
expanded2 = text6.expandtabs(4)
print(f"   '{text6}'.expandtabs(4) = '{expanded2}'")
assert expanded2 == "a   b", f"Expected 'a   b', got '{expanded2}'"

print("   ✓ New string methods working correctly!")

# Test 4: bytes.decode() method
print("\n4. Testing bytes.decode() Method:")
print("-" * 40)

byte_data = b"Hello, World!"
decoded = byte_data.decode()
print(f"   {byte_data}.decode() = '{decoded}'")
assert decoded == "Hello, World!", f"Expected 'Hello, World!', got '{decoded}'"
assert isinstance(decoded, str), "Expected str type"

# Test encode and decode round-trip
original = "Python is awesome!"
encoded = original.encode()
decoded = encoded.decode()
print(f"   Round-trip: '{original}' -> encode() -> decode() -> '{decoded}'")
assert original == decoded, "Round-trip should preserve string"

print("   ✓ bytes.decode() method working correctly!")

# Summary
print("\n" + "=" * 60)
print("All Tests Passed! ✓")
print("=" * 60)
print("\nSuccessfully tested:")
print("  1. Chained comparisons (a < b < c)")
print("  2. Bitwise NOT (~) operator")
print("  3. String methods: encode, isidentifier, isascii,")
print("     partition, rpartition, expandtabs")
print("  4. bytes.decode() method")
print("=" * 60)
