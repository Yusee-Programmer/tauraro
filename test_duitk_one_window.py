# Test DUITK with just one window

import duitk

print("\n=== Creating DUITK Application ===")
app = duitk.Application("Test Application")

print("\n=== Creating Window ===")
window1 = app.create_window("Test Window", 400, 300)

print(f"\n=== Window HWND: {window1.hwnd} ===")

print("\n=== Keeping window visible for 5 seconds ===")
app.run_simple(5)

print("\nDone!")
