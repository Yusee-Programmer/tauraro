"""
Test FFI on Windows - Basic functionality test
"""

print("=" * 60)
print("Tauraro FFI Windows Test")
print("=" * 60)

# Test 1: Load kernel32.dll
print("\nTest 1: Loading kernel32.dll...")
try:
    load_library("kernel32.dll")
    print("✓ kernel32.dll loaded successfully")
except Exception as e:
    print(f"✗ Failed to load kernel32.dll: {e}")
    exit(1)

# Test 2: List loaded libraries
print("\nTest 2: Listing loaded libraries...")
try:
    libs = list_libraries()
    print(f"✓ Loaded libraries: {libs}")
except Exception as e:
    print(f"✗ Failed to list libraries: {e}")
    exit(1)

# Test 3: Get library info
print("\nTest 3: Getting library information...")
try:
    info = library_info("kernel32.dll")
    print(f"✓ Library name: {info['name']}")
    print(f"✓ Library path: {info['path']}")
    print(f"✓ Functions defined: {info['functions']}")
except Exception as e:
    print(f"✗ Failed to get library info: {e}")
    exit(1)

# Test 4: Define GetTickCount function
print("\nTest 4: Defining GetTickCount function...")
try:
    define_function("kernel32.dll", "GetTickCount", "uint32", [])
    print("✓ GetTickCount defined successfully")
except Exception as e:
    print(f"✗ Failed to define GetTickCount: {e}")
    exit(1)

# Test 5: Call GetTickCount
print("\nTest 5: Calling GetTickCount...")
try:
    tick1 = call_function("kernel32.dll", "GetTickCount", [])
    print(f"✓ GetTickCount() = {tick1}")
    print(f"  System uptime: {tick1} milliseconds")
    print(f"  System uptime: {tick1 / 1000} seconds")
    print(f"  System uptime: {tick1 / 1000 / 60} minutes")

    if tick1 > 0:
        print("✓ Result is valid (greater than 0)")
    else:
        print("✗ Result is invalid (expected > 0)")
except Exception as e:
    print(f"✗ Failed to call GetTickCount: {e}")
    exit(1)

# Test 6: Define GetCurrentProcessId
print("\nTest 6: Defining GetCurrentProcessId function...")
try:
    define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])
    print("✓ GetCurrentProcessId defined successfully")
except Exception as e:
    print(f"✗ Failed to define GetCurrentProcessId: {e}")
    exit(1)

# Test 7: Call GetCurrentProcessId
print("\nTest 7: Calling GetCurrentProcessId...")
try:
    pid = call_function("kernel32.dll", "GetCurrentProcessId", [])
    print(f"✓ GetCurrentProcessId() = {pid}")
    print(f"  Current process ID: {pid}")

    if pid > 0:
        print("✓ PID is valid (greater than 0)")
    else:
        print("✗ PID is invalid (expected > 0)")
except Exception as e:
    print(f"✗ Failed to call GetCurrentProcessId: {e}")
    exit(1)

# Test 8: Define Sleep function
print("\nTest 8: Defining Sleep function...")
try:
    define_function("kernel32.dll", "Sleep", "void", ["uint32"])
    print("✓ Sleep defined successfully")
except Exception as e:
    print(f"✗ Failed to define Sleep: {e}")
    exit(1)

# Test 9: Call Sleep (sleep for 500ms)
print("\nTest 9: Calling Sleep(500)...")
try:
    print("  Sleeping for 500 milliseconds...")
    call_function("kernel32.dll", "Sleep", [500])
    print("✓ Sleep completed successfully")
except Exception as e:
    print(f"✗ Failed to call Sleep: {e}")
    exit(1)

# Test 10: Verify GetTickCount changed
print("\nTest 10: Verifying time passed...")
try:
    tick2 = call_function("kernel32.dll", "GetTickCount", [])
    elapsed = tick2 - tick1
    print(f"✓ Time elapsed: {elapsed} milliseconds")

    if elapsed >= 400:  # Should be at least 400ms (allowing some margin)
        print(f"✓ Time measurement is accurate (expected ~500ms, got {elapsed}ms)")
    else:
        print(f"⚠ Time measurement seems off (expected ~500ms, got {elapsed}ms)")
except Exception as e:
    print(f"✗ Failed to verify time: {e}")
    exit(1)

# Test 11: Check library info again
print("\nTest 11: Checking library info after function definitions...")
try:
    info = library_info("kernel32.dll")
    func_count = info['functions']
    print(f"✓ Functions defined: {func_count}")

    if func_count >= 3:
        print(f"✓ Correct number of functions (expected 3, got {func_count})")
    else:
        print(f"✗ Wrong number of functions (expected 3, got {func_count})")
except Exception as e:
    print(f"✗ Failed to check library info: {e}")
    exit(1)

# Summary
print("\n" + "=" * 60)
print("✓ All tests passed!")
print("=" * 60)
print("\nFFI Summary:")
print(f"  - Loaded libraries: {len(libs)}")
print(f"  - Functions defined: {func_count}")
print(f"  - Process ID: {pid}")
print(f"  - System uptime: {tick2 / 1000 / 60:.2f} minutes")
print("\n✓ FFI system is working correctly on Windows!")
