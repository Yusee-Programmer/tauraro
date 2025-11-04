# Minimal Window GUI in Tauraro
# A simple GUI application that stays open until user closes it manually

print("Loading DUITK...")
import duitk

# Create the main application
app = duitk.Application("Minimal GUI Demo")

# Create a simple window
window = app.create_window("Minimal GUI Window", 600, 400)

# Show a message box to confirm the application is running
duitk.message_box(
    "The GUI window is now open.\nIt will stay open until you close it manually.",
    "GUI Running",
    0x00000040  # MB_ICONINFORMATION
)

print("Application running... Close the window to exit.")
# Run the application - this keeps it open until manually closed
app.run()