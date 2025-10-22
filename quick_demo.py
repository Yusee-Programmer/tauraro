# Quick Type System Demo
print("Tauraro Type System Quick Demo")
print("=" * 40)

# Static typing
print("\n1. Static Typing:")
age: int = 25
print(f"  age: int = {age}")
age = 30
print(f"  age = {age} (OK)")

# Dynamic typing
print("\n2. Dynamic Typing:")
value = 42
print(f"  value = {value}")
value = "changed"
print(f"  value = '{value}' (OK)")

# Function types
print("\n3. Typed Function:")
def add(a: int, b: int) -> int:
    return a + b

result = add(5, 3)
print(f"  add(5, 3) = {result}")

# Collections
print("\n4. Typed Collections:")
numbers: list = [1, 2, 3]
print(f"  numbers: list = {numbers}")

person: dict = {"name": "Alice"}
print(f"  person: dict = {person}")

print("\n✓ Hybrid typing works!")
print("=" * 40)
