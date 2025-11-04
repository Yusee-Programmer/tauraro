# Final Comprehensive Window GUI Demo in Tauraro
# A complete GUI application that demonstrates various DUITK features

print("=" * 60)
print("FINAL COMPREHENSIVE GUI DEMO")
print("DUITK - Desktop UI Toolkit for Tauraro")
print("=" * 60)
print()

# Import the DUITK package for GUI functionality
import duitk

print("1. Creating application...")
# Create the main application
app = duitk.Application("Comprehensive GUI Demo")

print("2. Creating main window...")
# Create the main window with specified dimensions
main_window = app.create_window("Comprehensive GUI Demo - Close to Exit", 700, 500)

# Check if window was created successfully
if main_window and main_window.hwnd:
    print(f"   ✓ Main window created successfully (HWND: {main_window.hwnd})")
else:
    print("   ✗ Failed to create main window!")
    exit(1)

print("3. Adding UI controls to the window...")

# Create title label at the top
title_label = main_window.create_label(
    "Welcome to the Tauraro Comprehensive GUI Demo!",
    20, 20, 660, 30
)
print("   ✓ Title label created")

# Create information section
info_label1 = main_window.create_label(
    "This is a native Windows application built with Tauraro DUITK.",
    20, 60, 660, 25
)

info_label2 = main_window.create_label(
    "The window will remain open until you close it manually.",
    20, 85, 660, 25
)
print("   ✓ Information labels created")

# Create interactive buttons
print("4. Creating interactive buttons...")

button_panel_label = main_window.create_label(
    "Interactive Controls:",
    20, 130, 200, 25
)

# Action buttons
btn_show_msg = main_window.create_button("Show Message", 20, 160, 150, 35)
btn_change_title = main_window.create_button("Change Window Title", 180, 160, 150, 35)
btn_move_window = main_window.create_button("Move Window", 340, 160, 150, 35)
print("   ✓ Action buttons created")

# Create text input section
print("5. Creating text input controls...")

input_panel_label = main_window.create_label(
    "Text Input Controls:",
    20, 210, 200, 25
)

# Text input fields
name_label = main_window.create_label("Your Name:", 20, 240, 100, 25)
name_input = main_window.create_edit("Enter your name", 120, 240, 200, 25)

message_label = main_window.create_label("Message:", 20, 275, 100, 25)
message_input = main_window.create_edit("Type your message here...", 120, 275, 300, 25)
print("   ✓ Text input controls created")

# Create a text display area
display_label = main_window.create_label(
    "Display Area:",
    20, 320, 200, 25
)

display_area = main_window.create_edit(
    "This is a multi-line display area.\nYou can enter text above and see it displayed.\nTry clicking the buttons to interact with the application!",
    20, 350, 500, 80
)
print("   ✓ Display area created")

print("\n" + "=" * 60)
print("APPLICATION READY")
print("=" * 60)
print("Window handle:", main_window.hwnd)
print("Total controls created:", len(main_window.controls))
print()
print("INSTRUCTIONS:")
print("1. A window titled '" + main_window.title + "' should be visible")
print("2. You can interact with all the controls")
print("3. Close the window manually when you're done")
print("4. The application will run until the window is closed")
print("=" * 60)

# Run the application message loop
# This will keep the application running until the user closes the window
print("\nStarting application message loop...")
print("The window is now visible on your screen.")
print("Close the window manually to exit the application.")
app.run()

print("\n" + "=" * 60)
print("APPLICATION HAS EXITED")
print("Thank you for trying the DUITK GUI framework!")
print("=" * 60)