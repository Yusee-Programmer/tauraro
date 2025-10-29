# Test GUI library constants and basic functionality
# This example tests that the GUI library loads correctly

import gui

print("=== Tauraro GUI Library Test ===")
print()

# Test that constants are accessible
print("Window Style Constants:")
print(f"  WS_OVERLAPPEDWINDOW = {gui.WS_OVERLAPPEDWINDOW} (hex: {hex(gui.WS_OVERLAPPEDWINDOW)})")
print(f"  WS_VISIBLE = {gui.WS_VISIBLE} (hex: {hex(gui.WS_VISIBLE)})")
print(f"  CW_USEDEFAULT = {gui.CW_USEDEFAULT} (hex: {hex(gui.CW_USEDEFAULT)})")
print()

print("Show Window Constants:")
print(f"  SW_SHOW = {gui.SW_SHOW}")
print(f"  SW_HIDE = {gui.SW_HIDE}")
print()

print("Message Box Constants:")
print(f"  MB_OK = {gui.MB_OK}")
print(f"  MB_OKCANCEL = {gui.MB_OKCANCEL}")
print(f"  MB_ICONINFORMATION = {gui.MB_ICONINFORMATION} (hex: {hex(gui.MB_ICONINFORMATION)})")
print(f"  MB_ICONWARNING = {gui.MB_ICONWARNING} (hex: {hex(gui.MB_ICONWARNING)})")
print(f"  MB_ICONERROR = {gui.MB_ICONERROR} (hex: {hex(gui.MB_ICONERROR)})")
print()

print("Window Message Constants:")
print(f"  WM_DESTROY = {gui.WM_DESTROY} (hex: {hex(gui.WM_DESTROY)})")
print(f"  WM_CLOSE = {gui.WM_CLOSE} (hex: {hex(gui.WM_CLOSE)})")
print(f"  WM_COMMAND = {gui.WM_COMMAND} (hex: {hex(gui.WM_COMMAND)})")
print()

print("GUI library loaded successfully!")
print()
print("Available features:")
print("  - Window style constants (WS_*)")
print("  - Message box constants (MB_*)")
print("  - Window show commands (SW_*)")
print("  - Window message constants (WM_*)")
print("  - Window class (gui.Window)")
print("  - message_box() function")
print()
print("Note: Full GUI functionality requires extended FFI signature support.")
print("The FFI system currently needs to support (pointer, pointer, pointer, int) -> int signature.")

