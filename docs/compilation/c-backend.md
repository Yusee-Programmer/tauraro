# C Backend and Compilation

Tauraro can compile your Python code to native C executables for maximum performance.

## Overview

The C backend transpiles Tauraro/Python code to C, then compiles it to a native binary. This provides:

- **Native performance** - 50-100x faster than interpreted
- **No runtime dependency** - Standalone executables
- **Ahead-of-time compilation** - Fast startup
- **Type-based optimizations** - Even faster with type annotations

## Basic Compilation

### Compile a Script

```bash
tauraro compile script.py -o output
```

This produces a native executable:

```bash
./output
# Runs at native C speed!
```

### Compilation Options

```bash
# Specify output name
tauraro compile script.py -o my_program

# Optimize for speed
tauraro compile script.py -o program --opt-level 3

# Include debug symbols
tauraro compile script.py -o program --debug

# Verbose output
tauraro compile script.py -o program --verbose
```

## How It Works

The compilation process has several stages:

### 1. Parse

```
script.py → AST (Abstract Syntax Tree)
```

Tauraro parses your Python code into an AST.

### 2. Generate IR

```
AST → IR (Intermediate Representation)
```

The AST is converted to a platform-independent IR.

### 3. Transpile to C

```
IR → C Code
```

The IR is transpiled to optimized C code.

### 4. Compile C

```
C Code → Native Binary
```

The C compiler (GCC/Clang) produces the final executable.

## Type-Based Optimizations

Type annotations enable significant optimizations:

### Without Types (Generic/Boxed)

```python
def add(a, b):
    return a + b
```

Generated C (simplified):
```c
tauraro_value_t* add(int argc, tauraro_value_t** argv) {
    tauraro_value_t* a = argv[0];  // Boxed value
    tauraro_value_t* b = argv[1];  // Boxed value
    return tauraro_add(a, b);       // Generic add
}
```

### With Types (Optimized/Primitive)

```python
def add(a: int, b: int) -> int:
    return a + b
```

Generated C (simplified):
```c
tauraro_value_t* add(int argc, tauraro_value_t** argv) {
    int64_t a = argv[0]->data.int_val;  // Optimized: typed int
    int64_t b = argv[1]->data.int_val;  // Optimized: typed int
    int64_t result = a + b;              // Optimized: direct arithmetic
    // ... box result for return
}
```

**Performance**: Direct primitive operations are 5-10x faster!

## Optimization Levels

### Level 0 (Debug)

```bash
tauraro compile script.py --opt-level 0
```

- No optimizations
- Fast compilation
- Easy debugging
- Large binary

### Level 1 (Basic)

```bash
tauraro compile script.py --opt-level 1
```

- Basic optimizations
- Reasonable speed
- Still debuggable

### Level 2 (Standard - Default)

```bash
tauraro compile script.py --opt-level 2
```

- Good performance
- Balanced compile time
- **Recommended for most use cases**

### Level 3 (Maximum)

```bash
tauraro compile script.py --opt-level 3
```

- Maximum performance
- Aggressive inlining
- Longer compile time
- **Recommended for production**

## What Gets Optimized

### Arithmetic Operations

```python
# With types - uses native int64_t operations
x: int = 10
y: int = 20
z: int = x + y  # Compiled to: z = x + y (native)

# Without types - uses boxed tauraro_value_t
a = 10
b = 20
c = a + b  # Compiled to: tauraro_add(a, b) (boxed)
```

### Variable Storage

```python
# Typed variable - stack-allocated primitive
count: int = 0  # Compiled to: int64_t count = 0;

# Untyped variable - heap-allocated boxed value
counter = 0  # Compiled to: tauraro_value_t* counter = ...;
```

### Function Calls

```python
# Typed function - optimized parameter passing
def calculate(x: int, y: int) -> int:
    return x * 2 + y

# Untyped function - generic parameter passing
def process(data):
    return data * 2
```

### Loops

```python
# Optimized loop with typed variable
for i in range(1000):
    count: int = i * 2  # Fast iteration

# Generic loop without types
for item in items:
    process(item)
```

## Performance Comparison

### Interpreted (VM)

```bash
time tauraro run script.py
# ~1.0 seconds
```

### Compiled without Types

```bash
tauraro compile script.py -o program
time ./program
# ~0.1 seconds (10x faster)
```

### Compiled with Types

```bash
# script.py uses type annotations
tauraro compile typed_script.py -o program
time ./program
# ~0.01 seconds (100x faster)
```

## Generated C Code

### Viewing Generated Code

```bash
# Generate C code without compiling
tauraro compile script.py --emit-c > output.c

# Compile with verbose output
tauraro compile script.py -o program --verbose
```

### Code Structure

The generated C code includes:

```c
// 1. Runtime type definitions
typedef enum {
    TAURARO_INT,
    TAURARO_FLOAT,
    TAURARO_STRING,
    // ...
} tauraro_type_t;

typedef struct tauraro_value {
    tauraro_type_t type;
    int ref_count;
    union {
        int64_t int_val;
        double float_val;
        char* str_val;
        // ...
    } data;
} tauraro_value_t;

// 2. Built-in function declarations
tauraro_value_t* tauraro_print(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_len(int argc, tauraro_value_t** argv);
// ...

// 3. Your compiled functions
tauraro_value_t* my_function(int argc, tauraro_value_t** argv) {
    // Optimized code here
}

// 4. Main function
int main(int argc, char** argv) {
    // Initialize runtime
    // Execute program
    // Cleanup
}
```

## Linking with C Libraries

### Using C Functions

```python
# Import FFI functions
from _tauraro_ffi import call_c_function

# Call C library functions
result = call_c_function("printf", "Hello from C!")
```

### Custom C Extensions

```c
// extension.c
#include "tauraro_runtime.h"

tauraro_value_t* my_fast_function(int argc, tauraro_value_t** argv) {
    // Implement in C for maximum speed
    int64_t value = argv[0]->data.int_val;
    // ... fast C code ...
    return tauraro_make_int(result);
}
```

Compile and link:

```bash
gcc -c extension.c -o extension.o
tauraro compile main.py -o program --link extension.o
```

## Cross-Compilation

### For Different Platforms

```bash
# Compile for Linux
tauraro compile script.py -o program-linux --target linux

# Compile for Windows
tauraro compile script.py -o program.exe --target windows

# Compile for macOS
tauraro compile script.py -o program-macos --target darwin
```

## Binary Size Optimization

### Strip Debug Symbols

```bash
tauraro compile script.py -o program --strip
# or
strip program
```

### Link Time Optimization (LTO)

```bash
tauraro compile script.py -o program --lto
```

### Static Linking

```bash
tauraro compile script.py -o program --static
# Produces standalone binary with no dependencies
```

## Deployment

### Single Binary Deployment

```bash
# Compile with all dependencies
tauraro compile app.py -o app --static

# Deploy just one file
scp app user@server:/usr/local/bin/
```

### No Runtime Required

Unlike Python, compiled Tauraro programs don't need:
- Python interpreter
- Virtual environments
- Dependency packages
- Runtime libraries (if statically linked)

Just copy the binary and run!

## Limitations

### Dynamic Features

Some Python features are harder to compile:

```python
# eval/exec - requires interpreter
eval("2 + 2")  # Works in VM, not in compiled code

# Dynamic imports
module_name = "math"
import module_name  # Works in VM, requires special handling

# Monkey-patching
MyClass.new_method = lambda: "hi"  # Harder to optimize
```

### Workarounds

Use static alternatives when possible:

```python
# Instead of eval
result = 2 + 2  # Direct computation

# Instead of dynamic imports
import math  # Static import

# Instead of monkey-patching
class ExtendedClass(MyClass):  # Inheritance
    def new_method(self):
        return "hi"
```

## Best Practices

### 1. Use Type Annotations

```python
# Good - optimizes well
def calculate(x: int, y: int) -> int:
    return x * y

# Less optimal - generic operations
def calculate(x, y):
    return x * y
```

### 2. Annotate Hot Paths

```python
def main():
    # Hot loop - annotate for speed
    total: int = 0
    for i in range(1000000):
        total += calculate_value(i)  # Type this!

    # Cold code - dynamic is fine
    print(f"Total: {total}")
```

### 3. Profile Before Optimizing

```bash
# Profile VM execution
tauraro run --profile script.py

# Compile with profiling
tauraro compile script.py -o program --profile
./program
```

### 4. Benchmark Different Approaches

```python
import time

def benchmark(func, *args):
    start = time.time()
    result = func(*args)
    end = time.time()
    print(f"{func.__name__}: {end - start:.4f}s")
    return result
```

## Troubleshooting

### Compilation Errors

```bash
# Check C compiler
gcc --version
clang --version

# Verbose compilation
tauraro compile script.py --verbose

# Keep intermediate files
tauraro compile script.py --keep-c
```

### Runtime Errors

```bash
# Compile with debug symbols
tauraro compile script.py -o program --debug

# Run with debugger
gdb ./program
```

### Performance Issues

```bash
# Check optimization level
tauraro compile script.py -o program --opt-level 3

# Profile execution
perf record ./program
perf report
```

## Next Steps

- [Optimization Guide](optimizations.md)
- [FFI Integration](ffi.md)
- [Performance Tuning](../advanced/performance.md)
- [Type System](../types/hybrid-typing.md)
