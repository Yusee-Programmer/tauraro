# Tauraro GUI Framework - Complete Reference Guide

## Overview
The Tauraro GUI library provides a complete native Windows GUI framework with proper message processing to prevent freezing and "not responding" issues.

## Available Widgets

### 1. Window
Main application window with responsive message processing.

```python
from gui import Window

window = Window("My App", 800, 600)
window.show()
window.keep_alive(30)  # Keep responsive for 30 seconds
window.destroy()
```

**Methods:**
- `show()` - Show the window
- `hide()` - Hide the window
- `set_title(title)` - Change window title
- `move(x, y, width, height)` - Move/resize window
- `keep_alive(seconds)` - Keep window responsive for duration
- `process_messages()` - Process Windows messages
- `destroy()` - Close and destroy window
- `is_visible()` - Check if window is visible

### 2. Label
Static text display.

```python
from gui import Label

label = Label("Hello World", 20, 20, 200, 25)
label.create(window)
label.set_text("New text")
```

### 3. Button
Clickable button control.

```python
from gui import Button

button = Button("Click Me", 20, 60, 120, 35)
button.create(window)
button.enable()
button.disable()
button.set_text("New Label")
```

### 4. TextBox
Text input field with optional multiline and password modes.

```python
from gui import TextBox

# Single-line textbox
textbox = TextBox("Default text", 20, 100, 250, 25)
textbox.create(window)

# Password field
password = TextBox("secret", 20, 140, 150, 25, False, True)
password.create(window)

# Multiline text area
comments = TextBox("Comments...", 20, 180, 300, 100, True)
comments.create(window)

textbox.set_text("New text")
textbox.set_readonly(True)
```

### 5. CheckBox
Checkbox control with checked/unchecked state.

```python
from gui import CheckBox

checkbox = CheckBox("Enable notifications", 20, 220, 200, 22, True)
checkbox.create(window)

if checkbox.is_checked():
    print("Checked!")

checkbox.set_checked(False)
```

### 6. RadioButton
Radio button for mutually exclusive options.

```python
from gui import RadioButton

radio1 = RadioButton("Option A", 20, 250, 150, 22, True)
radio1.create(window)

radio2 = RadioButton("Option B", 20, 280, 150, 22)
radio2.create(window)

if radio1.is_selected():
    print("Option A selected")

radio2.set_selected(True)
```

### 7. GroupBox
Container for grouping related controls.

```python
from gui import GroupBox

group = GroupBox("Settings", 20, 300, 300, 150)
group.create(window)

# Create other controls inside the group at appropriate positions
```

### 8. ListBox
List of selectable items.

```python
from gui import ListBox

listbox = ListBox(20, 350, 200, 100)
listbox.create(window)

listbox.add_item("Item 1")
listbox.add_item("Item 2")
listbox.add_item("Item 3")

selected = listbox.get_selected_index()
listbox.set_selected_index(0)
count = listbox.get_count()
```

### 9. ComboBox
Dropdown selection list.

```python
from gui import ComboBox

combo = ComboBox(240, 350, 200, 150)
combo.create(window)

combo.add_item("Choice 1")
combo.add_item("Choice 2")
combo.add_item("Choice 3")

selected = combo.get_selected_index()
combo.set_selected_index(0)
```

## Helper Functions

### Message Boxes

```python
from gui import show_info, show_warning, show_error, message_box

show_info("Information message", "Title")
show_warning("Warning message", "Warning")
show_error("Error message", "Error")

# Custom message box
message_box("Custom message", "Title", MB_OK | MB_ICONQUESTION)
```

### System Information

```python
from gui import get_screen_width, get_screen_height, beep

width = get_screen_width()
height = get_screen_height()
beep()
```

## Important Notes

### 1. **Use Positional Arguments**
Always use positional arguments when creating widgets (keyword arguments may not work properly in current Tauraro version):

```python
# Correct
label = Label("Text", 20, 20, 200, 25)

# May not work
label = Label("Text", x=20, y=20, width=200, height=25)
```

### 2. **Message Processing**
To prevent "not responding" issues, the window automatically processes messages every 10ms during `keep_alive()`. For long-running operations, call `window.process_messages()` periodically.

### 3. **Widget Creation Order**
Widgets must be created after the parent window:
```python
window = Window("App", 600, 400)
button = Button("OK", 20, 20, 100, 30)
button.create(window)  # Pass parent window
```

### 4. **Responsive Windows**
The framework includes built-in message processing that prevents freezing:
- `keep_alive(seconds)` processes messages every 10ms
- `process_messages()` can be called manually for custom loops

## Complete Example

```python
from gui import Window, Label, Button, TextBox, CheckBox, ListBox, show_info

# Create window
window = Window("My Application", 600, 450)

# Add widgets
title = Label("Welcome to My App", 20, 20, 560, 30)
title.create(window)

name_label = Label("Name:", 20, 70, 100, 25)
name_label.create(window)

name_input = TextBox("Enter name", 130, 67, 200, 25)
name_input.create(window)

checkbox = CheckBox("Subscribe to newsletter", 20, 110, 250, 22)
checkbox.create(window)

listbox = ListBox(20, 150, 200, 100)
listbox.create(window)
listbox.add_item("Item 1")
listbox.add_item("Item 2")

submit_btn = Button("Submit", 20, 270, 100, 35)
submit_btn.create(window)

# Show message
show_info("Application loaded successfully!", "Info")

# Keep window responsive
window.keep_alive(30)

# Cleanup
window.destroy()
```

## Demo Files

- `comprehensive_gui_demo.py` - Complete demo with all widgets
- `demo_app_simple.py` - Simple demo with basic widgets
- `simple_gui_demo.py` - Minimal demo

Run any demo with:
```bash
./target/debug/tauraro.exe run comprehensive_gui_demo.py
```

## Technical Details

The GUI library uses:
- Windows Win32 API via FFI
- Native window handles (HWND)
- Proper message loop processing
- WS_CHILD style for all child controls
- SendMessage for control manipulation

All widgets are responsive and prevent the "not responding" state through regular message processing.
