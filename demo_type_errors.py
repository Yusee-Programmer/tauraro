#!/usr/bin/env python3
"""
Demonstration of Type Error Detection in Tauraro
Shows how static typing catches errors at runtime
"""

print("=" * 70)
print("TYPE ERROR DETECTION DEMONSTRATION")
print("=" * 70)

# ============================================================================
# Example 1: Catching type mismatches in variables
# ============================================================================
print("\n[Example 1] Variable Type Mismatch")
print("-" * 70)

print("Creating typed variable: age: int = 25")
age: int = 25
print(f"✓ age = {age}")

print("\nReassigning with correct type: age = 30")
age = 30
print(f"✓ age = {age}")

print("\nAttempting to assign wrong type: age = 'thirty'")
print("Expected: TypeError")
try:
    age = "thirty"
    print("✗ ERROR: Type check should have failed!")
except Exception as e:
    print(f"✓ Caught error: {e}")

# ============================================================================
# Example 2: Function parameter type errors
# ============================================================================
print("\n\n[Example 2] Function Parameter Type Error")
print("-" * 70)

def calculate_area(width: int, height: int) -> int:
    return width * height

print("Function: calculate_area(width: int, height: int) -> int")

print("\nCorrect call: calculate_area(10, 5)")
result = calculate_area(10, 5)
print(f"✓ Result = {result}")

print("\nWrong parameter type: calculate_area('10', 5)")
print("Expected: TypeError in parameter 'width'")
try:
    bad_result = calculate_area("10", 5)
    print("✗ ERROR: Type check should have failed!")
except Exception as e:
    print(f"✓ Caught error: {e}")

# ============================================================================
# Example 3: Function return type errors
# ============================================================================
print("\n\n[Example 3] Function Return Type Error")
print("-" * 70)

def get_count() -> int:
    print("Function should return int, but returns str")
    return "five"

print("Function: get_count() -> int")
print("Calling get_count() which returns 'five' instead of 5")
print("Expected: TypeError in return value")
try:
    count = get_count()
    print("✗ ERROR: Return type check should have failed!")
except Exception as e:
    print(f"✓ Caught error: {e}")

# ============================================================================
# Example 4: Collection type errors
# ============================================================================
print("\n\n[Example 4] Collection Type Error")
print("-" * 70)

print("Creating typed list: scores: list = [95, 87, 92]")
scores: list = [95, 87, 92]
print(f"✓ scores = {scores}")

print("\nNote: Collection element type checking depends on")
print("      whether the List[T] generic syntax is used")

# ============================================================================
# Example 5: Dynamic typing (no errors)
# ============================================================================
print("\n\n[Example 5] Dynamic Typing (No Type Annotations)")
print("-" * 70)

print("Creating dynamic variable: value = 42")
value = 42
print(f"✓ value = {value}")

print("\nChanging to string: value = 'hello'")
value = "hello"
print(f"✓ value = '{value}' (No error - dynamic typing)")

print("\nChanging to list: value = [1, 2, 3]")
value = [1, 2, 3]
print(f"✓ value = {value} (No error - dynamic typing)")

print("\n✓ Dynamic variables can change types freely!")

# ============================================================================
# Summary
# ============================================================================
print("\n" + "=" * 70)
print("SUMMARY")
print("=" * 70)
print("""
✅ Static Typed Variables:
   • Type is enforced after declaration
   • Reassignment with wrong type raises TypeError
   • Catches bugs early

✅ Static Typed Functions:
   • Parameters are type-checked
   • Return values are type-checked
   • Ensures correct API usage

✅ Dynamic Typed Variables:
   • No type enforcement
   • Can change types freely
   • Maximum flexibility

Choose static typing for safety, dynamic typing for flexibility!
""")
print("=" * 70)
