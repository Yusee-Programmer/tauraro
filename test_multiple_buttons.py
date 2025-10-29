import gui

print("Testing multiple button creation...")

# Create main window
hwnd = gui.create_window("Multiple Buttons Test", 400, 300)
if hwnd:
    print(f"Window created: {hwnd}")
    
    # Create multiple buttons to test the overflow fix
    print("Creating 5 buttons...")
    buttons = []
    for i in range(5):
        button = gui.create_button(hwnd, f"Button {i+1}", 50, 50 + (i * 40), 100, 30, 1001 + i)
        if button:
            buttons.append(button)
            print(f"Button {i+1} created: {button}")
        else:
            print(f"Failed to create button {i+1}")
            break
    
    if len(buttons) == 5:
        print("All 5 buttons created successfully!")
    else:
        print(f"Only {len(buttons)} buttons created successfully.")
    
    # Create a few textboxes as well
    print("Creating 3 textboxes...")
    textboxes = []
    for i in range(3):
        textbox = gui.create_textbox(hwnd, 200, 50 + (i * 40), 150, 25, 2001 + i)
        if textbox:
            textboxes.append(textbox)
            print(f"Textbox {i+1} created: {textbox}")
        else:
            print(f"Failed to create textbox {i+1}")
            break
    
    if len(textboxes) == 3:
        print("All 3 textboxes created successfully!")
    else:
        print(f"Only {len(textboxes)} textboxes created successfully.")
    
    gui.destroy_window(hwnd)
    print("Window destroyed")
else:
    print("Window creation failed")

print("Test completed.")