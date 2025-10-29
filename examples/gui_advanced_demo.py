# Advanced Tauraro GUI Demo
# Demonstrates buttons, textboxes, and event handling

import gui

def main():
    # Initialize common controls
    gui.init_common_controls()
    
    # Create main window
    hwnd = gui.create_window("Advanced GUI Demo", 600, 400)
    if not hwnd:
        print("Failed to create window")
        return
    
    # Create some buttons
    button1 = gui.create_button(hwnd, "Click Me!", 50, 50, 100, 30, 1001)
    button2 = gui.create_button(hwnd, "Quit", 50, 100, 100, 30, 1002)
    
    # Create textboxes
    textbox1 = gui.create_textbox(hwnd, 200, 50, 200, 25, 2001)
    textbox2 = gui.create_textbox(hwnd, 200, 100, 200, 25, 2002)
    
    # Set some initial text
    gui.set_control_text(textbox1, "Enter text here")
    
    # Show the window
    gui.show_window(hwnd)
    
    # Run message loop
    print("Window created. Click buttons or close window to exit.")
    gui.message_loop()
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Application closed.")

if __name__ == "__main__":
    main()