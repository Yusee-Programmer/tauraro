import time

def simple_function(x):
    return x + 1

def benchmark_function_calls(iterations):
    start = time.time()
    result = 0
    i = 0
    while i < iterations:
        result = simple_function(i)
        i = i + 1
    end = time.time()
    return end - start

def recursive_fibonacci(n):
    if n <= 1:
        return n
    return recursive_fibonacci(n - 1) + recursive_fibonacci(n - 2)

def iterative_fibonacci(n):
    if n <= 1:
        return n
    a = 0
    b = 1
    i = 2
    while i <= n:
        temp = a + b
        a = b
        b = temp
        i = i + 1
    return b

def benchmark_recursion(n):
    start = time.time()
    result = recursive_fibonacci(n)
    end = time.time()
    return end - start

def benchmark_iterative(iterations):
    start = time.time()
    i = 0
    while i < iterations:
        result = iterative_fibonacci(20)
        i = i + 1
    end = time.time()
    return end - start

if __name__ == "__main__":
    print("=== Function Benchmarks ===")
    print()

    time_calls = benchmark_function_calls(1000000)
    print(f"Function calls (1M): {time_calls:.4f} seconds")

    time_recursive = benchmark_recursion(25)
    print(f"Recursive fibonacci(25): {time_recursive:.4f} seconds")

    time_iterative = benchmark_iterative(10000)
    print(f"Iterative fibonacci (10k calls): {time_iterative:.4f} seconds")

    total = time_calls + time_recursive + time_iterative
    print(f"\nTotal time: {total:.4f} seconds")
