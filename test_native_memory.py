# Test native memory management that compiles cleanly to C
# This version shows manual memory management concepts without FFI/runtime dependencies

print("=== Native Memory Management Test ===")

def calculate_sum(n: int) -> int:
    total: int = 0
    for i in range(n):
        total = total + i
    return total

def calculate_factorial(n: int) -> int:
    if n <= 1:
        return 1
    result: int = 1
    for i in range(2, n + 1):
        result = result * i
    return result

def sum_of_squares(n: int) -> float:
    total: float = 0.0
    for i in range(n):
        x: float = float(i)
        square: float = x * x
        total = total + square
    return total

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

def is_prime(n: int) -> bool:
    if n <= 1:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    i: int = 3
    while i * i <= n:
        if n % i == 0:
            return False
        i = i + 2
    return True

# Test calculations
print("\n1. Basic calculations:")
sum_result: int = calculate_sum(100)
print(f"Sum of 0-99 = {sum_result}")

fact_result: int = calculate_factorial(10)
print(f"Factorial of 10 = {fact_result}")

squares_result: float = sum_of_squares(50)
print(f"Sum of squares 0-49 = {squares_result}")

print("\n2. Fibonacci numbers:")
for n in [10, 15, 20, 25]:
    fib: int = fibonacci(n)
    print(f"fib({n}) = {fib}")

print("\n3. Testing prime check:")
print(f"2 is prime: {is_prime(2)}")
print(f"3 is prime: {is_prime(3)}")
print(f"4 is prime: {is_prime(4)}")
print(f"17 is prime: {is_prime(17)}")
print(f"20 is prime: {is_prime(20)}")

print("\n4. Performance test - large calculations:")
large_sum: int = calculate_sum(10000)
print(f"Sum of 0-9999 = {large_sum}")

large_squares: float = sum_of_squares(1000)
print(f"Sum of squares 0-999 = {large_squares}")

print("\n=== All Tests Completed ===")
