import gui

print("Testing basic GUI functionality...")

# Test screen size
screen_size = gui.get_screen_size()
print(f"Screen size: {screen_size[0]}x{screen_size[1]}")

# Test window creation
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Test button creation
    button = gui.create_button(hwnd, "Test Button", 50, 50, 100, 30, 1001)
    if button:
        print(f"Button created: {button}")
    else:
        print("Button creation failed")
    
    # Test textbox creation
    textbox = gui.create_textbox(hwnd, 50, 100, 100, 25, 2001)
    if textbox:
        print(f"Textbox created: {textbox}")
    else:
        print("Textbox creation failed")
    
    # Test window functions
    result = gui.set_window_position(hwnd, 100, 100)
    print(f"Window positioned: {result}")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")