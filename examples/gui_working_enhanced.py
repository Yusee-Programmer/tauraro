# Working Enhanced Tauraro GUI Demo
# Demonstrates buttons and textboxes without causing overflow

import gui

def main():
    print("=== Tauraro Working Enhanced GUI Demo ===")
    
    # Show a message box first
    result = gui.message_box("GUI Demo", "Welcome to the enhanced GUI demo!", gui.MB_OK | gui.MB_ICONINFORMATION)
    print(f"Message box result: {result}")
    
    # Create main window
    hwnd = gui.create_window("Enhanced GUI Demo", 400, 300)
    if not hwnd:
        print("Failed to create window")
        return
    
    print(f"Window created with handle: {hwnd}")
    
    # Create some buttons
    print("Creating buttons...")
    button1 = gui.create_button(hwnd, "Button 1", 50, 50, 100, 30, 1001)
    button2 = gui.create_button(hwnd, "Button 2", 50, 100, 100, 30, 1002)
    button3 = gui.create_button(hwnd, "Quit", 50, 150, 100, 30, 1003)
    
    if button1 and button2 and button3:
        print("All buttons created successfully")
    else:
        print("Failed to create one or more buttons")
    
    # Create textboxes
    print("Creating textboxes...")
    textbox1 = gui.create_textbox(hwnd, 200, 50, 150, 25, 2001)
    textbox2 = gui.create_textbox(hwnd, 200, 100, 150, 25, 2002)
    
    if textbox1 and textbox2:
        print("All textboxes created successfully")
    else:
        print("Failed to create one or more textboxes")
    
    # Show the window
    gui.show_window(hwnd)
    print("Window is now visible")
    
    print("\nDemo features:")
    print("  ✓ Window creation")
    print("  ✓ Button controls")
    print("  ✓ Text input controls")
    print("  ✓ Message boxes")
    print("  ✓ Window management")
    
    print("\nClose the window manually to exit the demo.")
    
    # Wait for user to close the window manually
    # In a real application, we would have a message loop here
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("\nWindow destroyed. Demo completed.")

if __name__ == "__main__":
    main()