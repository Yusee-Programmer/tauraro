# Simple Tauraro GUI Demo
# Testing widgets one by one

from gui import Window, Button, Label, show_info

print("Creating window...")
window = Window("Simple GUI Demo", 600, 400)
print(f"Window created: {window.hwnd}")

# Add a label
print("Adding label...")
label = Label("Welcome to Tauraro GUI!", x=20, y=20, width=300, height=25)
result = label.create(window)
print(f"Label created: {result}")

# Add a button
print("Adding button...")
button = Button("Click Me", x=20, y=60, width=120, height=35)
result = button.create(window)
print(f"Button created: {result}")

# Add another label
print("Adding second label...")
label2 = Label("This is a simple demo", x=20, y=110, width=300, height=25)
result = label2.create(window)
print(f"Label2 created: {result}")

print("\nShowing info dialog...")
show_info("Simple GUI Demo is running!", "Info")

print("\nKeeping window alive for 15 seconds...")
window.keep_alive(15)

print("Closing...")
window.destroy()
print("Done!")
