"""
FFI Linux Example - Calling Linux system libraries from Tauraro

This example demonstrates how to use Tauraro's FFI to call native Linux library functions.
"""

# Load the math library (libm.so)
print("Loading math library...")
load_library("m")  # Will find libm.so automatically

# Define sqrt function
# double sqrt(double x)
print("\nDefining sqrt function...")
define_function("m", "sqrt", "double", ["double"])

# Call sqrt
print("Calling sqrt(16.0)...")
result = call_function("m", "sqrt", [16.0])
print(f"sqrt(16.0) = {result}")

# Define pow function
# double pow(double x, double y)
print("\nDefining pow function...")
define_function("m", "pow", "double", ["double", "double"])

# Call pow
print("Calling pow(2.0, 10.0)...")
result = call_function("m", "pow", [2.0, 10.0])
print(f"pow(2.0, 10.0) = {result}")

# Define sin function
# double sin(double x)
print("\nDefining sin function...")
define_function("m", "sin", "double", ["double"])

# Call sin
import math
print("Calling sin(π/2)...")
pi_over_2 = 3.14159265359 / 2.0
result = call_function("m", "sin", [pi_over_2])
print(f"sin(π/2) = {result}")

# Define cos function
# double cos(double x)
print("\nDefining cos function...")
define_function("m", "cos", "double", ["double"])

# Call cos
print("Calling cos(0)...")
result = call_function("m", "cos", [0.0])
print(f"cos(0) = {result}")

# Load libc for additional functions
print("\nLoading C library...")
load_library("c")  # Will find libc.so automatically

# Define strlen function
# size_t strlen(const char *s)
print("\nDefining strlen function...")
define_function("c", "strlen", "size_t", ["string"])

# Call strlen
test_string = "Hello, Tauraro FFI!"
print(f"Calling strlen('{test_string}')...")
result = call_function("c", "strlen", [test_string])
print(f"strlen = {result}")

# Define getpid function
# pid_t getpid(void)
print("\nDefining getpid function...")
define_function("c", "getpid", "int32", [])

# Get current process ID
pid = call_function("c", "getpid", [])
print(f"Current Process ID: {pid}")

# List all loaded libraries
print("\n=== Loaded Libraries ===")
libs = list_libraries()
for lib in libs:
    print(f"- {lib}")
    info = library_info(lib)
    print(f"  Path: {info['path']}")
    print(f"  Functions: {info['functions']}")

print("\nFFI Linux example completed successfully!")
