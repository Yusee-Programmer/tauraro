# Comprehensive test of dict.get() with direct calls
d = {"a": 1, "b": 2}

print("=== Test 1: Direct call with existing key ===")
result = d.get("a")
print(f"d.get('a') = {result}")
assert result == 1, f"Expected 1, got {result}"

print("\n=== Test 2: Direct call with missing key (no default) ===")
result = d.get("c")
print(f"d.get('c') = {result}")
assert result == None, f"Expected None, got {result}"

print("\n=== Test 3: Direct call with missing key (with default) ===")
result = d.get("c", 99)
print(f"d.get('c', 99) = {result}")
assert result == 99, f"Expected 99, got {result}"

print("\n=== Test 4: Direct call with default (key exists) ===")
result = d.get("a", 99)
print(f"d.get('a', 99) = {result}")
assert result == 1, f"Expected 1, got {result}"

print("\n✅ All tests passed! dict.get() works perfectly!")
