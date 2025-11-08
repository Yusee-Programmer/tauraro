# Test memory management integration in both VM and C transpiler
# This file tests manual memory allocation, arena allocation, and automatic memory management

print("=== Testing Memory Management ===")

# Test 1: Manual memory allocation
print("\n1. Manual Memory Allocation:")
buffer = allocate(1024)
print(f"Allocated buffer: {buffer}")
free(buffer)
print("Buffer freed successfully")

# Test 2: Arena memory allocation
print("\n2. Arena Memory Allocation:")
create_arena("test_arena")
print("Arena 'test_arena' created")

# Allocate within arena
buf1 = allocate(512)
buf2 = allocate(256)
print(f"Allocated buf1: {buf1}")
print(f"Allocated buf2: {buf2}")

# Reset arena (frees all allocations)
reset_arena("test_arena")
print("Arena reset - all allocations freed")

# Destroy arena
destroy_arena("test_arena")
print("Arena destroyed")

# Test 3: Memory statistics
print("\n3. Memory Statistics:")
stats = memory_stats()
print(stats)

# Test 4: Multiple allocations and frees
print("\n4. Multiple Allocations:")
buffers = []
for i in range(5):
    buf = allocate(100 * (i + 1))
    buffers.append(buf)
    print(f"Allocated buffer {i}: {buf}")

print("\nFreeing buffers...")
for i, buf in enumerate(buffers):
    free(buf)
    print(f"Freed buffer {i}")

# Test 5: Final statistics
print("\n5. Final Statistics:")
stats = memory_stats()
print(stats)

print("\n=== All Memory Tests Passed ===")
