# Final GUI test with keep_alive
from gui import Window, show_info

print("=== Tauraro GUI Test ===")
print("\nCreating window...")
win = Window("Tauraro GUI Demo", 500, 350)

print(f"Window created: {win.title}")
print(f"Window handle: {win.hwnd}")

print("\nShowing info dialog...")
show_info("Welcome to Tauraro GUI!", "Hello")

print("\nChanging window title...")
win.set_title("New Window Title")

print("\nWindow will stay alive for 10 seconds...")
print("Hover over the window - it should stay responsive!")
win.keep_alive(10)

print("\nDestroying window...")
win.destroy()

print("\nTest completed successfully!")
