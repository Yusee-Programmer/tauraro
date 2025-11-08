# Language Interoperability

Tauraro is designed to work seamlessly with existing code and systems.

## Python Compatibility

Tauraro supports Python syntax, making it easy to run existing Python code.

### Running Python Code

```python
# Standard Python code works in Tauraro
def greet(name):
    return f"Hello, {name}!"

numbers = [1, 2, 3, 4, 5]
squares = [x ** 2 for x in numbers]

print(squares)
```

### Compatibility Notes

✅ **Fully Supported:**
- Functions, classes, decorators
- List/dict/set comprehensions
- Generators and iterators
- Context managers
- Exception handling
- Async/await

⚠️ **Partially Supported:**
- Dynamic imports (better in VM mode)
- `eval()` and `exec()` (VM only)
- Metaclasses (basic support)

❌ **Not Supported:**
- C Python extensions (use FFI instead)
- Some advanced metaclass features

## C Interoperability

### FFI (Foreign Function Interface)

Call C libraries directly from Tauraro:

```python
# Load C library
load_library("m")  # Math library

# Define function signature
define_function("m", "sqrt", "double", ["double"])

# Call C function
result = call_function("m", "sqrt", [144.0])
print(result)  # 12.0
```

### Calling System Libraries

```python
# Load system library
load_library("c")

# Call malloc/free
define_function("c", "malloc", "pointer", ["size_t"])
define_function("c", "free", "void", ["pointer"])

buffer = call_function("c", "malloc", [1024])
call_function("c", "free", [buffer])
```

### Type Mapping

| Tauraro Type | C Type |
|--------------|--------|
| int | int64_t |
| float | double |
| str | char* |
| bool | bool |
| bytes | uint8_t* |
| None | void |

## Compiled Code Interop

### Linking with C Libraries

```bash
# Compile Tauraro code and link with C library
tauraro compile program.py -o program -l math -l pthread
```

### Mixing Tauraro and C

**C header (mylib.h):**
```c
// C function
int add_numbers(int a, int b);
```

**C implementation (mylib.c):**
```c
int add_numbers(int a, int b) {
    return a + b;
}
```

**Tauraro code:**
```python
# Use the C function
load_library("./mylib.so")
define_function("mylib", "add_numbers", "int", ["int", "int"])

result = call_function("mylib", "add_numbers", [5, 10])
print(result)  # 15
```

**Compile:**
```bash
# Compile C library
gcc -shared -fPIC mylib.c -o mylib.so

# Use in Tauraro
tauraro run program.py
```

## Cross-Platform Development

### Platform Detection

```python
import sys

if sys.platform == "linux":
    # Linux-specific code
    load_library("linux_lib.so")
elif sys.platform == "darwin":
    # macOS-specific code
    load_library("macos_lib.dylib")
elif sys.platform == "win32":
    # Windows-specific code
    load_library("windows_lib.dll")
```

### Cross-Compilation

```bash
# Compile for different platforms
tauraro compile program.py -o program-linux --target x86_64-unknown-linux-gnu
tauraro compile program.py -o program-macos --target x86_64-apple-darwin
tauraro compile program.py -o program.exe --target x86_64-pc-windows-gnu
```

## Data Exchange

### JSON for Data Exchange

```python
import json

# Serialize to JSON
data = {"name": "Alice", "age": 30}
json_str = json.dumps(data)

# Deserialize from JSON
parsed = json.loads(json_str)
```

### Binary Data with Pickle

```python
import pickle

# Serialize object
data = [1, 2, 3, 4, 5]
serialized = pickle.dumps(data)

# Deserialize
restored = pickle.loads(serialized)
```

## Best Practices

### 1. Use FFI for C Libraries

```python
# Instead of calling subprocess
load_library("mylib")
define_function("mylib", "process", "int", ["pointer", "int"])
result = call_function("mylib", "process", [data, size])
```

### 2. Type Annotations for Interop

```python
# Clear type signatures
def process_data(data: bytes, size: int) -> int:
    return call_c_function(data, size)
```

### 3. Handle Platform Differences

```python
import sys

def get_library_name(base_name: str) -> str:
    if sys.platform == "win32":
        return f"{base_name}.dll"
    elif sys.platform == "darwin":
        return f"lib{base_name}.dylib"
    else:
        return f"lib{base_name}.so"
```

## Next Steps

- [FFI Guide](ffi.md) - Complete FFI documentation
- [C Backend](../compilation/c-backend.md) - Compilation details
- [Cross-Platform Development](#) - Platform-specific code
