# Test manual memory management with math operations
# This compiles cleanly to C without FFI dependencies

print("=== Manual Memory Management with Math ===")

# Simple function with automatic memory (default)
def calculate_sum(n: int) -> int:
    total = 0
    for i in range(n):
        total = total + i
    return total

print("\n1. Automatic memory management:")
result = calculate_sum(10)
print(f"Sum of 0-9 = {result}")

# Function using manual memory management (via decorator in C compilation)
def process_array() -> float:
    # Allocate buffer manually
    buffer = allocate(1024)
    print("Allocated 1KB buffer")

    # Perform calculations
    total = 0.0
    for i in range(100):
        x = float(i)
        # Use native math operations
        square = x * x
        total = total + square

    # Free buffer
    free(buffer)
    print("Buffer freed")

    return total

print("\n2. Manual memory management:")
sum_of_squares = process_array()
print(f"Sum of squares 0-99 = {sum_of_squares}")

# Arena memory management
def batch_process() -> int:
    create_arena("batch_arena")
    print("Arena created")

    # Allocate multiple buffers
    buf1 = allocate(256)
    buf2 = allocate(512)
    buf3 = allocate(1024)
    print("Allocated 3 buffers")

    # Perform calculations
    count = 0
    for i in range(50):
        if i % 2 == 0:
            count = count + 1

    # Destroy arena (frees all)
    destroy_arena("batch_arena")
    print("Arena destroyed")

    return count

print("\n3. Arena memory management:")
even_count = batch_process()
print(f"Even numbers count = {even_count}")

# Performance-critical calculation
def fibonacci(n: int) -> int:
    # Manual memory for performance
    buffer = allocate(2048)

    if n <= 1:
        free(buffer)
        return n

    a = 0
    b = 1
    for i in range(2, n + 1):
        temp = a + b
        a = b
        b = temp

    free(buffer)
    return b

print("\n4. Fibonacci with manual memory:")
for n in [10, 15, 20]:
    fib = fibonacci(n)
    print(f"fib({n}) = {fib}")

# Memory statistics
print("\n5. Memory statistics:")
stats = memory_stats()
print(stats)

print("\n=== Test Completed ===")
