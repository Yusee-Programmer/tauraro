# Simple GUI test in Tauraro
# Test to isolate the issue

print("Loading DUITK...")
import duitk

print("Creating application...")
app = duitk.Application("Test App")

print("Creating window...")
window = app.create_window("Test Window", 400, 300)

if window and window.hwnd:
    print(f"Window created successfully (HWND: {window.hwnd})")
    
    print("Creating a label...")
    label = window.create_label("Hello, World!", 10, 10, 200, 25)
    print(f"Label created (HWND: {label.hwnd})")
    
    print("Number of controls:", len(window.controls))
    
    print("Starting message loop...")
    app.run()
else:
    print("Failed to create window!")

print("Done.")