# Window Positioning Demo for Tauraro GUI
# Demonstrates window positioning and sizing functions

import gui

def main():
    # Get screen information
    screen_width, screen_height = gui.get_screen_size()
    print(f"Screen size: {screen_width}x{screen_height}")
    
    # Create main window
    hwnd = gui.create_window("Positioning Demo", 300, 200)
    if not hwnd:
        print("Failed to create window")
        return
    
    # Show the window
    gui.show_window(hwnd)
    
    # Move window to different positions
    print("Moving window to different positions...")
    
    # Move to top-left
    gui.set_window_position(hwnd, 0, 0)
    gui.beep_info()
    
    # Wait a bit (in a real app this would be handled by the message loop)
    # For demo purposes, we'll just show the capabilities
    
    # Move to center of screen
    center_x = (screen_width - 300) // 2
    center_y = (screen_height - 200) // 2
    gui.set_window_position(hwnd, center_x, center_y)
    gui.beep_info()
    
    # Get window rectangle
    rect = gui.get_window_rect(hwnd)
    if rect:
        print(f"Window position: {rect}")
    
    # Show window information
    title = gui.get_window_title(hwnd)
    print(f"Window title: {title}")
    
    # Change window title
    gui.set_window_title(hwnd, "Positioning Demo - Updated")
    
    # Check if window is visible
    visible = gui.is_window_visible(hwnd)
    print(f"Window visible: {visible}")
    
    # Run message loop for a short time
    print("Positioning demo completed. Close window to exit.")
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Application closed.")

if __name__ == "__main__":
    main()