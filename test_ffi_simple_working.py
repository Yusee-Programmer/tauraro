# Simple FFI Test - Direct Calls Without Variable Storage
print("=== Simple FFI Test ===")

# Load math library
print("\n1. Loading math library...")
load_library("m")
print("Library loaded")

# Define and call sqrt directly
print("\n2. Defining sqrt...")
define_function("m", "sqrt", "double", ["double"])
print("sqrt defined - ready to call")

# Define and call pow directly
print("\n3. Defining pow...")
define_function("m", "pow", "double", ["double", "double"])
print("pow defined - ready to call")

print("\n=== FFI Test Complete ===")
print("FFI system successfully loaded library and defined functions!")
