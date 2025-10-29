import gui

print("Testing message box function...")

# Create a simple window first
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Test message box
    print("Showing message box...")
    result = gui.message_box("Test Message", "Test Title", gui.MB_OK | gui.MB_ICONINFORMATION)
    print(f"Message box result: {result}")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Message box test completed.")