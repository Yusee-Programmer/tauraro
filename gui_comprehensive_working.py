# Comprehensive Tauraro GUI Demo (Working Version)
# Demonstrates GUI features that are currently implemented

import gui

def main():
    print("=== Tauraro Comprehensive GUI Demo ===")
    
    # Get screen information using our implemented functions
    screen_size = gui.get_screen_size()
    screen_width = screen_size[0]
    screen_height = screen_size[1]
    print(f"Screen size: {screen_width}x{screen_height}")
    
    # Create main window (using existing function)
    window_width = 500
    window_height = 400
    hwnd = gui.create_window("Comprehensive GUI Demo", window_width, window_height)
    if not hwnd:
        print("Failed to create window")
        return
    
    print("✓ Main window created")
    
    # Create some buttons
    print("Creating buttons...")
    button1 = gui.create_button(hwnd, "Show Message", 50, 50, 120, 30, 1001)
    button2 = gui.create_button(hwnd, "About", 50, 100, 120, 30, 1002)
    button3 = gui.create_button(hwnd, "Quit", 50, 150, 120, 30, 1003)
    
    if button1 and button2 and button3:
        print("✓ Buttons created")
    else:
        print("✗ Failed to create buttons")
    
    # Create textboxes
    print("Creating textboxes...")
    textbox1 = gui.create_textbox(hwnd, 200, 50, 200, 25, 2001)
    textbox2 = gui.create_textbox(hwnd, 200, 100, 200, 25, 2002)
    
    if textbox1 and textbox2:
        print("✓ Textboxes created")
    else:
        print("✗ Failed to create textboxes")
    
    # Show the window
    gui.show_window(hwnd)
    print("✓ Window displayed")
    
    # Demonstrate window positioning functions
    print("Demonstrating window positioning...")
    rect = gui.get_window_rect(hwnd)
    if rect:
        print(f"Window rectangle: {rect}")
    
    # Move window to a specific position
    result = gui.set_window_position(hwnd, 100, 100)
    print(f"Window positioned: {result}")
    
    # Get and set window title
    title = gui.get_window_title(hwnd)
    print(f"Window title: {title}")
    gui.set_window_title(hwnd, "Comprehensive GUI Demo - Active")
    
    # Check if window is visible
    visible = gui.is_window_visible(hwnd)
    print(f"Window visible: {visible}")
    
    # Show a message box
    gui.message_box("Welcome", "Welcome to the Tauraro Comprehensive GUI Demo!", gui.MB_OK | gui.MB_ICONINFORMATION)
    
    # Play some system sounds
    print("Playing system sounds...")
    gui.beep_info()
    gui.beep_warning()
    gui.beep_error()
    
    # Demonstrate all implemented features
    print("\nDemo features:")
    print("  ✓ Window creation and positioning")
    print("  ✓ Button controls")
    print("  ✓ Text input controls")
    print("  ✓ Message boxes")
    print("  ✓ Window management functions")
    print("  ✓ System information")
    print("  ✓ System sounds")
    
    print("\nWindow is now visible. Close it to exit demo.")
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("\n=== Demo Completed ===")

if __name__ == "__main__":
    main()