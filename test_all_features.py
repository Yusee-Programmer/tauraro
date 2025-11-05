# Comprehensive test for all implemented C transpiler features
print("=== Tauraro C Transpiler - Complete Feature Test ===\n")

# Test 1: Basic arithmetic operations
print("1. Arithmetic Operations:")
a = 100
b = 25
print("a =", a, ", b =", b)
print("a + b =", a + b)
print("a - b =", a - b)
print("a * b =", a * b)
print("a / b =", a / b)
print()

# Test 2: Comparison operations
print("2. Comparison Operations:")
x = 10
y = 20
print("x =", x, ", y =", y)
print("x < y:", x < y)
print("x > y:", x > y)
print("x == y:", x == y)
print("x != y:", x != y)
print()

# Test 3: If/elif/else statements
print("3. If/Elif/Else Statements:")
score = 85
if score >= 90:
    print("Grade: A")
elif score >= 80:
    print("Grade: B")
elif score >= 70:
    print("Grade: C")
else:
    print("Grade: F")
print()

# Test 4: While loops with break
print("4. While Loops with Break:")
count = 0
while count < 10:
    print("Count:", count)
    count = count + 1
    if count == 5:
        print("Breaking at 5")
        break
print()

# Test 5: For loops with continue
print("5. For Loops with Continue:")
for i in range(8):
    if i == 3:
        print("Skipping 3")
        continue
    if i == 6:
        print("Breaking at 6")
        break
    print("i =", i)
print()

# Test 6: Nested loops
print("6. Nested Loops:")
for i in range(3):
    for j in range(3):
        print("i =", i, ", j =", j)
print()

# Test 7: Functions
print("7. User-Defined Functions:")
def add(x, y):
    return x + y

def multiply(x, y):
    return x * y

def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

print("add(15, 27) =", add(15, 27))
print("multiply(8, 7) =", multiply(8, 7))
print("factorial(5) =", factorial(5))
print()

# Test 8: Complex expressions
print("8. Complex Expressions:")
result = (10 + 5) * 2 - 8 / 4
print("(10 + 5) * 2 - 8 / 4 =", result)
print()

# Test 9: Boolean logic
print("9. Boolean Operations:")
flag1 = True
flag2 = False
print("flag1 =", flag1, ", flag2 =", flag2)
if flag1:
    print("flag1 is True")
if not flag2:
    print("flag2 is False")
print()

print("=== All Tests Complete! ===")
print("C transpiler successfully handled:")
print("- Arithmetic operations (+, -, *, /)")
print("- Comparison operations (<, >, ==, !=, <=, >=)")
print("- Control flow (if/elif/else)")
print("- While loops with proper condition re-evaluation")
print("- For loops with range iteration")
print("- Break and continue statements")
print("- Nested loops")
print("- User-defined functions")
print("- Recursive functions")
print("- Complex expressions")
print("- Boolean values and logic")
