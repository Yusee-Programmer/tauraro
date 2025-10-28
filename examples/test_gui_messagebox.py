# Simple working GUI example - MessageBox demonstrations
# Shows actual Windows message boxes on screen!

import gui

print("=== Tauraro Native Windows GUI Demo ===")
print()

# Test 1: Information MessageBox
print("Test 1: Showing an information message box...")
result = gui.message_box("Hello from Tauraro! This is a native Windows message box.", "Information", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"User clicked: {result}")
print()

# Test 2: Warning MessageBox with OK/Cancel
print("Test 2: Showing a warning with OK/Cancel...")
result = gui.message_box("This is a warning message. Click OK or Cancel.", "Warning", gui.MB_OKCANCEL | gui.MB_ICONWARNING)
if result == 1:
    print("User clicked OK")
elif result == 2:
    print("User clicked Cancel")
print()

# Test 3: Error MessageBox
print("Test 3: Showing an error message...")
result = gui.message_box("This is an error message!", "Error", gui.MB_OK | gui.MB_ICONERROR)
print(f"User clicked: {result}")
print()

# Test 4: Plain message box
print("Test 4: Showing a plain message...")
result = gui.message_box("This is a plain message without an icon.", "Plain Message", gui.MB_OK)
print(f"User clicked: {result}")
print()

print("All GUI tests completed successfully!")
print("You should have seen 4 different Windows message boxes appear!")
