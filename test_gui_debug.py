# Debug GUI test
from gui import Window

print("Creating window...")
win = Window("Test Window", 400, 300)

print(f"\nAfter creation:")
print(f"win.hwnd = {win.hwnd}")
print(f"win.title = {win.title}")

print("\nTesting set_title...")
result = win.set_title("New Title")
print(f"set_title result: {result}")

print("\nDone!")
