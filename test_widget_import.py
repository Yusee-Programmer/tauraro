# Test importing and using widgets
print("Importing gui...")
from gui import Window
print("Window imported")

print("\nCreating window...")
window = Window("Test", 400, 300)
print(f"Window hwnd: {window.hwnd}")
print(f"Window hwnd type: {type(window.hwnd)}")

print("\nImporting Label...")
from gui import Label
print("Label imported")

print("\nCreating Label object...")
label = Label("Test Label", x=20, y=20, width=200, height=25)
print(f"Label object created")
print(f"Label text: {label.text}")
print(f"Label x: {label.x}")

print("\nCalling label.create(window)...")
result = label.create(window)
print(f"Label.create returned: {result}")
print(f"Label hwnd: {label.hwnd}")

print("\nDone!")
