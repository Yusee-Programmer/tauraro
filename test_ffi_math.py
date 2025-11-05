# Test FFI functionality - Load libm and call sqrt function

# Load the math library (libm)
print("Loading libm...")
load_library("libm.so.6")

# Define the sqrt function
print("Defining sqrt function...")
define_function("libm.so.6", "sqrt", "double", ["double"])

# Call sqrt using FFI
print("Calling sqrt(16.0)...")
result = call_function("libm.so.6", "sqrt", [16.0])
print(f"Result: {result}")

# Call sqrt again with different value
result2 = call_function("libm.so.6", "sqrt", [25.0])
print(f"sqrt(25.0) = {result2}")

print("FFI test completed successfully!")
