# Simple memory management test for C compilation (no f-strings)

print("=== Memory Management Simple C Test ===")

# Test 1: Manual memory decorator
@manual_memory
def calculate_sum(n: int) -> int:
    total: int = 0
    i: int = 0
    while i < n:
        total = total + i
        i = i + 1
    return total

print("1. Manual memory function:")
result1: int = calculate_sum(100)
print("Sum of 0-99 = ")
print(result1)

# Test 2: Arena memory decorator
@arena_memory
def calculate_factorial(n: int) -> int:
    if n <= 1:
        return 1
    result: int = 1
    i: int = 2
    while i <= n:
        result = result * i
        i = i + 1
    return result

print("2. Arena memory function:")
result2: int = calculate_factorial(10)
print("Factorial of 10 = ")
print(result2)

# Test 3: Auto memory decorator
@auto_memory
def fibonacci(n: int) -> int:
    if n <= 1:
        return n
    a: int = 0
    b: int = 1
    i: int = 2
    while i <= n:
        temp: int = a + b
        a = b
        b = temp
        i = i + 1
    return b

print("3. Auto memory function:")
result3: int = fibonacci(20)
print("Fibonacci(20) = ")
print(result3)

print("=== Tests Complete ===")
