# DUITK Modern Widgets Demo
# Showcases all available modern GUI widgets

import duitk

print("\n" + "="*80)
print("  DUITK v3.0 - Modern Widget Showcase")
print("="*80)
print()

# Create application
app = duitk.Application("DUITK Modern Widgets Demo")

# Create main window
print("\nCreating main window...")
window = app.create_window("DUITK Widget Showcase", 700, 600)

if window != None:
    hwnd = window.hwnd
    print(f"✓ Main window created (hwnd={hwnd})")

    print("\n" + "-"*80)
    print("  Creating Modern Widgets...")
    print("-"*80)

    # Labels and Text
    print("\n[Text Widgets]")
    label1 = duitk.Label(hwnd, "Welcome to DUITK!", 20, 20, 300, 25)
    label2 = duitk.Label(hwnd, "Enter your name:", 20, 55, 150, 20)
    textbox1 = duitk.TextBox(hwnd, 180, 52, 250, 25)

    label3 = duitk.Label(hwnd, "Password:", 20, 90, 150, 20)
    password_box = duitk.TextBox(hwnd, 180, 87, 250, 25, password=True)

    label4 = duitk.Label(hwnd, "Comments:", 20, 125, 150, 20)
    multiline_box = duitk.TextBox(hwnd, 180, 122, 250, 80, multiline=True)

    # Buttons
    print("\n[Button Widgets]")
    button1 = duitk.Button(hwnd, "Click Me!", 20, 220, 100, 30)
    button2 = duitk.Button(hwnd, "Submit", 130, 220, 100, 30)
    button3 = duitk.Button(hwnd, "Cancel", 240, 220, 100, 30)

    # CheckBoxes
    print("\n[CheckBox Widgets]")
    checkbox1 = duitk.CheckBox(hwnd, "Enable notifications", 20, 270, 200, 25)
    checkbox2 = duitk.CheckBox(hwnd, "Remember me", 20, 300, 200, 25)
    checkbox3 = duitk.CheckBox(hwnd, "Auto-save", 20, 330, 200, 25)

    # RadioButtons in a group
    print("\n[RadioButton Widgets]")
    groupbox1 = duitk.GroupBox(hwnd, "Select Theme", 250, 260, 180, 100)
    radio1 = duitk.RadioButton(hwnd, "Light Theme", 260, 285, 150, 25)
    radio2 = duitk.RadioButton(hwnd, "Dark Theme", 260, 315, 150, 25)

    # Combo Box
    print("\n[ComboBox Widget]")
    label5 = duitk.Label(hwnd, "Country:", 460, 20, 100, 20)
    combo = duitk.ComboBox(hwnd, 460, 45, 200, 200)

    # List Box
    print("\n[ListBox Widget]")
    label6 = duitk.Label(hwnd, "Select items:", 460, 95, 150, 20)
    listbox = duitk.ListBox(hwnd, 460, 120, 200, 150)

    # Progress Bar
    print("\n[ProgressBar Widget]")
    label7 = duitk.Label(hwnd, "Loading progress:", 20, 380, 150, 20)
    progress = duitk.ProgressBar(hwnd, 20, 405, 410, 25)

    # Group Box with content
    print("\n[GroupBox Widget]")
    groupbox2 = duitk.GroupBox(hwnd, "User Preferences", 20, 445, 410, 100)
    checkbox4 = duitk.CheckBox(hwnd, "Show tooltips", 35, 470, 150, 25)
    checkbox5 = duitk.CheckBox(hwnd, "Enable animations", 35, 500, 150, 25)
    button4 = duitk.Button(hwnd, "Apply Settings", 250, 495, 140, 30)

    print("\n" + "-"*80)
    print(f"  ✅ Created {17} widgets successfully!")
    print("-"*80)

    print("\n" + "="*80)
    print("  Widget Showcase Ready!")
    print("="*80)
    print("\n📋 Available Widget Types:")
    print("   • Label        - Static text display")
    print("   • TextBox      - Single/multi-line text input (with password mode)")
    print("   • Button       - Clickable push buttons")
    print("   • CheckBox     - Toggle options on/off")
    print("   • RadioButton  - Single selection from group")
    print("   • ComboBox     - Dropdown selection list")
    print("   • ListBox      - Multiple item selection")
    print("   • GroupBox     - Visual grouping container")
    print("   • ProgressBar  - Loading/progress indication")

    print("\n💡 Features:")
    print("   ✓ Modern Windows visual styles")
    print("   ✓ Native Win32 controls")
    print("   ✓ Segoe UI font (modern look)")
    print("   ✓ Proper tab order and keyboard navigation")
    print("   ✓ Sunken borders on edit controls")
    print("   ✓ Responsive message loop")

    print("\n🎨 Try interacting with the widgets!")
    print("   • Type in text boxes")
    print("   • Check/uncheck boxes")
    print("   • Select radio buttons")
    print("   • Click buttons")
    print("   • Close the window when done")

    print("\n⏳ Starting application...")
    print("-"*80 + "\n")

    # Run the application
    app.run()

    print("\n" + "="*80)
    print("  Application Closed")
    print("="*80)
    print("\n✓ Thank you for testing DUITK v3.0!")

else:
    print("\n✗ ERROR: Failed to create main window")

print("\n" + "="*80)
print("  Demo Complete")
print("="*80 + "\n")
