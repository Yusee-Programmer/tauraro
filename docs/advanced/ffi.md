# Foreign Function Interface (FFI)

Tauraro provides a powerful Foreign Function Interface (FFI) that allows you to call functions from native libraries written in C, C++, or other languages that expose a C-compatible API.

## Overview

The FFI system enables:
- **Cross-platform library loading** (Windows DLL, Linux SO, macOS dylib)
- **Automatic type marshalling** between Tauraro and C types
- **Multiple calling conventions** (C, stdcall, fastcall, etc.)
- **Memory buffer management** for structure passing
- **Dynamic function discovery** and calling

### Supported Platforms

- ✅ **Windows** - DLL files (`.dll`)
- ✅ **Linux** - Shared objects (`.so`, `.so.1`, etc.)
- ✅ **macOS** - Dynamic libraries (`.dylib`)
- ✅ **iOS** - Frameworks and dynamic libraries
- ✅ **Android** - NDK shared objects
- ✅ **Embedded Systems** - Generic SO and static libraries

## Core FFI Functions

### load_library(name)

Load a native library into memory.

```python
# Windows
load_library("kernel32")    # Loads kernel32.dll
load_library("msvcrt")      # Loads msvcrt.dll

# Linux (auto-adds 'lib' prefix and '.so' extension)
load_library("m")           # Loads libm.so (math library)
load_library("pthread")     # Loads libpthread.so

# macOS
load_library("System")      # Loads libSystem.dylib
load_library("c")           # Loads libc.dylib
```

**Platform-Specific Behavior:**

**Windows:**
- Searches: Current directory → System32 → SysWOW64 → PATH
- Automatically adds `.dll` extension
- Example: `"kernel32"` → `"kernel32.dll"`

**Linux:**
- Searches: `/lib`, `/usr/lib`, `/usr/local/lib`, `/lib64`, `/usr/lib64`
- Checks `LD_LIBRARY_PATH` environment variable
- Automatically adds `lib` prefix and `.so` extension
- Example: `"m"` → `"libm.so"`, `"libm.so.6"`

**macOS:**
- Searches: `/usr/lib`, `/usr/local/lib`, `/opt/homebrew/lib`, `/opt/local/lib`
- Checks `DYLD_LIBRARY_PATH` environment variable
- Automatically adds `lib` prefix and `.dylib` extension
- Example: `"c"` → `"libc.dylib"`

### define_function(library, name, return_type, param_types)

Define a function signature from a loaded library.

**Parameters:**
- `library` (str): Name of the loaded library
- `name` (str): Name of the function to call
- `return_type` (str): Return type (see Type System below)
- `param_types` (list): List of parameter types

**Returns:** `ExternFunction` object that can be called directly

```python
# Define a function with no parameters
define_function("kernel32", "GetTickCount", "int32", [])

# Define a function with one parameter
define_function("msvcrt", "sqrt", "double", ["double"])

# Define a function with multiple parameters
define_function("msvcrt", "pow", "double", ["double", "double"])

# Define a function returning void
define_function("mylib", "print_message", "void", ["string"])

# Store the function for later use
sqrt = define_function("m", "sqrt", "double", ["double"])
```

### call_function(library, name, args)

Call a previously defined native function.

**Parameters:**
- `library` (str): Name of the loaded library
- `name` (str): Name of the function
- `args` (list): List of arguments to pass

**Returns:** The return value converted to a Tauraro value

```python
# Call function with no arguments
ticks = call_function("kernel32", "GetTickCount", [])

# Call function with one argument
result = call_function("msvcrt", "sqrt", [16.0])  # Returns 4.0

# Call function with multiple arguments
result = call_function("msvcrt", "pow", [2.0, 3.0])  # Returns 8.0

# Call function via ExternFunction
sqrt = define_function("m", "sqrt", "double", ["double"])
result = sqrt([25.0])  # Returns 5.0
```

### unload_library(name)

Unload a library and free associated resources.

```python
load_library("mylib")
# ... use library ...
unload_library("mylib")  # Free the library
```

### list_libraries()

Get a list of all currently loaded libraries.

```python
load_library("kernel32")
load_library("msvcrt")
libs = list_libraries()
print(libs)  # ['kernel32', 'msvcrt']
```

### library_info(name)

Get detailed information about a loaded library.

```python
info = library_info("msvcrt")
print(info)
# {
#   'name': 'msvcrt',
#   'path': 'C:\\Windows\\System32\\msvcrt.dll',
#   'functions': ['sqrt', 'pow', 'printf', ...]
# }
```

### add_library_path(path)

Add a custom directory to the library search path.

```python
# Add custom search path
add_library_path("/opt/mylibs")
add_library_path("C:\\MyLibs")

# Now load_library will search these directories too
load_library("mycustomlib")
```

### allocate_buffer(size)

Allocate a memory buffer for passing structures to C functions.

```python
# Allocate 128 bytes
buffer = allocate_buffer(128)
print(buffer)  # Returns pointer address

# Use with FFI calls
call_function("mylib", "fill_struct", [buffer, 42])

# Always free when done
free_buffer(buffer)
```

### free_buffer(ptr)

Free a previously allocated buffer.

```python
buffer = allocate_buffer(64)
# ... use buffer ...
free_buffer(buffer)  # Must free to prevent memory leak
```

## Type System

### Basic Types

| Tauraro Type | C Type | Description | Size |
|--------------|--------|-------------|------|
| `"int"` | `int` | Signed integer | Platform (usually 4 bytes) |
| `"int8"` | `int8_t` | 8-bit signed integer | 1 byte |
| `"int16"` | `int16_t` | 16-bit signed integer | 2 bytes |
| `"int32"` | `int32_t` | 32-bit signed integer | 4 bytes |
| `"int64"` | `int64_t` | 64-bit signed integer | 8 bytes |
| `"uint"` | `unsigned int` | Unsigned integer | Platform |
| `"uint8"` | `uint8_t` | 8-bit unsigned integer | 1 byte |
| `"uint16"` | `uint16_t` | 16-bit unsigned integer | 2 bytes |
| `"uint32"` | `uint32_t` | 32-bit unsigned integer | 4 bytes |
| `"uint64"` | `uint64_t` | 64-bit unsigned integer | 8 bytes |

### Floating Point Types

| Tauraro Type | C Type | Description | Size |
|--------------|--------|-------------|------|
| `"float"` | `float` | Single-precision float | 4 bytes |
| `"double"` | `double` | Double-precision float | 8 bytes |

### String and Character Types

| Tauraro Type | C Type | Description |
|--------------|--------|-------------|
| `"char"` | `char` | Single character |
| `"string"` | `const char*` | Null-terminated string |
| `"wstring"` | `wchar_t*` | Wide string (UTF-16 on Windows) |

### Pointer Types

| Tauraro Type | C Type | Description |
|--------------|--------|-------------|
| `"pointer"` | `void*` | Generic mutable pointer |
| `"const_pointer"` | `const void*` | Generic const pointer |

### Size Types

| Tauraro Type | C Type | Description |
|--------------|--------|-------------|
| `"size_t"` | `size_t` | Unsigned size type |
| `"ssize_t"` | `ssize_t` | Signed size type |

### Long Types

| Tauraro Type | C Type | Description |
|--------------|--------|-------------|
| `"long"` | `long` | Long integer |
| `"ulong"` | `unsigned long` | Unsigned long |
| `"longlong"` | `long long` | 64-bit long long |
| `"ulonglong"` | `unsigned long long` | Unsigned 64-bit long long |

### Special Types

| Tauraro Type | C Type | Description |
|--------------|--------|-------------|
| `"void"` | `void` | No return value (for return type only) |
| `"bool"` | `bool` / `_Bool` | Boolean value |

## Type Conversion

Automatic conversion between Tauraro values and C types:

| Tauraro Value | C Type | Conversion |
|---------------|--------|------------|
| `Value::Int(n)` | `int`, `long`, etc. | Direct cast |
| `Value::Float(f)` | `float`, `double` | Direct cast |
| `Value::Str(s)` | `const char*` | Null-terminated CString |
| `Value::Bool(b)` | `bool` | true → 1, false → 0 |
| `Value::None` | `void*` | NULL pointer |

**Example:**
```python
# Tauraro automatically converts:
result = call_function("m", "sqrt", [16.0])
# 16.0 (Float) → double → sqrt() → double → 4.0 (Float)
```

## Platform-Specific Examples

### Windows Examples

```python
# System functions
load_library("kernel32")
define_function("kernel32", "GetTickCount", "uint32", [])
ticks = call_function("kernel32", "GetTickCount", [])
print(f"System uptime: {ticks}ms")

# Process ID
define_function("kernel32", "GetCurrentProcessId", "uint32", [])
pid = call_function("kernel32", "GetCurrentProcessId", [])
print(f"Process ID: {pid}")

# Math functions
load_library("msvcrt")
define_function("msvcrt", "sqrt", "double", ["double"])
define_function("msvcrt", "pow", "double", ["double", "double"])

print(call_function("msvcrt", "sqrt", [144.0]))    # 12.0
print(call_function("msvcrt", "pow", [2.0, 10.0])) # 1024.0

# String functions
define_function("msvcrt", "strlen", "size_t", ["string"])
length = call_function("msvcrt", "strlen", ["Hello, World!"])
print(f"String length: {length}")  # 13
```

### Linux Examples

```python
# Math library
load_library("m")  # Loads libm.so
define_function("m", "sin", "double", ["double"])
define_function("m", "cos", "double", ["double"])
define_function("m", "sqrt", "double", ["double"])

import math
print(call_function("m", "sin", [math.pi/2]))  # 1.0
print(call_function("m", "cos", [0.0]))        # 1.0
print(call_function("m", "sqrt", [2.0]))       # 1.414...

# C standard library
load_library("c")  # Loads libc.so
define_function("c", "strlen", "size_t", ["string"])
define_function("c", "strcmp", "int", ["string", "string"])

print(call_function("c", "strlen", ["Test"]))           # 4
print(call_function("c", "strcmp", ["abc", "abc"]))     # 0
print(call_function("c", "strcmp", ["abc", "def"]))     # negative

# POSIX functions
define_function("c", "getpid", "int", [])
pid = call_function("c", "getpid", [])
print(f"Process ID: {pid}")
```

### macOS Examples

```python
# System library (contains most C standard functions)
load_library("System")  # Loads libSystem.dylib

# Math functions
define_function("System", "sqrt", "double", ["double"])
define_function("System", "pow", "double", ["double", "double"])

print(call_function("System", "sqrt", [49.0]))        # 7.0
print(call_function("System", "pow", [3.0, 3.0]))     # 27.0

# String functions
define_function("System", "strlen", "size_t", ["string"])
length = call_function("System", "strlen", ["macOS"])
print(f"Length: {length}")  # 5

# Process functions
define_function("System", "getpid", "int32", [])
pid = call_function("System", "getpid", [])
print(f"Process ID: {pid}")
```

### Cross-Platform Example

```python
import sys

# Determine platform and load appropriate library
if sys.platform == "win32":
    lib = "msvcrt"
    load_library("msvcrt")
elif sys.platform == "darwin":
    lib = "System"
    load_library("System")
else:  # Linux
    lib = "m"
    load_library("m")

# Define math functions (same signature on all platforms)
define_function(lib, "sqrt", "double", ["double"])
define_function(lib, "pow", "double", ["double", "double"])
define_function(lib, "sin", "double", ["double"])
define_function(lib, "cos", "double", ["double"])

# Use the functions
print(call_function(lib, "sqrt", [16.0]))       # 4.0
print(call_function(lib, "pow", [2.0, 8.0]))    # 256.0
print(call_function(lib, "sin", [0.0]))         # 0.0
print(call_function(lib, "cos", [0.0]))         # 1.0
```

## Advanced Usage

### Using ExternFunction Objects

`define_function` returns an `ExternFunction` object that can be called directly:

```python
# Define and store function
sqrt = define_function("m", "sqrt", "double", ["double"])
pow = define_function("m", "pow", "double", ["double", "double"])

# Call directly (cleaner syntax)
print(sqrt([16.0]))         # 4.0
print(pow([2.0, 10.0]))     # 1024.0

# Pass as argument to other functions
def apply_function(func, value):
    return func([value])

result = apply_function(sqrt, 25.0)  # 5.0
```

### Working with Structures

Use buffers to pass structures to C functions:

```python
# Example: Calling a function that fills a structure
# C code:
# typedef struct {
#     int x;
#     int y;
#     int width;
#     int height;
# } Rect;
# void get_window_rect(Rect* rect);

load_library("mylib")

# Allocate buffer for structure (4 ints = 16 bytes)
rect = allocate_buffer(16)

# Call function to fill structure
define_function("mylib", "get_window_rect", "void", ["pointer"])
call_function("mylib", "get_window_rect", [rect])

# Read values from buffer (requires manual unpacking)
# In practice, you'd use struct.unpack or similar

# Free buffer when done
free_buffer(rect)
```

### Working with Arrays

Pass arrays using pointers:

```python
# Example: Sum an array of integers
# C code: int sum_array(int* arr, int count);

load_library("mylib")
define_function("mylib", "sum_array", "int", ["pointer", "int"])

# Allocate buffer for 10 integers (40 bytes)
arr = allocate_buffer(40)

# Write values to buffer (requires manual packing)
# ...

# Call function
total = call_function("mylib", "sum_array", [arr, 10])
print(f"Sum: {total}")

free_buffer(arr)
```

### String Return Values

Functions that return strings need special handling:

```python
# Example: Get version string
# C code: const char* get_version();

load_library("mylib")
define_function("mylib", "get_version", "string", [])

# Call and get string (automatically converted)
version = call_function("mylib", "get_version", [])
print(f"Version: {version}")
```

### Error Handling

Always use try-except for FFI calls:

```python
try:
    load_library("mylib")
except Exception as e:
    print(f"Failed to load library: {e}")
    # Library not found or incompatible

try:
    define_function("mylib", "my_function", "int", ["int"])
except Exception as e:
    print(f"Failed to define function: {e}")
    # Function not found in library

try:
    result = call_function("mylib", "my_function", [42])
except Exception as e:
    print(f"Failed to call function: {e}")
    # Wrong arguments or function crashed
```

## Calling Conventions

On Windows, you may need to specify calling conventions:

```python
# Most functions use C convention (default)
define_function("mylib", "func", "int", ["int"])  # Uses C convention

# For Windows API functions that use stdcall
define_function("kernel32", "ExitProcess", "void", ["uint"],
                calling_convention="stdcall")

# Available conventions:
# - "c" (default) - Standard C convention
# - "stdcall" - Windows stdcall (most Win32 API)
# - "fastcall" - Fastcall convention
# - "cdecl" - C declaration convention
# - "thiscall" - C++ member functions
# - "vectorcall" - Vector calling convention
```

## Safety and Best Practices

### ⚠️ Important Safety Notes

1. **Incorrect Signatures Can Crash Your Program**
   ```python
   # WRONG - will crash or corrupt memory
   define_function("m", "sqrt", "int", ["int"])  # sqrt expects double!

   # CORRECT
   define_function("m", "sqrt", "double", ["double"])
   ```

2. **Argument Count Must Match**
   ```python
   define_function("m", "pow", "double", ["double", "double"])

   # WRONG - pow expects 2 arguments
   call_function("m", "pow", [2.0])  # Will crash

   # CORRECT
   call_function("m", "pow", [2.0, 3.0])
   ```

3. **Always Free Allocated Buffers**
   ```python
   buffer = allocate_buffer(128)
   try:
       # Use buffer...
       pass
   finally:
       free_buffer(buffer)  # Always free!
   ```

4. **String Lifetime**
   ```python
   # String pointers must remain valid during the call
   name = "Hello"
   define_function("mylib", "process", "void", ["string"])
   call_function("mylib", "process", [name])  # OK
   ```

5. **Platform-Specific Functions**
   ```python
   # Check platform before calling platform-specific functions
   import sys
   if sys.platform == "win32":
       load_library("kernel32")
       define_function("kernel32", "GetTickCount", "uint32", [])
   else:
       print("GetTickCount only available on Windows")
   ```

### Best Practices

1. **Use Type Annotations in Wrappers**
   ```python
   def sqrt(x: float) -> float:
       """Calculate square root using C library."""
       return call_function("m", "sqrt", [x])
   ```

2. **Create Wrapper Classes**
   ```python
   class MathLib:
       def __init__(self):
           if sys.platform == "win32":
               load_library("msvcrt")
               self.lib = "msvcrt"
           else:
               load_library("m")
               self.lib = "m"

           self.sqrt = define_function(self.lib, "sqrt",
                                       "double", ["double"])
           self.pow = define_function(self.lib, "pow",
                                      "double", ["double", "double"])

       def square_root(self, x):
           return self.sqrt([x])

       def power(self, base, exp):
           return self.pow([base, exp])

   math = MathLib()
   print(math.square_root(16.0))
   print(math.power(2.0, 8.0))
   ```

3. **Document FFI Dependencies**
   ```python
   """
   This module requires:
   - Windows: msvcrt.dll (included with Windows)
   - Linux: libm.so.6 (install with: apt-get install libc6-dev)
   - macOS: libSystem.dylib (included with macOS)
   """
   ```

4. **Test on All Target Platforms**
   - FFI code is inherently platform-specific
   - Always test on Windows, Linux, and macOS if targeting multiple platforms
   - Use platform detection and conditional loading

## Real-World Examples

### Example 1: Simple Calculator Using C Math Library

```python
"""Cross-platform calculator using C math functions."""
import sys

class Calculator:
    def __init__(self):
        # Load platform-specific math library
        if sys.platform == "win32":
            load_library("msvcrt")
            self.lib = "msvcrt"
        elif sys.platform == "darwin":
            load_library("System")
            self.lib = "System"
        else:
            load_library("m")
            self.lib = "m"

        # Define functions
        self.sqrt = define_function(self.lib, "sqrt", "double", ["double"])
        self.pow = define_function(self.lib, "pow", "double", ["double", "double"])
        self.sin = define_function(self.lib, "sin", "double", ["double"])
        self.cos = define_function(self.lib, "cos", "double", ["double"])

    def square_root(self, x):
        return self.sqrt([x])

    def power(self, base, exponent):
        return self.pow([base, exponent])

    def sine(self, angle):
        return self.sin([angle])

    def cosine(self, angle):
        return self.cos([angle])

# Usage
calc = Calculator()
print(f"sqrt(144) = {calc.square_root(144.0)}")
print(f"2^10 = {calc.power(2.0, 10.0)}")
print(f"sin(0) = {calc.sine(0.0)}")
```

### Example 2: System Information (Windows)

```python
"""Get system information using Windows API."""
import sys

if sys.platform != "win32":
    print("This example is Windows-only")
    exit(1)

# Load kernel32
load_library("kernel32")

# Get tick count (system uptime in ms)
define_function("kernel32", "GetTickCount", "uint32", [])
ticks = call_function("kernel32", "GetTickCount", [])
uptime_seconds = ticks / 1000
uptime_hours = uptime_seconds / 3600
print(f"System uptime: {uptime_hours:.2f} hours")

# Get process ID
define_function("kernel32", "GetCurrentProcessId", "uint32", [])
pid = call_function("kernel32", "GetCurrentProcessId", [])
print(f"Current process ID: {pid}")

# Get thread ID
define_function("kernel32", "GetCurrentThreadId", "uint32", [])
tid = call_function("kernel32", "GetCurrentThreadId", [])
print(f"Current thread ID: {tid}")
```

### Example 3: File Operations (Cross-Platform)

```python
"""File operations using C standard library."""
import sys

# Load C library
if sys.platform == "win32":
    load_library("msvcrt")
    lib = "msvcrt"
else:
    load_library("c")
    lib = "c"

# String length function
define_function(lib, "strlen", "size_t", ["string"])

# Test with different strings
strings = ["Hello", "World", "Tauraro", ""]
for s in strings:
    length = call_function(lib, "strlen", [s])
    print(f"Length of '{s}': {length}")
```

## Performance Considerations

- **FFI calls have overhead**: ~100-500ns per call
- **Batch operations** when possible
- **Type conversion has cost**: Direct types (int, float) are fastest
- **String conversion** requires allocation (use sparingly in tight loops)
- **Buffer reuse**: Allocate buffers once and reuse them

```python
# SLOW - allocates buffer each time
def slow_version():
    for i in range(1000):
        buffer = allocate_buffer(64)
        call_function("mylib", "process", [buffer])
        free_buffer(buffer)

# FAST - reuses buffer
def fast_version():
    buffer = allocate_buffer(64)
    try:
        for i in range(1000):
            call_function("mylib", "process", [buffer])
    finally:
        free_buffer(buffer)
```

## Troubleshooting

### Library Not Found

```python
# Check search paths
add_library_path("/custom/path")

# Print current directory
import os
print(os.getcwd())

# Use absolute path
load_library("/absolute/path/to/library.so")
```

### Function Not Found

```python
# List all functions in library
info = library_info("mylib")
print(info['functions'])

# Check function name spelling (case-sensitive)
# Check if function is exported (use nm, dumpbin, or otool)
```

### Type Mismatch Crashes

```python
# Always verify function signature in C header
# Use correct types for each parameter
# Test with simple known functions first (like sqrt)
```

### Platform Differences

```python
# Use platform detection
import sys
if sys.platform == "win32":
    # Windows-specific code
elif sys.platform == "darwin":
    # macOS-specific code
else:
    # Linux/Unix-specific code
```

## Next Steps

- [Performance Optimization](performance.md)
- [C Backend Compilation](../compilation/c-backend.md)
- [Memory Management](memory.md)
- [Creating Native Extensions](native-extensions.md)
