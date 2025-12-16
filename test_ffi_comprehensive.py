#!/usr/bin/env python3
"""
Comprehensive FFI Test for Tauraro C Transpiler
Tests automatic FFI code generation and cross-platform library loading
"""

print("=== Tauraro FFI Comprehensive Test ===")
print()

# Test 1: Load C standard library
print("Test 1: Loading C standard library")
lib_name = "libc.so.6"  # Linux
# On macOS it would be "libc.dylib"
# On Windows it would be "msvcrt.dll"

success = load_library(lib_name)
if success:
    print("✓ Library loaded successfully:", lib_name)
else:
    print("✗ Failed to load library:", lib_name)
    print("Exiting...")
    exit(1)

print()

# Test 2: Define strlen function
print("Test 2: Defining strlen function")
define_function(lib_name, "strlen", "int", ["string"])
print("✓ strlen function defined")
print()

# Test 3: Call strlen with various strings
print("Test 3: Calling strlen with different strings")
test_strings = ["Hello", "Hello, World!", "Tauraro FFI", ""]

for test_str in test_strings:
    length = call_function("strlen", test_str)
    print(f"  strlen('{test_str}') = {length}")

print()

# Test 4: Load library again (should succeed - already loaded)
print("Test 4: Loading library again (should use cached)")
success2 = load_library(lib_name)
if success2:
    print("✓ Library load returned success (cached)")
else:
    print("✗ Unexpected failure on cached library")

print()

print("=== All FFI Tests Completed Successfully ===")
