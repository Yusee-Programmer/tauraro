"""
FFI Custom Library Example - Loading and using custom native libraries

This example shows how to load and use your own custom C/C++ libraries with Tauraro's FFI.

To test this, you would need to:
1. Create a custom C library (e.g., mymath.c)
2. Compile it to a shared library (.dll/.so/.dylib)
3. Place it in a directory
4. Use add_library_path() to add that directory
5. Load and use the library
"""

import sys

print("=== Tauraro FFI Custom Library Example ===\n")

# Example: Suppose we have a custom library with these functions:
#
# // mymath.h
# #ifdef __cplusplus
# extern "C" {
# #endif
#
# int add(int a, int b);
# int multiply(int a, int b);
# double average(double *arr, int count);
# const char* get_version();
#
# #ifdef __cplusplus
# }
# #endif

# Add custom library search path
custom_lib_path = "./lib"  # Replace with your actual path
print(f"Adding library search path: {custom_lib_path}")
add_library_path(custom_lib_path)

# Add platform-specific paths
if sys.platform == "win32":
    add_library_path("C:\\MyLibraries")
elif sys.platform == "linux":
    add_library_path("/usr/local/lib/mylibs")
elif sys.platform == "darwin":
    add_library_path("/usr/local/mylibs")

print("\n=== Example: Loading Custom Math Library ===")
print("(This is a demonstration - library may not exist)\n")

# Try to load custom library
library_name = "mymath"
try:
    print(f"Attempting to load {library_name}...")
    load_library(library_name)
    print(f"✓ {library_name} loaded successfully")

    # Define functions
    print("\nDefining functions...")

    # int add(int a, int b)
    define_function(library_name, "add", "int32", ["int32", "int32"])
    print("✓ add(int, int) -> int")

    # int multiply(int a, int b)
    define_function(library_name, "multiply", "int32", ["int32", "int32"])
    print("✓ multiply(int, int) -> int")

    # const char* get_version()
    define_function(library_name, "get_version", "string", [])
    print("✓ get_version() -> string")

    # Call functions
    print("\n=== Calling Functions ===")

    result = call_function(library_name, "add", [10, 20])
    print(f"add(10, 20) = {result}")

    result = call_function(library_name, "multiply", [7, 8])
    print(f"multiply(7, 8) = {result}")

    version = call_function(library_name, "get_version", [])
    print(f"Library version: {version}")

    print("\n✓ Custom library example completed successfully!")

except Exception as e:
    print(f"✗ Could not load library: {e}")
    print("\nThis is expected if the custom library doesn't exist.")
    print("To create a custom library:\n")

    if sys.platform == "win32":
        print("Windows (MSVC):")
        print("  cl /LD mymath.c /Fe:mymath.dll")
        print("\nWindows (MinGW):")
        print("  gcc -shared -o mymath.dll mymath.c")

    elif sys.platform == "linux":
        print("Linux:")
        print("  gcc -shared -fPIC -o libmymath.so mymath.c")

    elif sys.platform == "darwin":
        print("macOS:")
        print("  gcc -shared -fPIC -o libmymath.dylib mymath.c")

# Show platform-specific library extensions
print("\n=== Platform Library Information ===")
print(f"Platform: {sys.platform}")

if sys.platform == "win32":
    print("Library extension: .dll")
    print("Library naming: mylib.dll")
    print("Standard paths: C:\\Windows\\System32, current directory")
elif sys.platform == "linux":
    print("Library extension: .so")
    print("Library naming: libmylib.so")
    print("Standard paths: /lib, /usr/lib, /usr/local/lib")
    print("Environment: LD_LIBRARY_PATH")
elif sys.platform == "darwin":
    print("Library extension: .dylib")
    print("Library naming: libmylib.dylib")
    print("Standard paths: /usr/lib, /usr/local/lib")
    print("Environment: DYLD_LIBRARY_PATH")

print("\n=== Current Library Status ===")
libs = list_libraries()
if libs:
    print(f"Loaded libraries: {len(libs)}")
    for lib in libs:
        info = library_info(lib)
        print(f"\n{lib}:")
        print(f"  Path: {info['path']}")
        print(f"  Functions: {info['functions']}")
else:
    print("No libraries currently loaded")
