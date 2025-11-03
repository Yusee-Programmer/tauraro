# DUITK Calculator Demo - A simple calculator UI
# Demonstrates practical use of DUITK for building real applications

print("=" * 60)
print("DUITK Calculator Demo")
print("Building a Calculator UI with Native Win32")
print("=" * 60)
print()

import duitk

# Create application
app = duitk.Application("DUITK Calculator")

# Create calculator window
calc_window = app.create_window("Tauraro Calculator", 320, 420)

print("\nBuilding calculator UI...")

# Display (edit control at top)
display = calc_window.create_edit("0", 20, 20, 280, 40)
print("✓ Display created")

# Number buttons layout
button_layout = [
    ["7", "8", "9", "/"],
    ["4", "5", "6", "*"],
    ["1", "2", "3", "-"],
    ["0", ".", "=", "+"]
]

buttons = []
y_pos = 80
for row in button_layout:
    x_pos = 20
    for btn_text in row:
        btn = calc_window.create_button(btn_text, x_pos, y_pos, 60, 50)
        buttons.append(btn)
        x_pos += 70
    y_pos += 60

print(f"✓ Created {len(buttons)} calculator buttons")

# Clear and operation buttons
clear_btn = calc_window.create_button("C", 20, 320, 60, 50)
backspace_btn = calc_window.create_button("←", 90, 320, 60, 50)

print("\nCalculator UI complete!")
print(f"  - Display: {display.hwnd}")
print(f"  - Buttons: {len(buttons) + 2} total")
print(f"  - Window: {calc_window.hwnd}")

# Show calculator info
duitk.message_box(
    f"Tauraro Calculator\n\n"
    f"A demonstration of DUITK's capabilities\n\n"
    f"Created using:\n"
    f"  • Native Win32 APIs\n"
    f"  • Tauraro FFI System\n"
    f"  • DUITK GUI Toolkit\n\n"
    f"Total controls: {len(calc_window.controls)}\n\n"
    f"Click OK to view the calculator",
    "Calculator Demo",
    0x00000040  # MB_ICONINFORMATION
)

# Run the app
app.run_simple()

print("\n" + "=" * 60)
print("Calculator demo completed!")
print("=" * 60)
