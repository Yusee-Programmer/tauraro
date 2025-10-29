import gui

def main():
    print("=== Simple Responsive GUI App ===")
    
    # Create main window
    hwnd = gui.create_window("Simple App", 400, 200)
    if not hwnd:
        print("Failed to create window")
        return
    
    print(f"Window created successfully: {hwnd}")
    
    # Show window
    gui.show_window(hwnd)
    print("Window displayed")
    
    # Position window
    screen_size = gui.get_screen_size()
    x = (screen_size[0] - 400) // 2
    y = (screen_size[1] - 200) // 2
    gui.set_window_position(hwnd, x, y)
    print(f"Window positioned at: ({x}, {y})")
    
    print("Application is running. Close the window to exit.")
    
    # Simple loop to keep app running
    try:
        # Just wait for window to be closed
        while gui.is_window_visible(hwnd):
            pass  # In a real app, this would be a proper message loop
        print("Window closed by user")
    except:
        print("Application interrupted")
    
    print("Application exiting")

if __name__ == "__main__":
    main()