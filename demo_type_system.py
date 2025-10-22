#!/usr/bin/env python3
"""
Complete Demonstration of Tauraro's Hybrid Type System
Shows both Static (enforced) and Dynamic (flexible) typing
"""

print("=" * 70)
print("TAURARO HYBRID TYPE SYSTEM DEMONSTRATION")
print("Static Typing + Dynamic Typing in the Same Language!")
print("=" * 70)

# ============================================================================
# PART 1: DYNAMIC TYPING (Python-style flexibility)
# ============================================================================
print("\n" + "=" * 70)
print("PART 1: DYNAMIC TYPING (No Type Annotations)")
print("=" * 70)

print("\n--- Dynamic Variables (can change types) ---")
x = 10
print(f"x = {x}  (type: int)")

x = "hello"
print(f"x = '{x}'  (type: str)")

x = [1, 2, 3]
print(f"x = {x}  (type: list)")

x = {"name": "Alice"}
print(f"x = {x}  (type: dict)")

print("✓ Dynamic typing: Variable can hold any type!")

# ============================================================================
# PART 2: STATIC TYPING (Java-style type safety)
# ============================================================================
print("\n" + "=" * 70)
print("PART 2: STATIC TYPING (With Type Annotations)")
print("=" * 70)

print("\n--- Static Variables (type is enforced) ---")
age: int = 25
print(f"age: int = {age}")

name: str = "Bob"
print(f"name: str = '{name}'")

price: float = 19.99
print(f"price: float = {price}")

is_active: bool = True
print(f"is_active: bool = {is_active}")

print("✓ Static typing: Type is declared and enforced!")

# ============================================================================
# PART 3: TYPE SAFETY IN ACTION
# ============================================================================
print("\n" + "=" * 70)
print("PART 3: TYPE SAFETY - Reassignment Rules")
print("=" * 70)

print("\n--- Static Variable Reassignment (SAFE) ---")
counter: int = 0
print(f"counter: int = {counter}")

counter = 1
print(f"counter = {counter}  ✓ OK (int)")

counter = 2
print(f"counter = {counter}  ✓ OK (int)")

counter = 100
print(f"counter = {counter}  ✓ OK (int)")

print("\n--- Dynamic Variable Reassignment (FLEXIBLE) ---")
flexible = 42
print(f"flexible = {flexible}  (initially int)")

flexible = "changed to string"
print(f"flexible = '{flexible}'  ✓ OK (dynamic)")

flexible = [1, 2, 3]
print(f"flexible = {flexible}  ✓ OK (dynamic)")

# ============================================================================
# PART 4: COLLECTIONS WITH TYPE SAFETY
# ============================================================================
print("\n" + "=" * 70)
print("PART 4: TYPED COLLECTIONS")
print("=" * 70)

print("\n--- Typed List ---")
numbers: list = [1, 2, 3, 4, 5]
print(f"numbers: list = {numbers}")

print("\n--- Typed Dictionary ---")
student: dict = {"name": "Charlie", "grade": 95, "active": True}
print(f"student: dict = {student}")

print("\n--- Typed Tuple ---")
coordinates: tuple = (10, 20, 30)
print(f"coordinates: tuple = {coordinates}")

# ============================================================================
# PART 5: FUNCTIONS WITH TYPE SAFETY
# ============================================================================
print("\n" + "=" * 70)
print("PART 5: TYPED FUNCTIONS")
print("=" * 70)

print("\n--- Function with Parameter Types ---")
def greet(name: str, age: int) -> str:
    return f"Hello {name}, you are {age} years old"

result = greet("Alice", 30)
print(f"greet('Alice', 30) = '{result}'")
print("✓ Parameters type-checked at runtime!")

print("\n--- Function with Return Type ---")
def calculate_total(price: float, tax: float) -> float:
    return price + (price * tax)

total = calculate_total(100.0, 0.08)
print(f"calculate_total(100.0, 0.08) = {total}")
print("✓ Return type checked at runtime!")

print("\n--- Dynamic Function (No Types) ---")
def flexible_function(a, b):
    return a + b

r1 = flexible_function(5, 3)
print(f"flexible_function(5, 3) = {r1}")

r2 = flexible_function("Hello ", "World")
print(f"flexible_function('Hello ', 'World') = '{r2}'")

r3 = flexible_function([1, 2], [3, 4])
print(f"flexible_function([1, 2], [3, 4]) = {r3}")
print("✓ Dynamic function works with any types!")

# ============================================================================
# PART 6: CLASSES WITH TYPE SAFETY
# ============================================================================
print("\n" + "=" * 70)
print("PART 6: TYPED CLASSES")
print("=" * 70)

print("\n--- Class with Typed Attributes ---")
class Person:
    def __init__(self, name: str, age: int, city: str):
        self.name = name
        self.age = age
        self.city = city

    def introduce(self) -> str:
        return f"I'm {self.name}, {self.age} years old, from {self.city}"

    def birthday(self) -> int:
        self.age = self.age + 1
        return self.age

alice = Person("Alice", 28, "New York")
print(f"Person: {alice.introduce()}")

new_age = alice.birthday()
print(f"After birthday: age = {new_age}")
print("✓ Class methods with type safety!")

# ============================================================================
# PART 7: MIXED STATIC AND DYNAMIC
# ============================================================================
print("\n" + "=" * 70)
print("PART 7: MIXING STATIC AND DYNAMIC IN SAME CODE")
print("=" * 70)

print("\n--- Static + Dynamic Variables Together ---")
typed_var: int = 100
untyped_var = 200

print(f"typed_var: int = {typed_var}  (static)")
print(f"untyped_var = {untyped_var}  (dynamic)")

# Dynamic can change
untyped_var = "I can change!"
print(f"untyped_var = '{untyped_var}'  ✓ OK (dynamic)")

# Static stays enforced
typed_var = 150
print(f"typed_var = {typed_var}  ✓ OK (same type)")

print("\n✓ Both paradigms coexist perfectly!")

# ============================================================================
# PART 8: TYPE INFERENCE
# ============================================================================
print("\n" + "=" * 70)
print("PART 8: AUTOMATIC TYPE INFERENCE")
print("=" * 70)

print("\n--- Type Inferred from First Assignment ---")
inferred_int = 42
print(f"inferred_int = {inferred_int}  (inferred as int)")

inferred_str = "hello world"
print(f"inferred_str = '{inferred_str}'  (inferred as str)")

inferred_list = [1, 2, 3, 4, 5]
print(f"inferred_list = {inferred_list}  (inferred as List[int])")

inferred_dict = {"key": "value"}
print(f"inferred_dict = {inferred_dict}  (inferred as Dict[str, str])")

print("\n✓ Types automatically inferred when not specified!")

# ============================================================================
# PART 9: SUMMARY
# ============================================================================
print("\n" + "=" * 70)
print("SUMMARY: TAURARO'S UNIQUE TYPE SYSTEM")
print("=" * 70)

print("\n✅ STATIC TYPING Benefits:")
print("   • Early error detection")
print("   • Type safety like Java/C++")
print("   • Self-documenting code")
print("   • Performance optimization potential")

print("\n✅ DYNAMIC TYPING Benefits:")
print("   • Flexibility like Python/JavaScript")
print("   • Rapid prototyping")
print("   • Duck typing when needed")
print("   • No annotation overhead")

print("\n🎯 BEST OF BOTH WORLDS:")
print("   • Use static typing for critical code")
print("   • Use dynamic typing for quick scripts")
print("   • Mix them in the same file/project")
print("   • Type inference reduces boilerplate")

print("\n" + "=" * 70)
print("This is what makes Tauraro unique!")
print("=" * 70)
