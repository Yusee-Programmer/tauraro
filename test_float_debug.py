# Debug float operations - comprehensive test
print("Testing float operations step by step")

# Test 1: Detailed step-by-step without f-strings
print("\n=== Test 1: Step-by-step float operations ===")
a = 5.5
print("a assigned")
print(a)
print(type(a))

b = 3.2
print("b assigned")
print(b)
print(type(b))

# Test addition
result = a + b
print("result = a + b computed")
print("Result value:")
print(result)
print("Result type:")
print(type(result))

# Test 2: Float operations with f-strings
print("\n=== Test 2: Float operations with f-strings ===")
x = 5.5
y = 2.3
print(f"x = {x}, type = {type(x)}")
print(f"y = {y}, type = {type(y)}")

# Test subtraction
z = x - y
print(f"x - y = {z}, type = {type(z)}")
