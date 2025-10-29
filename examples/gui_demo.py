# Tauraro Native Windows GUI Demo
# Demonstrates native Windows message boxes using FFI

import gui

print("╔══════════════════════════════════════════════════════════════╗")
print("║      TAURARO NATIVE WINDOWS GUI DEMONSTRATION                ║")
print("║                                                              ║")
print("║  This demo shows Tauraro calling native Windows APIs        ║")
print("║  to display real GUI elements on your screen!               ║")
print("╚══════════════════════════════════════════════════════════════╝")
print()

print("Libraries loaded:")
print("  ✓ user32.dll")
print("  ✓ kernel32.dll")
print()

print("Press OK on each message box to continue...")
print()

# Demo 1
print("[1/4] Information Message...")
msg1 = "Welcome to Tauraro GUI! This is a native Windows message box created with Tauraro FFI."
result = gui.message_box(msg1, "Tauraro GUI Demo - Information", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"      Result: {result} (IDOK)")
print()

# Demo 2
print("[2/4] Warning with Choice...")
msg2 = "Do you want to continue with the demo?"
result = gui.message_box(msg2, "Tauraro GUI Demo - Warning", gui.MB_OKCANCEL | gui.MB_ICONWARNING)
if result == 1:
    print("      User chose: OK (continue)")
elif result == 2:
    print("      User chose: Cancel (stop)")
    print()
    print("Demo stopped by user choice.")
    exit(0)
print()

# Demo 3
print("[3/4] Error Message...")
msg3 = "This is a simulated error message. Don't worry, nothing is actually wrong!"
result = gui.message_box(msg3, "Tauraro GUI Demo - Error", gui.MB_OK | gui.MB_ICONERROR)
print(f"      Result: {result} (IDOK)")
print()

# Demo 4
print("[4/4] Success Message...")
msg4 = "Congratulations! You've successfully run Tauraro with native Windows GUI!"
result = gui.message_box(msg4, "Tauraro GUI Demo - Success", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"      Result: {result} (IDOK)")
print()

print("╔══════════════════════════════════════════════════════════════╗")
print("║                    DEMO COMPLETED! ✓                         ║")
print("╚══════════════════════════════════════════════════════════════╝")
print()
print("What just happened:")
print("  • Tauraro loaded native Windows DLLs via FFI")
print("  • Called Windows API functions (MessageBoxA)")
print("  • Displayed 4 native GUI message boxes")
print("  • Handled user input (button clicks)")
print("  • All using pure Tauraro code!")
print()
print("See 'tauraro_packages/gui/' for the library source code.")
print("See 'FINAL_GUI_SUMMARY.md' for complete documentation.")
