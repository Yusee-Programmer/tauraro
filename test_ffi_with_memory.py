# Test FFI with manual memory management
# This example loads the math library (libm) and uses its functions
# with manual memory management for performance-critical sections

print("=== FFI with Manual Memory Management ===")

# Load the math library
print("\n1. Loading native library...")
load_library("libm.so.6")  # libm.so.6 on Linux
print("   Library 'libm' loaded successfully")

# Define math functions
print("\n2. Defining functions from library...")
sqrt_func = define_function("libm.so.6", "sqrt", "double", ["double"])
pow_func = define_function("libm.so.6", "pow", "double", ["double", "double"])
sin_func = define_function("libm.so.6", "sin", "double", ["double"])
cos_func = define_function("libm.so.6", "cos", "double", ["double"])
print("   Functions defined: sqrt, pow, sin, cos")

# Use automatic memory management for simple calculations
print("\n3. Simple calculations (automatic memory):")
result = call_function("libm.so.6", "sqrt", [16.0])
print(f"   sqrt(16.0) = {result}")

result = call_function("libm.so.6", "pow", [2.0, 3.0])
print(f"   pow(2.0, 3.0) = {result}")

# Use manual memory management for batch calculations
print("\n4. Batch calculations (manual memory):")
@manual_memory
def batch_math_operations():
    print("   Allocating buffers for batch operations...")
    # Allocate buffer for results
    buffer = allocate(1024)

    # Perform multiple calculations
    for i in range(5):
        x = float(i + 1)
        sqrt_result = call_function("libm.so.6", "sqrt", [x])
        sin_result = call_function("libm.so.6", "sin", [x])
        print(f"   x={x}: sqrt={sqrt_result:.4f}, sin={sin_result:.4f}")

    # Free the buffer
    free(buffer)
    print("   Buffer freed")

    return True

batch_math_operations()

# Use arena memory for temporary calculations
print("\n5. Temporary calculations (arena memory):")
@arena_memory
def calculate_circle_properties():
    create_arena("circle_arena")
    print("   Arena created for circle calculations")

    # Allocate temporary buffers
    temp1 = allocate(512)
    temp2 = allocate(512)

    radius = 5.0
    pi = 3.14159265359

    # Calculate circle properties
    area_factor = call_function("libm.so.6", "pow", [radius, 2.0])
    area = pi * area_factor

    circumference = 2.0 * pi * radius

    print(f"   Circle with radius {radius}:")
    print(f"   - Area: {area:.4f}")
    print(f"   - Circumference: {circumference:.4f}")

    # Destroy arena (all allocations freed)
    destroy_arena("circle_arena")
    print("   Arena destroyed, all freed")

    return area

result = calculate_circle_properties()

# Performance-critical section with manual management
print("\n6. Performance-critical calculations:")
@manual_memory
def performance_math():
    buffer = allocate(2048)
    print("   Allocated 2KB buffer")

    # Calculate trigonometric values
    angles = [0.0, 0.785398, 1.5708, 3.14159, 4.71239, 6.28319]

    for angle in angles:
        sin_val = call_function("libm.so.6", "sin", [angle])
        cos_val = call_function("libm.so.6", "cos", [angle])
        print(f"   angle={angle:.4f}: sin={sin_val:.4f}, cos={cos_val:.4f}")

    free(buffer)
    print("   Buffer freed")

    return True

performance_math()

# Memory statistics
print("\n7. Memory statistics:")
stats = memory_stats()
print(stats)

# Unload library
print("\n8. Cleanup:")
unload_library("libm.so.6")
print("   Library unloaded")

print("\n=== All FFI and Memory Tests Completed ===")
