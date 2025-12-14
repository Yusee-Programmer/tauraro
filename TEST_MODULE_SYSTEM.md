# Module System Testing Guide

## Overview
This document describes how to test the new modular import system for Tauraro's C transpiler.

## Prerequisites
- Fresh build of tauraro with module system changes
- GCC or Clang compiler available
- Test files: `test_import_user.py`, `myutils.py`, `test_import_builtin.py`

## Test 1: User-Defined Module Import

### Files
**myutils.py** (user-defined module):
```python
def add(a: int, b: int) -> int:
    return a + b

def multiply(a: int, b: int) -> int:
    return a * b

def greet(name: str) -> str:
    return "Hello, " + name
```

**test_import_user.py** (main file):
```python
import myutils

def main() -> int:
    result: int = myutils.add(10, 20)
    product: int = myutils.multiply(5, 6)
    greeting: str = myutils.greet("World")

    print("Result:", result)
    print("Product:", product)
    print(greeting)

    return 0

main()
```

### Compilation Command
```bash
./target/release/tauraro.exe compile test_import_user.py --use-native-transpiler --native
```

### Expected Behavior
1. **Module Detection**:
   ```
   Compiling user module 'myutils' to header file...
   Generated header: build/headers/myutils.h
   Generated 1 user module header(s) in build/headers/
   ```

2. **Directory Structure Created**:
   ```
   build/
   ├── headers/
   │   └── myutils.h          # Header-only file with all functions
   └── test_import_user.c     # Generated main C file
   ```

3. **Generated C Code** (test_import_user.c should include):
   ```c
   #include "build/headers/myutils.h"
   ```

4. **Header File** (build/headers/myutils.h should contain):
   ```c
   #ifndef TAURARO_USER_MYUTILS_H
   #define TAURARO_USER_MYUTILS_H

   // ... helper declarations ...

   // Function implementations
   int add(int a, int b) {
       return a + b;
   }

   int multiply(int a, int b) {
       return a * b;
   }

   char* greet(char* name) {
       // ... string concatenation implementation ...
   }

   #endif
   ```

5. **Executable Created**: `test_import_user.exe`

6. **Execution Output**:
   ```
   Result: 30
   Product: 30
   Hello, World
   ```

## Test 2: Built-in Module Import

### Files
**test_import_builtin.py**:
```python
import math

def test_math() -> float:
    result: float = math.sqrt(16.0)
    return result

def main() -> int:
    value: float = test_math()
    print("Square root of 16:", value)
    return 0

main()
```

### Compilation Command
```bash
./target/release/tauraro.exe compile test_import_builtin.py --use-native-transpiler --native
```

### Expected Behavior
1. **Module Detection**:
   ```
   Compiled 1 builtin module(s) to object files in build/builtins/
   ```

2. **Directory Structure Created**:
   ```
   build/
   ├── builtins/
   │   ├── math_ffi.o         # Compiled Rust FFI object file
   │   └── math_ffi.h         # Reference header (optional)
   └── test_import_builtin.c  # Generated main C file
   ```

3. **Object File Compilation**:
   - `src/builtins_ffi/math_ffi.rs` compiled to `build/builtins/math_ffi.o`
   - Object file linked during final executable compilation

4. **Executable Created**: `test_import_builtin.exe`

5. **Execution Output**:
   ```
   Square root of 16: 4.000000
   ```

## Test 3: Mixed Imports

### File
**test_mixed.py**:
```python
import math
import myutils

def main() -> int:
    # Use builtin module
    root: float = math.sqrt(25.0)
    print("Square root:", root)

    # Use user module
    sum: int = myutils.add(10, 15)
    print("Sum:", sum)

    return 0

main()
```

### Expected Behavior
1. **Both Module Types Processed**:
   ```
   Compiled 1 builtin module(s) to object files in build/builtins/
   Compiling user module 'myutils' to header file...
   Generated 1 user module header(s) in build/headers/
   ```

2. **Directory Structure**:
   ```
   build/
   ├── builtins/
   │   └── math_ffi.o
   ├── headers/
   │   └── myutils.h
   └── test_mixed.c
   ```

3. **Linker Command Includes**:
   - `build/builtins/math_ffi.o` as object file
   - `build/headers/myutils.h` via include directive

## Verification Steps

### 1. Check Generated C Code
```bash
cat build/test_import_user.c | grep -A2 "#include"
```
Should show:
```c
#include "build/headers/myutils.h"
```

### 2. Check Header File Exists
```bash
ls -lh build/headers/myutils.h
```

### 3. Check Object File Exists
```bash
ls -lh build/builtins/math_ffi.o
```

### 4. Verify Header Content
```bash
cat build/headers/myutils.h
```
Should contain:
- Header guards
- All function definitions
- All function implementations (no separate .c file)

### 5. Run Executables
```bash
./test_import_user.exe
./test_import_builtin.exe
```

## Debugging

### If compilation fails:
1. Check that tauraro binary has the new changes:
   ```bash
   ./target/release/tauraro.exe --version
   ```

2. Verify build directories exist:
   ```bash
   mkdir -p build/builtins build/headers
   ```

3. Check for rustc (needed for builtin modules):
   ```bash
   rustc --version
   ```

4. Check for C compiler:
   ```bash
   gcc --version
   ```

### If module not found:
- User modules must be in the same directory as the main file
- File must be named exactly as imported (e.g., `import myutils` → `myutils.py`)

### If linking fails:
- Check that object files were generated in `build/builtins/`
- Verify GCC can find the object files

## Success Criteria

✅ User modules compile to header-only files in `build/headers/`
✅ Built-in modules compile to object files in `build/builtins/`
✅ Main C file includes user module headers
✅ Linker receives builtin module object files
✅ Executables run correctly with expected output
✅ No separate .c files for user modules (header-only)
✅ Module functions callable from main code

## Notes

- User modules are **header-only** - all implementations in .h file
- Built-in modules are **object files** - compiled Rust FFI code
- The system automatically categorizes modules based on the builtin list
- Multiple imports of the same module are deduplicated
- Module compilation follows the same pipeline as main file (lex → parse → type-check → transpile)
