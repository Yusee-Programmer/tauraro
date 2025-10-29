import gui

print("Testing simple GUI functionality...")

# Test basic functionality
result = gui.message_box("Test", "Test", gui.MB_OK)
print(f"Message box result: {result}")

# Test window creation
hwnd = gui.create_window("Test Window", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    gui.destroy_window(hwnd)
    print("Window destroyed")

print("Test completed.")