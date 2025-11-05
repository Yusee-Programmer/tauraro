# GTK - Cross-Platform GUI Toolkit for Tauraro

A modern, cross-platform desktop GUI framework for the Tauraro programming language, built on GTK3.

## Features

- 🖥️ **Cross-Platform**: Works on Windows, Linux, and macOS with native rendering
- 🎨 **Modern UI**: Beautiful, native-looking interfaces on all platforms
- 🔧 **Easy to Use**: High-level Tauraro API with familiar class-based structure
- 🚀 **FFI-Based**: Uses Tauraro's FFI capabilities to interface with native GTK3 libraries
- 📦 **Complete Widget Set**: Buttons, labels, text inputs, combo boxes, and more
- 📐 **Flexible Layouts**: Box and Grid layout managers for organizing widgets
- 💬 **Dialog Support**: Built-in message dialogs for user interaction

## Installation

### Prerequisites

GTK3 must be installed on your system:

#### Windows
Download and install GTK3 from:
- https://www.gtk.org/docs/installations/windows/
- Or use MSYS2: `pacman -S mingw-w64-x86_64-gtk3`

#### Linux
```bash
# Debian/Ubuntu
sudo apt-get install libgtk-3-0 libgtk-3-dev

# Fedora
sudo dnf install gtk3 gtk3-devel

# Arch Linux
sudo pacman -S gtk3
```

#### macOS
```bash
brew install gtk+3
```

### Using the GTK Package

Simply import the gtk package in your Tauraro programs:

```tauraro
import gtk
```

## Quick Start

### Basic Window Example

```tauraro
import gtk

# Initialize GTK
gtk.init_gtk()

# Create a window
window = gtk.Window("My First GTK Window", 400, 300)

# Add a label
label = gtk.Label("Hello, GTK from Tauraro!")
window.add(label)

# Show the window
window.show_all()

# Run the main loop
gtk.Application("My App").run()
```

### Application with Widgets

```tauraro
import gtk

# Initialize and create application
gtk.init_gtk()
app = gtk.Application("Demo App")

# Create window
window = app.create_window("Demo", 400, 300)
window.set_border_width(20)

# Create layout
vbox = gtk.VBox(spacing=10)
window.add(vbox)

# Add widgets
title = gtk.Label("Welcome!")
vbox.pack_start(title, False, False, 0)

button = gtk.Button("Click Me")
vbox.pack_start(button, False, False, 0)

# Show and run
window.show_all()
app.run()
```

## Core Components

### Application Class
The main application controller:

```tauraro
app = gtk.Application("My Application")
window = app.create_window("Title", 800, 600)
app.run()  # Starts the GTK main loop
```

### Window Class
Main application window:

```tauraro
window = gtk.Window("Window Title", 800, 600)
window.set_title("New Title")
window.set_resizable(False)
window.center()
window.show_all()
```

### Widgets

#### Button
```tauraro
button = gtk.Button("Click Me")
button.set_label("New Label")
```

#### Label
```tauraro
label = gtk.Label("Text")
label.set_text("New Text")
label.set_markup("<b>Bold</b> and <i>italic</i>")
```

#### Entry (Text Input)
```tauraro
entry = gtk.Entry()
entry.set_text("Initial text")
entry.set_placeholder("Enter text...")
entry.set_visibility(False)  # For password fields
text = entry.get_text()
```

#### CheckButton
```tauraro
checkbox = gtk.CheckButton("Accept terms")
checkbox.set_active(True)
is_checked = checkbox.is_checked()
```

#### ComboBox
```tauraro
combo = gtk.ComboBox()
combo.add_item("Option 1")
combo.add_item("Option 2")
combo.set_active(0)
selected = combo.get_active_text()
```

### Layout Managers

#### VBox (Vertical Box)
```tauraro
vbox = gtk.VBox(spacing=10)
vbox.pack_start(widget1, expand=True, fill=True, padding=0)
vbox.pack_start(widget2, expand=False, fill=False, padding=5)
```

#### HBox (Horizontal Box)
```tauraro
hbox = gtk.HBox(spacing=5)
hbox.pack_start(widget1, True, True, 0)
hbox.pack_end(widget2, False, False, 0)
```

#### Grid Layout
```tauraro
grid = gtk.Grid(row_spacing=5, column_spacing=10)
grid.attach(widget, column, row, width, height)

# Example: 2x2 grid
grid.attach(label, 0, 0, 1, 1)    # Top-left
grid.attach(entry, 1, 0, 1, 1)    # Top-right
grid.attach(button1, 0, 1, 1, 1)  # Bottom-left
grid.attach(button2, 1, 1, 1, 1)  # Bottom-right
```

### Dialogs

```tauraro
# Info dialog
gtk.info_dialog(window, "Information message")

# Warning dialog
gtk.warning_dialog(window, "Warning message")

# Error dialog
gtk.error_dialog(window, "Error occurred!")

# Question dialog (returns True/False)
if gtk.question_dialog(window, "Continue?"):
    print("User clicked Yes")

# Confirm dialog (returns True/False)
if gtk.confirm_dialog(window, "Save changes?"):
    print("User clicked OK")
```

## Widget Hierarchy

```
Widget (base class)
├── Container
│   ├── Window
│   ├── Box
│   │   ├── VBox
│   │   └── HBox
│   └── Grid
├── Button
├── Label
├── Entry
├── TextView
├── CheckButton
├── RadioButton
├── ComboBox
├── MenuItem
├── Menu
└── MenuBar
```

## Examples

The package includes several complete examples:

- **example_basic.tr** - Simple window with a label
- **example_buttons.tr** - Button demonstrations with layouts
- **example_form.tr** - Registration form with various input widgets
- **example_dialogs.tr** - Dialog box demonstrations
- **example_calculator.tr** - Calculator UI layout

Run examples with:
```bash
tauraro tauraro_packages/gtk/example_basic.tr
```

## API Reference

### Common Widget Methods

All widgets inherit these methods:

- `show()` - Show the widget
- `hide()` - Hide the widget
- `destroy()` - Destroy the widget
- `set_size(width, height)` - Set minimum size
- `set_sensitive(bool)` - Enable/disable the widget
- `get_width()` - Get allocated width
- `get_height()` - Get allocated height

### Container Methods

Containers (Window, Box, Grid) provide:

- `add(widget)` - Add a widget
- `remove(widget)` - Remove a widget
- `set_border_width(width)` - Set border padding

## Platform-Specific Notes

### Windows
- GTK3 runtime DLLs must be in your PATH
- Recommended: Use MSYS2 for easy GTK installation

### Linux
- GTK3 is usually pre-installed on most distributions
- If not, install via package manager

### macOS
- GTK3 can be installed via Homebrew
- May require XQuartz for X11 support

## Current Limitations

1. **Event Handlers**: Signal/event handling is not yet fully implemented
   - Planned for next version
   - Will support button clicks, text changes, window events, etc.

2. **Advanced Widgets**: Some widgets not yet available:
   - TreeView/ListView
   - Notebook (tabs)
   - Scrolled windows
   - File chooser dialogs
   - Custom drawing (Cairo integration)

3. **Styling**: GTK CSS theming not yet exposed
   - Coming in future version

## Roadmap

- [ ] Event handler system (signals/callbacks)
- [ ] More widgets (TreeView, Notebook, etc.)
- [ ] File chooser dialogs
- [ ] Custom drawing with Cairo
- [ ] GTK CSS theming support
- [ ] Clipboard operations
- [ ] Drag and drop
- [ ] Accessibility features

## Architecture

The GTK package uses Tauraro's FFI (Foreign Function Interface) to call native GTK3 functions:

1. **Library Loading**: `load_library()` loads platform-specific GTK3 shared libraries
2. **Function Definition**: `define_function()` declares GTK3 C functions
3. **Function Calls**: `call_function()` invokes GTK3 functions with proper type conversions
4. **High-Level API**: Tauraro classes wrap low-level FFI calls for ease of use

## Contributing

This is an early version of the GTK package. Contributions welcome for:
- Additional widgets
- Event handling implementation
- Documentation improvements
- Bug fixes
- Example programs

## License

This package is part of the Tauraro programming language project.

## Support

For issues, questions, or contributions:
- Tauraro GitHub: https://github.com/Yusee-Programmer/tauraro
- GTK3 Documentation: https://docs.gtk.org/gtk3/

---

**Version**: 1.0.0
**Author**: Tauraro Team
**Last Updated**: 2025-11-05
