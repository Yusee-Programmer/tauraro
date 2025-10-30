# Test the new class-based GUI library
from gui import Window, show_info

print("Creating a window using the Window class...")
win = Window("My Tauraro App", 600, 400)

print(f"Window created: {win.title}")
print(f"Window size: {win.width}x{win.height}")

# Show an info dialog
print("Showing info dialog...")
show_info("Testing the new class-based GUI!", "Tauraro GUI Test")

# Change the window title
print("\nChanging window title...")
win.set_title("Updated Title!")

# Keep the window alive and responsive for 5 seconds
print("Window will stay open for 5 seconds...")
print("Try hovering over the window - it should remain responsive!")
win.keep_alive(5)

print("\nDestroying window...")
win.destroy()
print("Done!")
