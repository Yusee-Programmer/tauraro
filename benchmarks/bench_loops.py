import time

def benchmark_for_loop(iterations):
    start = time.time()
    total = 0
    for i in range(iterations):
        total = total + i
    end = time.time()
    return end - start

def benchmark_while_loop(iterations):
    start = time.time()
    total = 0
    i = 0
    while i < iterations:
        total = total + i
        i = i + 1
    end = time.time()
    return end - start

def benchmark_nested_loops(iterations):
    start = time.time()
    total = 0
    i = 0
    while i < iterations:
        j = 0
        while j < 10:
            total = total + 1
            j = j + 1
        i = i + 1
    end = time.time()
    return end - start

if __name__ == "__main__":
    iterations = 1000000

    print("=== Loop Benchmarks ===")
    print(f"Iterations: {iterations}")
    print()

    time_for = benchmark_for_loop(iterations)
    print(f"For loop: {time_for:.4f} seconds")

    time_while = benchmark_while_loop(iterations)
    print(f"While loop: {time_while:.4f} seconds")

    time_nested = benchmark_nested_loops(iterations // 100)
    print(f"Nested loops (10k outer): {time_nested:.4f} seconds")

    total = time_for + time_while + time_nested
    print(f"\nTotal time: {total:.4f} seconds")
