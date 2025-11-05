# FFI Integration with C Compilation

When compiling Tauraro code to C, the FFI (Foreign Function Interface) system provides seamless integration with native libraries.

## Overview

The FFI system works with both:
- **Interpreted mode** - Dynamic library loading at runtime
- **Compiled mode** - Static linking with native libraries

## FFI in Compiled Code

When you compile Tauraro code that uses FFI, the compiler:

1. **Generates FFI wrapper code** in C
2. **Links against required libraries** automatically
3. **Optimizes FFI calls** to reduce overhead
4. **Handles type conversions** at compile time

### Example: Compiling FFI Code

**Tauraro code (math_ffi.py):**
```python
# Load math library
load_library("m")

# Define sqrt function
define_function("m", "sqrt", "double", ["double"])

# Use it
def calculate():
    return call_function("m", "sqrt", [144.0])

print(calculate())
```

**Compile to C:**
```bash
tauraro compile math_ffi.py -o math_ffi
```

**Generated C code includes:**
```c
#include <math.h>
#include <dlfcn.h>

// FFI function wrapper
double tauraro_ffi_sqrt(double x) {
    return sqrt(x);  // Direct call, no overhead!
}

// Your function
double calculate() {
    return tauraro_ffi_sqrt(144.0);
}
```

The compiler optimizes FFI calls to direct C function calls when possible.

## Static vs Dynamic Linking

### Dynamic Linking (Default)

```bash
# Compile with dynamic linking
tauraro compile ffi_app.py -o app

# Requires library at runtime
./app  # Loads libm.so dynamically
```

### Static Linking

```bash
# Compile with static linking
tauraro compile ffi_app.py -o app --static

# Embeds library code
./app  # No external dependencies
```

## Linking Options

### Specify Libraries

```bash
# Link against specific libraries
tauraro compile app.py -o app -l m -l pthread

# Equivalent to: gcc ... -lm -lpthread
```

### Library Search Paths

```bash
# Add library search paths
tauraro compile app.py -o app -L/usr/local/lib -L/opt/libs
```

### Include Paths

```bash
# Add header search paths (for FFI structures)
tauraro compile app.py -o app -I/usr/local/include
```

## FFI with Type Annotations

Type-annotated FFI code compiles to highly optimized C:

```python
# Without type annotations - uses boxed values
def sqrt_untyped(x):
    return call_function("m", "sqrt", [x])

# With type annotations - compiles to primitives
def sqrt_typed(x: float) -> float:
    return call_function("m", "sqrt", [x])
```

**Generated C:**
```c
// Untyped - slower
tauraro_value_t* sqrt_untyped(tauraro_value_t* x) {
    // Boxing/unboxing overhead
}

// Typed - faster
double sqrt_typed(double x) {
    return sqrt(x);  // Direct call!
}
```

**Performance difference: 5-10x faster** with type annotations.

## Optimization Levels

### -O0: No Optimization

```bash
tauraro compile app.py -o app -O0
```
- FFI calls via function pointers
- Dynamic library loading
- Maximum flexibility

### -O1: Basic Optimization

```bash
tauraro compile app.py -o app -O1
```
- FFI calls inlined where possible
- Type-based optimizations
- Faster than -O0

### -O2: Aggressive Optimization (Default)

```bash
tauraro compile app.py -o app -O2
```
- Direct C function calls
- Zero FFI overhead
- Type checking at compile time

### -O3: Maximum Optimization

```bash
tauraro compile app.py -o app -O3
```
- All O2 optimizations
- LTO (Link Time Optimization)
- Platform-specific optimizations

## Cross-Compilation with FFI

### Targeting Windows from Linux

```bash
# Cross-compile for Windows
tauraro compile app.py -o app.exe --target x86_64-pc-windows-gnu

# Specify Windows libraries
tauraro compile app.py -o app.exe --target x86_64-pc-windows-gnu \
    -l kernel32 -l msvcrt
```

### Targeting Linux from macOS

```bash
# Cross-compile for Linux
tauraro compile app.py -o app --target x86_64-unknown-linux-gnu

# Specify Linux libraries
tauraro compile app.py -o app --target x86_64-unknown-linux-gnu \
    -l m -l pthread -l dl
```

## Platform-Specific Compilation

### Windows

```bash
# Compile with Windows API
tauraro compile win_app.py -o app.exe -l kernel32 -l user32
```

Libraries available:
- `kernel32` - Core Windows API
- `user32` - Windows UI
- `gdi32` - Graphics
- `msvcrt` - C runtime

### Linux

```bash
# Compile with Linux libraries
tauraro compile linux_app.py -o app -l m -l pthread -l dl
```

Libraries available:
- `m` - Math functions
- `pthread` - Threading
- `dl` - Dynamic loading
- `rt` - Real-time extensions
- `c` - C standard library

### macOS

```bash
# Compile with macOS frameworks
tauraro compile macos_app.py -o app -framework System

# Link multiple frameworks
tauraro compile gui_app.py -o app \
    -framework Cocoa \
    -framework Foundation
```

## Debugging FFI in Compiled Code

### Enable Debug Symbols

```bash
# Compile with debug info
tauraro compile app.py -o app -g

# Debug with gdb (Linux)
gdb ./app

# Debug with lldb (macOS)
lldb ./app
```

### FFI Tracing

```bash
# Enable FFI call tracing
tauraro compile app.py -o app --trace-ffi

# Shows all FFI calls at runtime
./app
# Output:
# FFI: Loading library 'm'
# FFI: Defining function 'sqrt'
# FFI: Calling sqrt(144.0) -> 12.0
```

### Verify Linked Libraries

```bash
# Linux
ldd ./app

# macOS
otool -L ./app

# Windows
dumpbin /DEPENDENTS app.exe
```

## Best Practices for Compilation

1. **Use Type Annotations**
   ```python
   def compute(x: float) -> float:
       return call_function("m", "sqrt", [x])
   ```

2. **Batch FFI Operations**
   ```python
   # Bad - multiple FFI calls
   for i in range(1000):
       result = call_function("lib", "func", [i])

   # Good - single FFI call with array
   buffer = allocate_buffer(1000 * 8)
   call_function("lib", "process_array", [buffer, 1000])
   ```

3. **Static Linking for Distribution**
   ```bash
   tauraro compile app.py -o app --static --release
   ```

4. **Profile FFI Performance**
   ```bash
   # Profile with perf (Linux)
   tauraro compile app.py -o app -O2
   perf record ./app
   perf report
   ```

5. **Test on Target Platform**
   - FFI behavior varies by platform
   - Always test compiled binaries on target OS
   - Verify library dependencies

## Performance Comparison

| Mode | Speed | Startup | Binary Size |
|------|-------|---------|-------------|
| Interpreter + FFI | 1x | Fast | Small |
| Compiled -O0 + FFI | 5x | Fast | Medium |
| Compiled -O2 + FFI | 10x | Fast | Medium |
| Compiled -O2 + Static | 10x | Instant | Large |

## Complete Example

**File: graphics_app.py**
```python
"""Graphics application using native library."""

# Load graphics library
import sys
if sys.platform == "win32":
    load_library("gdi32")
    lib = "gdi32"
else:
    load_library("cairo")
    lib = "cairo"

# Define functions with type annotations
def init_graphics() -> None:
    define_function(lib, "init", "void", [])
    call_function(lib, "init", [])

def draw_circle(x: float, y: float, radius: float) -> None:
    define_function(lib, "draw_circle", "void",
                   ["double", "double", "double"])
    call_function(lib, "draw_circle", [x, y, radius])

def main() -> None:
    init_graphics()
    draw_circle(100.0, 100.0, 50.0)

main()
```

**Compile:**
```bash
# Windows
tauraro compile graphics_app.py -o graphics.exe -l gdi32 -O2

# Linux
tauraro compile graphics_app.py -o graphics -l cairo -O2

# Run
./graphics
```

## Next Steps

- **[Complete FFI Guide](../advanced/ffi.md)** - Comprehensive FFI documentation
- **[Performance Optimization](../advanced/performance.md)** - Optimize FFI calls
- **[C Backend Details](c-backend.md)** - C transpiler documentation
- **[Build System](build-system.md)** - Advanced build configuration

## See Also

- [FFI Type System](../advanced/ffi.md#type-system)
- [Platform-Specific APIs](../advanced/ffi.md#platform-specific-examples)
- [FFI Safety](../advanced/ffi.md#safety-and-best-practices)
- [Troubleshooting FFI](../advanced/ffi.md#troubleshooting)
