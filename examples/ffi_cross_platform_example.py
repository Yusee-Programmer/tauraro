"""
FFI Cross-Platform Example - Universal FFI code that works on Windows, Linux, and macOS

This example demonstrates how to write FFI code that works across multiple platforms.
"""

import sys

print("=== Tauraro FFI Cross-Platform Example ===\n")

# Detect platform
platform = sys.platform
print(f"Detected platform: {platform}")

# Load appropriate math library based on platform
if platform == "win32":
    print("Loading msvcrt.dll (Windows C Runtime)...")
    load_library("msvcrt.dll")
    math_lib = "msvcrt.dll"
elif platform == "linux":
    print("Loading libm.so (Linux Math Library)...")
    load_library("m")
    math_lib = "m"
elif platform == "darwin":
    print("Loading libSystem.dylib (macOS System Library)...")
    load_library("System")
    math_lib = "System"
else:
    print(f"Unknown platform: {platform}")
    math_lib = None

if math_lib:
    # Define common math functions
    print("\n=== Defining Math Functions ===")

    # sqrt - Square root
    define_function(math_lib, "sqrt", "double", ["double"])
    print("✓ sqrt defined")

    # pow - Power
    define_function(math_lib, "pow", "double", ["double", "double"])
    print("✓ pow defined")

    # sin - Sine
    define_function(math_lib, "sin", "double", ["double"])
    print("✓ sin defined")

    # cos - Cosine
    define_function(math_lib, "cos", "double", ["double"])
    print("✓ cos defined")

    # tan - Tangent
    define_function(math_lib, "tan", "double", ["double"])
    print("✓ tan defined")

    # log - Natural logarithm
    define_function(math_lib, "log", "double", ["double"])
    print("✓ log defined")

    # exp - Exponential
    define_function(math_lib, "exp", "double", ["double"])
    print("✓ exp defined")

    # Test all functions
    print("\n=== Testing Math Functions ===")

    # Test sqrt
    result = call_function(math_lib, "sqrt", [25.0])
    print(f"sqrt(25.0) = {result}")

    # Test pow
    result = call_function(math_lib, "pow", [2.0, 8.0])
    print(f"pow(2.0, 8.0) = {result}")

    # Test trigonometric functions
    pi = 3.14159265359
    result = call_function(math_lib, "sin", [pi/2])
    print(f"sin(π/2) = {result}")

    result = call_function(math_lib, "cos", [0.0])
    print(f"cos(0.0) = {result}")

    result = call_function(math_lib, "tan", [pi/4])
    print(f"tan(π/4) = {result}")

    # Test log and exp
    result = call_function(math_lib, "log", [2.71828])
    print(f"log(e) = {result}")

    result = call_function(math_lib, "exp", [1.0])
    print(f"exp(1.0) = {result}")

# Platform-specific demonstrations
print("\n=== Platform-Specific Features ===")

if platform == "win32":
    # Windows-specific: Get system info
    print("\nWindows-specific functions:")
    load_library("kernel32.dll")

    define_function("kernel32.dll", "GetTickCount", "uint32", [])
    uptime = call_function("kernel32.dll", "GetTickCount", [])
    print(f"System uptime: {uptime} ms ({uptime/1000/60:.2f} minutes)")

    define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])
    pid = call_function("kernel32.dll", "GetCurrentProcessId", [])
    print(f"Process ID: {pid}")

elif platform in ["linux", "darwin"]:
    # Unix-like: Get process info
    print("\nUnix-like functions:")

    if platform == "linux":
        load_library("c")
        libc = "c"
    else:  # darwin (macOS)
        libc = "System"

    define_function(libc, "getpid", "int32", [])
    pid = call_function(libc, "getpid", [])
    print(f"Process ID: {pid}")

    define_function(libc, "time", "int64", ["pointer"])
    timestamp = call_function(libc, "time", [0])
    print(f"Unix timestamp: {timestamp}")

# Summary
print("\n=== Summary ===")
libs = list_libraries()
print(f"Loaded {len(libs)} libraries:")
for lib in libs:
    info = library_info(lib)
    print(f"  - {lib}: {info['functions']} functions defined")

print("\n✓ FFI Cross-Platform example completed successfully!")
print(f"✓ All operations work on {platform}")
