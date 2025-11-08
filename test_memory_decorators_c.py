# Test memory management decorators for C compilation
# This tests that decorators compile to C even though they're no-ops in C

print("=== Memory Management Decorators C Test ===")

# Test 1: Manual memory decorator
@manual_memory
def calculate_sum(n: int) -> int:
    total: int = 0
    for i in range(n):
        total = total + i
    return total

print("\n1. Manual memory function:")
result1: int = calculate_sum(100)
print(f"Sum of 0-99 = {result1}")

# Test 2: Arena memory decorator
@arena_memory
def calculate_factorial(n: int) -> int:
    if n <= 1:
        return 1
    result: int = 1
    for i in range(2, n + 1):
        result = result * i
    return result

print("\n2. Arena memory function:")
result2: int = calculate_factorial(10)
print(f"Factorial of 10 = {result2}")

# Test 3: Auto memory decorator (default)
@auto_memory
def fibonacci(n: int) -> int:
    if n <= 1:
        return n
    a: int = 0
    b: int = 1
    for i in range(2, n + 1):
        temp: int = a + b
        a = b
        b = temp
    return b

print("\n3. Auto memory function:")
result3: int = fibonacci(20)
print(f"Fibonacci(20) = {result3}")

# Test 4: Mixed - using decorated functions together
print("\n4. Using all decorated functions together:")
sum_val: int = calculate_sum(50)
fact_val: int = calculate_factorial(5)
fib_val: int = fibonacci(10)
print(f"Sum: {sum_val}, Factorial: {fact_val}, Fibonacci: {fib_val}")

print("\n=== Tests Complete ===")
