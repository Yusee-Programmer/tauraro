import gui

# Print some basic information
print("GUI module loaded successfully")
print("Checking for specific functions...")

# Check if basic functions exist
if hasattr(gui, 'create_window'):
    print("✓ create_window function available")
else:
    print("✗ create_window function missing")

if hasattr(gui, 'create_button'):
    print("✓ create_button function available")
else:
    print("✗ create_button function missing")

if hasattr(gui, 'get_screen_size'):
    print("✓ get_screen_size function available")
else:
    print("✗ get_screen_size function missing")