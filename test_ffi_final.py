# Simple FFI test without sys module
if load_library("libc.so.6"):
    define_function("libc.so.6", "strlen", "int", ["string"])
    result = call_function("strlen", "Hello FFI!")
    print("Length:", result)
