import gui

# Check if basic functions exist
print("Checking GUI functions...")

if hasattr(gui, 'init_common_controls'):
    print("init_common_controls: YES")
else:
    print("init_common_controls: NO")

if hasattr(gui, 'create_window_centered'):
    print("create_window_centered: YES")
else:
    print("create_window_centered: NO")