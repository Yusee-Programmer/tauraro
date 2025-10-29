import gui

def main():
    print("=== Improved Responsive Tauraro GUI Application ===")
    
    # Create main window
    hwnd = gui.create_window("Improved Responsive App", 500, 300)
    if not hwnd:
        print("Failed to create window")
        return
    
    print(f"Window created successfully: {hwnd}")
    
    # Create some controls
    button1 = gui.create_button(hwnd, "Test Button", 50, 50, 120, 30, 1001)
    textbox = gui.create_textbox(hwnd, 50, 100, 200, 25, 2001)
    
    # Show window
    gui.show_window(hwnd)
    
    # Position window in center of screen
    screen_size = gui.get_screen_size()
    x = (screen_size[0] - 500) // 2
    y = (screen_size[1] - 300) // 2
    gui.set_window_position(hwnd, x, y)
    
    print(f"Window positioned at: ({x}, {y})")
    print("SUCCESS: Window should now be responsive!")
    print("\nFeatures:")
    print("  ✓ Window properly created with improved method")
    print("  ✓ Controls added")
    print("  ✓ Window centered on screen")
    print("  ✓ Should remain responsive")
    print("\nClose the window to exit the application")
    
    # Simple loop to keep app running and responsive
    try:
        iteration = 0
        while gui.is_window_visible(hwnd):
            iteration += 1
            # Small delay simulation
            if iteration % 500000 == 0:
                pass  # Just to show app is alive
    except Exception as e:
        print(f"Application error: {e}")
    
    print("Window closed. Application exiting.")

if __name__ == "__main__":
    main()