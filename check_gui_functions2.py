import gui

# Check if specific functions exist
functions_to_check = [
    'init_common_controls',
    'create_window_centered',
    'create_menu',
    'add_menu_item',
    'set_window_menu',
    'set_control_text',
    'get_control_text',
    'show_info'
]

print("Checking for advanced GUI functions...")
for func_name in functions_to_check:
    if hasattr(gui, func_name):
        print(f"✓ {func_name} function available")
    else:
        print(f"✗ {func_name} function missing")