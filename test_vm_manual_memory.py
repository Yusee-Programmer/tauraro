# Test VM manual memory management
print("=== Testing VM Manual Memory Management ===")

# Test allocate and free
print("\n1. Testing allocate():")
buffer = allocate(1024)
print("Allocated 1024 bytes")
print("Buffer:", buffer)

print("\n2. Testing free():")
free(buffer)
print("Freed buffer successfully")

# Test multiple allocations
print("\n3. Testing multiple allocations:")
buffers = []
for i in range(5):
    buf = allocate(256)
    buffers.append(buf)
    print("Allocated buffer", i)

print("\n4. Freeing all buffers:")
for i, buf in enumerate(buffers):
    free(buf)
    print("Freed buffer", i)

print("\n=== VM Manual Memory Management Test Complete ===")
