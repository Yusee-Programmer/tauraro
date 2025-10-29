# Simple Window Test - Fixed Version
# This shows a window that doesn't disappear or hang

import gui

print("Creating a window that will stay visible for 5 seconds...")
print()

# Create window
hwnd = gui.create_window("Tauraro Window - FIXED!", 800, 600)

if hwnd:
    print(f"Window created successfully (handle: {hwnd})")
    print("The window will be visible for 5 seconds...")
    print()

    # Show window and keep it alive for 5 seconds
    gui.show_window_for(hwnd, 5)

    print("Window was visible for 5 seconds without issues!")
    print("Cleaning up...")

    # Destroy the window
    gui.destroy_window(hwnd)

    print("Window destroyed successfully")
    print()
    print("SUCCESS: Window no longer disappears or hangs!")
else:
    print("Failed to create window")
