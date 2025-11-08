# Simple FFI test for C transpilation
print("=== Testing FFI with Math Library ===")

# Load math library
load_library("m")  # libm.so on Linux

# Define sqrt function
sqrt_func = define_function("m", "sqrt", "double", ["double"])

# Call sqrt
result = call_function(sqrt_func, [16.0])
print("sqrt(16.0) =", result)

# Define pow function
pow_func = define_function("m", "pow", "double", ["double", "double"])

# Call pow
result2 = call_function(pow_func, [2.0, 8.0])
print("pow(2.0, 8.0) =", result2)

print("=== FFI Test Complete ===")
