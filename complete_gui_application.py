import gui

def main():
    print("=== Complete Tauraro GUI Application ===")
    
    # Get screen information
    screen_size = gui.get_screen_size()
    print(f"Screen size: {screen_size[0]}x{screen_size[1]}")
    
    # Create main application window
    window_width = 600
    window_height = 400
    hwnd = gui.create_window("Complete GUI Application", window_width, window_height)
    if not hwnd:
        print("Failed to create main window")
        return
    
    print(f"Main window created: {hwnd}")
    
    # Create a variety of controls
    print("Creating UI controls...")
    
    # Row 1 - Buttons
    btn_message = gui.create_button(hwnd, "Show Message", 30, 30, 120, 30, 1001)
    btn_beep = gui.create_button(hwnd, "Play Beep", 160, 30, 120, 30, 1002)
    btn_about = gui.create_button(hwnd, "About", 290, 30, 120, 30, 1003)
    btn_exit = gui.create_button(hwnd, "Exit", 420, 30, 120, 30, 1004)
    
    # Row 2 - Textboxes
    txt_name = gui.create_textbox(hwnd, 30, 80, 200, 25, 2001)
    txt_email = gui.create_textbox(hwnd, 30, 120, 200, 25, 2002)
    txt_message = gui.create_textbox(hwnd, 30, 160, 300, 60, 2003)  # Larger for multi-line
    
    # Row 3 - Additional controls (would need more functions for checkboxes, etc.)
    
    # Check if all controls were created successfully
    controls = [btn_message, btn_beep, btn_about, btn_exit, txt_name, txt_email, txt_message]
    if all(controls):
        print("All UI controls created successfully")
    else:
        print("Warning: Some controls may not have been created")
    
    # Position window in the center of the screen
    center_x = (screen_size[0] - window_width) // 2
    center_y = (screen_size[1] - window_height) // 2
    gui.set_window_position(hwnd, center_x, center_y)
    
    # Show the window
    gui.show_window(hwnd)
    gui.set_window_title(hwnd, "Complete GUI Application - Ready")
    
    print(f"Window positioned at: ({center_x}, {center_y})")
    print("Window is now visible and ready for interaction")
    
    # Demonstrate some functionality
    print("\nApplication Features:")
    print("  ✓ Main window with title bar and borders")
    print("  ✓ Multiple button controls")
    print("  ✓ Text input controls")
    print("  ✓ Centered window positioning")
    print("  ✓ Screen size detection")
    print("  ✓ Window title management")
    
    print("\nTry interacting with the controls:")
    print("- Click buttons to see different actions")
    print("- Type in textboxes to enter data")
    print("- Close the window to exit the application")
    
    # In a full implementation, we would run a proper Windows message loop
    # For this demo, we'll just indicate that the application is running
    print("\n=== Application Running ===")
    print("The application will continue running until you close the window.")
    print("Press Ctrl+C in this terminal to force exit if needed.")
    
    # Simple event loop simulation
    try:
        # In a real Windows application, this would be replaced with:
        # gui.message_loop() or equivalent Windows message handling
        while True:
            # Check if window is still visible
            if not gui.is_window_visible(hwnd):
                break
            # In a real implementation, we would process messages here
    except KeyboardInterrupt:
        print("\nApplication interrupted by user")
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Application closed successfully")

if __name__ == "__main__":
    main()