# Simple type error test without exception handling

print("Creating typed variable...")
age: int = 25
print(f"age = {age}")

print("\nReassigning with correct type...")
age = 30
print(f"age = {age}")

print("\nAttempting to assign wrong type...")
age = "thirty"
print("ERROR: This line should not be reached!")
