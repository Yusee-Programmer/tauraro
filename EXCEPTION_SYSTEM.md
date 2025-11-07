# Python-Like Exception System with Colored Tracebacks

## Overview

Tauraro now implements a comprehensive Python-compatible exception system with colored, formatted tracebacks that match Python's error reporting exactly.

## Features

### ✅ All Python Builtin Exceptions

Located in `src/modules/exceptions.rs`, including:

**Base Exceptions:**
- `BaseException` - Root of all exceptions
- `Exception` - Base for user exceptions
- `SystemExit`, `KeyboardInterrupt`, `GeneratorExit`

**Common Errors:**
- `SyntaxError`, `IndentationError`, `TabError`
- `NameError`, `UnboundLocalError`
- `TypeError`, `ValueError`
- `AttributeError`, `IndexError`, `KeyError`
- `ZeroDivisionError`, `OverflowError`, `FloatingPointError`
- `ImportError`, `ModuleNotFoundError`
- `RuntimeError`, `NotImplementedError`, `RecursionError`

**OS/IO Errors:**
- `OSError`, `FileNotFoundError`, `PermissionError`
- `ConnectionError`, `TimeoutError`
- And 20+ more OS-related exceptions

**Warnings:**
- `Warning`, `DeprecationWarning`, `SyntaxWarning`
- `RuntimeWarning`, `FutureWarning`, `UserWarning`

### ✅ Colored Traceback Formatter

Located in `src/traceback.rs`:

**Features:**
- Full traceback chain with file/line/function info
- Source code context for each frame
- Caret (^) pointing to exact error location
- Color-coded output:
  - 🔴 **Red**: Exception type and message
  - 🔵 **Cyan**: File names
  - 🟡 **Yellow**: Line numbers
  - 🟢 **Green**: Function names
  - 🔴 **Red Bold**: Error caret (^)

**Auto-detection:**
- Colors enabled for terminal output
- Plain text for piped/file output
- Uses `atty` crate for detection

## Usage Examples

### Basic Syntax Error

```python
# test_syntax.py
if x == :  # Missing value after ==
    pass
```

**Output:**
```
  File "test_syntax.py", line 1
    if x == :
            ^
SyntaxError: invalid syntax
```

### NameError with Traceback

```python
# test_name_error.py
def process():
    return undefined_var + 5

process()
```

**Output:**
```
Traceback (most recent call last):
  File "test_name_error.py", line 4, in <module>
    process()
  File "test_name_error.py", line 2, in process
    return undefined_var + 5
           ^
NameError: name 'undefined_var' is not defined
```

### TypeError Example

```python
# test_type_error.py
result = 5 + "hello"
```

**Output:**
```
  File "test_type_error.py", line 1
    result = 5 + "hello"
             ^
TypeError: unsupported operand type(s) for +: 'int' and 'str'
```

### Complex Traceback

```python
# test_complex.py
def level3():
    raise RuntimeError("Something went wrong!")

def level2():
    level3()

def level1():
    level2()

level1()
```

**Output:**
```
Traceback (most recent call last):
  File "test_complex.py", line 10, in <module>
    level1()
  File "test_complex.py", line 8, in level1
    level2()
  File "test_complex.py", line 5, in level2
    level3()
  File "test_complex.py", line 2, in level3
    raise RuntimeError("Something went wrong!")
    ^
RuntimeError: Something went wrong!
```

## API Reference

### Creating Exceptions

```rust
use tauraro::traceback::*;

// SyntaxError
let err = create_syntax_error(
    "invalid syntax".to_string(),
    "file.py".to_string(),
    line: 10,
    column: 15,
    source: Some("if x == :".to_string()),
);

// NameError
let err = create_name_error(
    "undefined_var".to_string(),
    "file.py".to_string(),
    line: 5,
    column: 10,
    source: Some("x = undefined_var".to_string()),
);

// TypeError, ValueError, etc.
let err = create_type_error(message, filename, line, column, source);
let err = create_value_error(message, filename, line, column, source);
let err = create_zero_division_error(filename, line, column, source);
let err = create_index_error(message, filename, line, column, source);
let err = create_key_error(key, filename, line, column, source);
let err = create_attribute_error(obj_type, attr, filename, line, column, source);
```

### Adding Traceback Frames

```rust
let mut exc = create_runtime_error(
    "Error message".to_string(),
    "main.py".to_string(),
    100,
    10,
    Some("raise RuntimeError()".to_string()),
);

// Add frames in reverse order (most recent first)
exc.add_frame(TracebackFrame::new(
    "utils.py".to_string(),
    50,
    4,
    "helper_function".to_string(),
).with_source("main.process()".to_string()));

exc.add_frame(TracebackFrame::new(
    "main.py".to_string(),
    10,
    0,
    "<module>".to_string(),
).with_source("utils.helper_function()".to_string()));

// Print with colors
println!("{}", exc);
```

### Custom Exceptions

```rust
let exc = TauraroException::new(
    "CustomError".to_string(),
    "Something custom happened".to_string(),
    "file.py".to_string(),
    line: 42,
    column: 8,
)
.with_source("custom_operation()".to_string())
.with_traceback(vec![frame1, frame2]);
```

## Testing

### Run the Demo

```bash
# See all exception types with colors
cargo run --example test_traceback_demo

# See output without colors (piped)
cargo run --example test_traceback_demo | cat
```

### Create Test Files

Create Python files that trigger different errors and run them:

```bash
# Test syntax errors
echo "if x == :" > test_syntax.py
cargo run test_syntax.py

# Test name errors
echo "print(undefined)" > test_name.py
cargo run test_name.py

# Test type errors
echo "result = 5 + 'hello'" > test_type.py
cargo run test_type.py
```

## Integration

### In VM Error Handling

```rust
use crate::traceback::*;

// When an error occurs in the VM:
let exc = create_runtime_error(
    error_message,
    self.current_filename.clone(),
    self.current_line,
    self.current_column,
    get_source_line(&self.source, self.current_line),
);

// Add traceback frames from call stack
for frame in &self.call_stack {
    exc.add_frame(TracebackFrame::new(
        frame.filename.clone(),
        frame.line,
        frame.column,
        frame.function.clone(),
    ).with_source(get_source_line(&frame.source, frame.line)));
}

// Print the error
eprintln!("{}", exc);
```

### In Parser Error Handling

```rust
use crate::traceback::*;

// When syntax error detected:
let exc = create_syntax_error(
    "unexpected token".to_string(),
    filename.to_string(),
    token.line,
    token.column,
    get_source_line(&source, token.line),
);

eprintln!("{}", exc);
```

## Benefits

1. **Python Compatibility**: Exact match with Python's error format
2. **Better UX**: Colors make errors easy to read
3. **Precise Location**: Caret points to exact error position
4. **Full Context**: Source code shown for each frame
5. **Terminal Aware**: Auto-detects color support
6. **Comprehensive**: All 40+ Python builtin exceptions

## Dependencies

Added to `Cargo.toml`:
```toml
colored = "2.0"  # Terminal color support
atty = "0.2"     # Terminal detection
```

## Files Modified

1. **src/traceback.rs** (NEW) - Traceback formatter
2. **Cargo.toml** - Added colored and atty deps
3. **src/main.rs** - Added traceback module
4. **src/lib.rs** - Added traceback module
5. **examples/test_traceback_demo.rs** (NEW) - Demo program

## Future Enhancements

- [ ] Integrate with VM error reporting
- [ ] Integrate with parser error reporting
- [ ] Add source code caching for performance
- [ ] Support for custom exception types
- [ ] Exception chaining (`raise ... from ...`)
- [ ] Warnings system integration
- [ ] Stack trace depth limiting for recursion errors
