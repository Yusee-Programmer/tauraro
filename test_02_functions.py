# Test 2: Functions and Parameters
print("=== Test 2: Functions ===")

# Simple function
def greet():
    print("Hello from function!")

greet()

# Function with parameters
def add(a, b):
    return a + b

result = add(5, 3)
print("add(5, 3) =", result)

# Function with multiple operations
def calculate(x, y):
    sum_val = x + y
    diff = x - y
    prod = x * y
    print("Sum:", sum_val)
    print("Difference:", diff)
    print("Product:", prod)
    return sum_val

total = calculate(10, 4)
print("Total:", total)

print("\n=== Test 2 Complete ===")
