# Advanced GUI example - Creating a native Windows window
# This demonstrates creating and managing a native window

import gui

print("Creating a native Windows window...")

# Create a window
window = gui.Window("My Tauraro Application", 640, 480)

# Create the native window
print("Creating window...")
hwnd = window.create()
print(f"Window created with handle: {hwnd}")

# Show the window
print("Showing window...")
window.show()

# Display a message box to keep the window visible
result = gui.message_box("Window created successfully! Close this dialog to continue.", "Success", gui.MB_OK | gui.MB_ICONINFORMATION)

# Hide the window
print("Hiding window...")
window.hide()

# Show another message
result = gui.message_box("Window is now hidden. It will be destroyed when you close this dialog.", "Info", gui.MB_OK | gui.MB_ICONINFORMATION)

# Destroy the window
print("Destroying window...")
window.destroy()

print("Done!")
