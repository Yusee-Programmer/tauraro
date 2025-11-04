# Minimal test to understand FFI calls

print("Loading libraries...")
load_library("kernel32.dll")
load_library("user32.dll")

print("Defining functions...")
define_function("kernel32.dll", "GetModuleHandleA", "pointer", ["pointer"])
define_function("user32.dll", "CreateWindowExA", "pointer", ["uint32", "pointer", "pointer", "uint32", "int32", "int32", "int32", "int32", "pointer", "pointer", "pointer", "pointer"])
define_function("user32.dll", "ShowWindow", "int32", ["pointer", "int32"])
define_function("user32.dll", "UpdateWindow", "int32", ["pointer"])
define_function("kernel32.dll", "Sleep", "void", ["uint32"])
define_function("user32.dll", "DestroyWindow", "int32", ["pointer"])
define_function("kernel32.dll", "GetLastError", "uint32", [])

hinstance = call_function("kernel32.dll", "GetModuleHandleA", [0])
print(f"Module handle: {hinstance}")

# Try to create a simple window like in the working demo
print("Creating window...")

# Window style: WS_OVERLAPPEDWINDOW | WS_VISIBLE
style = 0x10000000 | 0x00CF0000

window = call_function("user32.dll", "CreateWindowExA", [
    0,
    "BUTTON",
    "Test Window",
    style,
    200,
    200,
    400,
    300,
    0,
    0,
    hinstance,
    0
])

print(f"Window HWND: {window}")

if window and window != 0:
    print("Window created successfully!")
    # Show the window
    call_function("user32.dll", "ShowWindow", [window, 5])
    call_function("user32.dll", "UpdateWindow", [window])
    
    # Keep it visible for a few seconds
    call_function("kernel32.dll", "Sleep", [5000])
    
    # Destroy the window
    call_function("user32.dll", "DestroyWindow", [window])
    print("Window destroyed")
else:
    print("Failed to create window")
    # Get last error
    try:
        error = call_function("kernel32.dll", "GetLastError", [])
        print(f"Last error code: {error}")
    except:
        pass

# Minimal test to isolate the issue

print("Loading DUITK...")
import duitk

print("Creating application...")
app = duitk.Application("Test App")

print("Creating window...")
window = app.create_window("Test Window", 400, 300)

if window and window.hwnd:
    print(f"Window created successfully (HWND: {window.hwnd})")
    print("Window attributes:", list(window.__dict__.keys()))
    
    # Try to access controls
    print("Controls list:", window.controls)
    print("Number of controls:", len(window.controls))
    
    print("Test completed successfully.")
else:
    print("Failed to create window!")

print("Done.")
