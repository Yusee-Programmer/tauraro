import gui

def main():
    print("=== Event-Driven Tauraro GUI Demo ===")
    
    # Create main window
    hwnd = gui.create_window("Event-Driven GUI Demo", 500, 300)
    if not hwnd:
        print("Failed to create window")
        return
    
    print(f"Window created: {hwnd}")
    
    # Create controls
    button1 = gui.create_button(hwnd, "Click Me!", 50, 50, 100, 30, 1001)
    button2 = gui.create_button(hwnd, "Play Sound", 50, 100, 100, 30, 1002)
    button3 = gui.create_button(hwnd, "Close", 50, 150, 100, 30, 1003)
    
    textbox = gui.create_textbox(hwnd, 200, 50, 200, 25, 2001)
    
    if all([button1, button2, button3, textbox]):
        print("All controls created")
    else:
        print("Failed to create some controls")
    
    # Show window
    gui.show_window(hwnd)
    print("Window displayed")
    
    # Set window position to center
    screen_size = gui.get_screen_size()
    x = (screen_size[0] - 500) // 2
    y = (screen_size[1] - 300) // 2
    gui.set_window_position(hwnd, x, y)
    
    print("\nApplication is running. Close the window to exit.")
    print("Features demonstrated:")
    print("- Multiple button controls")
    print("- Text input control")
    print("- Window positioning")
    print("- Event-driven behavior")
    
    # In a real implementation, we would have a message loop here
    # For this demo, we'll just wait for the window to be closed
    # The application will exit when you close the window manually
    
    # Simulate a simple event loop
    try:
        # This would normally be replaced with a proper Windows message loop
        input("Press Enter to close the application...")
    except KeyboardInterrupt:
        pass
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Application closed.")

if __name__ == "__main__":
    main()