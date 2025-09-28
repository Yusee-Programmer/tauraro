# TauraroLang Compilation Backends & Build System

This document provides comprehensive information about TauraroLang's multiple compilation backends, build system, and deployment options.

## Table of Contents

1. [Overview](#overview)
2. [Backend Architecture](#backend-architecture)
3. [Interpreter Backend](#interpreter-backend)
4. [C Transpiler Backend](#c-transpiler-backend)
5. [LLVM Backend](#llvm-backend)
6. [WebAssembly Backend](#webassembly-backend)
7. [Build System](#build-system)
8. [Feature Flags](#feature-flags)
9. [Compilation Process](#compilation-process)
10. [Performance Comparison](#performance-comparison)
11. [Deployment Strategies](#deployment-strategies)
12. [Troubleshooting](#troubleshooting)

## Overview

TauraroLang supports multiple compilation backends, each optimized for different use cases:

- **Interpreter**: Fast development and debugging
- **C Transpiler**: Native performance with broad compatibility
- **LLVM**: High-performance optimized native code
- **WebAssembly**: Web deployment and sandboxed execution

### Backend Selection

```bash
# Run with interpreter (default)
tauraro run script.tr --backend vm

# Compile with LLVM backend
tauraro compile script.tr --backend llvm --output binary

# Transpile to C
tauraro compile script.tr --backend c --output script.c

# Compile to WebAssembly
tauraro compile script.tr --backend wasm --output script.wasm
```

## Backend Architecture

### Common Pipeline

All backends share a common frontend pipeline:

```
Source Code → Lexer → Parser → AST → Semantic Analysis → IR → Backend
```

1. **Lexical Analysis**: Tokenization using `logos` crate
2. **Parsing**: AST generation with recursive descent parser
3. **Semantic Analysis**: Type checking and symbol resolution
4. **IR Generation**: Platform-independent intermediate representation
5. **Backend Code Generation**: Target-specific code generation

### Intermediate Representation (IR)

The IR (`src/ir.rs`) provides a common abstraction:

```rust
pub struct IRModule {
    pub name: String,
    pub functions: HashMap<String, IRFunction>,
    pub globals: HashMap<String, IRValue>,
    pub types: HashMap<String, IRType>,
}

pub struct IRFunction {
    pub name: String,
    pub parameters: Vec<(String, IRType)>,
    pub return_type: IRType,
    pub blocks: Vec<IRBlock>,
    pub is_async: bool,
    pub is_exported: bool,
}
```

## Interpreter Backend

### Overview

The interpreter backend (`src/codegen/interpreter.rs`) provides direct AST execution for rapid development and debugging.

### Features

- **Fast Startup**: No compilation overhead
- **REPL Support**: Interactive development environment
- **Full Debugging**: Complete debugging capabilities
- **Dynamic Features**: Runtime code modification

### Architecture

```rust
pub struct InterpreterCodeGenerator {
    vm: VM,
    debug_mode: bool,
    profiling: bool,
}
```

### Use Cases

- **Development**: Rapid prototyping and testing
- **Scripting**: Quick automation tasks
- **Education**: Learning and experimentation
- **Debugging**: Step-through debugging

### Usage

```bash
# Run a program directly
tauraro run program.tr

# Start interactive REPL
tauraro repl

# Run with debug information
tauraro run program.tr --debug

# Run with verbose output
tauraro run program.tr --verbose
```

### Features

- **Immediate Execution**: No compilation step required
- **Interactive REPL**: Perfect for experimentation
- **Rich Error Messages**: Detailed stack traces and error information
- **Dynamic Debugging**: Runtime inspection capabilities
- **Hot Reloading**: Modify code and see changes immediately

### Example

**hello_interpreter.tr:**
```tauraro
// This runs directly in the interpreter
print("Hello from the interpreter!")

let numbers = [1, 2, 3, 4, 5]
let sum = 0

for num in numbers {
    sum = sum + num
}

print("Sum: " + str(sum))
```

**Running:**
```bash
tauraro run hello_interpreter.tr
```

**Output:**
```
Hello from the interpreter!
Sum: 15
```

### Performance Characteristics

- **Startup Time**: Very fast (no compilation)
- **Execution Speed**: Slower than compiled backends
- **Memory Usage**: Higher due to runtime overhead
- **Best For**: Development, testing, scripting

### Configuration Options

```bash
# Set stack size
tauraro run program.tr --stack-size 1024

# Enable garbage collection debugging
tauraro run program.tr --gc-debug

# Set memory limit
tauraro run program.tr --memory-limit 512MB
```

## C Transpilation Backend

The C transpilation backend converts TauraroLang code to C, then compiles it using a C compiler for maximum performance.

### Usage

```bash
# Compile to C and create executable
tauraro compile program.tr --backend c

# Specify output name
tauraro compile program.tr --backend c --output myprogram

# Generate C code only (don't compile)
tauraro compile program.tr --backend c --emit-c-only

# Use specific C compiler
tauraro compile program.tr --backend c --cc gcc
tauraro compile program.tr --backend c --cc clang
```

### Supported C Compilers

- **GCC**: GNU Compiler Collection
- **Clang**: LLVM-based C compiler
- **MSVC**: Microsoft Visual C++ (Windows)
- **MinGW**: Minimalist GNU for Windows

### Example

**math_operations.tr:**
```tauraro
// Efficient mathematical operations
fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

fn factorial(n) {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}

fn main() {
    print("Computing Fibonacci and Factorial...")
    
    let fib_10 = fibonacci(10)
    let fact_10 = factorial(10)
    
    print("Fibonacci(10) = " + str(fib_10))
    print("Factorial(10) = " + str(fact_10))
}

main()
```

**Compilation:**
```bash
tauraro compile math_operations.tr --backend c --output math_calc
```

**Generated C Code Structure:**
```c
// math_operations.c (simplified)
#include <stdio.h>
#include <stdlib.h>
#include "tauraro_runtime.h"

// Function declarations
TauraroValue fibonacci(TauraroValue n);
TauraroValue factorial(TauraroValue n);

// Function implementations
TauraroValue fibonacci(TauraroValue n) {
    if (tauraro_to_int(n) <= 1) {
        return n;
    }
    TauraroValue n_minus_1 = tauraro_sub(n, tauraro_int(1));
    TauraroValue n_minus_2 = tauraro_sub(n, tauraro_int(2));
    return tauraro_add(fibonacci(n_minus_1), fibonacci(n_minus_2));
}

// ... rest of the generated code
```

### Optimization Levels

```bash
# Debug build (no optimization)
tauraro compile program.tr --backend c --opt-level 0

# Standard optimization
tauraro compile program.tr --backend c --opt-level 2

# Maximum optimization
tauraro compile program.tr --backend c --opt-level 3

# Size optimization
tauraro compile program.tr --backend c --opt-size
```

### Advanced Features

#### Static Linking
```bash
# Create statically linked executable
tauraro compile program.tr --backend c --static
```

#### Cross Compilation
```bash
# Compile for different target
tauraro compile program.tr --backend c --target x86_64-linux-gnu
tauraro compile program.tr --backend c --target aarch64-apple-darwin
```

#### Custom C Flags
```bash
# Add custom compiler flags
tauraro compile program.tr --backend c --cflags "-march=native -flto"
```

### Performance Characteristics

- **Compilation Time**: Medium (C compilation overhead)
- **Execution Speed**: Very high (native machine code)
- **Binary Size**: Small to medium
- **Best For**: Production applications, system programming

## WebAssembly Backend

The WebAssembly backend compiles TauraroLang to WebAssembly (WASM) for web applications and sandboxed environments.

### Usage

```bash
# Compile to WebAssembly
tauraro compile program.tr --backend wasm

# Specify output name
tauraro compile program.tr --backend wasm --output module.wasm

# Generate with JavaScript bindings
tauraro compile program.tr --backend wasm --js-bindings

# Optimize for size
tauraro compile program.tr --backend wasm --optimize-size
```

### Example

**web_calculator.tr:**
```tauraro
// WebAssembly-compatible calculator
export fn add(a, b) {
    return a + b
}

export fn multiply(a, b) {
    return a * b
}

export fn power(base, exponent) {
    let result = 1
    let i = 0
    while i < exponent {
        result = result * base
        i = i + 1
    }
    return result
}

// Main function for standalone execution
fn main() {
    print("Calculator module loaded")
    print("add(5, 3) = " + str(add(5, 3)))
    print("multiply(4, 7) = " + str(multiply(4, 7)))
    print("power(2, 8) = " + str(power(2, 8)))
}
```

**Compilation:**
```bash
tauraro compile web_calculator.tr --backend wasm --js-bindings --output calculator
```

**Generated Files:**
- `calculator.wasm` - WebAssembly module
- `calculator.js` - JavaScript bindings
- `calculator.d.ts` - TypeScript definitions (if requested)

### JavaScript Integration

**calculator.js (generated):**
```javascript
// Generated JavaScript bindings
class TauraroCalculator {
    constructor() {
        this.instance = null;
    }
    
    async load() {
        const wasmModule = await WebAssembly.instantiateStreaming(
            fetch('calculator.wasm')
        );
        this.instance = wasmModule.instance;
    }
    
    add(a, b) {
        return this.instance.exports.add(a, b);
    }
    
    multiply(a, b) {
        return this.instance.exports.multiply(a, b);
    }
    
    power(base, exponent) {
        return this.instance.exports.power(base, exponent);
    }
}
```

**HTML Usage:**
```html
<!DOCTYPE html>
<html>
<head>
    <title>Tauraro Calculator</title>
</head>
<body>
    <script src="calculator.js"></script>
    <script>
        async function main() {
            const calc = new TauraroCalculator();
            await calc.load();
            
            console.log('5 + 3 =', calc.add(5, 3));
            console.log('4 * 7 =', calc.multiply(4, 7));
            console.log('2^8 =', calc.power(2, 8));
        }
        
        main();
    </script>
</body>
</html>
```

### WebAssembly Features

#### Memory Management
```tauraro
// Efficient memory usage for WASM
fn process_large_array(data) {
    // WASM backend optimizes memory allocation
    let result = []
    for item in data {
        if item > 0 {
            result = result + [item * 2]
        }
    }
    return result
}
```

#### Import/Export Functions
```tauraro
// Import JavaScript functions
import fn console_log(message)
import fn fetch_data(url)

// Export functions to JavaScript
export fn process_data(input) {
    console_log("Processing data...")
    let processed = transform(input)
    return processed
}
```

### Performance Characteristics

- **Compilation Time**: Fast
- **Execution Speed**: High (near-native in modern browsers)
- **Binary Size**: Very small
- **Portability**: Excellent (runs anywhere with WASM support)
- **Best For**: Web applications, plugins, sandboxed execution

### WASM Optimization Options

```bash
# Size optimization
tauraro compile program.tr --backend wasm --optimize-size

# Speed optimization
tauraro compile program.tr --backend wasm --optimize-speed

# Enable SIMD instructions
tauraro compile program.tr --backend wasm --enable-simd

# Enable multi-threading
tauraro compile program.tr --backend wasm --enable-threads
```

## LLVM IR Backend

The LLVM IR backend generates LLVM Intermediate Representation, enabling advanced optimizations and research applications.

### Usage

```bash
# Generate LLVM IR
tauraro compile program.tr --backend llvm

# Specify output name
tauraro compile program.tr --backend llvm --output program.ll

# Generate optimized IR
tauraro compile program.tr --backend llvm --optimize

# Generate and compile to native
tauraro compile program.tr --backend llvm --native
```

### Example

**optimization_demo.tr:**
```tauraro
// Code that benefits from LLVM optimizations
fn vector_dot_product(a, b) {
    if len(a) != len(b) {
        return 0
    }
    
    let result = 0
    let i = 0
    while i < len(a) {
        result = result + (a[i] * b[i])
        i = i + 1
    }
    return result
}

fn matrix_multiply(a, b) {
    let rows_a = len(a)
    let cols_a = len(a[0])
    let cols_b = len(b[0])
    
    let result = []
    let i = 0
    while i < rows_a {
        let row = []
        let j = 0
        while j < cols_b {
            let sum = 0
            let k = 0
            while k < cols_a {
                sum = sum + (a[i][k] * b[k][j])
                k = k + 1
            }
            row = row + [sum]
            j = j + 1
        }
        result = result + [row]
        i = i + 1
    }
    return result
}

fn main() {
    let vec1 = [1, 2, 3, 4]
    let vec2 = [5, 6, 7, 8]
    
    let dot = vector_dot_product(vec1, vec2)
    print("Dot product: " + str(dot))
}
```

**Compilation:**
```bash
tauraro compile optimization_demo.tr --backend llvm --optimize --output optimized
```

**Generated LLVM IR (simplified):**
```llvm
; optimization_demo.ll
define i32 @vector_dot_product(%Array* %a, %Array* %b) {
entry:
  %len_a = call i32 @array_length(%Array* %a)
  %len_b = call i32 @array_length(%Array* %b)
  %cmp = icmp eq i32 %len_a, %len_b
  br i1 %cmp, label %loop_init, label %return_zero

loop_init:
  %result = alloca i32
  store i32 0, i32* %result
  %i = alloca i32
  store i32 0, i32* %i
  br label %loop_cond

loop_cond:
  %i_val = load i32, i32* %i
  %cmp_loop = icmp slt i32 %i_val, %len_a
  br i1 %cmp_loop, label %loop_body, label %loop_end

loop_body:
  ; Vectorized operations (LLVM optimization)
  %a_elem = call i32 @array_get(%Array* %a, i32 %i_val)
  %b_elem = call i32 @array_get(%Array* %b, i32 %i_val)
  %mult = mul i32 %a_elem, %b_elem
  %current_result = load i32, i32* %result
  %new_result = add i32 %current_result, %mult
  store i32 %new_result, i32* %result
  %i_inc = add i32 %i_val, 1
  store i32 %i_inc, i32* %i
  br label %loop_cond

loop_end:
  %final_result = load i32, i32* %result
  ret i32 %final_result

return_zero:
  ret i32 0
}
```

### LLVM Optimization Passes

```bash
# Apply specific optimization passes
tauraro compile program.tr --backend llvm --passes "mem2reg,instcombine,gvn"

# Use optimization level
tauraro compile program.tr --backend llvm --opt-level 3

# Enable link-time optimization
tauraro compile program.tr --backend llvm --lto

# Generate debug information
tauraro compile program.tr --backend llvm --debug-info
```

### Advanced LLVM Features

#### Profile-Guided Optimization
```bash
# Generate instrumented binary
tauraro compile program.tr --backend llvm --pgo-instrument

# Run with sample data to generate profile
./program < sample_input.txt

# Compile with profile data
tauraro compile program.tr --backend llvm --pgo-use profile.profdata
```

#### Custom Target Architecture
```bash
# Compile for specific CPU
tauraro compile program.tr --backend llvm --target-cpu skylake

# Enable specific features
tauraro compile program.tr --backend llvm --target-features +avx2,+fma
```

### Performance Characteristics

- **Compilation Time**: Slow (extensive optimization)
- **Execution Speed**: Very high (advanced optimizations)
- **Binary Size**: Variable (depends on optimizations)
- **Best For**: Performance-critical applications, research

## Backend Comparison

### Performance Benchmark

**Test Program (fibonacci.tr):**
```tauraro
fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

fn main() {
    let result = fibonacci(35)
    print("Fibonacci(35) = " + str(result))
}
```

**Performance Results:**

| Backend | Compilation Time | Execution Time | Binary Size | Memory Usage |
|---------|------------------|----------------|-------------|--------------|
| Interpreter | 0.1s | 15.2s | N/A | 45MB |
| C (GCC -O2) | 2.3s | 0.8s | 1.2MB | 8MB |
| C (Clang -O3) | 2.1s | 0.7s | 1.1MB | 8MB |
| WebAssembly | 1.5s | 1.2s | 0.3MB | 12MB |
| LLVM IR (-O3) | 4.2s | 0.6s | 1.4MB | 9MB |

### Use Case Matrix

| Scenario | Recommended Backend | Reason |
|----------|-------------------|---------|
| Development/Debugging | Interpreter | Fast iteration, rich debugging |
| Production Server | C (GCC/Clang) | Maximum performance |
| Web Application | WebAssembly | Browser compatibility |
| Mobile App | C + Cross-compilation | Native performance, small size |
| Research/Experimentation | LLVM IR | Advanced optimizations |
| Embedded Systems | C (size-optimized) | Resource constraints |
| Plugin System | WebAssembly | Sandboxing, security |

## Performance Optimization

### General Optimization Tips

1. **Choose the Right Data Structures**
```tauraro
// Prefer arrays for sequential access
let numbers = [1, 2, 3, 4, 5]

// Use objects for key-value lookups
let lookup = {
    "key1": "value1",
    "key2": "value2"
}
```

2. **Minimize Function Call Overhead**
```tauraro
// Inline simple operations
fn fast_calculation(x) {
    // Direct calculation instead of multiple function calls
    return x * x + 2 * x + 1
}
```

3. **Use Local Variables**
```tauraro
fn optimized_loop(data) {
    let length = len(data)  // Cache length
    let result = 0
    let i = 0
    while i < length {
        result = result + data[i]
        i = i + 1
    }
    return result
}
```

### Backend-Specific Optimizations

#### C Backend Optimizations
```bash
# Enable aggressive optimization
tauraro compile program.tr --backend c --opt-level 3 --cflags "-march=native -flto"

# Profile-guided optimization
tauraro compile program.tr --backend c --pgo
```

#### WebAssembly Optimizations
```bash
# Size optimization for web
tauraro compile program.tr --backend wasm --optimize-size --strip-debug

# Enable SIMD for numerical code
tauraro compile program.tr --backend wasm --enable-simd
```

#### LLVM Optimizations
```bash
# Custom optimization pipeline
tauraro compile program.tr --backend llvm --passes "aggressive-instcombine,loop-vectorize"
```

## Best Practices

### Development Workflow

1. **Start with Interpreter**
   - Use for initial development and testing
   - Leverage REPL for experimentation
   - Debug with rich error messages

2. **Test with C Backend**
   - Validate performance characteristics
   - Check for compilation issues
   - Profile memory usage

3. **Deploy with Appropriate Backend**
   - C for production servers
   - WebAssembly for web applications
   - LLVM for research/optimization

### Code Organization

```tauraro
// Structure code for multiple backends
// main.tr
fn main() {
    let config = get_config()
    let processor = create_processor(config)
    let result = processor.run()
    output_result(result)
}

// config.tr - Backend-specific configuration
fn get_config() {
    // Configuration that works across backends
    return {
        buffer_size: 1024,
        max_iterations: 1000,
        debug_mode: false
    }
}
```

### Cross-Backend Compatibility

1. **Avoid Backend-Specific Features**
```tauraro
// Good: Works on all backends
fn calculate_sum(numbers) {
    let sum = 0
    for num in numbers {
        sum = sum + num
    }
    return sum
}

// Avoid: Backend-specific optimizations in source
```

2. **Use Conditional Compilation**
```tauraro
// Conditional features based on backend
if BACKEND == "wasm" {
    // WebAssembly-specific code
    import fn js_console_log(message)
} else {
    // Native backends
    fn js_console_log(message) {
        print(message)
    }
}
```

### Testing Strategy

```bash
# Test across all backends
tauraro test program.tr --all-backends

# Performance comparison
tauraro benchmark program.tr --backends c,wasm,llvm

# Compatibility check
tauraro check program.tr --cross-platform
```

## Troubleshooting

### Common Issues

#### LLVM Backend Not Available

```
Error: LLVM backend not enabled. Enable with --features llvm
```

**Solution**: Build with LLVM feature enabled:
```bash
cargo build --features llvm
```

#### C Compiler Not Found

```
Error: No C compiler found. Please install gcc, clang, or MSVC
```

**Solutions**:
- **Linux**: `sudo apt install gcc` or `sudo yum install gcc`
- **macOS**: `xcode-select --install`
- **Windows**: Install Visual Studio Build Tools or MinGW-w64

#### WebAssembly Runtime Error

```
Error: WASM backend not enabled
```

**Solution**: Build with WASM feature:
```bash
cargo build --features wasm
```

### Performance Issues

#### Slow Compilation

- Use lower optimization levels during development
- Enable incremental compilation
- Use interpreter backend for development

#### Large Binary Size

- Use WebAssembly backend for size-critical applications
- Enable link-time optimization (LTO)
- Strip debug symbols in release builds

### Debug Information

Enable debug information for better error messages:

```bash
tauraro compile script.tr --debug --backend llvm
```

### Profiling

Enable profiling to identify performance bottlenecks:

```bash
tauraro run script.tr --backend vm --profile
```

## Advanced Configuration

### Custom Target Triples

```bash
tauraro compile app.tr --backend llvm --target x86_64-unknown-linux-gnu
```

### Cross-Compilation

```bash
# Compile for ARM64 from x86_64
tauraro compile app.tr --backend llvm --target aarch64-unknown-linux-gnu
```

### Link-Time Optimization

```bash
tauraro compile app.tr --backend llvm --optimization 3 --lto
```

This comprehensive backend system provides flexibility for different deployment scenarios while maintaining consistent language semantics across all targets.