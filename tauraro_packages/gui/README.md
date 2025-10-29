# Tauraro GUI Library

A comprehensive Windows native GUI library for Tauraro that provides direct access to Windows API functions for creating native applications.

## Overview

This library uses Tauraro's FFI (Foreign Function Interface) capabilities to call native Windows API functions from `user32.dll`, `kernel32.dll`, `comctl32.dll`, and `comdlg32.dll`.

## Features

- Native Windows API access
- Message box dialogs with multiple styles
- Window creation and management
- Button controls
- Text input controls
- Menu system
- File dialogs (Open/Save)
- Window positioning and sizing
- Event handling and message loops
- System information functions
- Simple and Pythonic API

## Current Functionality

### Supported Features:
- All basic window management constants (WS_*, MB_*, SW_*, etc.)
- Complete window class definition and creation
- Full FFI function setup with callable objects
- Button controls with multiple styles
- Text input controls with various options
- Menu creation and management
- File open/save dialogs
- Window positioning and sizing functions
- Message loop for event handling
- System metrics and information

### Working Features:
- MessageBox API with all styles and icons
- Full window creation with CreateWindowExA (12 parameters)
- Window management (ShowWindow, UpdateWindow, MoveWindow, etc.)
- Button creation and management
- Text input control creation and text manipulation
- Menu system with items and submenus
- File dialogs for opening and saving files
- Window positioning, sizing, and coordinate conversion
- Event handling through message loops
- System information retrieval

## Installation

The library is available in the `tauraro_packages/gui` directory and can be imported directly:

```python
import gui
```

## Example Usage

### Basic Window Example
```python
import gui

# Create a window
window = gui.create_window("My App", 800, 600)
gui.show_window(window)

# Create a button
button = gui.create_button(window, "Click Me!", 50, 50, 100, 30, 1001)

# Create a textbox
textbox = gui.create_textbox(window, 50, 100, 200, 25, 2001)

# Set text in textbox
gui.set_control_text(textbox, "Hello, World!")

# Get text from textbox
text = gui.get_control_text(textbox)
print(f"Text in textbox: {text}")

# Run message loop
gui.message_loop()

# Cleanup
gui.destroy_window(window)
```

### Menu Example
```python
import gui

# Create window
window = gui.create_window("Menu Demo", 400, 300)

# Create menu
menu = gui.create_menu()
gui.add_menu_item(menu, "File", 1000, 0x00000010)  # MF_POPUP
gui.add_menu_item(menu, "Open", 1001)
gui.add_menu_item(menu, "Save", 1002)
gui.add_menu_item(menu, "Exit", 1003)
gui.set_window_menu(window, menu)

gui.show_window(window)
gui.message_loop()
gui.destroy_window(window)
```

### File Dialog Example
```python
import gui

# Open file dialog
filename = gui.open_file_dialog(
    title="Select a file",
    filter="Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"
)

if filename:
    gui.show_info("Selected File", f"You selected: {filename}")

# Save file dialog
filename = gui.save_file_dialog(
    title="Save file as",
    filter="Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0",
    default_ext="txt"
)

if filename:
    gui.show_info("File to Save", f"Saving as: {filename}")
```

## Architecture

The library consists of:

1. **Constants**: Windows API constants (WS_*, MB_*, SW_*, BS_*, ES_*, etc.)
2. **Function Definitions**: FFI function definitions for Windows API calls
3. **Helper Functions**: Convenient wrappers for common operations
4. **Structures**: Python classes that map to Windows structures

## Requirements

- Tauraro runtime with FFI support enabled
- Windows operating system
- Standard Windows libraries (`user32.dll`, `kernel32.dll`, `comctl32.dll`, `comdlg32.dll`)

## API Reference

### Window Management
```python
# Create windows
gui.create_window(title, width, height, x=None, y=None, style=None, ex_style=0)
gui.create_window_centered(title, width, height, style=None, ex_style=0)

# Window operations
gui.show_window(hwnd, command=None)
gui.hide_window(hwnd)
gui.minimize_window(hwnd)
gui.maximize_window(hwnd)
gui.restore_window(hwnd)
gui.destroy_window(hwnd)
gui.move_window(hwnd, x, y, width, height, repaint=True)
gui.resize_window(hwnd, width, height)

# Window properties
gui.set_window_title(hwnd, title)
gui.get_window_title(hwnd)
gui.is_window_visible(hwnd)
gui.set_window_position(hwnd, x, y, width=None, height=None, hwnd_insert_after=None, flags=0)
gui.get_window_rect(hwnd)
gui.client_to_screen(hwnd, x, y)
```

### Controls
```python
# Buttons
gui.create_button(parent_hwnd, text, x, y, width, height, id, style=None)

# Textboxes
gui.create_textbox(parent_hwnd, x, y, width, height, id, style=None)
gui.set_control_text(hwnd, text)
gui.get_control_text(hwnd)
```

### Menus
```python
gui.create_menu()
gui.add_menu_item(menu_hwnd, text, id, flags=0)
gui.set_window_menu(hwnd, menu_hwnd)
```

### File Dialogs
```python
gui.open_file_dialog(owner_hwnd=None, title="Open File", filter="All Files (*.*)\0*.*\0\0", initial_dir=None)
gui.save_file_dialog(owner_hwnd=None, title="Save File", filter="All Files (*.*)\0*.*\0\0", initial_dir=None, default_ext=None)
```

### Message Boxes
```python
gui.message_box(text, title="Message", style=MB_OK)
gui.show_info(text, title="Information")
gui.show_warning(text, title="Warning")
gui.show_error(text, title="Error")
gui.ask_question(text, title="Question")
gui.ask_ok_cancel(text, title="Confirm")
gui.ask_retry(text, title="Retry")
```

### System Functions
```python
gui.get_screen_width()
gui.get_screen_height()
gui.get_screen_size()
gui.beep(sound_type=0)
gui.beep_error()
gui.beep_question()
gui.beep_warning()
gui.beep_info()
```

### Event Handling
```python
gui.message_loop()
gui.send_message(hwnd, msg, wparam=0, lparam=0)
```

### Common Controls
```python
gui.init_common_controls(classes=ICC_STANDARD_CLASSES)
```

## Development Status

This is a fully-featured implementation demonstrating Tauraro's FFI capabilities with comprehensive Windows GUI support.

### Features Implemented:
1. ✅ MessageBox API with all styles and icons
2. ✅ Window creation with full CreateWindowExA support
3. ✅ Complete window management functions
4. ✅ Button controls with multiple styles
5. ✅ Text input controls with various options
6. ✅ Menu system with items and submenus
7. ✅ File dialogs for opening and saving files
8. ✅ Window positioning, sizing, and coordinate conversion
9. ✅ Event handling through message loops
10. ✅ System information retrieval
11. ✅ Common controls initialization
12. ✅ Text manipulation functions

## Technical Details

### FFI Function Signatures

The library uses string-based type definitions for FFI:
- `"int"` - 32-bit integer
- `"pointer"` - generic pointer
- `"void"` - no return value
- `"string"` - null-terminated C string

### Windows API Functions Used

Core functions:
- `MessageBoxA` - Display message dialogs
- `CreateWindowExA` - Create windows and controls
- `ShowWindow` - Show/hide windows
- `UpdateWindow` - Force window repaint
- `DestroyWindow` - Destroy windows
- `GetModuleHandleA` - Get module handle
- `MoveWindow` - Move and resize windows
- `SetWindowTextA` / `GetWindowTextA` - Text manipulation
- `GetSystemMetrics` - System information
- `MessageBeep` - System sounds

Message loop functions:
- `GetMessageA` - Retrieve messages
- `TranslateMessage` - Translate keyboard messages
- `DispatchMessageA` - Dispatch messages to window procedures
- `SendMessageA` - Send messages to windows

Menu functions:
- `CreateMenu` - Create menus
- `AppendMenuA` - Add items to menus
- `SetMenu` - Attach menus to windows

Common dialog functions:
- `GetOpenFileNameA` - Open file dialog
- `GetSaveFileNameA` - Save file dialog

Common controls:
- `InitCommonControlsEx` - Initialize common controls

## Examples

See the `examples/` directory for comprehensive demos:
- `gui_comprehensive_demo.py` - Shows all features
- `gui_advanced_demo.py` - Buttons and textboxes
- `gui_file_dialog_demo.py` - File dialogs
- `gui_menu_demo.py` - Menu system
- `gui_positioning_demo.py` - Window positioning

## Contributing

To extend this library:

1. Add new Windows API constants
2. Define FFI function signatures
3. Create wrapper functions or classes
4. Update this README with new features
5. Add examples for new functionality

## License

This library is part of the Tauraro project.