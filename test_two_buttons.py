import gui

print("Testing two button creation...")

# Create main window
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Create two buttons
    print("Creating two buttons...")
    button1 = gui.create_button(hwnd, "Button 1", 50, 50, 100, 30, 1001)
    button2 = gui.create_button(hwnd, "Button 2", 50, 100, 100, 30, 1002)
    
    if button1 and button2:
        print(f"Buttons created successfully: {button1}, {button2}")
    else:
        print("Failed to create one or more buttons")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")