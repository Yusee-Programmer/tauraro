# Test step by step GUI
print("Importing gui_stepbystep...")
from gui_stepbystep import Window
print("Import successful!")

print("\nCreating window...")
win = Window("Step by Step Test", 400, 300)

print("\nKeeping window alive for 5 seconds...")
win.keep_alive(5)

print("\nDone!")
