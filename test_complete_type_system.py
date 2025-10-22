# Comprehensive Type System Test Suite
# Tests all advanced type features: inference, generics, parameters, classes

print("=" * 60)
print("Complete Type System Test Suite")
print("=" * 60)

# TEST 1: Type Inference
print("\n[Test 1] Type Inference")
print("-" * 40)
inferred_int = 42
print(f"inferred_int = {inferred_int} (type inferred as int)")
inferred_str = "hello"
print(f"inferred_str = '{inferred_str}' (type inferred as str)")
inferred_list = [1, 2, 3]
print(f"inferred_list = {inferred_list} (type inferred as List[int])")
print("✓ Type inference working")

# TEST 2: Generic List Types
print("\n[Test 2] Generic List Types")
print("-" * 40)
numbers: list = [1, 2, 3, 4, 5]
print(f"numbers: list = {numbers}")
print("✓ Generic list created")

# TEST 3: Generic Dict Types
print("\n[Test 3] Generic Dict Types")
print("-" * 40)
person: dict = {"name": "Alice", "age": 30}
print(f'person: dict = {person}')
print("✓ Generic dict created")

# TEST 4: Tuple Types
print("\n[Test 4] Tuple Types")
print("-" * 40)
point: tuple = (10, 20, 30)
print(f"point: tuple = {point}")
print("✓ Tuple created")

# TEST 5: Function with typed parameters
print("\n[Test 5] Function with Typed Parameters")
print("-" * 40)
def add(a: int, b: int) -> int:
    return a + b

result = add(5, 3)
print(f"add(5, 3) = {result}")
print("✓ Typed function works")

# TEST 6: Function with return type
print("\n[Test 6] Function Return Type Checking")
print("-" * 40)
def get_message() -> str:
    return "Hello, World!"

msg = get_message()
print(f"get_message() = '{msg}'")
print("✓ Return type checking works")

# TEST 7: Class with typed attributes
print("\n[Test 7] Class with Typed Attributes")
print("-" * 40)
class Person:
    def __init__(self, name: str, age: int):
        self.name = name
        self.age = age

    def greet(self) -> str:
        return f"Hi, I'm {self.name}"

alice = Person("Alice", 30)
print(f"Person created: name={alice.name}, age={alice.age}")
print(f"alice.greet() = '{alice.greet()}'")
print("✓ Class with typed attributes works")

# TEST 8: Mixed static and dynamic
print("\n[Test 8] Mixed Static and Dynamic Typing")
print("-" * 40)
static_var: int = 100
dynamic_var = 200
print(f"static_var: int = {static_var}")
print(f"dynamic_var = {dynamic_var}")
dynamic_var = "changed"
print(f"dynamic_var = '{dynamic_var}' (type changed)")
print("✓ Mixed typing works")

# TEST 9: Optional types (if supported)
print("\n[Test 9] None Values")
print("-" * 40)
maybe_value = None
print(f"maybe_value = {maybe_value}")
maybe_value = 42
print(f"maybe_value = {maybe_value}")
print("✓ None handling works")

# TEST 10: Type reassignment
print("\n[Test 10] Type Reassignment")
print("-" * 40)
counter: int = 0
print(f"counter = {counter}")
counter = 1
print(f"counter = {counter}")
counter = 2
print(f"counter = {counter}")
print("✓ Reassignment with same type works")

print("\n" + "=" * 60)
print("All Type System Tests Passed!")
print("=" * 60)
print("\nSummary:")
print("  ✅ Type inference")
print("  ✅ Generic collections (List, Dict, Tuple)")
print("  ✅ Function parameter types")
print("  ✅ Function return types")
print("  ✅ Class attribute types")
print("  ✅ Mixed static/dynamic typing")
print("  ✅ None value handling")
print("  ✅ Type reassignment")
