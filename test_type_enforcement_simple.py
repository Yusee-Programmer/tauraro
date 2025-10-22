# Type Enforcement Demonstration

print("=" * 70)
print("TYPE ENFORCEMENT DEMONSTRATION")
print("=" * 70)

# Example 1: Static typing with correct type
print("\n[Example 1] Static Typing - Correct Usage")
print("-" * 70)
age: int = 25
print(f"Created: age: int = {age}")
age = 30
print(f"Reassigned: age = {age} (OK - same type)")

# Example 2: Dynamic typing (no type annotation)
print("\n[Example 2] Dynamic Typing - Flexible")
print("-" * 70)
value = 42
print(f"Created: value = {value}")
value = "changed to string"
print(f"Reassigned: value = '{value}' (OK - no type constraint)")
value = [1, 2, 3]
print(f"Reassigned: value = {value} (OK - no type constraint)")

# Example 3: Function with type annotations
print("\n[Example 3] Typed Functions")
print("-" * 70)

def greet(name: str, age: int) -> str:
    return f"Hello {name}, you are {age} years old"

result = greet("Alice", 30)
print(f"greet('Alice', 30) = {result}")

# Example 4: Typed collections
print("\n[Example 4] Typed Collections")
print("-" * 70)
numbers: list = [1, 2, 3, 4, 5]
print(f"numbers: list = {numbers}")

scores: dict = {"Alice": 95, "Bob": 87}
print(f"scores: dict = {scores}")

# Example 5: Hybrid usage in same file
print("\n[Example 5] Hybrid Static/Dynamic in Same File")
print("-" * 70)
static_var: int = 100
dynamic_var = 200

print(f"static_var: int = {static_var}")
print(f"dynamic_var = {dynamic_var}")

static_var = 150
dynamic_var = "can change to string"

print(f"static_var = {static_var} (OK - still int)")
print(f"dynamic_var = '{dynamic_var}' (OK - no type constraint)")

print("\n" + "=" * 70)
print("✓ Tauraro supports both static and dynamic typing!")
print("=" * 70)
