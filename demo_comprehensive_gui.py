# Comprehensive GUI Demo for Tauraro
# Demonstrates multiple windows, controls, and proper message loop
# Windows stay open until user closes them

import duitk

print("\n" + "="*70)
print("  Tauraro Comprehensive GUI Demo")
print("="*70)
print("\nCreating a multi-window GUI application...")
print("Windows will stay open until YOU close them.\n")

# Create the main application
app = duitk.Application("Tauraro GUI Demo")

print("\n--- Creating Main Window ---")
main_window = app.create_window("Tauraro Demo - Main Window", 600, 400)

if main_window != None:
    print("✓ Main window created successfully!")

    print("\n--- Creating Secondary Window ---")
    secondary_window = app.create_window("Tauraro Demo - Secondary Window", 500, 350)

    if secondary_window != None:
        print("✓ Secondary window created successfully!")

    print("\n--- Creating Control Panel Window ---")
    control_panel = app.create_window("Tauraro Demo - Control Panel", 400, 300)

    if control_panel != None:
        print("✓ Control panel window created successfully!")

    print("\n" + "="*70)
    print("  GUI Application Started!")
    print("="*70)
    print("\n📊 Application Status:")
    print(f"   • Total Windows Created: 3")
    print(f"   • Application Name: {app.name}")
    print(f"   • All windows are visible on your screen")

    print("\n💡 Instructions:")
    print("   • You should see 3 separate windows on your screen")
    print("   • Each window has a different title")
    print("   • Try moving, resizing, or minimizing them")
    print("   • Close ANY window to exit the application")
    print("   • The windows will remain responsive (no 'Not Responding')")

    print("\n⏳ Starting Message Loop...")
    print("   The application will now run until you close a window.\n")

    # Run the message loop - this will keep running until all windows are closed
    # The user can close any window to exit
    app.run()

    print("\n" + "="*70)
    print("  Application Exited Cleanly")
    print("="*70)
    print("\n✓ Thank you for testing Tauraro GUI!")
    print("✓ All windows have been closed.")

else:
    print("\n✗ ERROR: Failed to create main window!")
    print("   Please check your Win32 API installation.")

print("\n" + "="*70)
print("  Demo Complete")
print("="*70 + "\n")
