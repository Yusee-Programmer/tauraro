"""
FFI macOS Example - Calling macOS system libraries from Tauraro

This example demonstrates how to use Tauraro's FFI to call native macOS library functions.
"""

# Load the System library (libSystem.dylib)
print("Loading System library...")
load_library("System")  # Will find libSystem.dylib automatically

# Define sqrt function from libm (part of libSystem on macOS)
# double sqrt(double x)
print("\nDefining sqrt function...")
define_function("System", "sqrt", "double", ["double"])

# Call sqrt
print("Calling sqrt(16.0)...")
result = call_function("System", "sqrt", [16.0])
print(f"sqrt(16.0) = {result}")

# Define pow function
# double pow(double x, double y)
print("\nDefining pow function...")
define_function("System", "pow", "double", ["double", "double"])

# Call pow
print("Calling pow(2.0, 10.0)...")
result = call_function("System", "pow", [2.0, 10.0])
print(f"pow(2.0, 10.0) = {result}")

# Define strlen function
# size_t strlen(const char *s)
print("\nDefining strlen function...")
define_function("System", "strlen", "size_t", ["string"])

# Call strlen
test_string = "Hello from macOS!"
print(f"Calling strlen('{test_string}')...")
result = call_function("System", "strlen", [test_string])
print(f"strlen = {result}")

# Define getpid function
# pid_t getpid(void)
print("\nDefining getpid function...")
define_function("System", "getpid", "int32", [])

# Get current process ID
pid = call_function("System", "getpid", [])
print(f"Current Process ID: {pid}")

# Define time function
# time_t time(time_t *tloc)
print("\nDefining time function...")
define_function("System", "time", "int64", ["pointer"])

# Get current Unix timestamp
timestamp = call_function("System", "time", [0])
print(f"Current Unix timestamp: {timestamp}")

# Trigonometric functions
print("\n=== Trigonometric Functions ===")

# Define sin function
define_function("System", "sin", "double", ["double"])
pi_over_2 = 3.14159265359 / 2.0
result = call_function("System", "sin", [pi_over_2])
print(f"sin(π/2) = {result}")

# Define cos function
define_function("System", "cos", "double", ["double"])
result = call_function("System", "cos", [0.0])
print(f"cos(0) = {result}")

# Define tan function
define_function("System", "tan", "double", ["double"])
pi_over_4 = 3.14159265359 / 4.0
result = call_function("System", "tan", [pi_over_4])
print(f"tan(π/4) = {result}")

# List all loaded libraries
print("\n=== Loaded Libraries ===")
libs = list_libraries()
for lib in libs:
    print(f"- {lib}")
    info = library_info(lib)
    print(f"  Path: {info['path']}")
    print(f"  Functions: {info['functions']}")

print("\nFFI macOS example completed successfully!")
