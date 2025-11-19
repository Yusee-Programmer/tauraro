import time

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

def is_prime(n):
    if n <= 1:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True

def count_primes(n):
    count = 0
    for i in range(n):
        if is_prime(i):
            count += 1
    return count

def matrix_mul(n):
    A = [[i for i in range(n)] for _ in range(n)]
    B = [[i for i in range(n)] for _ in range(n)]
    C = [[0 for _ in range(n)] for _ in range(n)]

    for i in range(n):
        for j in range(n):
            for k in range(n):
                C[i][j] += A[i][k] * B[k][j]
    return C[0][0]

def run_benchmark():
    print("Starting Benchmark...")
    
    start = time.time()
    print(f"Fibonacci(30): {fib(30)}")
    print(f"Fibonacci Time: {time.time() - start:.4f}s")

    start = time.time()
    print(f"Primes up to 10000: {count_primes(10000)}")
    print(f"Primes Time: {time.time() - start:.4f}s")

    start = time.time()
    print(f"Matrix Mul (100x100): {matrix_mul(100)}")
    print(f"Matrix Mul Time: {time.time() - start:.4f}s")

if __name__ == "__main__":
    run_benchmark()
