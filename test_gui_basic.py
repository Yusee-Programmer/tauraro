# Basic GUI test
from gui import Window, show_info

print("Creating window...")
win = Window("Test Window", 400, 300)

print("Window created successfully!")
print(f"Title: {win.title}")

print("Showing message box...")
show_info("Window created!", "Success")

print("Testing set_title...")
result = win.set_title("New Title")
print(f"set_title result: {result}")

print("Done!")
