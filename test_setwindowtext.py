# Test SetWindowTextA directly
user32 = load_library("user32.dll")
kernel32 = load_library("kernel32.dll")

CreateWindowExA = define_function("user32.dll", "CreateWindowExA", "pointer", ["int", "pointer", "pointer", "int", "int", "int", "int", "int", "pointer", "pointer", "pointer", "pointer"])
SetWindowTextA = define_function("user32.dll", "SetWindowTextA", "int", ["pointer", "pointer"])
GetModuleHandleA = define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])

h_instance = GetModuleHandleA(None)
hwnd = CreateWindowExA(0, "BUTTON", "Test", 0xcf0000 | 0x10000000, 0x80000000, 0x80000000, 400, 300, None, None, h_instance, None)

print(f"hwnd: {hwnd}")

print("Calling SetWindowTextA...")
result = SetWindowTextA(hwnd, "New Title")
print(f"Result: {result}")
print(f"Result type: {type(result)}")

print("Testing comparison...")
if result == None:
    print("Result is None")
else:
    print(f"Result is not None: {result}")

print("Done!")
