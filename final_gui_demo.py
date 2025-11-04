# Final Window GUI Demo in Tauraro
# A complete GUI application that stays open until user closes it manually

print("Loading DUITK...")
import duitk

# Create the main application
app = duitk.Application("Final GUI Demo")

# Create a simple window
window = app.create_window("Final GUI Window - Close to Exit", 600, 400)

# Add a label to the window
label = window.create_label("This window will stay open until you close it manually.", 20, 20, 560, 30)

# Add a button to the window
button = window.create_button("Click Me!", 20, 60, 100, 30)

print("Application running... Close the window to exit.")
print("Window handle:", window.hwnd)

# Run the application - this keeps it open until manually closed
app.run()

print("Application has exited.")