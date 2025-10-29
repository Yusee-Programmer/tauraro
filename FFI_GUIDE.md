# Tauraro FFI (Foreign Function Interface) Guide

## Overview

Tauraro provides comprehensive FFI support for calling native functions from dynamic libraries across multiple platforms. This allows you to integrate with system APIs, third-party libraries, and custom native code.

## Supported Platforms

- **Windows**: DLL files
- **Linux**: SO (Shared Object) files
- **macOS**: dylib files
- **iOS**: dylib/framework files
- **Android**: SO files
- **Embedded Systems**: Platform-specific libraries
- **Unix-like Systems**: SO files

## Quick Start

```python
# 1. Load a library
load_library("kernel32.dll")  # Windows
load_library("m")             # Linux/macOS (libm.so/libSystem.dylib)

# 2. Define a function signature
define_function("kernel32.dll", "GetTickCount", "uint32", [])

# 3. Call the function
result = call_function("kernel32.dll", "GetTickCount", [])
print(f"System uptime: {result} ms")
```

## FFI Functions

### load_library(library_name: str)

Load a dynamic library into memory.

**Parameters:**
- `library_name`: Name or path of the library

**Platform-specific naming:**
- Windows: `"kernel32.dll"` or `"kernel32"`
- Linux: `"libm.so"` or `"m"` (auto-detects lib prefix)
- macOS: `"libSystem.dylib"` or `"System"`

**Examples:**
```python
# Windows
load_library("kernel32.dll")
load_library("user32")  # Automatically finds user32.dll

# Linux
load_library("libm.so")
load_library("m")  # Automatically finds libm.so

# macOS
load_library("libSystem.dylib")
load_library("System")  # Automatically finds libSystem.dylib

# Absolute path
load_library("/usr/local/lib/custom.so")
load_library("C:\\MyLibs\\mylib.dll")
```

### define_function(library: str, function_name: str, return_type: str, param_types: list)

Define a function signature for an external function.

**Parameters:**
- `library`: Name of the loaded library
- `function_name`: Name of the function in the library
- `return_type`: Return type of the function
- `param_types`: List of parameter types

**Supported Types:**
- **Integers**: `int8`, `int16`, `int32`, `int64`, `uint8`, `uint16`, `uint32`, `uint64`
- **Floating Point**: `float`, `double`
- **Strings**: `string` (null-terminated C string)
- **Pointers**: `pointer`, `const_pointer`
- **Special**: `void`, `bool`, `char`
- **Size Types**: `size_t`, `ssize_t`

**Examples:**
```python
# Function with no parameters
define_function("kernel32.dll", "GetTickCount", "uint32", [])

# Function with one parameter
define_function("m", "sqrt", "double", ["double"])

# Function with multiple parameters
define_function("m", "pow", "double", ["double", "double"])

# Function with void return
define_function("kernel32.dll", "Sleep", "void", ["uint32"])

# Function with string parameters
define_function("user32.dll", "MessageBoxA", "int32", ["pointer", "string", "string", "uint32"])
```

### call_function(library: str, function_name: str, args: list) -> value

Call an external function.

**Parameters:**
- `library`: Name of the loaded library
- `function_name`: Name of the function to call
- `args`: List of arguments to pass

**Returns:** The return value of the function (type depends on function signature)

**Examples:**
```python
# Function with no arguments
uptime = call_function("kernel32.dll", "GetTickCount", [])

# Function with one argument
result = call_function("m", "sqrt", [16.0])  # Returns 4.0

# Function with multiple arguments
result = call_function("m", "pow", [2.0, 10.0])  # Returns 1024.0

# Function with void return
call_function("kernel32.dll", "Sleep", [1000])  # Sleep for 1 second
```

### unload_library(library_name: str)

Unload a previously loaded library.

**Example:**
```python
unload_library("kernel32.dll")
```

### list_libraries() -> list

Get a list of all loaded libraries.

**Returns:** List of library names

**Example:**
```python
libs = list_libraries()
print(libs)  # ["kernel32.dll", "m", "user32.dll"]
```

### library_info(library_name: str) -> dict

Get information about a loaded library.

**Returns:** Dictionary with keys:
- `name`: Library name
- `path`: Full path to the library file
- `functions`: Number of defined functions

**Example:**
```python
info = library_info("kernel32.dll")
print(f"Name: {info['name']}")
print(f"Path: {info['path']}")
print(f"Functions: {info['functions']}")
```

### add_library_path(path: str)

Add a custom search path for libraries.

**Example:**
```python
add_library_path("/usr/local/mylibs")
add_library_path("C:\\MyLibraries")
```

## Type Mapping

### Tauraro to C Type Mapping

| Tauraro Type | C Type | Description |
|--------------|--------|-------------|
| `int8` | `int8_t` | 8-bit signed integer |
| `int16` | `int16_t` | 16-bit signed integer |
| `int32` | `int32_t` | 32-bit signed integer |
| `int64` | `int64_t` | 64-bit signed integer |
| `uint8` | `uint8_t` | 8-bit unsigned integer |
| `uint16` | `uint16_t` | 16-bit unsigned integer |
| `uint32` | `uint32_t` | 32-bit unsigned integer |
| `uint64` | `uint64_t` | 64-bit unsigned integer |
| `float` | `float` | 32-bit floating point |
| `double` | `double` | 64-bit floating point |
| `string` | `const char*` | Null-terminated string |
| `pointer` | `void*` | Generic pointer |
| `void` | `void` | No return value |
| `bool` | `bool` / `uint8_t` | Boolean value |
| `size_t` | `size_t` | Size type |

### Value Conversion

- **Integers**: Tauraro `Int` ↔ C integer types
- **Floats**: Tauraro `Float` ↔ C float/double
- **Strings**: Tauraro `Str` ↔ C null-terminated strings
- **Booleans**: Tauraro `Bool` ↔ C bool (0/1)
- **None**: Used for `void` return types and null pointers

## Platform-Specific Examples

### Windows

```python
# Load Windows libraries
load_library("kernel32.dll")
load_library("user32.dll")

# Get system uptime
define_function("kernel32.dll", "GetTickCount", "uint32", [])
uptime = call_function("kernel32.dll", "GetTickCount", [])
print(f"Uptime: {uptime} ms")

# Sleep
define_function("kernel32.dll", "Sleep", "void", ["uint32"])
call_function("kernel32.dll", "Sleep", [1000])

# Get process ID
define_function("kernel32.dll", "GetCurrentProcessId", "uint32", [])
pid = call_function("kernel32.dll", "GetCurrentProcessId", [])
print(f"PID: {pid}")
```

### Linux

```python
# Load math library
load_library("m")

# Math functions
define_function("m", "sqrt", "double", ["double"])
define_function("m", "pow", "double", ["double", "double"])
define_function("m", "sin", "double", ["double"])

result = call_function("m", "sqrt", [16.0])
result = call_function("m", "pow", [2.0, 10.0])
result = call_function("m", "sin", [1.5708])  # π/2

# Load libc
load_library("c")

# String functions
define_function("c", "strlen", "size_t", ["string"])
length = call_function("c", "strlen", ["Hello"])

# Process functions
define_function("c", "getpid", "int32", [])
pid = call_function("c", "getpid", [])
```

### macOS

```python
# Load System library (contains libm and libc)
load_library("System")

# Math functions
define_function("System", "sqrt", "double", ["double"])
define_function("System", "pow", "double", ["double", "double"])

# String functions
define_function("System", "strlen", "size_t", ["string"])

# Process functions
define_function("System", "getpid", "int32", [])

# Time functions
define_function("System", "time", "int64", ["pointer"])
timestamp = call_function("System", "time", [0])
```

## Cross-Platform Code

```python
import sys

# Load appropriate library based on platform
if sys.platform == "win32":
    load_library("msvcrt.dll")
    math_lib = "msvcrt.dll"
elif sys.platform == "linux":
    load_library("m")
    math_lib = "m"
elif sys.platform == "darwin":
    load_library("System")
    math_lib = "System"

# Define functions (same across platforms)
define_function(math_lib, "sqrt", "double", ["double"])
define_function(math_lib, "pow", "double", ["double", "double"])

# Call functions
result = call_function(math_lib, "sqrt", [25.0])
result = call_function(math_lib, "pow", [2.0, 8.0])
```

## Custom Libraries

### Creating a Custom Library

**C Code (mymath.c):**
```c
#ifdef __cplusplus
extern "C" {
#endif

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

const char* get_version() {
    return "1.0.0";
}

#ifdef __cplusplus
}
#endif
```

**Compile:**
```bash
# Windows
cl /LD mymath.c /Fe:mymath.dll

# Linux
gcc -shared -fPIC -o libmymath.so mymath.c

# macOS
gcc -shared -fPIC -o libmymath.dylib mymath.c
```

**Use in Tauraro:**
```python
# Add library path
add_library_path("./lib")

# Load library
load_library("mymath")

# Define functions
define_function("mymath", "add", "int32", ["int32", "int32"])
define_function("mymath", "multiply", "int32", ["int32", "int32"])
define_function("mymath", "get_version", "string", [])

# Call functions
result = call_function("mymath", "add", [10, 20])
result = call_function("mymath", "multiply", [7, 8])
version = call_function("mymath", "get_version", [])
```

## Advanced Features

### Library Search Paths

Tauraro automatically searches standard system paths:

**Windows:**
- `%SystemRoot%\System32`
- `%SystemRoot%\SysWOW64`
- Current directory

**Linux:**
- `/lib`
- `/usr/lib`
- `/usr/local/lib`
- `/lib64`
- `/usr/lib64`
- `$LD_LIBRARY_PATH`

**macOS:**
- `/usr/lib`
- `/usr/local/lib`
- `/opt/homebrew/lib`
- `/opt/local/lib`
- `$DYLD_LIBRARY_PATH`

**Android:**
- `/system/lib`
- `/system/lib64`
- `/vendor/lib`
- `/vendor/lib64`

### Adding Custom Search Paths

```python
# Add multiple paths
add_library_path("/usr/local/mylibs")
add_library_path("/opt/custom/lib")
add_library_path("C:\\MyLibraries")

# Then load library
load_library("mylib")  # Will search all paths
```

## Error Handling

```python
try:
    load_library("mylib")
except Exception as e:
    print(f"Failed to load library: {e}")

try:
    define_function("mylib", "myfunc", "int32", ["int32"])
except Exception as e:
    print(f"Failed to define function: {e}")

try:
    result = call_function("mylib", "myfunc", [42])
except Exception as e:
    print(f"Failed to call function: {e}")
```

## Best Practices

1. **Always load the library first** before defining functions
2. **Match types exactly** - incorrect types can cause crashes
3. **Handle errors** - use try/except for robustness
4. **Unload libraries** when no longer needed to free memory
5. **Use cross-platform code** when possible for portability
6. **Document function signatures** from library documentation
7. **Test on all target platforms** to ensure compatibility

## Security Considerations

- **Only load trusted libraries** - malicious libraries can execute arbitrary code
- **Validate inputs** before passing to native functions
- **Be careful with pointers** - incorrect pointer usage can cause crashes
- **Avoid loading libraries from untrusted sources**
- **Use sandboxing** when loading third-party libraries

## Limitations

- **No struct support yet** - complex structures not fully supported
- **String lifetime** - strings must remain valid during function calls
- **Callback functions** - callbacks from C to Tauraro not yet supported
- **Variadic functions** - functions with variable arguments not fully supported

## Performance

- **Library loading**: One-time cost per library
- **Function definition**: One-time cost per function
- **Function calls**: Near-native performance using libffi
- **Type marshalling**: Minimal overhead for simple types

## Examples

See the `examples/` directory for complete examples:
- `ffi_windows_example.py` - Windows-specific FFI
- `ffi_linux_example.py` - Linux-specific FFI
- `ffi_macos_example.py` - macOS-specific FFI
- `ffi_cross_platform_example.py` - Cross-platform FFI
- `ffi_custom_library_example.py` - Custom library integration

## Troubleshooting

### Library Not Found
```
Error: Failed to find library: mylib
```
**Solution:** Use `add_library_path()` or provide full path

### Function Not Found
```
Error: Function not found: myfunc
```
**Solution:** Check function name spelling and ensure it's exported with `extern "C"`

### Type Mismatch
```
Error: Cannot convert Int to float
```
**Solution:** Ensure argument types match the function signature

### Crash on Function Call
**Solution:**
- Verify function signature is correct
- Check parameter count and types
- Ensure strings are null-terminated
- Validate pointer values

## Additional Resources

- [libffi documentation](https://sourceware.org/libffi/)
- [Windows API documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [Linux man pages](https://man7.org/linux/man-pages/)
- [macOS developer documentation](https://developer.apple.com/documentation/)
