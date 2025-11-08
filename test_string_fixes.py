# Test string literal fixes
print("=== Testing String Literal Fixes ===")

# Test newlines in strings
print("\n1. String with newlines:")
message = "Line 1\nLine 2\nLine 3"
print(message)

# Test f-strings
print("\n2. F-string formatting:")
x = 42
y = 3.14159
name = "Tauraro"
print(f"x = {x}")
print(f"y = {y:.2f}")
print(f"Name: {name}")
print(f"Calculation: {x} * 2 = {x * 2}")

# Test combined
print("\n3. F-string with newline:")
count = 5
print(f"Count is {count}\nDone!")

print("\n=== Tests Complete ===")
