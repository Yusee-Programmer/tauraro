# Tauraro GUI Library

A simple Windows native GUI library for Tauraro that provides direct access to Windows API functions for creating native applications.

## Overview

This library uses Tauraro's FFI (Foreign Function Interface) capabilities to call native Windows API functions from `user32.dll` and `kernel32.dll`.

## Features

- Native Windows API access
- Message box dialogs
- Window creation and management
- Simple and Pythonic API

## Current Limitations

Due to the current FFI implementation, some complex function signatures are not yet supported. The library currently provides:

### Supported Features:
- Basic window management constants
- Window class definition
- Simple FFI function setup

### Working on:
- MessageBox API (requires extended FFI signature support)
- Full window creation (requires extended FFI signature support)
- Event handling

## Installation

The library is available in the `tauraro_packages/gui` directory and can be imported directly:

```python
import gui
```

## Example Usage

### Basic Example (Concept)

```python
import gui

# Constants are available
print(gui.MB_OK)  # 0
print(gui.WS_OVERLAPPEDWINDOW)  # 0x00CF0000

# Create a window (when FFI is fully supported)
window = gui.Window("My App", 800, 600)
window.create()
window.show()
```

## Architecture

The library consists of:

1. **Constants**: Windows API constants (WS_*, MB_*, SW_*, etc.)
2. **Function Definitions**: FFI function definitions for Windows API calls
3. **Window Class**: High-level wrapper for window management
4. **Helper Functions**: Convenient wrappers like `message_box()`

## Requirements

- Tauraro runtime with FFI support enabled
- Windows operating system
- `user32.dll` and `kernel32.dll` (standard Windows libraries)

## Development Status

This is an initial implementation demonstrating Tauraro's FFI capabilities.

### Next Steps:
1. Extend FFI to support more complex function signatures (e.g., `(ptr, ptr, ptr, int) -> int`)
2. Implement full MessageBox support
3. Add window procedure and event handling
4. Add more GUI controls (buttons, text boxes, etc.)
5. Implement window message loop
6. Add examples for common GUI patterns

## Technical Details

### FFI Function Signatures

The library uses string-based type definitions for FFI:
- `"int"` - 32-bit integer
- `"pointer"` - generic pointer
- `"void"` - no return value

### Windows API Functions Used

- `MessageBoxA` - Display message dialogs
- `CreateWindowExA` - Create windows
- `ShowWindow` - Show/hide windows
- `UpdateWindow` - Force window repaint
- `DestroyWindow` - Destroy windows
- `GetModuleHandleA` - Get module handle

## Contributing

To extend this library:

1. Add new Windows API constants
2. Define FFI function signatures
3. Create wrapper functions or classes
4. Update this README with new features

## License

This library is part of the Tauraro project.
