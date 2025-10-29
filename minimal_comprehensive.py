import gui

print("Minimal comprehensive test...")

# Test screen size
screen_size = gui.get_screen_size()
print(f"Screen: {screen_size[0]}x{screen_size[1]}")

# Create window
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window: {hwnd}")
    
    # Create controls
    button = gui.create_button(hwnd, "Button", 50, 50, 100, 30, 1001)
    textbox = gui.create_textbox(hwnd, 50, 100, 100, 25, 2001)
    print(f"Button: {button}, Textbox: {textbox}")
    
    # Window functions
    gui.set_window_position(hwnd, 100, 100)
    rect = gui.get_window_rect(hwnd)
    title = gui.get_window_title(hwnd)
    visible = gui.is_window_visible(hwnd)
    print(f"Rect: {rect}, Title: {title}, Visible: {visible}")
    
    # Update title
    gui.set_window_title(hwnd, "Updated")
    new_title = gui.get_window_title(hwnd)
    print(f"New title: {new_title}")
    
    # Beep functions
    print("Testing beeps...")
    gui.beep_info()
    gui.beep_warning()
    gui.beep_error()
    gui.beep_question()
    
    # Message box
    print("Testing message box...")
    result = gui.message_box("Test", "Test Message", gui.MB_OK)
    print(f"Message box result: {result}")
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Cleanup complete")

print("Test completed.")