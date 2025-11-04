# Test DUITK Window Creation - Debugging why windows aren't showing

print("=" * 60)
print("DUITK Window Creation Test - Debug Version")
print("=" * 60)
print()

# Import the DUITK package
import duitk

print("Creating application...")
app = duitk.Application("DUITK Test App")

print("\nCreating window...")
window = app.create_window("Test Window", 400, 300)

print(f"\nWindow details:")
print(f"  HWND: {window.hwnd}")
print(f"  Title: {window.title}")
print(f"  Size: {window.width}x{window.height}")
print(f"  Visible: {window.visible}")

# Try to explicitly show the window
if window.hwnd:
    print("\nExplicitly showing window...")
    window.show()
    
    # Add a label to test control creation
    print("\nCreating a test label...")
    label = window.create_label("Test Label", 10, 10, 200, 25)
    print(f"Label HWND: {label.hwnd}")
    
    # Show the label explicitly
    if label.hwnd:
        label.show()

print("\nWindow should now be visible. If not, there's an issue with DUITK.")

# Keep the program running briefly
# Use the FFI functions directly like in the working demo
call_function("kernel32.dll", "Sleep", [5000])

print("\nTest completed.")