import gui

print("Testing textbox GUI functionality...")

# Test basic functionality
result = gui.message_box("Test", "Test", gui.MB_OK)
print(f"Message box result: {result}")

# Test window creation
hwnd = gui.create_window("Test Window", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Test textbox creation
    textbox_hwnd = gui.create_textbox(hwnd, 50, 50, 100, 25, 2001)
    if textbox_hwnd:
        print(f"Textbox created: {textbox_hwnd}")
    else:
        print("Failed to create textbox")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Failed to create window")

print("Test completed.")