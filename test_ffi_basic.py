# Test basic FFI functionality
print("Testing basic FFI...")

# Try loading a library
print("Loading user32.dll...")
user32 = load_library("user32.dll")
print(f"Loaded: {user32}")

# Try defining a simple function
print("Defining MessageBoxA...")
MessageBoxA = define_function("user32.dll", "MessageBoxA", "int", ["pointer", "pointer", "pointer", "int"])
print(f"Defined: {MessageBoxA}")

print("FFI test complete!")
