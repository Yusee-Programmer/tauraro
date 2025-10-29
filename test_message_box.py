import gui

print("Testing message box function...")

# Test message box
result = gui.message_box("Test Message", "Test Title", gui.MB_OK | gui.MB_ICONINFORMATION)
print(f"Message box result: {result}")

print("Message box test completed.")