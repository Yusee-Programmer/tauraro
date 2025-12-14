# Tauraro System-Level Software Implementation Plan

**Date:** 2025-12-14
**Purpose:** Technical implementation guide for missing system-level features

---

## 1. CRITICAL FEATURE: FILE I/O SYSTEM

### 1.1 Type System Extension

```c
// Add to types.rs:
typedef struct TauFile {
    FILE* handle;
    char* mode;       // "r", "w", "rb", "wb", etc.
    char* filename;
    bool is_open;
    bool is_binary;
    int refcount;
} TauFile;
```

### 1.2 Builtin Function Implementations

#### `open(filename, mode='r')`
```c
TauValue tauraro_open(int argc, TauValue* argv) {
    if (argc < 1) {
        // Error: filename required
        return tauraro_none();
    }

    char* filename = argv[0].value.s;
    char* mode = (argc >= 2) ? argv[1].value.s : "r";

    FILE* fp = fopen(filename, mode);
    if (!fp) {
        // Error: file not found or permission denied
        // TODO: Raise FileNotFoundError exception
        return tauraro_none();
    }

    TauFile* file = (TauFile*)malloc(sizeof(TauFile));
    file->handle = fp;
    file->mode = strdup(mode);
    file->filename = strdup(filename);
    file->is_open = true;
    file->is_binary = (strchr(mode, 'b') != NULL);
    file->refcount = 1;

    TauValue result;
    result.type = 9;  // TAURARO_FILE
    result.value.ptr = file;
    result.refcount = 1;
    return result;
}
```

#### File Methods
```c
// file.read(size=-1) - Read entire file or N bytes
TauValue tauraro_file_read(TauValue file_val, TauValue size_val) {
    TauFile* file = (TauFile*)file_val.value.ptr;
    if (!file->is_open) {
        // Error: file closed
        return tauraro_none();
    }

    long size = (size_val.type == 0) ? size_val.value.i : -1;

    if (size == -1) {
        // Read entire file
        fseek(file->handle, 0, SEEK_END);
        size = ftell(file->handle);
        fseek(file->handle, 0, SEEK_SET);
    }

    char* buffer = (char*)malloc(size + 1);
    size_t bytes_read = fread(buffer, 1, size, file->handle);
    buffer[bytes_read] = '\0';

    TauValue result;
    result.type = 2;  // TAURARO_STRING
    result.value.s = buffer;
    result.refcount = 1;
    return result;
}

// file.write(data) - Write string to file
TauValue tauraro_file_write(TauValue file_val, TauValue data_val) {
    TauFile* file = (TauFile*)file_val.value.ptr;
    if (!file->is_open) {
        return tauraro_none();
    }

    char* data = data_val.value.s;
    size_t bytes_written = fwrite(data, 1, strlen(data), file->handle);

    TauValue result;
    result.type = 0;  // TAURARO_INT
    result.value.i = bytes_written;
    return result;
}

// file.readline() - Read single line
TauValue tauraro_file_readline(TauValue file_val) {
    TauFile* file = (TauFile*)file_val.value.ptr;
    if (!file->is_open) {
        return tauraro_none();
    }

    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), file->handle)) {
        TauValue result;
        result.type = 2;
        result.value.s = strdup(buffer);
        result.refcount = 1;
        return result;
    }

    return tauraro_str(0, NULL);  // Empty string at EOF
}

// file.close() - Close file
TauValue tauraro_file_close(TauValue file_val) {
    TauFile* file = (TauFile*)file_val.value.ptr;
    if (file->is_open) {
        fclose(file->handle);
        file->is_open = false;
    }
    return tauraro_none();
}

// file.__enter__() - Context manager entry
TauValue tauraro_file_enter(TauValue file_val) {
    return file_val;  // Return self
}

// file.__exit__() - Context manager exit
TauValue tauraro_file_exit(TauValue file_val, TauValue exc_type,
                           TauValue exc_val, TauValue exc_tb) {
    tauraro_file_close(file_val);
    return tauraro_none();
}
```

### 1.3 IR Extensions

```rust
// Add to ir.rs:
pub enum IRInstruction {
    // ... existing ...

    // File I/O
    FileOpen { filename: String, mode: String, result: String },
    FileRead { file: String, size: Option<String>, result: String },
    FileWrite { file: String, data: String, result: String },
    FileClose { file: String },
    FileReadline { file: String, result: String },
}
```

### 1.4 Code Generation

```rust
// In functions.rs - generate_instruction():
IRInstruction::FileOpen { filename, mode, result } => {
    local_vars.insert(result.clone(), "TauValue".to_string());
    Ok(format!(
        "TauValue {} = tauraro_open(2, (TauValue[]){{{}. {}}});",
        result, filename, mode
    ))
}

IRInstruction::FileRead { file, size, result } => {
    let size_arg = size.as_ref().map(|s| s.as_str()).unwrap_or("tauraro_int(-1)");
    local_vars.insert(result.clone(), "TauValue".to_string());
    Ok(format!(
        "TauValue {} = tauraro_file_read({}, {});",
        result, file, size_arg
    ))
}
```

### 1.5 Example Usage

```python
# Tauraro code:
with open("input.txt", "r") as f:
    content = f.read()
    print(content)

# Compiles to C:
TauValue file_1 = tauraro_open(2, (TauValue[]){
    tauraro_str("input.txt"),
    tauraro_str("r")
});
TauValue content = tauraro_file_read(file_1, tauraro_int(-1));
tauraro_print(1, &content);
tauraro_file_close(file_1);
```

---

## 2. CRITICAL FEATURE: EXCEPTION HANDLING

### 2.1 Exception Type System

```c
// Exception types
typedef enum {
    EXC_NONE = 0,
    EXC_BASE,
    EXC_SYSTEM_EXIT,
    EXC_KEYBOARD_INTERRUPT,
    EXC_EXCEPTION,
    EXC_STOP_ITERATION,
    EXC_ARITHMETIC_ERROR,
    EXC_OVERFLOW_ERROR,
    EXC_ZERO_DIVISION_ERROR,
    EXC_ASSERTION_ERROR,
    EXC_ATTRIBUTE_ERROR,
    EXC_IMPORT_ERROR,
    EXC_INDEX_ERROR,
    EXC_KEY_ERROR,
    EXC_NAME_ERROR,
    EXC_RUNTIME_ERROR,
    EXC_TYPE_ERROR,
    EXC_VALUE_ERROR,
    EXC_OS_ERROR,
    EXC_IO_ERROR,
    EXC_FILE_NOT_FOUND_ERROR,
    EXC_PERMISSION_ERROR,
} TauraroExceptionType;

// Exception structure
typedef struct TauraroException {
    TauraroExceptionType type;
    char* message;
    char* traceback;
    int lineno;
    char* filename;
    struct TauraroException* cause;  // Chained exceptions
    int refcount;
} TauraroException;

// Exception context (per thread)
typedef struct {
    jmp_buf* handler_stack[32];  // Stack of exception handlers
    int handler_depth;
    TauraroException* current_exception;
} TauraroExceptionContext;

// Global exception context (thread-local in multi-threaded env)
static TauraroExceptionContext g_exc_ctx = { .handler_depth = 0, .current_exception = NULL };
```

### 2.2 Exception Handling Macros

```c
// Exception handling macros
#define TAURARO_TRY \
    { \
        jmp_buf __exc_buf; \
        g_exc_ctx.handler_stack[g_exc_ctx.handler_depth++] = &__exc_buf; \
        int __exc_code = setjmp(__exc_buf); \
        if (__exc_code == 0) {

#define TAURARO_EXCEPT(exc_type) \
        } else if (g_exc_ctx.current_exception && \
                   g_exc_ctx.current_exception->type == exc_type) { \
            g_exc_ctx.handler_depth--;

#define TAURARO_EXCEPT_ANY \
        } else { \
            g_exc_ctx.handler_depth--;

#define TAURARO_FINALLY \
        } \
        g_exc_ctx.handler_depth--; \
        {

#define TAURARO_END_TRY \
        } \
    }

// Raise an exception
void tauraro_raise(TauraroExceptionType type, const char* message) {
    TauraroException* exc = (TauraroException*)malloc(sizeof(TauraroException));
    exc->type = type;
    exc->message = strdup(message);
    exc->traceback = NULL;  // TODO: Generate stack trace
    exc->lineno = 0;        // TODO: Fill from debug info
    exc->filename = NULL;
    exc->cause = NULL;
    exc->refcount = 1;

    g_exc_ctx.current_exception = exc;

    // Unwind stack to nearest handler
    if (g_exc_ctx.handler_depth > 0) {
        longjmp(*g_exc_ctx.handler_stack[g_exc_ctx.handler_depth - 1], 1);
    } else {
        // Unhandled exception - print and abort
        fprintf(stderr, "Unhandled exception: %s\n", message);
        abort();
    }
}
```

### 2.3 IR Extensions

```rust
pub enum IRInstruction {
    // ... existing ...

    // Enhanced exception handling
    TryBlock {
        try_body: Vec<IRInstruction>,
        except_clauses: Vec<ExceptClause>,
        finally_body: Option<Vec<IRInstruction>>,
        result: Option<String>,
    },
    RaiseException {
        exc_type: String,  // "ValueError", "RuntimeError", etc.
        message: String,
    },
}

pub struct ExceptClause {
    pub exception_type: Option<String>,  // None = catch all
    pub variable: Option<String>,        // Variable to bind exception to
    pub body: Vec<IRInstruction>,
}
```

### 2.4 Code Generation

```rust
IRInstruction::TryBlock { try_body, except_clauses, finally_body, .. } => {
    let mut code = String::new();

    // Generate try block
    code.push_str("TAURARO_TRY {\n");
    for instr in try_body {
        code.push_str(&generate_instruction(instr, local_vars, param_types, class_names)?);
        code.push_str("\n");
    }

    // Generate except clauses
    for except in except_clauses {
        if let Some(exc_type) = &except.exception_type {
            let exc_enum = match exc_type.as_str() {
                "ValueError" => "EXC_VALUE_ERROR",
                "TypeError" => "EXC_TYPE_ERROR",
                "ZeroDivisionError" => "EXC_ZERO_DIVISION_ERROR",
                "FileNotFoundError" => "EXC_FILE_NOT_FOUND_ERROR",
                _ => "EXC_EXCEPTION",
            };
            code.push_str(&format!("TAURARO_EXCEPT({}) {{\n", exc_enum));
        } else {
            code.push_str("TAURARO_EXCEPT_ANY {\n");
        }

        // Bind exception to variable if specified
        if let Some(var) = &except.variable {
            code.push_str(&format!("    TauValue {} = tauraro_exception_to_value(g_exc_ctx.current_exception);\n", var));
        }

        for instr in &except.body {
            code.push_str(&generate_instruction(instr, local_vars, param_types, class_names)?);
        }
        code.push_str("}\n");
    }

    // Generate finally block
    if let Some(finally) = finally_body {
        code.push_str("TAURARO_FINALLY {\n");
        for instr in finally {
            code.push_str(&generate_instruction(instr, local_vars, param_types, class_names)?);
        }
    }

    code.push_str("TAURARO_END_TRY\n");
    Ok(code)
}

IRInstruction::RaiseException { exc_type, message } => {
    let exc_enum = match exc_type.as_str() {
        "ValueError" => "EXC_VALUE_ERROR",
        "TypeError" => "EXC_TYPE_ERROR",
        _ => "EXC_EXCEPTION",
    };
    Ok(format!("tauraro_raise({}, \"{}\");", exc_enum, message))
}
```

### 2.5 Example Usage

```python
# Tauraro code:
try:
    value = int(user_input)
    result = 10 / value
except ValueError as e:
    print("Invalid number:", e)
except ZeroDivisionError:
    print("Cannot divide by zero!")
finally:
    cleanup()

# Compiles to:
TAURARO_TRY {
    TauValue value = tauraro_int(1, &user_input);
    TauValue result = tauraro_div(tauraro_int_literal(10), value);
TAURARO_EXCEPT(EXC_VALUE_ERROR) {
    TauValue e = tauraro_exception_to_value(g_exc_ctx.current_exception);
    tauraro_print(2, (TauValue[]){tauraro_str("Invalid number:"), e});
}
TAURARO_EXCEPT(EXC_ZERO_DIVISION_ERROR) {
    tauraro_print(1, &tauraro_str("Cannot divide by zero!"));
}
TAURARO_FINALLY {
    cleanup(0, NULL);
}
TAURARO_END_TRY
```

---

## 3. CRITICAL FEATURE: COMMAND-LINE ARGUMENTS

### 3.1 sys Module Implementation

```c
// sys module globals
typedef struct {
    TauValue argv;      // List of command-line arguments
    TauValue path;      // List of module search paths
    int exit_code;      // Program exit code
    TauValue platform;  // Platform identifier ("linux", "win32", etc.)
} TauraroSysModule;

static TauraroSysModule g_sys_module;

// Initialize sys module from main()
void tauraro_sys_init(int argc, char* argv[]) {
    // Create sys.argv list
    TauList* argv_list = tauraro_create_list(argc);
    for (int i = 0; i < argc; i++) {
        TauValue arg;
        arg.type = 2;  // String
        arg.value.s = strdup(argv[i]);
        arg.refcount = 1;
        tauraro_list_append(argv_list, arg);
    }

    g_sys_module.argv.type = 4;  // List
    g_sys_module.argv.value.list = argv_list;
    g_sys_module.exit_code = 0;

    // Set platform
    g_sys_module.platform.type = 2;
    #ifdef _WIN32
        g_sys_module.platform.value.s = strdup("win32");
    #elif __linux__
        g_sys_module.platform.value.s = strdup("linux");
    #elif __APPLE__
        g_sys_module.platform.value.s = strdup("darwin");
    #else
        g_sys_module.platform.value.s = strdup("unknown");
    #endif
}

// sys.exit(code)
void tauraro_sys_exit(TauValue code) {
    int exit_code = (code.type == 0) ? code.value.i : 0;
    exit(exit_code);
}

// Access sys.argv from Tauraro code
TauValue tauraro_sys_get_argv() {
    return g_sys_module.argv;
}

// Access sys.platform
TauValue tauraro_sys_get_platform() {
    return g_sys_module.platform;
}
```

### 3.2 Code Generation

```rust
// In module main():
fn generate_main(&self, module: &IRModule) -> String {
    let mut code = String::new();

    code.push_str("int main(int argc, char* argv[]) {\n");
    code.push_str("    // Initialize sys module\n");
    code.push_str("    tauraro_sys_init(argc, argv);\n");
    code.push_str("\n");

    // ... rest of main code ...

    code.push_str("    return g_sys_module.exit_code;\n");
    code.push_str("}\n");
    code
}
```

### 3.3 Example Usage

```python
# Tauraro code:
import sys

if len(sys.argv) < 2:
    print("Usage: program <filename>")
    sys.exit(1)

filename = sys.argv[1]
print(f"Processing: {filename}")

# Compiles to:
TauValue sys_argv = tauraro_sys_get_argv();
TauValue argv_len = tauraro_len(1, &sys_argv);

if (argv_len.value.i < 2) {
    tauraro_print(1, &tauraro_str("Usage: program <filename>"));
    tauraro_sys_exit(tauraro_int_literal(1));
}

TauValue filename = tauraro_list_get(sys_argv.value.list, 1);
tauraro_print(1, &tauraro_format_string("Processing: %s", filename.value.s));
```

---

## 4. CRITICAL FEATURE: STRING FORMATTING

### 4.1 F-String Implementation

```c
// Format string with variable substitution
TauValue tauraro_format_string(const char* format_str, ...) {
    char buffer[4096];
    va_list args;
    va_start(args, format_str);
    vsnprintf(buffer, sizeof(buffer), format_str, args);
    va_end(args);

    TauValue result;
    result.type = 2;  // String
    result.value.s = strdup(buffer);
    result.refcount = 1;
    return result;
}

// String.format() method
TauValue tauraro_str_format(TauValue format_val, int argc, TauValue* argv) {
    char* format = format_val.value.s;
    char result[4096];
    char* out = result;
    int arg_idx = 0;

    for (char* p = format; *p; p++) {
        if (*p == '{' && *(p+1) == '}') {
            // Empty placeholder - use next argument
            if (arg_idx < argc) {
                TauValue arg = argv[arg_idx++];
                char* str = tauraro_to_string(arg);
                strcpy(out, str);
                out += strlen(str);
                free(str);
            }
            p++;  // Skip '}'
        } else if (*p == '{' && isdigit(*(p+1))) {
            // Numbered placeholder {0}, {1}, etc.
            int idx = *(p+1) - '0';
            if (idx < argc) {
                TauValue arg = argv[idx];
                char* str = tauraro_to_string(arg);
                strcpy(out, str);
                out += strlen(str);
                free(str);
            }
            p += 2;  // Skip number and '}'
        } else {
            *out++ = *p;
        }
    }
    *out = '\0';

    TauValue ret;
    ret.type = 2;
    ret.value.s = strdup(result);
    ret.refcount = 1;
    return ret;
}
```

### 4.2 IR Extensions

```rust
pub enum IRInstruction {
    // ... existing ...

    // F-string formatting
    FormatString {
        template: String,      // "Hello, {name}! You are {age} years old."
        values: Vec<String>,   // ["name", "age"]
        result: String,
    },

    // .format() method
    StrFormat {
        format_string: String,
        args: Vec<String>,
        result: String,
    },
}
```

### 4.3 Code Generation

```rust
IRInstruction::FormatString { template, values, result } => {
    let mut format_c = template.clone();
    let mut args_c = Vec::new();

    // Replace {var} with %s in template
    for var in values {
        format_c = format_c.replacen(&format!("{{{}}}", var), "%s", 1);
        args_c.push(format!("tauraro_to_string({})", var));
    }

    let args_joined = args_c.join(", ");
    local_vars.insert(result.clone(), "TauValue".to_string());
    Ok(format!(
        "TauValue {} = tauraro_format_string(\"{}\", {});",
        result, format_c, args_joined
    ))
}

IRInstruction::StrFormat { format_string, args, result } => {
    let args_list = args.join(", ");
    local_vars.insert(result.clone(), "TauValue".to_string());
    Ok(format!(
        "TauValue {} = tauraro_str_format({}, {}, (TauValue[]){{{}}});",
        result, format_string, args.len(), args_list
    ))
}
```

### 4.4 Example Usage

```python
# F-strings:
name = "Alice"
age = 30
msg = f"Hello, {name}! You are {age} years old."
print(msg)

# Compiles to:
TauValue name = tauraro_str("Alice");
TauValue age = tauraro_int_literal(30);
TauValue msg = tauraro_format_string("Hello, %s! You are %s years old.",
                                      tauraro_to_string(name),
                                      tauraro_to_string(age));
tauraro_print(1, &msg);

# .format() method:
template = "Value: {}, Count: {}"
result = template.format(42, 10)

# Compiles to:
TauValue template = tauraro_str("Value: {}, Count: {}");
TauValue result = tauraro_str_format(template, 2,
                                      (TauValue[]){
                                          tauraro_int_literal(42),
                                          tauraro_int_literal(10)
                                      });
```

---

## 5. IMPLEMENTATION PRIORITIES

### Week 1-2: File I/O
1. Implement `TauFile` type
2. Add `open()`, `close()`, `read()`, `write()` builtins
3. Test basic file reading/writing
4. Add context manager support (`with` statement)

### Week 3-4: Exception Handling
1. Implement exception types enum
2. Add setjmp/longjmp macros
3. Generate try/except code
4. Test exception propagation

### Week 5-6: CLI Arguments & String Formatting
1. Implement `sys.argv` initialization
2. Add `sys.exit()`
3. Implement f-string parsing
4. Add `.format()` method

### Week 7-8: Integration Testing
1. Build complete CLI tools
2. Test error handling
3. Performance benchmarking
4. Documentation

---

## 6. TESTING STRATEGY

### Unit Tests

```python
# test_file_io.py
def test_read_file():
    with open("test.txt", "w") as f:
        f.write("Hello, World!")

    with open("test.txt", "r") as f:
        content = f.read()

    assert content == "Hello, World!"

# test_exceptions.py
def test_exception_handling():
    try:
        result = 10 / 0
        assert False  # Should not reach here
    except ZeroDivisionError:
        pass  # Expected

# test_cli_args.py
def test_argv():
    import sys
    assert len(sys.argv) > 0
    assert sys.argv[0] == "./test_program"

# test_formatting.py
def test_fstring():
    name = "Bob"
    msg = f"Hello, {name}!"
    assert msg == "Hello, Bob!"
```

### Integration Tests

```python
# File processor tool
import sys

if len(sys.argv) < 2:
    print("Usage: process <file>")
    sys.exit(1)

filename = sys.argv[1]

try:
    with open(filename, "r") as f:
        lines = f.readlines()

    count = len(lines)
    print(f"File has {count} lines")

except FileNotFoundError:
    print(f"Error: File '{filename}' not found")
    sys.exit(2)
```

---

## 7. PERFORMANCE CONSIDERATIONS

### File I/O Buffering
```c
// Add buffered I/O for performance
typedef struct {
    FILE* handle;
    char* buffer;
    size_t buffer_size;
    size_t buffer_pos;
    bool dirty;
} BufferedFile;

TauValue tauraro_open_buffered(const char* filename, const char* mode, size_t buffer_size) {
    BufferedFile* file = (BufferedFile*)malloc(sizeof(BufferedFile));
    file->handle = fopen(filename, mode);
    file->buffer = (char*)malloc(buffer_size);
    file->buffer_size = buffer_size;
    file->buffer_pos = 0;
    file->dirty = false;

    // ... set up TauValue ...
}
```

### Exception Overhead Reduction
- Use `__builtin_expect()` to hint that exceptions are rare
- Compile with `-fno-exceptions` for C++ interop
- Zero-cost exceptions where possible

### String Formatting Optimization
- Pre-allocate string buffers based on format string analysis
- Use stack buffers for small strings (< 256 bytes)
- Intern common format strings

---

## 8. BACKWARD COMPATIBILITY

All new features must:
1. Not break existing compiled code
2. Be disabled with feature flags if needed
3. Maintain ABI stability for runtime library

---

## 9. DOCUMENTATION REQUIREMENTS

For each feature:
1. **User Guide:** How to use the feature in Tauraro code
2. **C API Reference:** C function signatures and semantics
3. **Examples:** Working code samples
4. **Performance Notes:** Expected overhead, optimization tips

---

## 10. FUTURE ENHANCEMENTS

### Phase 2 (After Core Features)
- **Async I/O:** Non-blocking file operations
- **Memory-Mapped Files:** `mmap()` support
- **Directory Operations:** `os.listdir()`, `os.walk()`
- **Binary I/O:** `struct.pack()`, `struct.unpack()`

### Phase 3 (Advanced)
- **Custom Exceptions:** User-defined exception types
- **Exception Chaining:** PEP 3134 support
- **Stack Traces:** Full backtrace generation
- **Debugger Integration:** GDB pretty-printers

---

**Status:** Ready for Implementation
**Next Step:** Begin Week 1 - File I/O Type System
