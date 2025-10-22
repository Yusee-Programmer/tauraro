# Debug test to see if type checking instructions are being emitted

print("Debug Test: Type Checking Instructions")
print("=" * 40)

# This should emit RegisterType and CheckType instructions
x: int = 42
print(f"x = {x}")

# Try to assign wrong type
print("Assigning string to int variable...")
x = "wrong type"
print(f"x = {x}")
print("If you see this, type checking is not working!")
