# Test lambda expressions
print("Testing lambda expressions...")

# Test 1: Simple lambda
add = lambda x, y: x + y
print(f"lambda x, y: x + y -> add(3, 5) = {add(3, 5)}")

# Test 2: Lambda with subscript (like in dashboard)
get_key = lambda x: x["name"]
data = {"name": "Alice", "age": 30}
print(f"lambda x: x['name'] -> {get_key(data)}")

# Test 3: Lambda in sorted() with key parameter
items = [{"val": 3}, {"val": 1}, {"val": 2}]
sorted_items = sorted(items, key=lambda x: x["val"])
print(f"Sorted by val: {sorted_items}")

# Test 4: Lambda with reverse=True
sorted_desc = sorted(items, key=lambda x: x["val"], reverse=True)
print(f"Sorted descending: {sorted_desc}")

print("\nAll lambda tests passed!")
