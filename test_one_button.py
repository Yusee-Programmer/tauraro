import gui

print("Testing one button creation...")

# Create main window
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Create one button
    print("Creating one button...")
    button1 = gui.create_button(hwnd, "Button 1", 50, 50, 100, 30, 1001)
    
    if button1:
        print(f"Button created successfully: {button1}")
    else:
        print("Failed to create button")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")