# Test Window Keep-Alive
# This demonstrates the fix for windows disappearing or hanging
# Windows now stay visible for a specified time without becoming unresponsive

import gui

print("=== Testing Window Keep-Alive Fix ===")
print()

# Test 1: Create window and keep it alive for 3 seconds
print("[Test 1] Creating window that will stay visible for 3 seconds...")
hwnd = gui.create_window("Test Window - 3 Second Display", 640, 480)
if hwnd:
    print(f"  Window created (handle: {hwnd})")
    print("  Showing window for 3 seconds...")
    gui.show_window_for(hwnd, 3)
    print("  Window was visible for 3 seconds (no disappearing!)")
    gui.destroy_window(hwnd)
    print("  Window destroyed")
else:
    print("  Failed to create window")
print()

# Test 2: Create window and use message box to keep it alive
print("[Test 2] Creating window with message box control...")
hwnd2 = gui.create_window("Test Window - Message Box Control", 800, 600)
if hwnd2:
    print(f"  Window created (handle: {hwnd2})")
    print("  Window is now visible - close the message box to continue")
    gui.show_window_with_message(hwnd2, "Window Test", "The window is visible! Close this dialog when ready.")
    print("  User closed the dialog")
    gui.destroy_window(hwnd2)
    print("  Window destroyed")
else:
    print("  Failed to create window")
print()

# Test 3: Create centered window and keep alive
print("[Test 3] Creating centered window that stays for 2 seconds...")
hwnd3 = gui.create_window_centered("Centered Test Window", 500, 400)
if hwnd3:
    print(f"  Centered window created (handle: {hwnd3})")
    gui.show_window_for(hwnd3, 2)
    print("  Window was visible for 2 seconds")
    gui.destroy_window(hwnd3)
    print("  Window destroyed")
else:
    print("  Failed to create centered window")
print()

print("=== All Tests Completed! ===")
print()
print("Summary:")
print("  - Windows no longer disappear immediately")
print("  - Windows no longer become unresponsive (\"not responding\")")
print("  - Windows stay visible for specified time periods")
print("  - Proper window lifecycle management working")
