# Tauraro FFI (Foreign Function Interface) Documentation

## Overview

Tauraro provides a comprehensive Foreign Function Interface (FFI) that allows you to load and call functions from native libraries across multiple platforms:

- Windows (DLL)
- Linux (SO - Shared Objects)
- macOS (dylib)
- iOS (dylib/framework)
- Android (SO)
- Embedded systems
- Unix-like systems

## FFI Functions

### `load_library(library_name)`
Load a dynamic library.

**Parameters:**
- `library_name`: Name or path of the library (e.g., "mylib", "libmath.so", "kernel32.dll")

**Examples:**
```python
# Windows
load_library("kernel32.dll")
load_library("user32")

# Linux
load_library("libm.so")
load_library("m")  # auto-detects libm.so

# macOS
load_library("libSystem.dylib")
```

### `define_function(library_name, function_name, return_type, param_types)`
Define a function signature from a loaded library.

**Parameters:**
- `library_name`: Name of the library containing the function
- `function_name`: Name of the function in the library
- `return_type`: Return type of the function
- `param_types`: List of parameter types

**Supported Types:**
- `int`, `int8`, `int16`, `int32`, `int64`
- `uint`, `uint8`, `uint16`, `uint32`, `uint64`
- `float`, `double`
- `char`, `string`
- `pointer`, `void`
- `bool`, `size_t`, `ssize_t`
- `long`, `ulong`

### `call_function(library_name, function_name, args)`
Call an external function from a loaded library.

**Parameters:**
- `library_name`: Name of the library
- `function_name`: Name of the function
- `args`: List of arguments to pass to the function

**Returns:** The result of the function call

### `unload_library(library_name)`
Unload a previously loaded library.

### `list_libraries()`
List all loaded libraries.

**Returns:** List of library names

### `library_info(library_name)`
Get information about a loaded library.

**Returns:** Dictionary with library information

### `add_library_path(path)`
Add a custom search path for libraries.

## Example Usage

```python
# Load a system library
load_library("kernel32.dll")  # Windows
# or
load_library("libm.so")       # Linux

# Define a function signature
define_function("kernel32.dll", "GetTickCount", "int32", [])
define_function("libm.so", "sqrt", "double", ["double"])

# Call the function
tick_count = call_function("kernel32.dll", "GetTickCount", [])
square_root = call_function("libm.so", "sqrt", [16.0])

print(f"Tick count: {tick_count}")
print(f"Square root of 16: {square_root}")
```

## Supported Platforms

### Windows
- Library extensions: `.dll`, `.DLL`
- System paths: `System32`, `SysWOW64`
- Common libraries: `kernel32.dll`, `user32.dll`, `msvcrt.dll`

### Linux
- Library extensions: `.so`, `.so.1`, `.so.2`, `.so.3`
- System paths: `/lib`, `/usr/lib`, `/usr/local/lib`, `/lib64`, `/usr/lib64`
- Environment variables: `LD_LIBRARY_PATH`
- Common libraries: `libc.so.6`, `libm.so`, `libdl.so`

### macOS
- Library extensions: `.dylib`, `.so`
- System paths: `/usr/lib`, `/usr/local/lib`, `/opt/homebrew/lib`, `/opt/local/lib`
- Environment variables: `DYLD_LIBRARY_PATH`
- Common libraries: `libSystem.dylib`, `libm.dylib`

### Android
- Library extensions: `.so`
- System paths: `/system/lib`, `/system/lib64`, `/vendor/lib`, `/vendor/lib64`

### iOS
- Library extensions: `.dylib`, `.framework`
- System paths: `/System/Library/Frameworks`, `/System/Library/PrivateFrameworks`

## Testing FFI

To test the FFI functionality:

1. Build the test library:
   ```bash
   cd examples
   python build_test_lib.py
   ```

2. Run the test script:
   ```bash
   cargo run -- run examples/test_ffi.tauraro
   ```

## Error Handling

FFI functions will raise exceptions for various error conditions:
- Library not found
- Function not found
- Argument type mismatch
- Invalid library name
- Invalid function signature

Always wrap FFI calls in try-except blocks for robust error handling.