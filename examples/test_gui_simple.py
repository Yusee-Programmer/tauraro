# Simple GUI example using the tauraro gui package
# This demonstrates creating a simple message box with native Windows API

import gui

# Test 1: Simple message box
print("Test 1: Showing a simple message box...")
result = gui.message_box("Hello from Tauraro!", "Tauraro GUI Demo", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"Message box result: {result}")

# Test 2: Message box with different icon
print("\nTest 2: Showing a warning message box...")
result = gui.message_box("This is a warning!", "Warning", gui.MB_OKCANCEL | gui.MB_ICONWARNING)
print(f"Message box result: {result}")

# Test 3: Message box with error icon
print("\nTest 3: Showing an error message box...")
result = gui.message_box("This is an error!", "Error", gui.MB_OK | gui.MB_ICONERROR)
print(f"Message box result: {result}")

print("\nGUI tests completed!")
