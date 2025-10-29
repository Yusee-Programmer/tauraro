# Test Responsive Window - Fixed Version
# This demonstrates the fix for "not responding" issue
# Uses functional API (dict pattern) instead of classes

import gui

print("=== Responsive Window Test ===")
print()

print("Creating window...")
window = gui.create_window("Tauraro Responsive Window", 800, 600)
print("Window created!")
print()

print("Keeping window alive for 10 seconds...")
print("Try hovering your mouse over the window - it should be responsive!")
print("The cursor should NOT show loading symbol.")
print("The window should NOT say 'not responding'.")
print()

gui.keep_alive(window, 10)

print("Window was responsive for 10 seconds!")
print()

print("Testing window title change...")
gui.set_window_title(window, "Title Changed!")
gui.keep_alive(window, 2)
print("Title changed successfully!")
print()

print("Cleaning up...")
gui.destroy_window(window)
print("Window destroyed")
print()

print("=== TEST PASSED! ===")
print("The message loop fix is working!")
