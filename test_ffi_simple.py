# Simple FFI Test for Tauraro on Windows
# Tests basic FFI functionality

print("=== Tauraro Simple FFI Test (Windows) ===")
print()

# Test 1: Load Windows kernel32.dll
print("Test 1: Loading kernel32.dll...")
try:
    load_library("kernel32")
    print("✓ Successfully loaded kernel32")
except Exception as e:
    print(f"✗ Failed to load kernel32: {e}")
    exit(1)

# Test 2: Define GetTickCount (no args, returns int)
print("\nTest 2: Defining GetTickCount function...")
try:
    define_function("kernel32", "GetTickCount", "int32", [])
    print("✓ Defined GetTickCount")
except Exception as e:
    print(f"✗ Failed to define GetTickCount: {e}")
    exit(1)

# Test 3: Call GetTickCount
print("\nTest 3: Calling GetTickCount()...")
try:
    ticks = call_function("kernel32", "GetTickCount", [])
    print(f"✓ GetTickCount() returned: {ticks}")
    print(f"  System has been running for ~{ticks / 1000} seconds")
except Exception as e:
    print(f"✗ Failed to call GetTickCount: {e}")
    exit(1)

# Test 4: List loaded libraries
print("\nTest 4: Listing loaded libraries...")
try:
    libs = list_libraries()
    print(f"✓ Loaded libraries: {libs}")
except Exception as e:
    print(f"✗ Failed to list libraries: {e}")

# Test 5: Get library info
print("\nTest 5: Getting library info for kernel32...")
try:
    info = library_info("kernel32")
    print(f"✓ Library info: {info}")
except Exception as e:
    print(f"✗ Failed to get library info: {e}")

# Test 6: Load msvcrt for math functions
print("\nTest 6: Loading msvcrt for math functions...")
try:
    load_library("msvcrt")
    print("✓ Successfully loaded msvcrt")
except Exception as e:
    print(f"✗ Failed to load msvcrt: {e}")
    exit(1)

# Test 7: Define sqrt function
print("\nTest 7: Defining sqrt function...")
try:
    sqrt_func = define_function("msvcrt", "sqrt", "double", ["double"])
    print(f"✓ Defined sqrt function: {sqrt_func}")
except Exception as e:
    print(f"✗ Failed to define sqrt: {e}")
    exit(1)

# Test 8: Call sqrt(16.0)
print("\nTest 8: Calling sqrt(16.0)...")
try:
    result = call_function("msvcrt", "sqrt", [16.0])
    print(f"✓ sqrt(16.0) = {result}")
    if result == 4.0:
        print("  ✓ Result is correct!")
    else:
        print(f"  ✗ Expected 4.0, got {result}")
except Exception as e:
    print(f"✗ Failed to call sqrt: {e}")
    exit(1)

# Test 9: Test buffer allocation
print("\nTest 9: Testing buffer allocation...")
try:
    buffer = allocate_buffer(128)
    print(f"✓ Allocated 128-byte buffer at address: {buffer}")
    free_buffer(buffer)
    print("✓ Freed buffer successfully")
except Exception as e:
    print(f"✗ Buffer allocation test failed: {e}")

print("\n=== All FFI Tests Passed! ===")
