# Test DUITK - Debug window handles

import duitk

print("\n=== DUITK Handle Debug Test ===\n")

# Create application
app = duitk.Application("Debug Test")

print(f"Initial window_handles: {app.window_handles}")
print(f"Type: {type(app.window_handles)}")
print(f"Length: {len(app.window_handles)}")

# Create window
window = app.create_window("Test Window", 400, 300)

print(f"\nAfter creating window:")
print(f"window_handles: {app.window_handles}")
print(f"Type: {type(app.window_handles)}")
print(f"Length: {len(app.window_handles)}")

if len(app.window_handles) > 0:
    print(f"\nFirst handle value: {app.window_handles[0]}")
    print(f"First handle type: {type(app.window_handles[0])}")

    # Try calling IsWindow on it
    print("\nTrying to call IsWindow...")
    result = call_function("user32.dll", "IsWindow", [app.window_handles[0]])
    print(f"IsWindow result: {result}")
else:
    print("\nERROR: No handles in list!")

print("\nTest complete.")
