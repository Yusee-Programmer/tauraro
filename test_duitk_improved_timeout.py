# Test DUITK with improved timeout - responsive window that auto-closes
# This demonstrates the improved run_simple() with proper message processing

import duitk

print("\n=== DUITK Improved Timeout Test ===")
print("This test creates a window with a 10-second auto-close timeout.")
print("The window will be responsive and you can close it manually.\n")

# Create application
app = duitk.Application("Timeout Test")

# Create window
window = app.create_window("Auto-close in 10 seconds", 500, 350)

# Check if window was created (workaround for attribute access bug)
if window != None:
    print(f"\n✓ Window created successfully")
    print("\n" + "="*60)
    print("INSTRUCTIONS:")
    print("  - The window is now visible and responsive")
    print("  - It will auto-close after 10 seconds")
    print("  - OR you can close it manually by clicking the X button")
    print("  - The window should NOT show 'Not Responding'")
    print("="*60 + "\n")

    # Run with 10 second timeout (but can be closed manually)
    app.run_simple(10)

    print("\n✓ Application exited cleanly")
else:
    print("\n✗ Failed to create window")
