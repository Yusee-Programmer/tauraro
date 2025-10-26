# Test 09: All Operators
# Tests arithmetic, comparison, logical, bitwise operators

print("=== Testing All Operators ===")

# Arithmetic operators
print("\n--- Arithmetic Operators ---")
a = 10
b = 3
print("a = 10, b = 3")
print("Addition (a + b):", a + b)
print("Subtraction (a - b):", a - b)
print("Multiplication (a * b):", a * b)
print("Division (a / b):", a / b)
print("Modulo (a % b):", a % b)
print("Floor Division (a // b):", a // b)
print("Exponentiation (a ** b):", a ** b)

# Comparison operators
print("\n--- Comparison Operators ---")
x = 5
y = 10
print("x = 5, y = 10")
print("Equal (x == y):", x == y)
print("Not Equal (x != y):", x != y)
print("Less Than (x < y):", x < y)
print("Less or Equal (x <= y):", x <= y)
print("Greater Than (x > y):", x > y)
print("Greater or Equal (x >= y):", x >= y)

# Logical operators
print("\n--- Logical Operators ---")
t = True
f = False
print("t = True, f = False")
print("AND (t and f):", t and f)
print("OR (t or f):", t or f)
print("NOT (!t):", not t)
print("NOT (!f):", not f)

# Bitwise operators
print("\n--- Bitwise Operators ---")
p = 12  # 1100 in binary
q = 10  # 1010 in binary
print("p = 12, q = 10")
print("Bitwise AND (p & q):", p & q)
print("Bitwise OR (p | q):", p | q)
print("Bitwise XOR (p ^ q):", p ^ q)
print("Bitwise NOT (~p):", ~p)
print("Left Shift (p << 1):", p << 1)
print("Right Shift (p >> 1):", p >> 1)

# Assignment operators
print("\n--- Assignment Operators ---")
num = 5
print("Initial num:", num)
num += 3
print("After num += 3:", num)
num -= 2
print("After num -= 2:", num)
num *= 4
print("After num *= 4:", num)
num /= 2
print("After num /= 2:", num)

# Unary operators
print("\n--- Unary Operators ---")
pos = 5
neg = -pos
print("Positive:", pos)
print("Negative:", neg)
print("Negation of negative:", -neg)

# String operators
print("\n--- String Operators ---")
str1 = "Hello"
str2 = "World"
print("Concatenation:", str1 + " " + str2)
print("Repetition:", str1 * 3)

print("\n=== All Operators Tests Passed ===")
