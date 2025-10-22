import time

def benchmark_integer_arithmetic(iterations):
    start = time.time()
    result = 0
    i = 0
    while i < iterations:
        result = result + i
        result = result - 1
        result = result * 2
        result = result // 3
        i = i + 1
    end = time.time()
    return end - start

def benchmark_float_arithmetic(iterations):
    start = time.time()
    result = 0.0
    i = 0
    while i < iterations:
        result = result + float(i)
        result = result - 1.5
        result = result * 2.0
        result = result / 3.0
        i = i + 1
    end = time.time()
    return end - start

def benchmark_mixed_operations(iterations):
    start = time.time()
    result = 0
    i = 0
    while i < iterations:
        result = (i + 5) * 3 - 2
        result = result % 100
        result = result ** 2
        result = result // 10
        i = i + 1
    end = time.time()
    return end - start

if __name__ == "__main__":
    iterations = 1000000

    print("=== Arithmetic Benchmarks ===")
    print(f"Iterations: {iterations}")
    print()

    time_int = benchmark_integer_arithmetic(iterations)
    print(f"Integer arithmetic: {time_int:.4f} seconds")

    time_float = benchmark_float_arithmetic(iterations)
    print(f"Float arithmetic: {time_float:.4f} seconds")

    time_mixed = benchmark_mixed_operations(iterations)
    print(f"Mixed operations: {time_mixed:.4f} seconds")

    total = time_int + time_float + time_mixed
    print(f"\nTotal time: {total:.4f} seconds")
