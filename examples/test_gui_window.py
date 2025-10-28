# Advanced GUI example - Creating a native Windows window
# This demonstrates creating and managing a native window

import gui

print("Creating a native Windows window...")
print()

# Create a native window
print("Creating window...")
hwnd = gui.create_window("My Tauraro Application", 640, 480)
print(f"Window created with handle: {hwnd}")
print()

# Show the window
print("Showing window...")
gui.show_window(hwnd)
print("Window is now visible on your screen!")
print()

# Display a message box to keep the window visible
result = gui.message_box("Window created successfully! Close this dialog to continue.", "Success", gui.MB_OK | gui.MB_ICONINFORMATION)
print()

# Hide the window
print("Hiding window...")
gui.hide_window(hwnd)
print("Window is now hidden.")
print()

# Show another message
result = gui.message_box("Window is now hidden. It will be destroyed when you close this dialog.", "Info", gui.MB_OK | gui.MB_ICONINFORMATION)
print()

# Destroy the window
print("Destroying window...")
gui.destroy_window(hwnd)
print("Window destroyed.")
print()

print("Done!")
