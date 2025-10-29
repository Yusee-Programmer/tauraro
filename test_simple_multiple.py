import gui

print("Testing simple multiple button creation...")

# Create main window
hwnd = gui.create_window("Simple Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Create 3 buttons one by one
    print("Creating button 1...")
    button1 = gui.create_button(hwnd, "Button 1", 50, 50, 100, 30, 1001)
    print(f"Button 1: {button1}")
    
    print("Creating button 2...")
    button2 = gui.create_button(hwnd, "Button 2", 50, 100, 100, 30, 1002)
    print(f"Button 2: {button2}")
    
    print("Creating button 3...")
    button3 = gui.create_button(hwnd, "Button 3", 50, 150, 100, 30, 1003)
    print(f"Button 3: {button3}")
    
    if button1 and button2 and button3:
        print("All 3 buttons created successfully!")
    else:
        print("Some buttons failed to create")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")