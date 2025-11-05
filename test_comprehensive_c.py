# Comprehensive test for C transpiler
print("=== Tauraro C Transpiler Test ===")

# Test 1: Arithmetic operations
print("\n1. Arithmetic Operations:")
a = 10
b = 3
print("a =", a, ", b =", b)
print("a + b =", a + b)
print("a - b =", a - b)
print("a * b =", a * b)
print("a / b =", a / b)

# Test 2: If statements
print("\n2. If Statements:")
x = 5
y = 10
if x < y:
    print("x < y: True")
else:
    print("x < y: False")

if x > y:
    print("x > y: True")
else:
    print("x > y: False")

# Test 3: While loops
print("\n3. While Loops:")
count = 0
while count < 3:
    print("Count:", count)
    count = count + 1

# Test 4: For loops with range
print("\n4. For Loops:")
for i in range(3):
    print("Loop", i)

# Test 5: Functions
print("\n5. Functions:")
def add(x, y):
    return x + y

def multiply(x, y):
    return x * y

result1 = add(5, 3)
result2 = multiply(4, 7)
print("add(5, 3) =", result1)
print("multiply(4, 7) =", result2)

print("\n=== All Tests Complete ===")
