# Complete Tauraro GUI Demo Application
# Showcases all available widgets in the GUI library

from gui import Window, Button, Label, TextBox, CheckBox, RadioButton, ListBox, ComboBox, GroupBox, show_info

print("=" * 60)
print("         Tauraro Complete GUI Demo")
print("=" * 60)

# Create main window
print("\nCreating main window...")
window = Window("Complete GUI Demo - Tauraro", 700, 600)
print(f"Window created with handle: {window.hwnd}")

# Title Label
title = Label("Tauraro GUI Framework Demo", x=20, y=10, width=660, height=30)
title.create(window)

# Section 1: Text Input
section1_label = Label("1. Text Input Controls:", x=20, y=50, width=300, height=20)
section1_label.create(window)

textbox_label = Label("Enter your name:", x=40, y=75, width=150, height=20)
textbox_label.create(window)

name_textbox = TextBox("John Doe", x=40, y=95, width=250, height=25)
name_textbox.create(window)

password_label = Label("Password field:", x=40, y=130, width=150, height=20)
password_label.create(window)

password_textbox = TextBox("secret", x=40, y=150, width=250, height=25, password=True)
password_textbox.create(window)

multiline_label = Label("Comments:", x=40, y=185, width=150, height=20)
multiline_label.create(window)

comments_textbox = TextBox("Enter your comments here...", x=40, y=205, width=300, height=80, multiline=True)
comments_textbox.create(window)

# Section 2: Buttons
section2_label = Label("2. Buttons:", x=360, y=50, width=300, height=20)
section2_label.create(window)

submit_button = Button("Submit", x=380, y=75, width=100, height=30)
submit_button.create(window)

cancel_button = Button("Cancel", x=490, y=75, width=100, height=30)
cancel_button.create(window)

disabled_button = Button("Disabled", x=380, y=115, width=100, height=30)
disabled_button.create(window)
disabled_button.disable()

# Section 3: Checkboxes
section3_label = Label("3. Checkboxes:", x=20, y=300, width=300, height=20)
section3_label.create(window)

checkbox1 = CheckBox("Enable notifications", x=40, y=325, width=200, height=20, checked=True)
checkbox1.create(window)

checkbox2 = CheckBox("Remember me", x=40, y=350, width=200, height=20)
checkbox2.create(window)

checkbox3 = CheckBox("Auto-save", x=40, y=375, width=200, height=20, checked=True)
checkbox3.create(window)

# Section 4: Radio Buttons with GroupBox
section4_label = Label("4. Radio Buttons:", x=260, y=300, width=200, height=20)
section4_label.create(window)

radio_group = GroupBox("Select your preference:", x=280, y=320, width=180, height=100)
radio_group.create(window)

radio1 = RadioButton("Option A", x=295, y=345, width=150, height=20, checked=True)
radio1.create(window)

radio2 = RadioButton("Option B", x=295, y=370, width=150, height=20)
radio2.create(window)

radio3 = RadioButton("Option C", x=295, y=395, width=150, height=20)
radio3.create(window)

# Section 5: ListBox
section5_label = Label("5. ListBox:", x=20, y=410, width=200, height=20)
section5_label.create(window)

listbox = ListBox(x=40, y=435, width=180, height=100)
listbox.create(window)
listbox.add_item("Item 1")
listbox.add_item("Item 2")
listbox.add_item("Item 3")
listbox.add_item("Item 4")
listbox.add_item("Item 5")
listbox.set_selected_index(0)

# Section 6: ComboBox
section6_label = Label("6. ComboBox/Dropdown:", x=240, y=410, width=200, height=20)
section6_label.create(window)

combo_label = Label("Choose an option:", x=260, y=435, width=150, height=20)
combo_label.create(window)

combobox = ComboBox(x=260, y=460, width=180, height=150)
combobox.create(window)
combobox.add_item("First Choice")
combobox.add_item("Second Choice")
combobox.add_item("Third Choice")
combobox.add_item("Fourth Choice")
combobox.set_selected_index(0)

# Action buttons at the bottom
show_info_btn = Button("Show Info", x=480, y=450, width=100, height=30)
show_info_btn.create(window)

exit_btn = Button("Exit", x=480, y=490, width=100, height=30)
exit_btn.create(window)

# Show welcome message
print("\nShowing welcome message...")
show_info("Welcome to the Tauraro GUI Demo!\n\nThis application demonstrates all available widgets:\n- Text fields (normal, password, multiline)\n- Buttons (enabled and disabled)\n- Checkboxes\n- Radio buttons\n- List boxes\n- Combo boxes\n- Group boxes", "Welcome")

print("\n" + "=" * 60)
print("Window is now active!")
print("All widgets have been created and displayed.")
print("The window will stay open for 30 seconds...")
print("=" * 60)

# Keep window alive and responsive
window.keep_alive(30)

# Cleanup
print("\nClosing application...")
window.destroy()
print("Demo completed successfully!")
