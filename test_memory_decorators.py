# Test memory management decorators
# Tests @manual_memory, @arena_memory, and @auto_memory decorators

print("=== Testing Memory Management Decorators ===")

# Test 1: Default automatic memory management
def auto_function():
    x = 42
    y = x * 2
    return y

result = auto_function()
print(f"1. Auto function result: {result}")

# Test 2: Manual memory management decorator
@manual_memory
def manual_function():
    # Allocate manually
    buffer = allocate(1024)
    print("Manual: Allocated buffer")

    # Must free explicitly
    free(buffer)
    print("Manual: Freed buffer")

    return True

manual_function()
print("2. Manual memory function completed")

# Test 3: Arena memory management decorator
@arena_memory
def arena_function():
    # Create arena
    create_arena("func_arena")
    print("Arena: Created arena")

    # Allocations use arena
    buf1 = allocate(512)
    buf2 = allocate(256)
    print(f"Arena: Allocated buffers")

    # Arena is destroyed at end
    destroy_arena("func_arena")
    print("Arena: Destroyed arena")

    return True

arena_function()
print("3. Arena memory function completed")

# Test 4: Nested functions with different strategies
@manual_memory
def outer_manual():
    print("Outer: Using manual memory")
    buffer = allocate(100)

    @auto_memory
    def inner_auto():
        print("Inner: Using automatic memory")
        x = 10
        y = 20
        return x + y

    result = inner_auto()
    print(f"Inner result: {result}")

    free(buffer)
    print("Outer: Freed buffer")
    return result

outer_result = outer_manual()
print(f"4. Nested function result: {outer_result}")

# Test 5: Performance-critical section with manual management
@manual_memory
def performance_critical():
    # Allocate exactly what we need
    data_buffer = allocate(4096)
    print("Performance: Allocated 4KB buffer")

    # Process data (simulated)
    result = 42

    # Clean up
    free(data_buffer)
    print("Performance: Freed buffer")

    return result

perf_result = performance_critical()
print(f"5. Performance result: {perf_result}")

# Test 6: Batch processing with arena
@arena_memory
def batch_processor():
    create_arena("batch_arena")
    print("Batch: Processing batch with arena")

    results = []
    for i in range(3):
        # Each iteration allocates from arena
        temp_buf = allocate(128)
        results.append(i * 10)

    print(f"Batch: Processed {len(results)} items")

    # Bulk cleanup
    destroy_arena("batch_arena")
    print("Batch: Arena destroyed, all freed")

    return results

batch_results = batch_processor()
print(f"6. Batch results: {batch_results}")

# Final statistics
print("\n=== Final Memory Statistics ===")
stats = memory_stats()
print(stats)

print("\n=== All Decorator Tests Passed ===")
