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

## Recent Improvements

### Enhanced Type Inference (Latest Update)

The C code generator now features improved type inference for variables with mixed-type usage:

**Problem Before:**
Variables used for different types (e.g., string literals and function results) were incorrectly optimized to a single native type, causing compilation errors:

```c
// Incorrect - variable used for both string and dynamic values
char* arg_0 = strdup("Hello");  // String literal
// ...later...
arg_0 = some_function();  // ERROR: function returns tauraro_value_t*
```

**Solution Now:**
Variables with mixed-type usage are correctly identified and declared as `tauraro_value_t*`:

```c
// Correct - handles both string and dynamic values
tauraro_value_t* arg_0 = tauraro_make_string("Hello");  // String literal
// ...later...
arg_0 = some_function();  // OK: both are tauraro_value_t*
```

**What This Means:**
- More reliable C code generation
- Fewer compilation errors with complex code
- Better handling of polymorphic variables
- Improved type tracking through IR instructions

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

## Bare-Metal Compilation (NEW!)

Tauraro supports compiling for bare-metal targets like OS kernels, bootloaders, and embedded systems.

### Freestanding Mode

```bash
# Basic freestanding compilation
tauraro compile kernel.tr -o kernel.c --backend c --freestanding

# Full bare-metal setup
tauraro compile kernel.tr -o kernel.c --backend c \
    --freestanding \
    --no-stdlib \
    --entry-point kernel_main \
    --target-arch x86_64 \
    --inline-asm
```

### Bare-Metal Flags

| Flag | Description |
|------|-------------|
| `--freestanding` | No C standard library, real hardware access |
| `--no-stdlib` | Don't link standard library |
| `--entry-point <name>` | Custom entry point (default: `main`) |
| `--target-arch <arch>` | Target architecture: `x86`, `x86_64`, `arm`, `aarch64`, `riscv32`, `riscv64` |
| `--inline-asm` | Enable inline assembly support |

### Standard vs Freestanding Mode

| Feature | Standard Mode | Freestanding Mode |
|---------|---------------|-------------------|
| C stdlib | ✅ Available | ❌ Not used |
| Hardware I/O | Stub (returns 0) | Real hardware access |
| Interrupts | No-op | Real CLI/STI |
| Entry point | `main()` | Custom (e.g., `kernel_main`) |
| Target | User-space apps | OS kernels, drivers |

### Generated Hardware Access Code

**Standard Mode (Safe Testing):**
```c
// Stub - safe for testing on regular OS
static inline uint8_t mmio_read8(uintptr_t addr) {
    (void)addr;
    return 0;
}
```

**Freestanding Mode (Real Hardware):**
```c
// Real hardware access
static inline uint8_t mmio_read8(uintptr_t addr) {
    return *(volatile uint8_t*)addr;
}
```

### Example: Compiling an OS Kernel

```bash
# 1. Generate freestanding C code
tauraro compile kernel.tr -o kernel.c --backend c \
    --freestanding --entry-point kernel_main

# 2. Cross-compile for target
x86_64-elf-gcc -ffreestanding -nostdlib -c kernel.c -o kernel.o

# 3. Link with bootloader
x86_64-elf-ld -T linker.ld -o kernel.elf boot.o kernel.o
```

See [Bare-Metal Development Guide](../advanced/baremetal.md) for complete details.

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

## Native Transpiler with Type Annotations (NEW in v0.2.0)

The native C transpiler generates highly optimized C code that uses native types instead of dynamic runtime types when type annotations are present.

### Using the Native Transpiler

```bash
# Use native transpiler (recommended for type-annotated code)
tauraro compile --use-native-transpiler -b c script.tr

# Compile the generated C code
gcc script.c -o script.exe -lm

# Run
./script.exe
```

### Performance Comparison

| Code Type | Generated C | Performance |
|-----------|-------------|-------------|
| Dynamic (no types) | `tauraro_value_t*` everywhere | Baseline |
| Type-annotated | Native C types (`int64_t`, `double`) | 10-1000x faster |
| Type-annotated + Native Transpiler | Optimized native C | Maximum performance |

### Features

#### 1. Native Type Mapping

**Tauraro Code:**
```python
x: int = 10
y: float = 3.14
name: str = "Alice"
flag: bool = True
```

**Generated C Code (Native Transpiler):**
```c
int64_t x = 10;
double y = 3.14;
char* name = strdup("Alice");
bool flag = true;
```

#### 2. Native Operators

**Tauraro Code:**
```python
def calculate(a: int, b: int) -> int:
    return a * a + b * b
```

**Generated C Code:**
```c
int64_t calculate(int64_t a, int64_t b) {
    return ((a * a) + (b * b));  // Native C arithmetic!
}
```

No runtime function calls like `tauraro_add()` or `tauraro_mul()`!

#### 3. Class to Struct Compilation

**Tauraro Code:**
```python
class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def distance_squared(self) -> int:
        return self.x * self.x + self.y * self.y

p: Point = Point(3, 4)
dist: int = p.distance_squared()
```

**Generated C Code:**
```c
// Native C struct
struct Point {
    int ref_count;
    int64_t x;
    int64_t y;
};

// Constructor
struct Point* Point(int64_t x, int64_t y) {
    struct Point* self = malloc(sizeof(struct Point));
    self->x = x;
    self->y = y;
    return self;
}

// Method becomes C function
int64_t Point_distance_squared(struct Point* self) {
    return ((self->x * self->x) + (self->y * self->y));
}

// Usage
struct Point* p = Point(3, 4);
int64_t dist = Point_distance_squared(p);
```

#### 4. Method Calls

**Tauraro Code:**
```python
class Calculator:
    def __init__(self, value: int):
        self.value = value

    def add(self, n: int) -> int:
        return self.value + n

calc: Calculator = Calculator(10)
result: int = calc.add(5)
print(result)
```

**Generated C Code:**
```c
struct Calculator {
    int ref_count;
    int64_t value;
};

struct Calculator* Calculator(int64_t value) {
    struct Calculator* self = malloc(sizeof(struct Calculator));
    self->value = value;
    return self;
}

int64_t Calculator_add(struct Calculator* self, int64_t n) {
    return (self->value + n);
}

int main() {
    struct Calculator* calc = Calculator(10);
    int64_t result = Calculator_add(calc, 5);
    printf("%lld\n", result);  // Correct format specifier!
    return 0;
}
```

#### 5. FFI Support

The native transpiler properly handles FFI:

**Tauraro Code:**
```python
user32 = load_library("user32.dll")
MessageBoxA = define_function("user32.dll", "MessageBoxA", "int",
    ["pointer", "string", "string", "int"])

result: int = call_function(MessageBoxA, 0, "Hello!", "Title", 0)
```

**Generated C Code:**
```c
#ifdef _WIN32
    #define FFI_DLOPEN(name) LoadLibraryA(name)
    #define FFI_DLSYM(handle, name) GetProcAddress(handle, name)
#endif

ffi_lib_handle _ffi_lib_0 = FFI_DLOPEN("user32.dll");
int32_t (*_ffi_func_0)(void*, char*, char*, int32_t);
_ffi_func_0 = FFI_DLSYM(_ffi_lib_0, "MessageBoxA");

int32_t result = _ffi_func_0(0, "Hello!", "Title", 0);
```

### Type Inference

The native transpiler includes intelligent type inference:

**Constructor Calls:**
```python
p: Point = Point(3, 4)  # Infers NativeType::Struct("Point")
```

**Method Return Types:**
```python
result: int = calc.add(5)  # Infers return type from method signature
```

**Binary Operations:**
```python
x: int = 10
y: int = x * 2 + 5  # Infers int throughout expression
```

### Optimizations

The native transpiler performs several optimizations:

1. **Direct Field Access**: `obj->field` instead of hash table lookup
2. **Native Operators**: `a + b` instead of `tauraro_add(a, b)`
3. **Stack Allocation**: Where possible, avoiding heap
4. **Inline Opportunities**: C compiler can inline methods
5. **No vtable**: Direct function calls, no virtual dispatch

### Limitations

Current limitations (will be improved in future versions):

- ❌ Dynamic attributes without type annotations
- ❌ Complex inheritance hierarchies
- ❌ Magic methods (`__add__`, `__str__`)
- ❌ Decorators (`@property`, `@classmethod`)
- ❌ Metaclasses

Use standard C transpiler for these features (they work, just not with native optimizations).

### Best Practices

1. **Add Type Annotations**: Always annotate for native compilation
2. **Use Simple Classes**: Flat class hierarchies work best
3. **Profile First**: Compile hot paths with native transpiler
4. **Test Both**: Verify in VM mode before compiling
5. **Check Generated Code**: Use `--keep-c` to inspect output

### Example Workflow

```bash
# 1. Write type-annotated Tauraro code
cat > fast_math.tr << 'EOF'
class Vector:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

    def magnitude(self) -> float:
        return (self.x * self.x + self.y * self.y) ** 0.5

v: Vector = Vector(3.0, 4.0)
print(v.magnitude())
EOF

# 2. Compile with native transpiler
tauraro compile --use-native-transpiler -b c fast_math.tr

# 3. Compile C code
gcc fast_math.c -o fast_math.exe -lm -O3

# 4. Run (blazingly fast!)
./fast_math.exe
# Output: 5.0
```

The final executable has **zero** interpreter overhead and runs at native C speed!

## Next Steps

- [Optimization Guide](optimizations.md)
- [FFI Integration](ffi.md)
- [Performance Tuning](../advanced/performance.md)
- [Type System](../types/hybrid-typing.md)
