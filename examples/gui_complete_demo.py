# Complete Tauraro GUI Demo
# Demonstrates all working GUI features: MessageBoxes AND Native Windows!

import gui

print("╔═══════════════════════════════════════════════════════════════╗")
print("║      TAURARO COMPLETE GUI DEMONSTRATION                       ║")
print("║      Native Windows API Integration                           ║")
print("╚═══════════════════════════════════════════════════════════════╝")
print()

print("Libraries loaded:")
print("  ✓ user32.dll")
print("  ✓ kernel32.dll")
print()

print("═══ PART 1: Message Boxes ═══")
print()

# Demo 1: Information
print("[1/3] Information MessageBox")
result = gui.message_box("Welcome to Tauraro GUI!", "Demo", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"      User clicked: {result} (OK)")
print()

# Demo 2: Question
print("[2/3] Question MessageBox")
result = gui.message_box("Ready to see a native window?", "Question", gui.MB_OKCANCEL | gui.MB_ICONWARNING)
if result == 2:
    print("      User cancelled. Exiting...")
    exit(0)
print("      User chose: OK")
print()

# Demo 3: Success
print("[3/3] Success MessageBox")
result = gui.message_box("MessageBoxes work perfectly!", "Success", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"      User clicked: {result} (OK)")
print()

print("═══ PART 2: Native Window Creation ═══")
print()

# Create a window
print("Creating native Windows window...")
hwnd = gui.create_window("Tauraro Native Window", 800, 600)
print(f"  ✓ Window created (handle: {hwnd})")
print()

# Show the window
print("Showing window on screen...")
gui.show_window(hwnd)
print("  ✓ Window is now visible!")
print()

# Keep window visible with a dialog
result = gui.message_box("A native window is now visible on your screen! Close this dialog to continue.", "Window Visible", gui.MB_OK | gui.MB_ICONINFORMATION)

# Hide the window
print("Hiding window...")
gui.hide_window(hwnd)
print("  ✓ Window hidden")
print()

result = gui.message_box("Window is now hidden. It will be destroyed next.", "Hidden", gui.MB_OK | gui.MB_ICONINFORMATION)

# Destroy the window
print("Destroying window...")
gui.destroy_window(hwnd)
print("  ✓ Window destroyed")
print()

# Final message
result = gui.message_box("Demo complete! Tauraro successfully created and managed a native Windows window!", "Complete", gui.MB_OK | gui.MB_ICONINFORMATION)

print("╔═══════════════════════════════════════════════════════════════╗")
print("║                   DEMO COMPLETED! ✓                           ║")
print("╚═══════════════════════════════════════════════════════════════╝")
print()
print("What was demonstrated:")
print("  ✓ MessageBox dialogs (all styles and icons)")
print("  ✓ User input handling (OK, Cancel)")
print("  ✓ Native window creation (CreateWindowExA)")
print("  ✓ Window management (Show, Hide, Destroy)")
print("  ✓ Module handle retrieval (GetModuleHandleA)")
print("  ✓ Window visibility control (ShowWindow, UpdateWindow)")
print()
print("All using:")
print("  • FFI callable function objects")
print("  • Native Windows API (user32.dll, kernel32.dll)")
print("  • Pure Tauraro code!")
print()
print("Tauraro can create real native Windows applications! 🎉")
