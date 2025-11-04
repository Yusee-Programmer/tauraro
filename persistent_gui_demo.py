# Persistent Window GUI Demo in Tauraro
# A complete GUI application that stays open until user closes it manually

print("Loading DUITK...")
import duitk

# Create the main application
app = duitk.Application("Persistent GUI Demo")

# Create a simple window
window = app.create_window("Persistent GUI Window - Close to Exit", 600, 400)

print("Application running... Close the window to exit.")
print("Window handle:", window.hwnd)
print("Window title:", window.title)
print("Window size:", window.width, "x", window.height)

# Show a simple message to confirm the window is running
print("\nA window titled '" + window.title + "' should now be visible on your screen.")
print("The application will continue running until you close the window.")
print("You can close the window by clicking the 'X' button in the title bar.")

# Run the application - this keeps it open until manually closed
# Using run_simple with a long timeout to ensure the window stays open
app.run()

print("\nApplication has exited.")
print("Thank you for trying the DUITK GUI framework!")