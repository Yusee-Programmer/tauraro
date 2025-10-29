import gui

def main():
    print("=== Responsive Tauraro GUI Application ===")
    
    # Create main window
    hwnd = gui.create_window("Responsive GUI App", 500, 300)
    if not hwnd:
        print("Failed to create window")
        return
    
    print(f"Window created: {hwnd}")
    
    # Create controls
    button1 = gui.create_button(hwnd, "Show Message", 50, 50, 120, 30, 1001)
    button2 = gui.create_button(hwnd, "Play Sound", 50, 100, 120, 30, 1002)
    button3 = gui.create_button(hwnd, "Get Info", 50, 150, 120, 30, 1003)
    
    textbox = gui.create_textbox(hwnd, 200, 50, 200, 25, 2001)
    
    # Show window
    gui.show_window(hwnd)
    gui.set_window_title(hwnd, "Responsive GUI App - Active")
    
    # Position window in center
    screen_size = gui.get_screen_size()
    x = (screen_size[0] - 500) // 2
    y = (screen_size[1] - 300) // 2
    gui.set_window_position(hwnd, x, y)
    
    print(f"Window positioned at: ({x}, {y})")
    print("Application is running and responsive!")
    print("Close the window to exit.")
    
    # For this demo, we'll use a simple approach:
    # Keep checking if the window is still visible
    # In a real application, we would use a proper message loop
    try:
        while gui.is_window_visible(hwnd):
            # Small delay to prevent excessive CPU usage
            # In a real app, this would be handled by the message loop
            pass
    except:
        # Handle any exceptions
        pass
    
    print("Window closed. Application exiting.")
    
    # Note: In a real Windows application, the window cleanup would be handled
    # by the window procedure when it receives WM_DESTROY message

if __name__ == "__main__":
    main()