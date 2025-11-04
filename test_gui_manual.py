# Manual GUI Test - Close windows yourself
import duitk

print("Creating GUI application...")
print("The windows will stay open until you close one.")
print()

app = duitk.Application("Tauraro Test")
window1 = app.create_window("Test Window 1", 400, 300)
window2 = app.create_window("Test Window 2", 400, 300)

if window1 != None and window2 != None:
    print("\n✓ Windows created!")
    print("✓ Now running message loop...")
    print("✓ Try clicking, moving, resizing the windows")
    print("✓ Close any window to exit\n")

    app.run()

    print("\n✓ Application exited cleanly!")
else:
    print("\n✗ Failed to create windows")

print("\nTest complete!")
