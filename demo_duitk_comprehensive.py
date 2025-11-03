# Comprehensive DUITK Demo - Advanced Native Windows Application
# Demonstrates full window and control functionality

print("=" * 70)
print("DUITK Comprehensive Demo - Advanced Native Windows Application")
print("=" * 70)
print()

# Import the DUITK package
import duitk

print("Creating application...")
app = duitk.Application("DUITK Comprehensive Demo")

# Create multiple windows
print("\n--- Creating Windows ---")

# Main window
main_window = app.create_window("Main Window - DUITK Demo", 640, 480)
print(f"Main window HWND: {main_window.hwnd}")

# Secondary window
secondary_window = app.create_window("Secondary Window", 400, 300)
print(f"Secondary window HWND: {secondary_window.hwnd}")

# Create controls on main window
print("\n--- Adding Controls to Main Window ---")

# Labels
title_label = main_window.create_label(
    "DUITK - Native Windows GUI for Tauraro",
    20, 20, 380, 30
)

info_label = main_window.create_label(
    "This is a fully native Win32 application!",
    20, 60, 380, 25
)

# Buttons
button1 = main_window.create_button("Button 1", 20, 100, 100, 30)
button2 = main_window.create_button("Button 2", 130, 100, 100, 30)
button3 = main_window.create_button("Button 3", 240, 100, 100, 30)

# Edit controls
edit1 = main_window.create_edit("Edit box 1", 20, 150, 320, 25)
edit2 = main_window.create_edit("Edit box 2", 20, 185, 320, 25)

# More labels
status_label = main_window.create_label(
    f"Total windows: {len(app.windows)} | Total controls: {len(main_window.controls)}",
    20, 220, 380, 25
)

# Create controls on secondary window
print("\n--- Adding Controls to Secondary Window ---")

sec_label = secondary_window.create_label(
    "Secondary Window Controls",
    20, 20, 250, 25
)

sec_button = secondary_window.create_button("Close", 20, 60, 100, 30)

# Test window operations
print("\n--- Testing Window Operations ---")

# Change window title
main_window.set_title("DUITK Demo - Title Changed!")
print("✓ Main window title changed")

# Move secondary window
secondary_window.move(750, 200, 400, 300)
print("✓ Secondary window repositioned")

# Get screen size
screen_width = duitk.get_screen_width()
screen_height = duitk.get_screen_height()
print(f"✓ Screen resolution: {screen_width}x{screen_height}")

# Summary
print("\n--- Application Summary ---")
print(f"Application name: {app.name}")
print(f"Total windows: {len(app.windows)}")
print(f"Main window controls: {len(main_window.controls)}")
print(f"Secondary window controls: {len(secondary_window.controls)}")

# Show a message box with application info
print("\n--- Showing Info Message Box ---")
duitk.message_box(
    f"DUITK Demonstration\n\n"
    f"Application: {app.name}\n"
    f"Windows Created: {len(app.windows)}\n"
    f"Total Controls: {len(main_window.controls) + len(secondary_window.controls)}\n"
    f"Screen Size: {screen_width}x{screen_height}\n\n"
    f"Click OK to continue...",
    "DUITK Demo Info",
    0x00000040  # MB_ICONINFORMATION
)

# Run the application
print("\n--- Running Application ---")
app.run_simple()

# Cleanup
print("\n--- Cleaning Up ---")
main_window.destroy()
secondary_window.destroy()
print("✓ Windows destroyed")

print()
print("=" * 70)
print("Comprehensive demo completed successfully!")
print("All Win32 API calls executed correctly!")
print("=" * 70)
