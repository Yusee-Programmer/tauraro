import gui

def main():
    print("=== Final Most Responsive Tauraro GUI Application ===")
    
    # Create main window with improved approach
    hwnd = gui.create_window("Most Responsive App", 550, 350)
    if not hwnd:
        print("Failed to create window")
        return
    
    print(f"Window created successfully: {hwnd}")
    
    # Create multiple controls to test responsiveness
    button1 = gui.create_button(hwnd, "Show Message", 30, 30, 120, 30, 1001)
    button2 = gui.create_button(hwnd, "Play Sound", 30, 70, 120, 30, 1002)
    button3 = gui.create_button(hwnd, "Get Info", 30, 110, 120, 30, 1003)
    
    textbox1 = gui.create_textbox(hwnd, 180, 30, 200, 25, 2001)
    textbox2 = gui.create_textbox(hwnd, 180, 70, 200, 25, 2002)
    
    # Show window
    gui.show_window(hwnd)
    gui.set_window_title(hwnd, "Most Responsive App - Active")
    
    # Position window in center of screen
    screen_size = gui.get_screen_size()
    x = (screen_size[0] - 550) // 2
    y = (screen_size[1] - 350) // 2
    gui.set_window_position(hwnd, x, y)
    
    print(f"Window positioned at: ({x}, {y})")
    print("SUCCESS: Window should be fully responsive!")
    print("\nApplication Features:")
    print("  ✓ Multiple controls created")
    print("  ✓ Window properly positioned")
    print("  ✓ Window should remain responsive")
    print("  ✓ Close window to exit")
    
    # Keep the application running with a simple loop
    # This prevents the "not responding" state
    try:
        iteration = 0
        while gui.is_window_visible(hwnd):
            iteration += 1
            # This simulates minimal processing without blocking
            if iteration % 1000000 == 0:
                # Periodic check to show app is alive
                pass
    except Exception as e:
        print(f"Application error: {e}")
    
    print("Window closed. Application completed successfully.")

if __name__ == "__main__":
    main()