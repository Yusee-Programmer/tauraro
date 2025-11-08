# Comprehensive FFI Test for C Compilation
# Tests loading native libraries and calling functions

print("=== Comprehensive FFI Test ===")

# Test 1: Load math library
print("\n1. Loading math library...")
load_library("m")
print("Math library loaded successfully")

# Test 2: Define sqrt function
print("\n2. Defining sqrt function...")
sqrt_func = define_function("m", "sqrt", "double", ["double"])
print("sqrt function defined")

# Test 3: Call sqrt
print("\n3. Calling sqrt(16.0)...")
result1 = call_function(sqrt_func, [16.0])
print("sqrt(16.0) =", result1)

# Test 4: Define pow function
print("\n4. Defining pow function...")
pow_func = define_function("m", "pow", "double", ["double", "double"])
print("pow function defined")

# Test 5: Call pow
print("\n5. Calling pow(2.0, 8.0)...")
result2 = call_function(pow_func, [2.0, 8.0])
print("pow(2.0, 8.0) =", result2)

# Test 6: Define sin function
print("\n6. Defining sin function...")
sin_func = define_function("m", "sin", "double", ["double"])
print("sin function defined")

# Test 7: Call sin
print("\n7. Calling sin(0.0)...")
result3 = call_function(sin_func, [0.0])
print("sin(0.0) =", result3)

print("\n=== FFI Tests Complete ===")
