# DUITK Demo - Fixed Version with Visible Windows
# This version uses the fixed DUITK that displays windows on screen

print("=" * 70)
print("DUITK Demo - Native Windows GUI with Visible Windows!")
print("=" * 70)
print()

# Import DUITK
import duitk

# Create application
print("Creating application...")
app = duitk.Application("DUITK Demo App")

# Create main window
print("Creating main window...")
window = app.create_window("DUITK Demo - Main Window", 800, 600)

# Add controls
print("Adding controls to window...")
label1 = window.create_label("Welcome to DUITK v2.0!", 20, 20, 300, 30)
label2 = window.create_label("All windows are now visible!", 20, 55, 300, 25)

button1 = window.create_button("Button 1", 20, 90, 100, 30)
button2 = window.create_button("Button 2", 130, 90, 100, 30)
button3 = window.create_button("Button 3", 240, 90, 100, 30)

edit1 = window.create_edit("Text input 1", 20, 130, 320, 25)
edit2 = window.create_edit("Text input 2", 20, 165, 320, 25)

info_label = window.create_label(f"Created {len(window.controls)} controls successfully!", 20, 200, 400, 25)

print(f"\n✓ Window created with {len(window.controls)} controls")
print(f"  Window HWND: {window.hwnd}")
print(f"  Controls:")
for i, ctrl in enumerate(window.controls):
    print(f"    {i+1}. HWND: {ctrl.hwnd}")

# Create a second window
print("\nCreating second window...")
window2 = app.create_window("DUITK Demo - Second Window", 400, 300)
window2.create_label("This is a second window!", 20, 20, 250, 30)
window2.create_button("Click Me", 20, 60, 120, 30)

print(f"✓ Second window created! HWND: {window2.hwnd}")

print("\n" + "=" * 70)
print("Ready to display windows!")
print("=" * 70)

# Run the application (windows will be visible for 15 seconds)
app.run_simple(15)

print("\n" + "=" * 70)
print("Demo Complete!")
print("All windows were visible on your screen!")
print("=" * 70)
