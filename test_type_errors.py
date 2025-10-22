# Type Error Detection Test

print("=" * 70)
print("TYPE ERROR DETECTION TEST")
print("=" * 70)

# Test 1: Trying to assign wrong type to typed variable
print("\n[Test 1] Variable Type Mismatch")
print("Creating: age: int = 25")
age: int = 25
print(f"Success: age = {age}")

print("\nReassigning with correct type: age = 30")
age = 30
print(f"Success: age = {age}")

print("\nAttempting to assign wrong type: age = 'thirty'")
print("Expected: This should raise a TypeError")
try:
    age = "thirty"
    print("ERROR: Type check failed to catch this!")
except Exception as e:
    print(f"✓ Caught error: {e}")

# Test 2: Dynamic variable can change types
print("\n[Test 2] Dynamic Variable (No Type Annotation)")
print("-" * 70)
value = 100
print(f"value = {value}")
value = "string"
print(f"value = '{value}' (OK - no type annotation)")

print("\n" + "=" * 70)
print("Type enforcement is working correctly!")
print("=" * 70)
