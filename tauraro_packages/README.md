# Tauraro Packages

Official package repository for the Tauraro programming language.

## Available Packages

### 1. GTK - Cross-Platform GUI Toolkit ⭐ NEW!

A modern, cross-platform desktop GUI framework built on GTK3.

**Features:**
- 🖥️ Cross-platform support (Windows, Linux, macOS)
- 🎨 Native rendering on all platforms
- 📦 Complete widget set (buttons, labels, inputs, etc.)
- 📐 Flexible layout managers (Box, Grid)
- 💬 Built-in dialog support

**Quick Start:**
```tauraro
import gtk

gtk.init_gtk()
app = gtk.Application("My App")
window = app.create_window("Hello", 400, 300)

label = gtk.Label("Hello, GTK!")
window.add(label)

window.show_all()
app.run()
```

**Documentation:** [gtk/README.md](gtk/README.md)

**Examples:**
- `gtk/example_basic.tr` - Simple window
- `gtk/example_buttons.tr` - Buttons and layouts
- `gtk/example_form.tr` - Form with input widgets
- `gtk/example_calculator.tr` - Calculator UI

---

### 2. DUITK - Desktop UI Toolkit (Windows)

A high-level GUI framework for Windows using native Win32 APIs.

**Features:**
- Native Windows controls
- Direct Win32 API integration
- Window and control management
- Event handling

**Quick Start:**
```tauraro
import duitk

app = duitk.Application("My App")
window = app.create_window("Hello Windows", 640, 480)
window.show()
app.run()
```

**Documentation:** [duitk/README.md](duitk/README.md)

---

### 3. Win32 - Windows API Bindings

Low-level Windows API bindings for advanced Windows programming.

**Modules:**
- `kernel32` - Core Windows functions
- `user32` - Window and UI functions
- `gdi32` - Graphics Device Interface
- `shell32` - Shell functions
- `ole32` - COM support
- `comctl32` - Common controls
- `d2d1` - Direct2D graphics
- `advapi32` - Advanced Windows API

**Quick Start:**
```tauraro
import win32

# Load required DLL
load_library("user32.dll")
define_function("user32.dll", "MessageBoxA", "int32", ["pointer", "pointer", "pointer", "uint32"])

# Show message box
call_function("user32.dll", "MessageBoxA", [0, "Hello!", "Title", 0])
```

**Documentation:** [win32/README.md](win32/README.md)

---

## Package Structure

Each package follows this structure:

```
package_name/
├── __init__.tr          # Main package file
├── README.md            # Documentation
├── module1.tr           # Additional modules
├── module2.tr
├── example_*.tr         # Example programs
└── test_*.tr           # Test files
```

## Using Packages

### Import a Package

```tauraro
import package_name
```

### Import Specific Module

```tauraro
import package_name.module_name
```

## Creating New Packages

To create a new package:

1. Create a directory in `tauraro_packages/`
2. Add `__init__.tr` as the main package file
3. Add additional modules as needed
4. Create `README.md` with documentation
5. Add example and test files

## Platform Support

| Package | Windows | Linux | macOS |
|---------|---------|-------|-------|
| gtk     | ✅      | ✅    | ✅    |
| duitk   | ✅      | ❌    | ❌    |
| win32   | ✅      | ❌    | ❌    |

## Requirements

### GTK Package
- **Windows**: GTK3 runtime (from gtk.org or MSYS2)
- **Linux**: `libgtk-3-0` (usually pre-installed)
- **macOS**: GTK3 via Homebrew (`brew install gtk+3`)

### DUITK/Win32 Packages
- **Windows**: Windows Vista or later (native)

## Examples

Run examples with:

```bash
tauraro tauraro_packages/gtk/example_basic.tr
tauraro tauraro_packages/duitk/test_minimal.tr
```

## Contributing

We welcome contributions! To add a new package or improve existing ones:

1. Follow the package structure guidelines
2. Include comprehensive documentation
3. Add example programs
4. Test on all supported platforms
5. Submit a pull request

## License

All packages are part of the Tauraro programming language project.

## Support

- **GitHub**: https://github.com/Yusee-Programmer/tauraro
- **Issues**: Report bugs and request features on GitHub

---

**Last Updated**: 2025-11-05
**Tauraro Version**: 1.0+
