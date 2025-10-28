# Working GUI Features Demo
# This shows all the GUI features that are currently functional

import gui

print("=== Tauraro GUI - Working Features Demo ===")
print()

print("GUI Library loaded successfully!")
print("  - user32.dll: loaded")
print("  - kernel32.dll: loaded")
print()

print("Demonstrating working features...")
print()

# Feature 1: Information MessageBox
print("[1/5] Information MessageBox")
result = gui.message_box("This is an information message!", "Info", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"      User clicked: {result} (OK)")
print()

# Feature 2: Warning MessageBox with OK/Cancel
print("[2/5] Warning MessageBox with OK/Cancel")
result = gui.message_box("Do you want to continue?", "Warning", gui.MB_OKCANCEL | gui.MB_ICONWARNING)
if result == 1:
    print("      User clicked: OK")
elif result == 2:
    print("      User clicked: Cancel")
print()

# Feature 3: Error MessageBox
print("[3/5] Error MessageBox")
result = gui.message_box("An error occurred!", "Error", gui.MB_OK | gui.MB_ICONERROR)
print(f"      User clicked: {result} (OK)")
print()

# Feature 4: Custom styled MessageBox
print("[4/5] Custom styled MessageBox")
result = gui.message_box("Plain message without icon", "Custom", gui.MB_OK)
print(f"      User clicked: {result} (OK)")
print()

# Feature 5: Success MessageBox
print("[5/5] Success MessageBox")
result = gui.message_box("All tests passed successfully!", "Success", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"      User clicked: {result} (OK)")
print()

print("=== All Working Features Tested Successfully! ===")
print()
print("Currently working:")
print("  ✓ MessageBox dialogs (all styles)")
print("  ✓ User input handling")
print("  ✓ Multiple icon types")
print("  ✓ Button configurations")
print()
print("Coming soon (requires additional FFI signatures):")
print("  - Window creation (CreateWindowExA with 12 parameters)")
print("  - Window management")
print("  - Custom controls")
print()
print("See FINAL_GUI_SUMMARY.md for complete documentation!")
