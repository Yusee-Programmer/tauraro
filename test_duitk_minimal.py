# Minimal DUITK Test - Just create a window, no controls

print("=" * 60)
print("Minimal DUITK Test")
print("=" * 60)

import duitk

print("Creating application...")
app = duitk.Application("Test App")

print("Creating window...")
window = app.create_window("Test Window", 640, 480)

print(f"Window HWND: {window.hwnd}")
print("Running app for 10 seconds...")

app.run_simple(10)

print("Done!")
