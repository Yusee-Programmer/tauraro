# Tauraro Exception Handling Fix Action Plan

## Overview

Comprehensive diagnostic testing has identified **12 exception handling bugs** in the Tauraro bytecode VM. This document provides a detailed action plan for fixing them.

---

## Current Status

- **Test Suite Created:** `benchmarks/test_builtin_errors.tr` (14 tests)
- **Success Rate:** 2/14 (14%)
- **Root Cause:** Builtin function errors return Rust `Result::Err` which bypasses Python exception handlers
- **Architecture:** VM has exception handling code but builtin errors don't trigger it

---

## Bug Categories & Fixes

### Category 1: ValueError (Conversion Functions)

#### Bug 1.1: min() with empty list ✅ PARTIALLY WORKING
- **Test:** `min([])`
- **Current Behavior:** Crashes with RuntimeError instead of ValueError
- **Expected:** ValueError with message "min() arg is an empty sequence"
- **Issue:** Wrong exception type being raised
- **Fix Location:** `src/bytecode/vm.rs` - builtin_min() function or VM handler
- **Difficulty:** Medium
- **Impact:** High (common edge case)

**Status:** This one throws an exception but wrong type. Needs exception type correction.

---

### Category 2: TypeError (Type Mismatches)

#### Bug 2.1: len() on non-sequence
- **Test:** `len(42)`
- **Current Behavior:** Crashes with RuntimeError
- **Expected:** TypeError with message "object of type 'int' has no len()"
- **Issue:** Builtin len() error not caught by try-except
- **Fix Location:** `src/bytecode/vm.rs` - builtin_len() function or BuiltinCall handler
- **Difficulty:** Medium
- **Impact:** High (very common operation)

#### Bug 2.2: Binary operator type mismatch (no exception)
- **Test:** `1 + "str"`
- **Current Behavior:** No exception raised - silent behavior change
- **Expected:** TypeError with message "unsupported operand type(s) for +: 'int' and 'str'"
- **Issue:** Binary operation doesn't enforce type checking
- **Fix Location:** `src/bytecode/vm.rs` - BinaryOp handler for Add opcode
- **Difficulty:** Hard
- **Impact:** High (fundamental operation)

#### Bug 2.3: String multiplication with float
- **Test:** `"hello" * 3.5`
- **Current Behavior:** Crashes with TypeError
- **Expected:** TypeError with message "can't multiply sequence by non-int of type 'float'"
- **Issue:** String multiplication doesn't accept float multiplier
- **Fix Location:** `src/bytecode/vm.rs` - BinaryOp handler for Multiply opcode
- **Difficulty:** Medium
- **Impact:** Medium (less common than int multiplication)

---

### Category 3: IndexError (Sequence Indexing)

#### Bug 3.1: List index out of bounds
- **Test:** `[1, 2, 3][10]`
- **Current Behavior:** Crashes with IndexError (not caught)
- **Expected:** IndexError caught by try-except
- **Issue:** Indexing error doesn't propagate to exception handler
- **Fix Location:** `src/bytecode/vm.rs` - Index opcode handler
- **Difficulty:** Medium
- **Impact:** High (very common operation)

#### Bug 3.2: String index out of bounds
- **Test:** `"hello"[100]`
- **Current Behavior:** Crashes with IndexError (not caught)
- **Expected:** IndexError caught by try-except
- **Issue:** String indexing error doesn't propagate to exception handler
- **Fix Location:** `src/bytecode/vm.rs` - Index opcode handler
- **Difficulty:** Medium
- **Impact:** High (very common operation)

#### Bug 3.3: Tuple index out of bounds
- **Test:** `(1, 2, 3)[5]`
- **Current Behavior:** Crashes with IndexError (not caught)
- **Expected:** IndexError caught by try-except
- **Issue:** Tuple indexing error doesn't propagate to exception handler
- **Fix Location:** `src/bytecode/vm.rs` - Index opcode handler
- **Difficulty:** Medium
- **Impact:** High (very common operation)

---

### Category 4: KeyError (Dictionary Access)

#### Bug 4.1: Dict missing key
- **Test:** `{'a': 1}['b']`
- **Current Behavior:** Crashes with KeyError (not caught)
- **Expected:** KeyError caught by try-except
- **Issue:** Dictionary key access doesn't trigger exception handler
- **Fix Location:** `src/bytecode/vm.rs` - Index opcode handler (for dict subscript)
- **Difficulty:** Medium
- **Impact:** High (very common operation)

#### Bug 4.2: Empty dict access
- **Test:** `{}['key']`
- **Current Behavior:** Crashes with KeyError (not caught)
- **Expected:** KeyError caught by try-except
- **Issue:** Dictionary key access doesn't trigger exception handler
- **Fix Location:** `src/bytecode/vm.rs` - Index opcode handler (for dict subscript)
- **Difficulty:** Medium
- **Impact:** High (very common operation)

---

### Category 5: ZeroDivisionError (Arithmetic)

#### Bug 5.1: Integer division by zero
- **Test:** `10 / 0`
- **Current Behavior:** Crashes with ZeroDivisionError (not caught)
- **Expected:** ZeroDivisionError caught by try-except
- **Issue:** Division by zero doesn't trigger exception handler
- **Fix Location:** `src/bytecode/vm.rs` - BinaryOp handler for Divide opcode
- **Difficulty:** Medium
- **Impact:** High (fundamental operation)

#### Bug 5.2: Float division by zero
- **Test:** `10.0 / 0.0`
- **Current Behavior:** Crashes with ZeroDivisionError (not caught)
- **Expected:** ZeroDivisionError caught by try-except
- **Issue:** Float division by zero doesn't trigger exception handler
- **Fix Location:** `src/bytecode/vm.rs` - BinaryOp handler for Divide opcode
- **Difficulty:** Medium
- **Impact:** High (fundamental operation)

#### Bug 5.3: Modulo by zero
- **Test:** `10 % 0`
- **Current Behavior:** Crashes with ZeroDivisionError (not caught)
- **Expected:** ZeroDivisionError caught by try-except
- **Issue:** Modulo by zero doesn't trigger exception handler
- **Fix Location:** `src/bytecode/vm.rs` - BinaryOp handler for Modulo opcode
- **Difficulty:** Medium
- **Impact:** Medium (less common than division)

---

## Recommended Fix Order

### Phase 1: Foundation (Day 1)
**Goal:** Establish exception handling pattern that other fixes can follow

1. **Fix: Index opcode exception handling** (Bugs 3.1, 3.2, 3.3, 4.1, 4.2)
   - Covers 5 bugs at once (IndexError and KeyError)
   - Single code path fix
   - High impact
   - Estimated effort: 2-3 hours

   **Implementation Steps:**
   ```
   a) In src/bytecode/vm.rs, Index opcode handler:
      - Current: Returns error and crashes
      - Change: Convert error to Exception value
      - Call existing exception handler mechanism
      - Set up proper exception object
   ```

### Phase 2: Arithmetic (Day 2)
**Goal:** Handle all arithmetic operation errors

2. **Fix: Binary operator error handling** (Bugs 2.2, 5.1, 5.2, 5.3)
   - Covers 4 bugs
   - Reuse pattern from Phase 1
   - Handles Add, Divide, Modulo opcodes
   - Estimated effort: 2-3 hours

   **Implementation Steps:**
   ```
   a) In src/bytecode/vm.rs, BinaryOp handler:
      - For Add: Check type compatibility before operation
      - For Divide/Modulo: Check for zero divisor
      - Convert errors to proper Exception values
      - Use exception handler mechanism
   ```

3. **Fix: String multiplication type checking** (Bug 2.3)
   - Specialized case of BinaryOp
   - Reuse pattern from Phase 2
   - Estimated effort: 1 hour

### Phase 3: Builtins (Day 3)
**Goal:** Standardize builtin function error handling

4. **Fix: len() builtin exception handling** (Bug 2.1)
   - Standardize how builtins check types
   - Reuse exception pattern
   - Estimated effort: 1-2 hours

5. **Fix: min() exception type** (Bug 1.1)
   - Currently throws RuntimeError, should throw ValueError
   - Simple fix: Change exception type
   - Estimated effort: 30 minutes

---

## Implementation Pattern

Once the first fix is done, subsequent fixes follow this pattern:

```rust
// Current (broken) pattern:
match operation_result {
    Ok(value) => /* continue */,
    Err(e) => return Err(e), // Crashes!
}

// New (fixed) pattern:
match operation_result {
    Ok(value) => /* continue */,
    Err(e) => {
        // Convert Rust error to Python exception
        let exception = create_exception_from_error(e);
        
        // Check if we're in a try-except block
        if let Some(handler) = self.find_exception_handler() {
            // Store exception and jump to handler
            self.current_exception = Some(exception);
            self.pc = handler.address;
            continue; // Jump to handler
        } else {
            // Not in try-except, propagate error
            return Err(e);
        }
    }
}
```

---

## Testing Strategy

1. **Before each fix:**
   - Run `benchmarks/test_builtin_errors.tr`
   - Note current success rate
   
2. **After each fix:**
   - Run `benchmarks/test_builtin_errors.tr`
   - Verify improvement in success rate
   - Ensure no regression in `benchmarks/test_all_builtins.tr`
   
3. **Final verification:**
   - All 14 tests should pass
   - Success rate should be 100%
   - Both test suites should pass

---

## Key Code Locations

### Exception Handling Infrastructure
- **File:** `src/bytecode/vm.rs`
- **Lines:** 935-990 (exception handler code)
- **Key structs:** 
  - `BlockStack` - tracks try-except blocks
  - `ExceptionHandler` - handler information
  - `SetupExcept` opcode - sets up handler

### Opcode Handlers
- **File:** `src/bytecode/vm.rs`
- **Function:** `execute_instruction_fast()`
- **Key opcodes:**
  - `Index` - sequence/dict subscripting
  - `BinaryOp` - arithmetic operations
  - `Call` - function calls (includes builtins)

### Builtin Functions
- **File:** `src/bytecode/vm.rs`
- **Pattern:** `builtin_*()` functions
- **Example:** `builtin_len()`, `builtin_min()`

---

## Success Metrics

- [ ] Bug 3.1-3.3, 4.1-4.2 fixed (IndexError + KeyError) - 0% → 36%
- [ ] Bug 5.1-5.3 fixed (ZeroDivisionError) - 36% → 57%
- [ ] Bug 2.1, 2.3 fixed (len TypeError, string multiply) - 57% → 79%
- [ ] Bug 1.1 fixed (min() ValueError type) - 79% → 86%
- [ ] Bug 2.2 fixed (binary operator TypeError) - 86% → 100%

---

## Risk Assessment

**Low Risk:**
- Bug 1.1 (min() exception type) - simple type change
- Bug 2.3 (string multiply) - specialized case

**Medium Risk:**
- Bug 3.1-3.3, 4.1-4.2 (IndexError/KeyError) - common operations, high visibility
- Bug 5.1-5.3 (ZeroDivisionError) - fundamental arithmetic

**High Risk:**
- Bug 2.1 (len() TypeError) - very common, could affect many programs
- Bug 2.2 (binary operator) - fundamental operation, could expose design issues

---

## Potential Blockers

1. **Exception object creation** - May need to standardize how Python exception objects are created from Rust errors
2. **Exception matching** - May need to verify `except TypeError:` works correctly for all TypeError sources
3. **Stack unwinding** - May need to ensure proper cleanup when jumping to exception handler
4. **Variable scope** - May need to verify variables are accessible correctly after exception handler

---

## Related Issues

See `BUILTINS_FIXES_SUMMARY.md` for:
- Setattr() optimization status
- Map/filter with user functions status
- Performance improvements applied
- Complete list of working builtins

---

## References

- VM Backend: `src/bytecode/vm.rs`
- Exception Handling: `src/bytecode/vm.rs` lines 935-990
- Test Suite: `benchmarks/test_builtin_errors.tr`
- Results: `benchmarks/ERROR_TEST_RESULTS.md`

