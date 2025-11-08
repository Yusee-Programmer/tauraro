# Simple FFI test with manual memory management
# This version works in both VM and C compilation

print("=== FFI with Manual Memory Management ===")

# Load the math library
print("\n1. Loading native library...")
load_library("libm.so.6")
print("   Library loaded successfully")

# Define math functions
print("\n2. Defining functions...")
sqrt_func = define_function("libm.so.6", "sqrt", "double", ["double"])
pow_func = define_function("libm.so.6", "pow", "double", ["double", "double"])
sin_func = define_function("libm.so.6", "sin", "double", ["double"])
cos_func = define_function("libm.so.6", "cos", "double", ["double"])
print("   Functions defined: sqrt, pow, sin, cos")

# Simple calculations
print("\n3. Testing functions:")
result = call_function("libm.so.6", "sqrt", [16.0])
print(f"   sqrt(16.0) = {result}")

result = call_function("libm.so.6", "pow", [2.0, 3.0])
print(f"   pow(2.0, 3.0) = {result}")

result = call_function("libm.so.6", "sin", [1.5708])
print(f"   sin(1.5708) = {result}")

# Manual memory management
print("\n4. Manual memory management:")
buffer = allocate(1024)
print("   Allocated 1KB buffer")

# Batch calculations
print("   Batch calculations:")
for i in range(5):
    x = float(i + 1)
    sqrt_result = call_function("libm.so.6", "sqrt", [x])
    print(f"   sqrt({x}) = {sqrt_result:.4f}")

free(buffer)
print("   Buffer freed")

# Arena memory management
print("\n5. Arena memory management:")
create_arena("math_arena")
print("   Arena created")

temp1 = allocate(512)
temp2 = allocate(512)
print("   Allocated 2 buffers in arena")

# More calculations
for angle in [0.0, 0.785398, 1.5708]:
    sin_val = call_function("libm.so.6", "sin", [angle])
    cos_val = call_function("libm.so.6", "cos", [angle])
    print(f"   angle={angle:.4f}: sin={sin_val:.4f}, cos={cos_val:.4f}")

destroy_arena("math_arena")
print("   Arena destroyed")

# Memory statistics
print("\n6. Memory statistics:")
stats = memory_stats()
print(stats)

# Cleanup
print("\n7. Cleanup:")
unload_library("libm.so.6")
print("   Library unloaded")

print("\n=== Test Completed Successfully ===")
