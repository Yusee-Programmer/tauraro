# Menu Demo for Tauraro GUI
# Demonstrates menu creation and handling

import gui

def main():
    # Create main window
    hwnd = gui.create_window("Menu Demo", 400, 300)
    if not hwnd:
        print("Failed to create window")
        return
    
    # Create a menu
    menu = gui.create_menu()
    if not menu:
        print("Failed to create menu")
        return
    
    # Add menu items
    gui.add_menu_item(menu, "File", 1000, 0x00000010)  # MF_POPUP
    gui.add_menu_item(menu, "Open", 1001)
    gui.add_menu_item(menu, "Save", 1002)
    gui.add_menu_item(menu, "Exit", 1003)
    
    # Add another menu
    gui.add_menu_item(menu, "Help", 2000, 0x00000010)  # MF_POPUP
    gui.add_menu_item(menu, "About", 2001)
    
    # Set menu for window
    gui.set_window_menu(hwnd, menu)
    
    # Show the window
    gui.show_window(hwnd)
    
    # Run message loop
    print("Menu demo running. Close window to exit.")
    gui.message_loop()
    
    # Cleanup
    gui.destroy_window(hwnd)
    print("Application closed.")

if __name__ == "__main__":
    main()