# Test nested literals fix
# This tests that nested structures like {"items": ["a", "b", "c"]}
# are properly converted to nested Value objects instead of strings

# Test 1: Simple nested dict with list
data = {"items": ["a", "b", "c"]}
print("Test 1 - Dict with list:")
print(data)
print(data["items"])
print(data["items"][0])

# Test 2: Nested dicts
config = {"server": {"host": "localhost", "port": 8080}}
print("\nTest 2 - Nested dicts:")
print(config)
print(config["server"])
print(config["server"]["host"])

# Test 3: Complex nested structure
complex_data = {
    "users": [
        {"name": "Alice", "age": 30},
        {"name": "Bob", "age": 25}
    ],
    "settings": {
        "theme": "dark",
        "notifications": True
    }
}
print("\nTest 3 - Complex nested structure:")
print(complex_data)
print(complex_data["users"])
print(complex_data["users"][0])
print(complex_data["users"][0]["name"])

# Test 4: List of lists
matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
print("\nTest 4 - List of lists:")
print(matrix)
print(matrix[0])
print(matrix[1][1])

print("\nAll tests completed!")
