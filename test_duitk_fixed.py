# Test DUITK with StoreFast bug fix
# This should now work correctly!

import duitk

print("\n=== Creating DUITK Application ===")
app = duitk.Application("Test Application - StoreFast Fixed!")

print("\n=== Creating Windows ===")
window1 = app.create_window("DUITK Test Window 1", 400, 300)
window2 = app.create_window("DUITK Test Window 2", 350, 250)

if window1 != None:
    print(f"\n=== Window 1 HWND: {window1.hwnd} ===")
else:
    print("\n=== Window 1: None ===")

if window2 != None:
    print(f"=== Window 2 HWND: {window2.hwnd} ===")
else:
    print("=== Window 2: None ===")

print("\n=== Running Application (5 seconds) ===")
app.run_simple(5)

print("\nTest complete!")
