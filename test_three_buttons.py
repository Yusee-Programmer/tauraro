import gui

print("Testing three button creation...")

# Create main window
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Create three buttons
    print("Creating three buttons...")
    button1 = gui.create_button(hwnd, "Button 1", 50, 50, 100, 30, 1001)
    button2 = gui.create_button(hwnd, "Button 2", 50, 100, 100, 30, 1002)
    button3 = gui.create_button(hwnd, "Button 3", 50, 150, 100, 30, 1003)
    
    if button1 and button2 and button3:
        print(f"Buttons created successfully: {button1}, {button2}, {button3}")
    else:
        print("Failed to create one or more buttons")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")