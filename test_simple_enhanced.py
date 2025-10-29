import gui

print("Testing enhanced GUI functionality...")

# Test basic window creation (this should work)
hwnd = gui.create_window("Test", 300, 200)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Test button creation (this might cause the overflow)
    try:
        button = gui.create_button(hwnd, "Test", 50, 50, 100, 30, 1001)
        if button:
            print(f"Button created: {button}")
        else:
            print("Button creation failed")
    except Exception as e:
        print(f"Button creation error: {e}")
    
    # Test textbox creation (this might also cause issues)
    try:
        textbox = gui.create_textbox(hwnd, 50, 100, 100, 25, 2001)
        if textbox:
            print(f"Textbox created: {textbox}")
        else:
            print("Textbox creation failed")
    except Exception as e:
        print(f"Textbox creation error: {e}")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")