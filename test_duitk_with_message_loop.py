# Test DUITK with proper message loop - stays open until you close it
# This demonstrates the window staying responsive and not showing "Not Responding"

import duitk

print("\n=== DUITK Message Loop Test ===")
print("This test creates a window that stays open until YOU close it.")
print("The window will be responsive and won't show 'Not Responding'.\n")

# Create application
app = duitk.Application("Message Loop Test")

# Create window
window = app.create_window("Close Me to Exit!", 500, 350)

# Check if window was created (workaround for attribute access bug)
if window != None:
    print(f"\n✓ Window created successfully")
    print("\n" + "="*60)
    print("INSTRUCTIONS:")
    print("  - The window is now visible on your screen")
    print("  - It will stay open UNTIL YOU CLOSE IT")
    print("  - Try moving it, resizing it (it should be responsive)")
    print("  - Click the X button to close the window and exit")
    print("="*60 + "\n")

    # Run the message loop - stays open until user closes window
    app.run()

    print("\n✓ Application exited cleanly")
else:
    print("\n✗ Failed to create window")
