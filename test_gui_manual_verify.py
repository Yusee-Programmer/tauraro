# Manual Verification Test - DUITK GUI
# This will create a simple window with various widgets
# MANUALLY VERIFY: Widgets are visible and window is responsive

import duitk

print("\n" + "="*60)
print("  DUITK Manual Verification Test")
print("="*60)
print("\n✨ Creating application with sample widgets...")

app = duitk.Application("DUITK Verification Test")
window = app.create_window("DUITK Test - Verify Widget Visibility", 500, 400)

if window != None:
    hwnd = window.hwnd
    print(f"\n✓ Window created (hwnd={hwnd})")
    print("\n📋 Creating test widgets...")

    # Create test widgets
    label1 = duitk.Label(hwnd, "Welcome to DUITK!", 20, 20, 300, 25)
    label2 = duitk.Label(hwnd, "Name:", 20, 60, 80, 20)
    textbox1 = duitk.TextBox(hwnd, 110, 57, 200, 25)

    button1 = duitk.Button(hwnd, "Click Me!", 20, 100, 120, 35)
    button2 = duitk.Button(hwnd, "Test Button", 150, 100, 120, 35)

    checkbox1 = duitk.CheckBox(hwnd, "Enable feature", 20, 150, 150, 25)
    checkbox2 = duitk.CheckBox(hwnd, "Remember me", 20, 180, 150, 25)

    groupbox = duitk.GroupBox(hwnd, "Options", 200, 145, 180, 90)
    radio1 = duitk.RadioButton(hwnd, "Option A", 210, 170, 150, 25)
    radio2 = duitk.RadioButton(hwnd, "Option B", 210, 200, 150, 25)

    label3 = duitk.Label(hwnd, "Select country:", 20, 250, 120, 20)
    combo = duitk.ComboBox(hwnd, 150, 247, 200, 200)

    progress = duitk.ProgressBar(hwnd, 20, 290, 350, 25)

    print("\n✓ 13 widgets created successfully!")

    print("\n" + "="*60)
    print("  🎯 MANUAL VERIFICATION CHECKLIST:")
    print("="*60)
    print("\n  Please verify the following:")
    print("  [ ] Window is visible on screen")
    print("  [ ] All labels are readable")
    print("  [ ] Text box is visible with sunken border")
    print("  [ ] Both buttons are visible and styled")
    print("  [ ] Both checkboxes are visible")
    print("  [ ] GroupBox with 'Options' title is visible")
    print("  [ ] Both radio buttons are inside the group box")
    print("  [ ] ComboBox dropdown is visible")
    print("  [ ] Progress bar is visible")
    print("  [ ] Window responds to clicks (no 'Not Responding')")
    print("  [ ] You can type in the text box")
    print("  [ ] You can check/uncheck boxes")
    print("  [ ] You can select radio buttons")
    print("  [ ] Window doesn't close by itself")
    print("\n  Close the window when done testing.")
    print("="*60 + "\n")

    print("⏳ Starting application...\n")

    # Run the application - will stay open until user closes
    app.run()

    print("\n✅ Application closed cleanly!")
    print("="*60 + "\n")

else:
    print("\n❌ ERROR: Failed to create window\n")
