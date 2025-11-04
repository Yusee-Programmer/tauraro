# Working Window GUI Demo in Tauraro
# A complete GUI application that stays open until user closes it manually

print("Loading DUITK...")
import duitk

# Create the main application
app = duitk.Application("Working GUI Demo")

# Create a simple window
window = app.create_window("Working GUI Window - Close to Exit", 600, 400)

print("Application running... Close the window to exit.")
print("Window handle:", window.hwnd)

# Run the application - this keeps it open until manually closed
app.run()

print("Application has exited.")