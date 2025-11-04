# Test if IsWindow works correctly

load_library("kernel32.dll")
load_library("user32.dll")

define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])
define_function("user32.dll", "CreateWindowExA", "pointer", ["uint32", "pointer", "pointer", "uint32", "int32", "int32", "int32", "int32", "pointer", "pointer", "pointer", "pointer"])
define_function("user32.dll", "ShowWindow", "int32", ["pointer", "int32"])
define_function("user32.dll", "IsWindow", "int32", ["pointer"])
define_function("kernel32.dll", "Sleep", "void", ["uint32"])

print("Creating window...")
hinstance = call_function("kernel32.dll", "GetModuleHandleA", [0])
style = 0x10CF0000

hwnd = call_function("user32.dll", "CreateWindowExA", [
    0, "BUTTON", "Test Window", style,
    200, 200, 400, 300,
    0, 0, hinstance, 0
])

print(f"Window created: HWND={hwnd}")
print(f"HWND type: {type(hwnd)}")

# Show the window
call_function("user32.dll", "ShowWindow", [hwnd, 5])

# Test IsWindow immediately
print("\nTesting IsWindow immediately after creation...")
result = call_function("user32.dll", "IsWindow", [hwnd])
print(f"IsWindow({hwnd}) = {result}")
print(f"Result type: {type(result)}")

# Sleep for 5 seconds while checking IsWindow
print("\nTesting IsWindow in a loop for 5 seconds...")
count = 0
while count < 50:
    is_valid = call_function("user32.dll", "IsWindow", [hwnd])
    if count % 10 == 0:
        print(f"  Iteration {count}: IsWindow = {is_valid}")
    call_function("kernel32.dll", "Sleep", [100])
    count += 1

print("\nTest complete!")
