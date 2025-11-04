# Simple test to isolate the issue

import duitk

app = duitk.Application("Test")
window = app.create_window("Test", 300, 200)

if window:
    print("Window created successfully")
else:
    print("Failed to create window")