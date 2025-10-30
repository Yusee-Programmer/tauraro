# Test with positional arguments
from gui import Window, Label

print("Creating window...")
window = Window("Test", 400, 300)
print(f"Window hwnd: {window.hwnd}")

print("\nCreating Label with positional args...")
label = Label("Test Label", 20, 20, 200, 25)
print(f"Label text: {label.text}")
print(f"Label x: {label.x}")
print(f"Label y: {label.y}")

print("\nCalling label.create(window)...")
result = label.create(window)
print(f"Result: {result}")
print(f"Label hwnd: {label.hwnd}")

print("\nSuccess!")
