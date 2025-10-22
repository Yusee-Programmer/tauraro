# ==========================================================================
# TYPE ERROR DETECTION DEMONSTRATION
# Shows how Tauraro catches type mismatches at runtime
# ==========================================================================

print("=" * 75)
print("TYPE ERROR DETECTION - Tauraro Type Safety Demo")
print("=" * 75)

print("\nThis script will intentionally cause a type error to demonstrate")
print("that Tauraro enforces type safety at runtime.\n")

print("-" * 75)
print("Creating a statically typed variable:")
print("-" * 75)

age: int = 25
print(f"  age: int = {age}  [SUCCESS]")

print("\nReassigning with correct type (int):")
age = 30
print(f"  age = {age}  [SUCCESS]")

print("\nAttempting to reassign with WRONG type (str):")
print("  age = 'thirty'  [This should fail...]")
print()
print("Expected error: TypeError - 'thirty' is not an int")
print("-" * 75)

age = "thirty"

print("\nERROR: This line should never execute!")
print("If you see this, type checking failed!")
