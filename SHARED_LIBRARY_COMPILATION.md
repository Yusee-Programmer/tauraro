# Shared Library Compilation Feature

## Overview

Tauraro now supports compiling Tauraro scripts to shared libraries (dynamic libraries) in addition to executables. This allows you to create reusable libraries that can be loaded by other programs.

## Usage

### Command Line Flag

Use the `--lib-type` flag with the `compile` command to specify the output type:

```bash
# Compile to executable (default)
tauraro compile script.py --backend c --native

# Compile to shared library
tauraro compile script.py --backend c --native --lib-type shared

# Explicitly specify executable
tauraro compile script.py --backend c --native --lib-type executable
```

### Platform-Specific Output

The shared library extension is automatically determined based on the target platform:

- **Linux/Unix**: `.so` (Shared Object)
- **Windows**: `.dll` (Dynamic Link Library)
- **macOS**: `.dylib` (Dynamic Library)

### Cross-Platform Compilation

You can specify a target platform using the `--target` flag:

```bash
# Compile for Linux
tauraro compile script.py --backend c --native --lib-type shared --target linux -o libmylib.so

# Compile for Windows
tauraro compile script.py --backend c --native --lib-type shared --target windows -o mylib.dll

# Compile for macOS
tauraro compile script.py --backend c --native --lib-type shared --target macos -o libmylib.dylib
```

## Compilation Flags

### Linux/Unix (GCC/Clang)

```bash
gcc -shared -fPIC script.c -o libscript.so -lm
clang -shared -fPIC script.c -o libscript.so -lm
```

**Flags:**
- `-shared`: Create a shared library
- `-fPIC`: Generate position-independent code (required for shared libraries)
- `-lm`: Link math library
- `-ldl`: Link dynamic loading library (automatically added when FFI is detected)

### Windows (MSVC)

```bash
cl /LD script.c /Fe:script.dll
```

**Flags:**
- `/LD`: Create a DLL (shared library)
- `/Fe:`: Specify output file name

### Windows (Clang-CL)

```bash
clang-cl -shared script.c -o script.dll
```

**Flags:**
- `-shared`: Create a shared library

## Examples

### Example 1: Simple Math Library

**math_lib.py:**
```python
def add(a, b):
    return a + b

def multiply(a, b):
    return a * b

def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

**Compile:**
```bash
tauraro compile math_lib.py --backend c --native --lib-type shared -o libmath.so
```

### Example 2: String Utilities Library

**string_utils.py:**
```python
def reverse_string(s):
    result = ""
    i = len(s) - 1
    while i >= 0:
        result = result + s[i]
        i = i - 1
    return result

def count_vowels(s):
    vowels = "aeiouAEIOU"
    count = 0
    for char in s:
        if char in vowels:
            count = count + 1
    return count
```

**Compile:**
```bash
tauraro compile string_utils.py --backend c --native --lib-type shared -o libstrutils.so
```

### Example 3: Cross-Platform Library

Compile the same source for multiple platforms:

```bash
# Linux
tauraro compile mylib.py --backend c --native --lib-type shared --target linux -o libmylib.so

# Windows
tauraro compile mylib.py --backend c --native --lib-type shared --target windows -o mylib.dll

# macOS
tauraro compile mylib.py --backend c --native --lib-type shared --target darwin -o libmylib.dylib
```

## Technical Details

### Platform Detection

The compiler automatically detects the target platform:

1. If `--target` is specified, use that platform
2. Otherwise, use the current OS (`std::env::consts::OS`)

### File Extension Resolution

The output file extension is determined as follows:

```
if lib_type == "shared":
    if target == "windows":
        extension = "dll"
    elif target in ["macos", "darwin"]:
        extension = "dylib"
    else:  # Linux, Unix, etc.
        extension = "so"
else:  # executable
    extension = EXE_EXTENSION  # "" on Unix, "exe" on Windows
```

### Compilation Process

1. **C Code Generation**: Tauraro script → C code
2. **FFI Detection**: Automatically detect if FFI functions are used
3. **Compiler Selection**: Try GCC, Clang, or MSVC in order
4. **Flag Configuration**: Add platform-specific shared library flags
5. **Linking**: Link with required libraries (-lm, -ldl if FFI detected)
6. **Output**: Generate shared library with correct extension

### Automatic Features

#### FFI Support

If your Tauraro script uses FFI functions (`load_library`, `define_function`, `call_function`), the compiler automatically:

1. Includes FFI implementation in generated C code
2. Adds `-ldl` flag on Unix/Linux platforms
3. Uses appropriate linking on Windows

#### Object File Linking

If your script imports builtin modules, the compiler automatically:

1. Compiles Rust FFI modules to object files
2. Links them with the shared library
3. Handles dependencies correctly

## Use Cases

### 1. Plugin Systems

Create plugins for applications:

```python
# plugin_audio.py
def process_audio(data):
    # Audio processing logic
    return processed_data
```

Compile to: `libplugin_audio.so`

### 2. Language Bindings

Create bindings for other languages:

```python
# tauraro_bindings.py
def init_tauraro():
    return "Tauraro v1.0"

def execute_script(code):
    # Execute Tauraro code
    return result
```

Compile to: `libtauraro_bindings.so`

### 3. Embedded Systems

Create libraries for embedded applications:

```python
# sensor_lib.py
def read_temperature():
    return temperature

def read_pressure():
    return pressure
```

Compile to: `libsensor.so`

### 4. Microservices

Create shared libraries for microservice components:

```python
# auth_service.py
def authenticate_user(username, password):
    # Authentication logic
    return token

def validate_token(token):
    # Validation logic
    return valid
```

Compile to: `libauth_service.so`

## Limitations

### Current Limitations

1. **Export Symbols**: Currently, all functions are exported. Future versions may support selective export.
2. **ABI Compatibility**: The generated shared library uses C ABI. Cross-language calling requires matching calling conventions.
3. **Name Mangling**: Function names are preserved from Tauraro. Consider using C-friendly names.

### Known Issues

1. **Rust FFI Object Files**: Multiple definition errors with Rust FFI object files are a known issue (pre-existing).
2. **Warning Messages**: Some format warnings in generated C code (pre-existing).

## Best Practices

### 1. API Design

Design your Tauraro library with a clear API:

```python
# Good: Clear, simple interface
def process_data(input_data):
    return result

# Avoid: Complex nested structures
def complex_function(nested_dict_list):
    return complex_result
```

### 2. Error Handling

Include proper error handling:

```python
def safe_divide(a, b):
    if b == 0:
        return None  # Or raise exception
    return a / b
```

### 3. Documentation

Document your library functions:

```python
def calculate_sum(numbers):
    """
    Calculate the sum of a list of numbers.

    Args:
        numbers: List of numeric values

    Returns:
        Sum of all numbers
    """
    total = 0
    for num in numbers:
        total = total + num
    return total
```

### 4. Testing

Test your library before deploying:

```python
# Test the library functions
if __name__ == "__main__":
    assert add(2, 3) == 5
    assert multiply(4, 5) == 20
    print("All tests passed!")
```

## Troubleshooting

### Issue: Compilation Failed

**Problem:** Compiler reports errors during shared library creation.

**Solution:**
1. Check the generated C code for errors
2. Try compiling manually with the suggested command
3. Ensure all dependencies are available

### Issue: Symbol Not Found

**Problem:** Loading the shared library fails with "symbol not found".

**Solution:**
1. Verify function names match exactly
2. Check that the function is not static
3. Use `nm -D libname.so` to list exported symbols

### Issue: Platform Mismatch

**Problem:** Shared library won't load on target platform.

**Solution:**
1. Ensure you compiled for the correct target platform
2. Use `--target` flag to specify the target explicitly
3. Check architecture (32-bit vs 64-bit)

## Future Enhancements

Planned improvements for shared library compilation:

1. **Symbol Visibility Control**: Export only specified functions
2. **Versioning Support**: Add version information to shared libraries
3. **Header Generation**: Automatically generate C header files
4. **Static Linking Option**: Compile to static libraries (.a, .lib)
5. **Size Optimization**: Reduce shared library size with stripping and optimization
6. **Debug Symbols**: Separate debug information for easier debugging

## Summary

The shared library compilation feature enables Tauraro to generate reusable dynamic libraries for:

- ✅ **Cross-platform support**: Linux (.so), Windows (.dll), macOS (.dylib)
- ✅ **Automatic flag handling**: Platform-specific compilation flags
- ✅ **FFI integration**: Automatic FFI support and linking
- ✅ **Easy usage**: Simple `--lib-type shared` flag
- ✅ **Target selection**: Compile for different platforms with `--target`

This opens up new possibilities for using Tauraro in plugin systems, language bindings, embedded systems, and microservice architectures.
