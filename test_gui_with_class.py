# Test GUI with class
print("Importing gui_test...")
from gui_test import Window, show_info
print("Import successful!")

print("Creating window instance...")
win = Window("My App")
win.show()

show_info("Class test passed!", "GUI Test")
print("Done!")
