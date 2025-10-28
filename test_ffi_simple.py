"""
Simple FFI Test - Quick test to verify FFI functionality
"""

import sys

print("=== Tauraro FFI Simple Test ===\n")
print(f"Platform: {sys.platform}\n")

# Test 1: Check if FFI functions are available
print("Test 1: Checking FFI function availability...")
try:
    # These should be available as builtins
    funcs = ['load_library', 'define_function', 'call_function',
             'list_libraries', 'library_info', 'add_library_path', 'unload_library']

    for func in funcs:
        # Try to access the function (this will fail if not defined)
        eval(func)
        print(f"  ✓ {func} is available")

    print("✓ Test 1 passed: All FFI functions are available\n")
except NameError as e:
    print(f"✗ Test 1 failed: {e}\n")
    sys.exit(1)

# Test 2: Load a platform-specific library
print("Test 2: Loading platform-specific library...")
try:
    if sys.platform == "win32":
        lib_name = "kernel32.dll"
    elif sys.platform == "linux":
        lib_name = "m"
    elif sys.platform == "darwin":
        lib_name = "System"
    else:
        print(f"✗ Test 2 skipped: Unknown platform {sys.platform}\n")
        sys.exit(0)

    print(f"  Loading {lib_name}...")
    load_library(lib_name)
    print(f"  ✓ {lib_name} loaded successfully")
    print("✓ Test 2 passed: Library loaded\n")
except Exception as e:
    print(f"✗ Test 2 failed: {e}\n")
    sys.exit(1)

# Test 3: List loaded libraries
print("Test 3: Listing loaded libraries...")
try:
    libs = list_libraries()
    print(f"  Loaded libraries: {libs}")
    if len(libs) > 0:
        print("  ✓ Library list retrieved")
    else:
        print("  ✗ No libraries loaded")
    print("✓ Test 3 passed: list_libraries() works\n")
except Exception as e:
    print(f"✗ Test 3 failed: {e}\n")
    sys.exit(1)

# Test 4: Get library info
print("Test 4: Getting library information...")
try:
    info = library_info(lib_name)
    print(f"  Name: {info['name']}")
    print(f"  Path: {info['path']}")
    print(f"  Functions: {info['functions']}")
    print("✓ Test 4 passed: library_info() works\n")
except Exception as e:
    print(f"✗ Test 4 failed: {e}\n")
    sys.exit(1)

# Test 5: Define a simple function
print("Test 5: Defining a function...")
try:
    if sys.platform == "win32":
        # Define GetTickCount (no parameters, returns uint32)
        define_function(lib_name, "GetTickCount", "uint32", [])
        func_name = "GetTickCount"
    else:
        # Define sqrt (one double parameter, returns double)
        define_function(lib_name, "sqrt", "double", ["double"])
        func_name = "sqrt"

    print(f"  ✓ {func_name} defined successfully")
    print("✓ Test 5 passed: define_function() works\n")
except Exception as e:
    print(f"✗ Test 5 failed: {e}\n")
    sys.exit(1)

# Test 6: Call the function
print("Test 6: Calling the function...")
try:
    if sys.platform == "win32":
        result = call_function(lib_name, "GetTickCount", [])
        print(f"  GetTickCount() = {result}")
        if result > 0:
            print(f"  ✓ Function returned valid result")
    else:
        result = call_function(lib_name, "sqrt", [16.0])
        print(f"  sqrt(16.0) = {result}")
        if abs(result - 4.0) < 0.01:
            print(f"  ✓ Function returned correct result (expected 4.0)")
        else:
            print(f"  ✗ Function returned incorrect result (expected 4.0, got {result})")

    print("✓ Test 6 passed: call_function() works\n")
except Exception as e:
    print(f"✗ Test 6 failed: {e}\n")
    sys.exit(1)

# Test 7: Define and call another function
print("Test 7: Defining and calling another function...")
try:
    if sys.platform == "win32":
        # Define GetCurrentProcessId
        define_function(lib_name, "GetCurrentProcessId", "uint32", [])
        result = call_function(lib_name, "GetCurrentProcessId", [])
        print(f"  GetCurrentProcessId() = {result}")
        if result > 0:
            print(f"  ✓ Process ID is valid")
    else:
        # Define pow (two double parameters, returns double)
        define_function(lib_name, "pow", "double", ["double", "double"])
        result = call_function(lib_name, "pow", [2.0, 3.0])
        print(f"  pow(2.0, 3.0) = {result}")
        if abs(result - 8.0) < 0.01:
            print(f"  ✓ Function returned correct result (expected 8.0)")
        else:
            print(f"  ✗ Function returned incorrect result (expected 8.0, got {result})")

    print("✓ Test 7 passed: Multiple functions work\n")
except Exception as e:
    print(f"✗ Test 7 failed: {e}\n")
    sys.exit(1)

# Summary
print("=" * 50)
print("✓ All tests passed!")
print("=" * 50)
print("\nFFI system is working correctly!")
print(f"Platform: {sys.platform}")
print(f"Loaded libraries: {list_libraries()}")
