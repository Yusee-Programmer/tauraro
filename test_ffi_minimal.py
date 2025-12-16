#!/usr/bin/env python3
"""
Minimal FFI Test - No sys module dependency
Tests basic FFI functionality: load library and call strlen
"""

print("=== Minimal FFI Test ===")

# Load C standard library
success = load_library("libc.so.6")
if success:
    print("Library loaded OK")

    # Define strlen function
    define_function("libc.so.6", "strlen", "int", ["string"])

    # Call strlen
    result = call_function("strlen", "Hello FFI")
    print("Result:", result)
else:
    print("Failed to load library")

print("Test complete")
