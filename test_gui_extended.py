import gui

print("Testing extended GUI functionality...")

# Test screen size functions
print("Testing screen size functions...")
screen_width = gui.get_screen_width()
screen_height = gui.get_screen_height()
screen_size = gui.get_screen_size()
print(f"Screen width: {screen_width}")
print(f"Screen height: {screen_height}")
print(f"Screen size: {screen_size[0]}x{screen_size[1]}")

# Test window creation
print("\nTesting window creation...")
hwnd = gui.create_window("Test Window", 300, 200)
if hwnd:
    print(f"Window created with handle: {hwnd}")
    
    # Test window positioning
    print("Testing window positioning...")
    result = gui.set_window_position(hwnd, 100, 100)
    print(f"Window positioned: {result}")
    
    # Test window rectangle
    print("Testing window rectangle...")
    rect = gui.get_window_rect(hwnd)
    print(f"Window rectangle: {rect}")
    
    # Test window title
    print("Testing window title functions...")
    original_title = gui.get_window_title(hwnd)
    print(f"Original title: {original_title}")
    
    result = gui.set_window_title(hwnd, "Updated Title")
    print(f"Title update result: {result}")
    
    new_title = gui.get_window_title(hwnd)
    print(f"New title: {new_title}")
    
    # Test window visibility
    print("Testing window visibility...")
    visible = gui.is_window_visible(hwnd)
    print(f"Window visible: {visible}")
    
    # Test move window
    print("Testing move window...")
    result = gui.move_window(hwnd, 150, 150, 350, 250)
    print(f"Window moved: {result}")
    
    # Test beep functions
    print("Testing beep functions...")
    gui.beep_info()
    gui.beep_warning()
    gui.beep_error()
    gui.beep_question()
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Failed to create window")

print("Extended GUI test completed.")