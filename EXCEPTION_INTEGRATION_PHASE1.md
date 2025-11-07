# Exception System Integration - Phase 1

## Overview

This phase adds the infrastructure for Python-like colored tracebacks to the Tauraro VM. The colored traceback system was built in the previous commit, and this phase integrates it with the VM runtime.

## What Was Completed

### 1. VM Source Code Storage

**File**: `src/bytecode/vm.rs` (lines 77-82)

Added fields to `SuperBytecodeVM`:
```rust
// EXCEPTION SYSTEM: Source code storage for accurate error reporting
// Maps filename to source code for traceback generation
source_code: HashMap<String, String>,
// Current file being executed (for error reporting)
current_filename: String,
```

**Initialization** (lines 177-180):
```rust
// Initialize exception system
source_code: HashMap::new(),
current_filename: "<unknown>".to_string(),
```

### 2. Helper Methods for Traceback Generation

**File**: `src/bytecode/vm.rs` (lines 415-493)

Added public and private methods:

**`set_source_code(filename, source)`** (line 418)
- Stores source code for a file
- Used for traceback generation

**`set_current_filename(filename)`** (line 423)
- Tracks currently executing file
- Used for error context

**`get_source_line(filename, line)`** (line 428)
- Retrieves specific source line from stored code
- Private helper for traceback generation

**`build_traceback()`** (line 435)
- Builds full traceback from VM call stack
- Iterates through frames in reverse order
- Extracts filename, line number, function name
- Retrieves source code for each frame

**`create_exception(exception_type, message)`** (line 464)
- Creates `TauraroException` with full traceback
- Automatically includes source context
- Ready to use for error reporting

### 3. VM Integration

**File**: `src/vm/core.rs` (lines 26-28)

Updated `run_file_with_options`:
```rust
// EXCEPTION SYSTEM: Store source code for traceback generation
vm.bytecode_vm.set_source_code(filename.to_string(), source.to_string());
vm.bytecode_vm.set_current_filename(filename.to_string());
```

Now every file execution stores its source code for traceback generation.

## Architecture

### Call Stack to Traceback Flow

```
User Code Error
     ↓
VM detect error
     ↓
create_exception("ZeroDivisionError", "division by zero")
     ↓
build_traceback() → iterates through self.frames
     ↓
For each frame:
  - Extract filename, line, function name
  - get_source_line(filename, line)
  - Create TracebackFrame with source
     ↓
TauraroException with full traceback + colors
     ↓
Display to user with Python-like format
```

### Source Code Storage

```
run_file("test.py", source)
     ↓
set_source_code("test.py", source)
     ↓
VM executes code
     ↓
Error occurs
     ↓
get_source_line("test.py", error_line)
     ↓
Retrieved from source_code HashMap
```

## What's Next (Phase 2)

### Update Error Generation Sites

Currently, errors are generated with `anyhow!()`:
```rust
// OLD (current):
return Err(anyhow!("Division by zero"));

// NEW (todo):
let exc = self.create_exception("ZeroDivisionError".to_string(), "division by zero".to_string());
eprintln!("{}", exc);  // Displays with colors!
return Err(anyhow!(exc.format_plain()));  // Plain text for Result
```

### Error Sites to Update

Approximately 500+ error generation sites in `src/bytecode/vm.rs`:
- Division by zero (~20 sites)
- Name errors (~50 sites)
- Type errors (~100 sites)
- Index errors (~30 sites)
- Key errors (~20 sites)
- Attribute errors (~40 sites)
- Import errors (~10 sites)
- Runtime errors (~200+ sites)

### Parser Integration (Phase 3)

Update `src/parser.rs` to use `create_syntax_error()`:
```rust
// OLD:
return Err(format!("Syntax error: unexpected token"));

// NEW:
let exc = crate::traceback::create_syntax_error(
    "unexpected token".to_string(),
    self.filename.clone(),
    token.line,
    token.column,
    Some(source_line),
);
eprintln!("{}", exc);
return Err(format!("{}", exc.format_plain()));
```

## Testing

### Infrastructure Test

```bash
# Run the traceback demo to see colored output
cargo run --example test_traceback_demo
```

Shows 9 exception types with proper formatting.

### Integration Test

Currently errors still use old format:
```bash
$ cargo run test_zero_div.py
Division by zero  # ← Old format
```

After Phase 2:
```bash
$ cargo run test_zero_div.py
Traceback (most recent call last):
  File "test_zero_div.py", line 6, in <module>
    result = calculate(10, 0)
  File "test_zero_div.py", line 3, in calculate
    return a / b
           ^
ZeroDivisionError: division by zero  # ← New colored format!
```

## Benefits of This Phase

1. **Foundation Complete**: Infrastructure ready for error reporting
2. **Source Storage**: All source code stored for traceback generation
3. **Helper Methods**: Easy-to-use API for creating exceptions
4. **Call Stack Tracking**: Automatic traceback building from frames
5. **No Breaking Changes**: Old errors still work, gradual migration possible

## Files Modified

1. **src/bytecode/vm.rs**
   - Added source_code and current_filename fields
   - Added 5 helper methods for exception handling
   - +90 lines

2. **src/vm/core.rs**
   - Updated run_file_with_options to store source
   - +3 lines

3. **test_zero_div.py** (NEW)
   - Test for division by zero error

4. **test_name_error.py** (NEW)
   - Test for name error with nested calls

## Next Steps

**Phase 2**: Update error generation throughout VM (large refactoring)
**Phase 3**: Update parser to use colored syntax errors
**Phase 4**: REPL integration with error history

## Impact

- Infrastructure: ✅ Complete
- Error reporting: 🚧 Ready to integrate
- Parser errors: ⏳ Pending
- REPL errors: ⏳ Pending

This phase establishes the foundation. Phase 2 will make errors beautiful!
