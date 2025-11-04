# Comprehensive Window GUI in Tauraro
# A complete GUI application that stays open until user closes it manually

print("=" * 70)
print("Comprehensive Window GUI Demo")
print("Native Windows Application - Stays Open Until Manually Closed")
print("=" * 70)
print()

# Import the DUITK package for GUI functionality
import duitk

# Create the main application
app = duitk.Application("Comprehensive GUI Demo")

print("Creating main application window...")
# Create the main window with specified dimensions
main_window = app.create_window("Comprehensive GUI Application", 800, 600)

# Check if window was created successfully
if main_window and main_window.hwnd:
    print(f"✓ Main window created successfully (HWND: {main_window.hwnd})")
else:
    print("✗ Failed to create main window!")
    exit(1)

print("\nAdding UI controls to the window...")

# Create title label at the top
title_label = main_window.create_label(
    "Welcome to the Tauraro Comprehensive GUI Demo!",
    20, 20, 760, 30
)
print("✓ Title label created")

# Create information section
info_label1 = main_window.create_label(
    "This is a fully native Windows application built with Tauraro.",
    20, 60, 760, 25
)

info_label2 = main_window.create_label(
    "The window will remain open until you close it manually.",
    20, 85, 760, 25
)
print("✓ Information labels created")

# Create interactive buttons
print("\nCreating interactive buttons...")

button_panel_label = main_window.create_label(
    "Interactive Controls:",
    20, 130, 200, 25
)

# Action buttons
btn_show_msg = main_window.create_button("Show Message", 20, 160, 150, 35)
btn_change_title = main_window.create_button("Change Window Title", 180, 160, 150, 35)
btn_move_window = main_window.create_button("Move Window", 340, 160, 150, 35)
btn_get_info = main_window.create_button("Get System Info", 500, 160, 150, 35)
print("✓ Action buttons created")

# Create text input section
print("\nCreating text input controls...")

input_panel_label = main_window.create_label(
    "Text Input Controls:",
    20, 210, 200, 25
)

# Text input fields
name_label = main_window.create_label("Your Name:", 20, 240, 100, 25)
name_input = main_window.create_edit("Enter your name", 120, 240, 200, 25)

email_label = main_window.create_label("Email:", 20, 275, 100, 25)
email_input = main_window.create_edit("user@example.com", 120, 275, 200, 25)

message_label = main_window.create_label("Message:", 20, 310, 100, 25)
message_input = main_window.create_edit("Type your message here...", 120, 310, 300, 25)
print("✓ Text input controls created")

# Create a text display area
display_label = main_window.create_label(
    "Display Area:",
    20, 350, 200, 25
)

display_area = main_window.create_edit(
    "This is a multi-line display area.\nYou can enter text here and see it displayed.\nTry clicking the buttons above to interact with the application!",
    20, 380, 500, 100
)
print("✓ Display area created")

# Create status bar
status_label = main_window.create_label(
    f"Status: Application ready | Controls: {len(main_window.controls)} | HWND: {main_window.hwnd}",
    0, 570, 800, 30
)
print("✓ Status bar created")

# Show welcome message
print("\nShowing welcome message...")
duitk.message_box(
    "Welcome to the Comprehensive GUI Demo!\n\nThis application demonstrates the capabilities of DUITK,\nTauraro's native Windows GUI framework.\n\nThe window will remain open until you close it manually.\nFeel free to interact with all the controls.",
    "Welcome - DUITK Demo",
    0x00000040  # MB_ICONINFORMATION
)

print("\nApplication is ready!")
print("Total controls created:", len(main_window.controls))
print("Window handle:", main_window.hwnd)

# Instructions for the user
print("\n" + "=" * 70)
print("APPLICATION RUNNING")
print("=" * 70)
print("The window is now visible on your screen.")
print("You can interact with all the controls.")
print("Close the window manually when you're done.")
print("The application will run until all windows are closed.")
print("=" * 70)

# Run the application message loop
# This will keep the application running until the user closes the window
try:
    app.run()
except KeyboardInterrupt:
    print("\nApplication interrupted by user")
except Exception as e:
    print(f"\nError running application: {e}")

print("\n" + "=" * 70)
print("Application has exited")
print("Thank you for trying the DUITK GUI framework!")
print("=" * 70)