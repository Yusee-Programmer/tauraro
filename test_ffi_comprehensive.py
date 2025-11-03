# Comprehensive FFI Test for Tauraro
# Tests FFI functionality across different platforms

print("=== Tauraro FFI Comprehensive Test ===")
print()

# Test 1: Load platform-specific math library
print("Test 1: Loading math library...")
try:
    # This should work on all platforms
    # Windows: msvcrt.dll, Linux/macOS: libm.so/dylib
    import sys
    if sys.platform == "win32":
        load_library("msvcrt")
        print("✓ Loaded msvcrt.dll on Windows")
    elif sys.platform == "darwin":
        load_library("libSystem")
        print("✓ Loaded libSystem.dylib on macOS")
    else:
        load_library("m")
        print("✓ Loaded libm.so on Linux")
except Exception as e:
    print(f"✗ Failed to load math library: {e}")

# Test 2: Define a simple function
print("\nTest 2: Defining sqrt function...")
try:
    if sys.platform == "win32":
        sqrt_func = define_function("msvcrt", "sqrt", "double", ["double"])
    elif sys.platform == "darwin":
        sqrt_func = define_function("libSystem", "sqrt", "double", ["double"])
    else:
        sqrt_func = define_function("m", "sqrt", "double", ["double"])
    print(f"✓ Defined sqrt function: {sqrt_func}")
except Exception as e:
    print(f"✗ Failed to define sqrt: {e}")

# Test 3: Call the function
print("\nTest 3: Calling sqrt(16.0)...")
try:
    if sys.platform == "win32":
        result = call_function("msvcrt", "sqrt", [16.0])
    elif sys.platform == "darwin":
        result = call_function("libSystem", "sqrt", [16.0])
    else:
        result = call_function("m", "sqrt", [16.0])
    print(f"✓ sqrt(16.0) = {result}")
    assert result == 4.0, f"Expected 4.0, got {result}"
except Exception as e:
    print(f"✗ Failed to call sqrt: {e}")

# Test 4: Call function with ExternFunction value directly
print("\nTest 4: Calling sqrt function directly...")
try:
    if 'sqrt_func' in dir():
        result = sqrt_func([25.0])
        print(f"✓ sqrt_func(25.0) = {result}")
        assert result == 5.0, f"Expected 5.0, got {result}"
except Exception as e:
    print(f"✗ Failed to call sqrt_func: {e}")

# Test 5: List loaded libraries
print("\nTest 5: Listing loaded libraries...")
try:
    libs = list_libraries()
    print(f"✓ Loaded libraries: {libs}")
except Exception as e:
    print(f"✗ Failed to list libraries: {e}")

# Test 6: Get library info
print("\nTest 6: Getting library info...")
try:
    if sys.platform == "win32":
        info = library_info("msvcrt")
    elif sys.platform == "darwin":
        info = library_info("libSystem")
    else:
        info = library_info("m")
    print(f"✓ Library info: {info}")
except Exception as e:
    print(f"✗ Failed to get library info: {e}")

# Test 7: Test with multiple arguments (pow function)
print("\nTest 7: Testing pow(2.0, 3.0)...")
try:
    if sys.platform == "win32":
        define_function("msvcrt", "pow", "double", ["double", "double"])
        result = call_function("msvcrt", "pow", [2.0, 3.0])
    elif sys.platform == "darwin":
        define_function("libSystem", "pow", "double", ["double", "double"])
        result = call_function("libSystem", "pow", [2.0, 3.0])
    else:
        define_function("m", "pow", "double", ["double", "double"])
        result = call_function("m", "pow", [2.0, 3.0])
    print(f"✓ pow(2.0, 3.0) = {result}")
    assert result == 8.0, f"Expected 8.0, got {result}"
except Exception as e:
    print(f"✗ Failed to test pow: {e}")

# Test 8: Test buffer allocation (for structures)
print("\nTest 8: Testing buffer allocation...")
try:
    buffer = allocate_buffer(64)
    print(f"✓ Allocated 64-byte buffer at address: {buffer}")
    free_buffer(buffer)
    print(f"✓ Freed buffer")
except Exception as e:
    print(f"✗ Failed buffer allocation test: {e}")

# Test 9: Add custom library path
print("\nTest 9: Adding custom library path...")
try:
    add_library_path("/usr/local/lib")
    print("✓ Added custom library path")
except Exception as e:
    print(f"✗ Failed to add library path: {e}")

print("\n=== FFI Tests Complete ===")
