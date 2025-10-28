print("=== Tauraro FFI Direct Call Test ===")
print("")

# Load the kernel32 library
print("Loading kernel32.dll...")
load_library("kernel32.dll")
print("✓ Library loaded")
print("")

# Define functions and get callable function objects
print("Defining GetTickCount function...")
GetTickCount = define_function("kernel32.dll", "GetTickCount", "uint32", [])
print(f"✓ GetTickCount function object: {GetTickCount}")
print(f"✓ Function type: {type(GetTickCount)}")
print("")

print("Defining GetCurrentProcessId function...")
GetCurrentProcessId = define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])
print("✓ GetCurrentProcessId function object created")
print("")

print("Defining Sleep function...")
Sleep = define_function("kernel32.dll", "Sleep", "void", ["uint32"])
print("✓ Sleep function object created")
print("")

# Test calling the functions directly
print("=== Testing Direct Function Calls ===")

print("Calling GetTickCount() directly...")
tick1 = GetTickCount()
print(f"✓ Tick count: {tick1}")
print("")

print("Calling GetCurrentProcessId() directly...")
pid = GetCurrentProcessId()
print(f"✓ Process ID: {pid}")
print("")

print("Calling Sleep(100) directly...")
start_time = GetTickCount()
Sleep(100)
end_time = GetTickCount()
elapsed = end_time - start_time
print(f"✓ Slept for approximately {elapsed}ms")
print("")

print("=== Test Complete ===")
print("✓ FFI functions can be called directly as Tauraro function objects!")