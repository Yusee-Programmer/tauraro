# Simple DUITK Demo - Native Windows Application
# Demonstrates basic window creation using Tauraro's Win32 FFI

print("=" * 60)
print("DUITK Simple Demo - Native Windows Application")
print("=" * 60)
print()

# Import the DUITK package
import duitk

# Create an application
app = duitk.Application("My First Tauraro App")

# Create a main window
window = app.create_window("Hello from Tauraro!", 800, 600)

# Add some controls to the window
label = window.create_label("Welcome to DUITK!", 10, 10, 200, 30)
button1 = window.create_button("Click Me!", 10, 50, 120, 30)
button2 = window.create_button("Another Button", 140, 50, 120, 30)
edit = window.create_edit("Type here...", 10, 90, 250, 25)

# Show some window info
print()
print(f"Window created with handle: {window.hwnd}")
print(f"Controls created: {len(window.controls)}")
print()

# Run the application (simple mode with MessageBox)
print("Running application...")
app.run_simple()

print()
print("=" * 60)
print("Demo completed successfully!")
print("=" * 60)
