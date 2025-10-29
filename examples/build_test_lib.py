#!/usr/bin/env python3
# Build script for test_lib.c

import os
import sys
import subprocess
import platform

def build_test_library():
    """Build the test library for the current platform"""
    
    # Determine the platform and library name
    system = platform.system().lower()
    
    if system == "windows":
        lib_name = "test_lib.dll"
        compile_cmd = ["gcc", "-shared", "-fPIC", "-o", lib_name, "test_lib.c"]
    elif system == "darwin":  # macOS
        lib_name = "libtest_lib.dylib"
        compile_cmd = ["gcc", "-shared", "-fPIC", "-o", lib_name, "test_lib.c"]
    else:  # Linux and other Unix-like systems
        lib_name = "libtest_lib.so"
        compile_cmd = ["gcc", "-shared", "-fPIC", "-o", lib_name, "test_lib.c"]
    
    print(f"Building {lib_name} on {system}...")
    
    try:
        # Change to the examples directory
        os.chdir(os.path.dirname(__file__))
        
        # Compile the library
        result = subprocess.run(compile_cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            print(f"Successfully built {lib_name}")
            print(f"Library location: {os.path.abspath(lib_name)}")
            return lib_name
        else:
            print(f"Failed to build {lib_name}")
            print(f"Error: {result.stderr}")
            return None
            
    except Exception as e:
        print(f"Error building library: {e}")
        return None

def test_with_tauraro(lib_name):
    """Create a Tauraro test script to test the library"""
    
    test_script = f"""
# Test script for FFI with {lib_name}

print("Testing FFI with custom library...")

# Load the library
try:
    load_library("{lib_name}")
    print("Library loaded successfully")
except Exception as e:
    print(f"Failed to load library: {{e}}")
    exit(1)

# Define functions
try:
    define_function("{lib_name}", "add_integers", "int32", ["int32", "int32"])
    define_function("{lib_name}", "multiply_doubles", "double", ["double", "double"])
    define_function("{lib_name}", "string_length", "int32", ["string"])
    define_function("{lib_name}", "greet", "string", ["string"])
    define_function("{lib_name}", "say_hello", "void", [])
    print("Functions defined successfully")
except Exception as e:
    print(f"Failed to define functions: {{e}}")
    exit(1)

# Test function calls
try:
    # Test integer addition
    result = call_function("{lib_name}", "add_integers", [5, 3])
    print(f"5 + 3 = {{result}}")
    
    # Test double multiplication
    result = call_function("{lib_name}", "multiply_doubles", [2.5, 4.0])
    print(f"2.5 * 4.0 = {{result}}")
    
    # Test string length
    result = call_function("{lib_name}", "string_length", ["Hello, World!"])
    print(f"Length of 'Hello, World!' = {{result}}")
    
    # Test string greeting
    result = call_function("{lib_name}", "greet", ["Tauraro"])
    print(f"Greeting: {{result}}")
    
    # Test void function
    call_function("{lib_name}", "say_hello", [])
    print("say_hello() called successfully")
    
    print("All tests passed!")
    
except Exception as e:
    print(f"Error calling functions: {{e}}")
    import traceback
    traceback.print_exc()
"""

    # Write the test script
    with open("test_ffi.tauraro", "w") as f:
        f.write(test_script)
    
    print("Created test_ffi.tauraro")
    print("Run with: cargo run -- run examples/test_ffi.tauraro")

if __name__ == "__main__":
    lib_name = build_test_library()
    
    if lib_name:
        test_with_tauraro(lib_name)
        print("\nTo test the FFI functionality:")
        print("1. Make sure the library is in a location where it can be found")
        print("2. Run: cargo run -- run examples/test_ffi.tauraro")
    else:
        print("Failed to build test library")