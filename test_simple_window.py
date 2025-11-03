# Simple Window Test - Verify window displays on screen
# This creates a real visible window using native Win32 APIs

print("=== Simple Window Display Test ===")
print()

# Load libraries
print("Loading libraries...")
load_library("kernel32.dll")
load_library("user32.dll")

# Get module handle
print("Getting module handle...")
hinstance = call_function("kernel32.dll", "GetModuleHandleA", [0])
print(f"Module handle: {hinstance}")

# Create a window using a predefined window class
# We'll use "BUTTON" which can be used as a top-level window
print("\nCreating window...")

# Window styles: WS_OVERLAPPEDWINDOW | WS_VISIBLE
# WS_OVERLAPPED=0, WS_CAPTION=0x00C00000, WS_SYSMENU=0x00080000,
# WS_THICKFRAME=0x00040000, WS_MINIMIZEBOX=0x00020000, WS_MAXIMIZEBOX=0x00010000
# WS_VISIBLE=0x10000000
style = 0x10000000 | 0x00C00000 | 0x00080000 | 0x00040000 | 0x00020000 | 0x00010000

hwnd = call_function("user32.dll", "CreateWindowExA", [
    0,                    # dwExStyle
    "BUTTON",             # lpClassName - using standard BUTTON class
    "Tauraro Test Window - Click to Close",  # window title
    style,                # dwStyle
    100,                  # x position
    100,                  # y position
    600,                  # width
    400,                  # height
    0,                    # hWndParent
    0,                    # hMenu
    hinstance,            # hInstance
    0                     # lpParam
])

if hwnd == 0:
    print("ERROR: Failed to create window!")
    error_code = call_function("kernel32.dll", "GetLastError", [])
    print(f"Error code: {error_code}")
    exit(1)

print(f"✓ Window created! HWND: {hwnd}")

# Show the window
print("Showing window...")
show_result = call_function("user32.dll", "ShowWindow", [hwnd, 5])  # SW_SHOW = 5
print(f"ShowWindow result: {show_result}")

# Update the window
update_result = call_function("user32.dll", "UpdateWindow", [hwnd])
print(f"UpdateWindow result: {update_result}")

# Bring window to foreground
call_function("user32.dll", "SetForegroundWindow", [hwnd])

print("\n✓ Window should now be visible on your screen!")
print("  Look for a button-style window titled 'Tauraro Test Window'")

# Keep the window alive by showing a message box
# The window will stay visible until you close the message box
print("\nShowing message box (window will remain visible)...")
call_function("user32.dll", "MessageBoxA", [
    0,
    "The Tauraro window is now visible on your screen!\n\nClick OK to close the window.",
    "Window Visible!",
    0
])

# Destroy the window
print("\nDestroying window...")
call_function("user32.dll", "DestroyWindow", [hwnd])
print("✓ Window destroyed")

print("\n=== Test Complete ===")
