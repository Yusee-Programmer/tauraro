print("Testing FFI function objects")
print("")

print("Step 1: Load library")
load_library("kernel32.dll")
print("✓ Library loaded")

print("")
print("Step 2: Define function")
my_function = define_function("kernel32.dll", "GetTickCount", "uint32", [])
print(f"✓ Function defined: {my_function}")
print(f"Function type: {type(my_function)}")

print("")
print("Step 3: Call function directly using call_function")
result1 = call_function("kernel32.dll", "GetTickCount", [])
print(f"✓ Direct call result: {result1}")

print("")
print("Step 4: Try calling through variable")
result2 = my_function()
print(f"✓ Variable call result: {result2}")
print(f"Result type: {type(result2)}")
