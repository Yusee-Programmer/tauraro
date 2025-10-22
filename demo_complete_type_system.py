# ==========================================================================
# TAURARO COMPLETE TYPE SYSTEM DEMONSTRATION
# Shows hybrid static/dynamic typing in action
# ==========================================================================

print("=" * 75)
print("TAURARO - HYBRID STATIC/DYNAMIC TYPE SYSTEM")
print("=" * 75)

# ==========================================================================
# PART 1: DYNAMIC TYPING (Python-style flexibility)
# ==========================================================================
print("\n" + "=" * 75)
print("PART 1: DYNAMIC TYPING (No Type Annotations)")
print("=" * 75)

print("\nDynamic variables can change types freely:")
x = 42
print(f"  x = {x} (int)")

x = "now a string"
print(f"  x = '{x}' (str)")

x = [1, 2, 3]
print(f"  x = {x} (list)")

x = {"key": "value"}
print(f"  x = {x} (dict)")

print("\n✓ Dynamic typing allows complete flexibility!")

# ==========================================================================
# PART 2: STATIC TYPING (Java-style type safety)
# ==========================================================================
print("\n" + "=" * 75)
print("PART 2: STATIC TYPING (With Type Annotations)")
print("=" * 75)

print("\nStatic typed variables enforce type safety:")
age: int = 25
print(f"  age: int = {age}")

age = 30
print(f"  age = {age} (OK - still int)")

age = 35
print(f"  age = {age} (OK - still int)")

name: str = "Alice"
print(f"\n  name: str = '{name}'")

name = "Bob"
print(f"  name = '{name}' (OK - still str)")

score: float = 95.5
print(f"\n  score: float = {score}")

score = 87.3
print(f"  score = {score} (OK - still float)")

print("\n✓ Static typing prevents type mismatches!")

# ==========================================================================
# PART 3: TYPED FUNCTIONS
# ==========================================================================
print("\n" + "=" * 75)
print("PART 3: TYPED FUNCTIONS")
print("=" * 75)

def add(a: int, b: int) -> int:
    return a + b

def greet(name: str, age: int) -> str:
    return f"Hello {name}, you are {age} years old"

def calculate_area(width: int, height: int) -> int:
    return width * height

print("\nTyped functions:")
result1 = add(10, 20)
print(f"  add(10, 20) = {result1}")

result2 = greet("Charlie", 28)
print(f"  greet('Charlie', 28) = {result2}")

result3 = calculate_area(5, 8)
print(f"  calculate_area(5, 8) = {result3}")

print("\n✓ Function parameters and return types are enforced!")

# ==========================================================================
# PART 4: TYPED COLLECTIONS
# ==========================================================================
print("\n" + "=" * 75)
print("PART 4: TYPED COLLECTIONS")
print("=" * 75)

print("\nTyped collections:")
numbers: list = [1, 2, 3, 4, 5]
print(f"  numbers: list = {numbers}")

scores: dict = {"Alice": 95, "Bob": 87, "Charlie": 92}
print(f"  scores: dict = {scores}")

coordinates: tuple = (10, 20, 30)
print(f"  coordinates: tuple = {coordinates}")

print("\n✓ Collection types are supported!")

# ==========================================================================
# PART 5: HYBRID USAGE (Mix both in same file!)
# ==========================================================================
print("\n" + "=" * 75)
print("PART 5: HYBRID STATIC + DYNAMIC IN SAME FILE")
print("=" * 75)

print("\nMixing static and dynamic typing:")

static_count: int = 100
dynamic_data = 200

print(f"  static_count: int = {static_count} (typed)")
print(f"  dynamic_data = {dynamic_data} (untyped)")

static_count = 150
dynamic_data = "can be anything"

print(f"  static_count = {static_count} (still enforced as int)")
print(f"  dynamic_data = '{dynamic_data}' (type changed - OK)")

dynamic_data = [1, 2, 3, 4]
print(f"  dynamic_data = {dynamic_data} (changed again - OK)")

print("\n✓ Static and dynamic typing coexist perfectly!")

# ==========================================================================
# SUMMARY
# ==========================================================================
print("\n" + "=" * 75)
print("SUMMARY - TAURARO TYPE SYSTEM FEATURES")
print("=" * 75)

print()
print("DYNAMIC TYPING:")
print("   - No type annotations needed")
print("   - Variables can change types freely")
print("   - Maximum flexibility for scripting")
print("   - 100% Python-compatible behavior")
print()
print("STATIC TYPING:")
print("   - Optional type annotations")
print("   - Type enforcement at runtime")
print("   - Catches type errors before bad operations")
print("   - Java-like type safety")
print()
print("TYPED FUNCTIONS:")
print("   - Parameter type checking")
print("   - Return type validation")
print("   - Ensures correct API usage")
print()
print("TYPED COLLECTIONS:")
print("   - list, dict, tuple, set support")
print("   - Type-safe data structures")
print()
print("HYBRID APPROACH:")
print("   - Mix both paradigms in same file")
print("   - Use static typing for critical code")
print("   - Use dynamic typing for flexibility")
print("   - Best of both worlds!")
print()
print("TAURARO: Flexibility of Python + Safety of Java")

print("=" * 75)
