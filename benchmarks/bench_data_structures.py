import time

def benchmark_list_operations(iterations):
    start = time.time()
    lst = []
    i = 0
    while i < iterations:
        lst.append(i)
        i = i + 1

    total = 0
    i = 0
    while i < len(lst):
        total = total + lst[i]
        i = i + 1

    end = time.time()
    return end - start

def benchmark_dict_operations(iterations):
    start = time.time()
    d = {}
    i = 0
    while i < iterations:
        d[str(i)] = i
        i = i + 1

    total = 0
    i = 0
    while i < iterations:
        total = total + d[str(i)]
        i = i + 1

    end = time.time()
    return end - start

def benchmark_list_comprehension(iterations):
    start = time.time()
    lst = [i for i in range(iterations)]
    total = 0
    for x in lst:
        total = total + x
    end = time.time()
    return end - start

if __name__ == "__main__":
    iterations = 100000

    print("=== Data Structure Benchmarks ===")
    print(f"Iterations: {iterations}")
    print()

    time_list = benchmark_list_operations(iterations)
    print(f"List operations: {time_list:.4f} seconds")

    time_dict = benchmark_dict_operations(iterations)
    print(f"Dict operations: {time_dict:.4f} seconds")

    time_comp = benchmark_list_comprehension(iterations)
    print(f"List comprehension: {time_comp:.4f} seconds")

    total = time_list + time_dict + time_comp
    print(f"\nTotal time: {total:.4f} seconds")
