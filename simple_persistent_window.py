# Simple Persistent Window in Tauraro
# A minimal GUI application that stays open until user closes it manually

# Import the DUITK package for GUI functionality
import duitk

# Create the main application
app = duitk.Application("Persistent Window Demo")

# Create a simple window that stays open
window = app.create_window("Persistent Window - Close to Exit", 500, 300)

# Add a label to the window with instructions
label = window.create_label(
    "This window will stay open until you close it manually. Click the 'X' button in the title bar to exit.",
    20, 20, 460, 80
)

# Print confirmation that the window was created
print("Window created successfully!")
print("Window handle:", window.hwnd)
print("Window title:", window.title)
print("Close the window manually to exit the application.")

# Run the application - this keeps it open until manually closed
app.run()

print("Application has exited.")