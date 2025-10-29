import gui

print("Testing button GUI functionality...")

# Test basic functionality
result = gui.message_box("Test", "Test", gui.MB_OK)
print(f"Message box result: {result}")

# Test window creation
hwnd = gui.create_window("Test Window", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Test button creation
    button_hwnd = gui.create_button(hwnd, "Click Me!", 50, 50, 100, 30, 1001)
    if button_hwnd:
        print(f"Button created: {button_hwnd}")
    else:
        print("Failed to create button")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Failed to create window")

print("Test completed.")