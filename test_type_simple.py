# Simple Type Enforcement Test for Tauraro

print("========================================")
print("Simple Type Enforcement Test")
print("========================================")

# Test 1: Integer type with correct value
print("\nTest 1: Integer type with correct value")
age: int = 20
print("age: int = 20")
print(f"Success! age = {age}")

# Test 2: String type with correct value
print("\nTest 2: String type with correct value")
name: str = "Alice"
print("name: str = 'Alice'")
print(f"Success! name = {name}")

# Test 3: Float type with correct value
print("\nTest 3: Float type with correct value")
price: float = 19.99
print("price: float = 19.99")
print(f"Success! price = {price}")

# Test 4: List type with correct value
print("\nTest 4: List type with correct value")
numbers: list = [1, 2, 3]
print("numbers: list = [1, 2, 3]")
print(f"Success! numbers = {numbers}")

# Test 5: Dict type with correct value
print("\nTest 5: Dict type with correct value")
person: dict = {"name": "Bob"}
print('person: dict = {"name": "Bob"}')
print(f"Success! person = {person}")

# Test 6: Dynamic typing (no annotation)
print("\nTest 6: Dynamic typing (no type hint)")
dynamic_var = 42
print(f"dynamic_var = {dynamic_var}")
dynamic_var = "now string"
print(f"dynamic_var = {dynamic_var}")
print("Success! Dynamic typing works")

# Test 7: Reassignment with same type
print("\nTest 7: Reassignment with same type")
count: int = 10
print(f"count = {count}")
count = 20
print(f"count = {count}")
count = 30
print(f"count = {count}")
print("Success! Reassignment with same type works")

print("\n========================================")
print("All basic tests passed!")
print("========================================")

# Test 8: Type error test (integer with string value)
print("\nTest 8: Type violation - int with string (should error)")
print("Attempting: wrong_age: int = 'twenty'")
wrong_age: int = "twenty"
print("ERROR: This should have failed!")
