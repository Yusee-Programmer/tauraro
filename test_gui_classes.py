# Test the new class-based GUI library
from gui import Window, Button, Label, show_info

print("Creating a window using the Window class...")
win = Window("My Tauraro App", 600, 400)

print(f"Window created: {win.title}")
print(f"Window size: {win.width}x{win.height}")
print(f"Window visible: {win.is_visible()}")

# Show an info dialog
show_info("Testing the new class-based GUI!", "Tauraro GUI Test")

# Change the window title
print("\nChanging window title...")
win.set_title("Updated Title!")

# Keep the window alive and responsive for 10 seconds
print("Window will stay open for 10 seconds...")
print("Try hovering over the window - it should remain responsive!")
win.keep_alive(10)

print("\nDestroying window...")
win.destroy()
print("Done!")
