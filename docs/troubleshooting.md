# Troubleshooting Guide

This guide helps you diagnose and resolve common issues when developing with TauraroLang. It covers compilation errors, runtime issues, performance problems, and debugging techniques.

## Table of Contents

1. [Installation Issues](#installation-issues)
2. [Compilation Errors](#compilation-errors)
3. [Runtime Errors](#runtime-errors)
4. [Performance Issues](#performance-issues)
5. [Memory Problems](#memory-problems)
6. [FFI Issues](#ffi-issues)
7. [Debugging Techniques](#debugging-techniques)
8. [Common Gotchas](#common-gotchas)
9. [Error Reference](#error-reference)
10. [Getting Help](#getting-help)

## Installation Issues

### Rust Toolchain Problems

**Issue:** `rustc` not found or wrong version
```bash
error: rustc version 1.70.0 or higher required, found 1.69.0
```

**Solution:**
```bash
# Update Rust toolchain
rustup update

# Check version
rustc --version

# If still having issues, reinstall rustup
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

### Build Dependencies Missing

**Issue:** Missing system dependencies during build
```bash
error: failed to run custom build command for `tauraro`
note: ld: library not found for -lssl
```

**Solutions:**

**On macOS:**
```bash
# Install required libraries
brew install openssl pkg-config

# Set environment variables
export PKG_CONFIG_PATH="/opt/homebrew/lib/pkgconfig"
export OPENSSL_DIR="/opt/homebrew/opt/openssl"
```

**On Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential pkg-config libssl-dev
```

**On Windows:**
```powershell
# Install Visual Studio Build Tools
# Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/

# Or use vcpkg for dependencies
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### Path Issues

**Issue:** `tauraro` command not found after installation
```bash
tauraro: command not found
```

**Solution:**
```bash
# Add Cargo bin to PATH
echo 'export PATH="$HOME/.cargo/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc

# Or install globally
cargo install --path . --force
```

## Compilation Errors

### Syntax Errors

**Issue:** Unexpected token errors
```
Error: Unexpected token 'fn' at line 5, column 1
Expected: expression, found function declaration
```

**Common Causes & Solutions:**

1. **Missing semicolons:**
```tauraro
// Wrong
let x = 5
let y = 10

// Correct
let x = 5;
let y = 10;
```

2. **Incorrect function syntax:**
```tauraro
// Wrong
function add(a, b) {
    return a + b
}

// Correct
fn add(a: int, b: int) -> int {
    return a + b;
}
```

3. **Mismatched braces:**
```tauraro
// Wrong - missing closing brace
fn main() {
    print("Hello");
    // Missing }

// Correct
fn main() {
    print("Hello");
}
```

### Type Errors

**Issue:** Type mismatch errors
```
Error: Type mismatch at line 10
Expected: int, found: string
```

**Solutions:**

1. **Explicit type conversion:**
```tauraro
// Wrong
let age: int = "25";

// Correct
let age: int = int("25");
```

2. **Check function return types:**
```tauraro
// Wrong
fn get_age() -> int {
    return "25";  // Returns string, not int
}

// Correct
fn get_age() -> int {
    return 25;
}
```

3. **Array type consistency:**
```tauraro
// Wrong
let numbers: array[int] = [1, 2, "3"];

// Correct
let numbers: array[int] = [1, 2, 3];
```

### Import/Module Errors

**Issue:** Module not found
```
Error: Module 'utils' not found
Cannot resolve import: utils.math
```

**Solutions:**

1. **Check file structure:**
```
src/
├── main.tr
├── utils/
│   ├── mod.tr      # Module declaration
│   └── math.tr     # Math utilities
```

2. **Correct import syntax:**
```tauraro
// Wrong
import utils.math

// Correct (if utils is a module)
import utils.math.{add, subtract}

// Or import entire module
import utils.math.*
```

3. **Module declaration in mod.tr:**
```tauraro
// utils/mod.tr
export module math from "./math.tr"
```

## Runtime Errors

### Null Pointer Exceptions

**Issue:** Accessing null values
```
Runtime Error: Null pointer dereference at line 15
Attempted to access property 'name' on null value
```

**Solutions:**

1. **Use null checks:**
```tauraro
// Wrong
fn get_user_name(user: User?) -> string {
    return user.name;  // Crashes if user is null
}

// Correct
fn get_user_name(user: User?) -> string {
    if user == null {
        return "Unknown";
    }
    return user.name;
}
```

2. **Use safe navigation:**
```tauraro
// Safe access with default
let name = user?.name ?? "Unknown";
```

3. **Pattern matching:**
```tauraro
fn process_user(user: User?) {
    match user {
        Some(u) => print("User: " + u.name),
        None => print("No user found")
    }
}
```

### Array Index Out of Bounds

**Issue:** Accessing invalid array indices
```
Runtime Error: Index out of bounds at line 8
Array length: 5, attempted index: 7
```

**Solutions:**

1. **Bounds checking:**
```tauraro
// Wrong
fn get_item(items: array[string], index: int) -> string {
    return items[index];  // May crash
}

// Correct
fn get_item(items: array[string], index: int) -> string? {
    if index < 0 or index >= len(items) {
        return null;
    }
    return items[index];
}
```

2. **Use safe access methods:**
```tauraro
// Safe get with default
let item = items.get(index) ?? "default";

// Or use Result type
fn safe_get(items: array[T], index: int) -> Result[T, IndexError> {
    if index < 0 or index >= len(items) {
        return Err(IndexError.OutOfBounds { index, length: len(items) });
    }
    return Ok(items[index]);
}
```

### Stack Overflow

**Issue:** Infinite recursion
```
Runtime Error: Stack overflow
Maximum call stack size exceeded
```

**Solutions:**

1. **Add base case to recursion:**
```tauraro
// Wrong - infinite recursion
fn factorial(n: int) -> int {
    return n * factorial(n - 1);  // No base case
}

// Correct
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;  // Base case
    }
    return n * factorial(n - 1);
}
```

2. **Use iteration instead:**
```tauraro
fn factorial_iterative(n: int) -> int {
    let result = 1;
    for i in range(2, n + 1) {
        result = result * i;
    }
    return result;
}
```

3. **Tail recursion optimization:**
```tauraro
fn factorial_tail(n: int, acc: int = 1) -> int {
    if n <= 1 {
        return acc;
    }
    return factorial_tail(n - 1, acc * n);
}
```

## Performance Issues

### Slow Compilation

**Issue:** Compilation takes too long

**Solutions:**

1. **Use incremental compilation:**
```bash
# Enable incremental compilation
export TAURARO_INCREMENTAL=1
tauraro build
```

2. **Optimize imports:**
```tauraro
// Wrong - imports entire module
import std.*

// Better - import only what you need
import std.{collections.HashMap, io.File}
```

3. **Reduce generic complexity:**
```tauraro
// Complex generic that slows compilation
class ComplexGeneric[T, U, V, W] where T: Clone, U: Debug, V: Send, W: Sync {
    // Simplify if possible
}
```

### Slow Runtime Performance

**Issue:** Program runs slower than expected

**Diagnostic steps:**

1. **Profile your code:**
```tauraro
// Add timing measurements
let start = time.now();
expensive_operation();
let duration = time.now() - start;
print("Operation took: " + str(duration) + "ms");
```

2. **Check for common performance issues:**

**Inefficient string concatenation:**
```tauraro
// Slow
let result = "";
for item in items {
    result = result + item + ", ";  // Creates new string each time
}

// Fast
let builder = StringBuilder();
for item in items {
    builder.append(item);
    builder.append(", ");
}
let result = builder.to_string();
```

**Unnecessary allocations in loops:**
```tauraro
// Slow
for i in range(1000000) {
    let temp = Vec[int]();  // Allocates every iteration
    temp.push(i);
    process(temp);
}

// Fast
let temp = Vec[int]();
for i in range(1000000) {
    temp.clear();  // Reuse allocation
    temp.push(i);
    process(temp);
}
```

**Inefficient data structures:**
```tauraro
// Slow for lookups
let users = Vec[User]();
for user in users {
    if user.id == target_id {  // O(n) lookup
        return user;
    }
}

// Fast for lookups
let users = HashMap[UserId, User]();
return users.get(target_id);  // O(1) lookup
```

## Memory Problems

### Memory Leaks

**Issue:** Memory usage keeps growing

**Diagnostic:**
```tauraro
// Monitor memory usage
fn monitor_memory() {
    let initial = memory.usage();
    
    // Your code here
    run_application();
    
    let final = memory.usage();
    print("Memory delta: " + str(final - initial) + " bytes");
}
```

**Common causes and solutions:**

1. **Circular references:**
```tauraro
// Problem: Circular reference
class Parent {
    children: Vec[Child]
}

class Child {
    parent: Parent  // Strong reference creates cycle
}

// Solution: Use weak references
class Child {
    parent: WeakRef[Parent]  // Weak reference breaks cycle
}
```

2. **Unclosed resources:**
```tauraro
// Problem: File not closed
fn process_file(filename: string) {
    let file = File.open(filename);
    // File never closed - resource leak
    return process_data(file.read_all());
}

// Solution: Use RAII or explicit cleanup
fn process_file(filename: string) {
    let file = File.open(filename);
    defer file.close();  // Ensures cleanup
    return process_data(file.read_all());
}
```

### Out of Memory Errors

**Issue:** Program crashes with OOM
```
Runtime Error: Out of memory
Failed to allocate 1048576 bytes
```

**Solutions:**

1. **Process data in chunks:**
```tauraro
// Problem: Loading entire file
fn process_large_file(filename: string) {
    let content = File.read_all(filename);  // May be GBs
    return process_data(content);
}

// Solution: Stream processing
fn process_large_file(filename: string) {
    let file = File.open(filename);
    let buffer = array[u8](8192);
    
    while true {
        let bytes_read = file.read(buffer);
        if bytes_read == 0 { break; }
        
        process_chunk(buffer[0:bytes_read]);
    }
}
```

2. **Use memory-efficient data structures:**
```tauraro
// Memory-heavy
let data = HashMap[string, Vec[LargeObject]]();

// More efficient for sparse data
let data = SparseMap[string, Vec[SmallObject]]();
```

## FFI Issues

### Library Loading Errors

**Issue:** Cannot load external library
```
FFI Error: Library 'libmath.so' not found
```

**Solutions:**

1. **Check library path:**
```bash
# Linux/macOS
export LD_LIBRARY_PATH="/path/to/lib:$LD_LIBRARY_PATH"

# Windows
set PATH=C:\path\to\lib;%PATH%
```

2. **Use absolute paths:**
```tauraro
// Instead of relative path
extern "C" from "libmath.so" {
    fn sqrt(x: float) -> float;
}

// Use absolute path
extern "C" from "/usr/local/lib/libmath.so" {
    fn sqrt(x: float) -> float;
}
```

### Type Conversion Errors

**Issue:** FFI type mismatch
```
FFI Error: Cannot convert TauraroLang 'string' to C 'char*'
```

**Solutions:**

1. **Explicit conversions:**
```tauraro
extern "C" {
    fn c_function(str: ptr) -> int;
}

fn safe_wrapper(text: string) -> int {
    let c_str = text.to_c_string();  // Convert to C string
    defer c_str.free();  // Clean up
    return c_function(c_str.as_ptr());
}
```

2. **Use proper FFI types:**
```tauraro
// Wrong
extern "C" {
    fn process_array(arr: array[int]) -> int;
}

// Correct
extern "C" {
    fn process_array(arr: ptr, len: int) -> int;
}

fn wrapper(arr: array[int]) -> int {
    return process_array(arr.as_ptr(), len(arr));
}
```

## Debugging Techniques

### Debug Prints

```tauraro
// Conditional debug output
const DEBUG: bool = true;

fn debug_print(message: string) {
    if DEBUG {
        print("[DEBUG] " + message);
    }
}

// Usage
fn complex_function(data: array[int]) -> int {
    debug_print("Input length: " + str(len(data)));
    
    let result = process_data(data);
    debug_print("Result: " + str(result));
    
    return result;
}
```

### Assertions

```tauraro
fn divide(a: float, b: float) -> float {
    assert(b != 0.0, "Division by zero");
    assert(a.is_finite(), "Invalid dividend");
    
    let result = a / b;
    assert(result.is_finite(), "Invalid result");
    
    return result;
}
```

### Error Context

```tauraro
fn process_user_file(filename: string) -> Result[User, ProcessingError> {
    let content = File.read_all(filename)
        .map_err(|e| ProcessingError.FileRead { 
            filename: filename, 
            cause: e 
        })?;
    
    let user_data = parse_json(content)
        .map_err(|e| ProcessingError.JsonParse { 
            filename: filename, 
            line: e.line, 
            cause: e 
        })?;
    
    let user = User.from_json(user_data)
        .map_err(|e| ProcessingError.UserValidation { 
            filename: filename, 
            cause: e 
        })?;
    
    return Ok(user);
}
```

### Stack Traces

```tauraro
fn get_stack_trace() -> array[string] {
    return runtime.get_stack_trace();
}

fn error_handler(error: Error) {
    print("Error: " + error.message());
    print("Stack trace:");
    
    let trace = get_stack_trace();
    for frame in trace {
        print("  " + frame);
    }
}
```

## Common Gotchas

### Variable Shadowing

```tauraro
// Confusing shadowing
fn process_data() {
    let count = 10;
    
    if some_condition {
        let count = 20;  // Shadows outer count
        print(count);    // Prints 20
    }
    
    print(count);  // Prints 10, not 20
}

// Better: Use different names
fn process_data() {
    let total_count = 10;
    
    if some_condition {
        let batch_count = 20;
        print(batch_count);
    }
    
    print(total_count);
}
```

### Mutable vs Immutable

```tauraro
// Gotcha: Trying to modify immutable
let numbers = [1, 2, 3];
numbers.push(4);  // Error: cannot modify immutable array

// Solution: Use mutable
let mut numbers = [1, 2, 3];
numbers.push(4);  // OK
```

### Reference vs Value Semantics

```tauraro
// Gotcha: Unexpected copying
let original = [1, 2, 3];
let copy = original;  // Creates copy, not reference
copy.push(4);
print(len(original));  // Still 3, not 4

// Solution: Use explicit references
let original = [1, 2, 3];
let reference = &original;  // Reference, not copy
```

## Error Reference

### Compilation Errors

| Error Code | Description | Common Cause |
|------------|-------------|--------------|
| E001 | Syntax error | Missing semicolon, brace mismatch |
| E002 | Type mismatch | Wrong type in assignment/function call |
| E003 | Undefined variable | Typo in variable name, out of scope |
| E004 | Module not found | Wrong import path, missing module |
| E005 | Function not found | Typo in function name, wrong module |

### Runtime Errors

| Error Code | Description | Common Cause |
|------------|-------------|--------------|
| R001 | Null pointer dereference | Accessing null value |
| R002 | Index out of bounds | Array/string index too large |
| R003 | Stack overflow | Infinite recursion |
| R004 | Out of memory | Large allocations, memory leaks |
| R005 | Division by zero | Math operation with zero divisor |

### FFI Errors

| Error Code | Description | Common Cause |
|------------|-------------|--------------|
| F001 | Library not found | Wrong path, missing library |
| F002 | Symbol not found | Wrong function name, ABI mismatch |
| F003 | Type conversion error | Incompatible types |
| F004 | Memory access violation | Invalid pointer, buffer overflow |

## Getting Help

### Diagnostic Information

When reporting issues, include:

1. **TauraroLang version:**
```bash
tauraro --version
```

2. **System information:**
```bash
# Operating system
uname -a

# Rust version
rustc --version

# Environment variables
env | grep TAURARO
```

3. **Minimal reproduction case:**
```tauraro
// Minimal example that reproduces the issue
fn main() {
    // Simplest code that shows the problem
}
```

4. **Full error message:**
```
Include the complete error output, including:
- Error message
- File name and line number
- Stack trace (if available)
- Compilation flags used
```

### Community Resources

- **GitHub Issues:** Report bugs and feature requests
- **Documentation:** Check the official docs first
- **Examples:** Look at example projects
- **Stack Overflow:** Tag questions with `tauraro-lang`

### Debug Build

For better error messages, use debug builds:

```bash
# Debug build with extra information
tauraro build --debug --verbose

# Enable all debug features
export TAURARO_DEBUG=1
export RUST_BACKTRACE=1
tauraro build
```

---

This troubleshooting guide should help you resolve most common issues. If you encounter problems not covered here, please check the documentation or reach out to the community for help.