# Simple Demo App with positional arguments
from gui import Window, Label, Button, show_info

print("Creating Tauraro GUI Application...")

# Create window
window = Window("Tauraro Demo App", 600, 450)
print(f"Window created")

# Create title
title = Label("Welcome to Tauraro GUI!", 50, 30, 500, 30)
title.create(window)
print("Title label created")

# Create description
desc = Label("This is a simple GUI application", 50, 70, 500, 25)
desc.create(window)
print("Description label created")

# Create button 1
btn1 = Button("Button 1", 50, 120, 120, 35)
btn1.create(window)
print("Button 1 created")

# Create button 2
btn2 = Button("Button 2", 180, 120, 120, 35)
btn2.create(window)
print("Button 2 created")

# Create button 3
btn3 = Button("Button 3", 310, 120, 120, 35)
btn3.create(window)
print("Button 3 created")

# More labels
label1 = Label("Enter your name:", 50, 180, 200, 25)
label1.create(window)

label2 = Label("Select an option:", 50, 240, 200, 25)
label2.create(window)

label3 = Label("Status: Ready", 50, 300, 400, 25)
label3.create(window)

# Exit button
exit_btn = Button("Exit", 250, 360, 100, 35)
exit_btn.create(window)

print("\nAll widgets created!")
print("Showing info dialog...")
show_info("Application is ready!\n\nAll widgets have been created.", "Success")

print("\nKeeping window alive for 20 seconds...")
window.keep_alive(20)

print("\nClosing application...")
window.destroy()
print("Done!")
