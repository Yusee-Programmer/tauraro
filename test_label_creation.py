# Test label creation step by step
user32 = load_library("user32.dll")
kernel32 = load_library("kernel32.dll")

CreateWindowExA = define_function("user32.dll", "CreateWindowExA", "pointer", ["int", "pointer", "pointer", "int", "int", "int", "int", "int", "pointer", "pointer", "pointer", "pointer"])
GetModuleHandleA = define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])

# Constants
WS_OVERLAPPEDWINDOW = 0xcf0000
WS_VISIBLE = 0x10000000
WS_CHILD = 0x40000000
CW_USEDEFAULT = 0x80000000

print("Creating main window...")
h_instance = GetModuleHandleA(None)
style = WS_OVERLAPPEDWINDOW | WS_VISIBLE

hwnd_parent = CreateWindowExA(0, "BUTTON", "Parent Window", style, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, None, None, h_instance, None)

print(f"Parent window created: {hwnd_parent}")
print(f"Parent type: {type(hwnd_parent)}")

print("\nCreating label...")
label_style = WS_VISIBLE | WS_CHILD
print(f"Label style: {label_style}")
print(f"Label style type: {type(label_style)}")

print("Calling CreateWindowExA for label...")
hwnd_label = CreateWindowExA(0, "STATIC", "Test Label", label_style, 20, 20, 300, 25, hwnd_parent, None, h_instance, None)

print(f"Label created: {hwnd_label}")
print("Success!")
