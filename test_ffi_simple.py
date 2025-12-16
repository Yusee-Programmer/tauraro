# Simple FFI test for C transpiler
# Test loading a library and calling a function

# Load the C standard library
if load_library("libc.so.6"):
    print("Library loaded successfully")

    # Define the strlen function
    define_function("libc.so.6", "strlen", "int", ["string"])

    # Call strlen
    test_str = "Hello, FFI!"
    length = call_function("strlen", test_str)
    print("String length:", length)
else:
    print("Failed to load library")
