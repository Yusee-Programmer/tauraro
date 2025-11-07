# Phase 3 Incremental Implementation Plan

## Current JIT Architecture

The current JIT compiler (`src/bytecode/jit_compiler.rs`) is **loop-focused**:
- Only compiles **range-based for loops** (e.g., `for i in range(100000):`)
- Compiles the loop body into a single Cranelift function
- Passes registers as a pointer array
- Executes iterations natively without interpreter overhead

**Supported in JIT** (from Phases 1-2):
- Integer/Float arithmetic: +, -, *, /, //, %, **
- Bitwise operations: &, |, ^, <<, >>
- Comparisons: ==, !=, <, <=, >, >=
- Unary operations: -, ~, not
- Load/Store: registers, locals, globals
- Fused operations: LoadAddStore, LoadMulStore, etc.

**NOT Supported** (fallback to interpreter):
- Control flow within loops (Jump, JumpIfTrue, JumpIfFalse)
- Collection operations (SubscrLoad, SubscrStore, BuildList)
- Iterator operations (GetIter, ForIter)
- Function/method calls
- String operations

---

## Realistic Phase 3: Focus on High-Impact, Low-Complexity Additions

### Strategy
Instead of implementing complex control flow, focus on:
1. **External function calls** from JIT code to Rust runtime
2. **Inline simple operations** that are common in loops
3. **Type guards** for safe fast paths

This approach:
- ✅ Works within current loop-focused architecture
- ✅ Provides measurable speedups on real benchmarks
- ✅ Maintains stability and correctness
- ✅ Allows incremental testing

---

## Phase 3A: SubscrLoad via Runtime Calls (Week 1)

### Goal
Enable `lst[i]` to work in JIT-compiled loops by calling a Rust runtime function.

### Implementation

**Step 1: Create runtime function**
```rust
// In src/bytecode/jit_compiler.rs

/// Runtime function for list subscript load
/// Returns the value at lst[index], or -1 on error
#[no_mangle]
pub unsafe extern "C" fn tauraro_subscr_load_i64(
    registers_ptr: *mut i64,
    list_reg: u32,
    index_reg: u32,
    result_reg: u32
) -> i32 {
    // Get list from register (pointer to List object)
    // Get index from register (i64)
    // Perform bounds check
    // Load value from list
    // Store in result_reg
    // Return 0 on success, -1 on error
}
```

**Step 2: Emit call from JIT**
```rust
OpCode::SubscrLoad => {
    // Emit call to runtime function
    let runtime_fn = module.declare_function(
        "tauraro_subscr_load_i64",
        Linkage::Import,
        &signature
    )?;

    let call = builder.ins().call(runtime_fn, &[
        registers_ptr,
        list_reg_val,
        index_reg_val,
        result_reg_val
    ]);

    // Check return code
    // Continue or bail to interpreter
}
```

**Expected Speedup**: 1.5-2x for loops with list indexing
**Risk**: Low (runtime function handles complexity)
**Lines of Code**: ~150

---

## Phase 3B: Inline len() for Lists (Week 2)

### Goal
Optimize `len(lst)` when used in loop conditions.

### Implementation

**Approach**: Recognize `CallFunction` with builtin `len` and inline the length check.

```rust
// Detect pattern: CallFunction(len, [list_reg])
// Emit inline code:
let list_ptr = builder.ins().load(...);  // Load list pointer
let len_offset = builder.ins().iconst(I64, 8);  // Offset to length field
let len_addr = builder.ins().iadd(list_ptr, len_offset);
let len_val = builder.ins().load(I64, len_addr, 0);  // Direct length read
```

**Challenge**: Need to detect builtin function calls during JIT compilation.

**Expected Speedup**: 2-3x for loops using `len(lst)` in condition
**Risk**: Medium (need to ensure it's the builtin, not user-defined)
**Lines of Code**: ~100

---

## Phase 3C: Simple Control Flow (Week 3)

### Goal
Support simple if/else within loop bodies.

### Implementation

**Limited scope**: Only support simple if/else, not arbitrary jumps.

```python
for i in range(10000):
    if i % 2 == 0:    # ← JIT-compiled
        total = total + i
    else:
        total = total - i
```

**Approach**:
1. Pre-scan loop body for Jump opcodes
2. Create blocks for each jump target
3. Emit conditional branches using `builder.ins().brif()`
4. Properly seal blocks

**Expected Speedup**: 2-3x for loops with conditionals
**Risk**: High (complex block management, PHI nodes)
**Lines of Code**: ~300-400

**Decision**: Defer to Phase 4 - too complex for Phase 3.

---

## Phase 3D: list.append() Fast Path (Week 4)

### Goal
Optimize `lst.append(x)` in loops.

### Implementation

**Option 1**: Runtime call (like SubscrLoad)
```rust
#[no_mangle]
pub unsafe extern "C" fn tauraro_list_append_i64(
    registers_ptr: *mut i64,
    list_reg: u32,
    value_reg: u32
) -> i32 {
    // Append value to list
    // Handle capacity growth
    // Return 0 on success
}
```

**Option 2**: Inline with capacity check
```rust
// Check if list has capacity
// If yes: direct append
// If no: call runtime to grow
```

**Expected Speedup**: 3-4x for list building loops
**Risk**: Medium (need to handle capacity correctly)
**Lines of Code**: ~200

---

## Revised Phase 3 Timeline

### Week 1: Infrastructure
- **Task**: Add Cranelift external function declaration support
- **Task**: Create runtime function module for JIT helpers
- **Deliverable**: Can call Rust functions from JIT code

### Week 2: SubscrLoad
- **Task**: Implement `tauraro_subscr_load_i64` runtime function
- **Task**: Add `OpCode::SubscrLoad` to JIT compiler
- **Test**: `total = total + lst[i]` in hot loop
- **Deliverable**: List indexing works in JIT, 1.5-2x faster

### Week 3: List Operations
- **Task**: Implement `tauraro_list_append_i64` runtime function
- **Task**: Detect and optimize `list.append()` calls
- **Test**: List building benchmark
- **Deliverable**: List building 3x faster

### Week 4: Testing & Documentation
- **Task**: Comprehensive benchmark suite
- **Task**: Update documentation
- **Task**: Performance comparison report
- **Deliverable**: Phase 3 complete, documented, tested

---

## Testing Strategy

### Micro-Benchmarks

```python
# Test 1: List indexing in loop
def test_list_index_jit():
    lst = [i for i in range(10000)]
    total = 0
    for i in range(len(lst)):
        total = total + lst[i]  # ← Should JIT compile
    return total

# Test 2: List building
def test_list_append_jit():
    lst = []
    for i in range(10000):
        lst.append(i)  # ← Should JIT compile
    return len(lst)

# Test 3: Combined operations
def test_combined():
    lst = []
    for i in range(10000):
        lst.append(i * 2)

    total = 0
    for i in range(len(lst)):
        total = total + lst[i]
    return total
```

### Expected Results

| Benchmark | Before (Interpreter) | After (JIT) | Speedup |
|-----------|---------------------|-------------|---------|
| List indexing | 150ms | 50ms | 3.0x |
| List append | 200ms | 70ms | 2.9x |
| Combined | 350ms | 120ms | 2.9x |

---

## Technical Challenges

### 1. Value Representation
**Problem**: JIT uses i64 registers, but Values are boxed (pointers)
**Solution**: Registers hold pointers to Value objects, runtime functions dereference

### 2. Error Handling
**Problem**: Runtime functions can fail (index out of bounds)
**Solution**: Return error code, JIT checks and bails to interpreter

### 3. Memory Safety
**Problem**: JIT code manipulates pointers, could cause UB
**Solution**: All complex operations go through safe Rust runtime functions

### 4. Type Guards
**Problem**: `lst[i]` could be called on non-list
**Solution**: Runtime function checks type, returns error if wrong type

---

## What's NOT in Phase 3

These are complex features deferred to Phase 4+:

- ❌ General control flow (arbitrary jumps, nested ifs)
- ❌ Dictionary operations (more complex than lists)
- ❌ String operations (UTF-8 complexity)
- ❌ Iterator protocol (StopIteration, complex state)
- ❌ General function inlining
- ❌ Escape analysis
- ❌ Type inference across instructions

---

## Success Metrics

### Primary Goals
- ✅ List indexing (`lst[i]`) works in JIT loops
- ✅ List append (`lst.append(x)`) works in JIT loops
- ✅ 2-3x speedup on list-heavy benchmarks
- ✅ All existing tests pass
- ✅ No correctness regressions

### Secondary Goals
- 2x speedup on real-world programs (not just micro-benchmarks)
- JIT compiles 70%+ of hot loops (up from ~50%)
- Error messages remain clear when JIT bails to interpreter

---

## Implementation Priority

### Start with:
1. **Runtime function infrastructure** - Foundation for everything
2. **SubscrLoad** - Highest impact, relatively simple
3. **Testing** - Ensure correctness before adding more

### Then:
4. **list.append()** - Common pattern, builds on SubscrLoad
5. **Comprehensive benchmarks** - Measure real impact

### Later (Phase 4):
6. Control flow (needs major refactoring)
7. Dictionary operations
8. Function inlining

---

## Risk Mitigation

### For each feature:
1. **Test in interpreter first** - Ensure behavior is well-defined
2. **Implement runtime function** - Test independently
3. **Add JIT emission** - Simple call to runtime
4. **Micro-benchmark** - Verify speedup
5. **Integration test** - Ensure correctness

### Fallback strategy:
- If JIT compilation fails, ALWAYS fall back to interpreter
- Never sacrifice correctness for performance
- Clear error messages when JIT bails

---

## Conclusion

This revised Phase 3 is **realistic and achievable**:
- Focuses on high-impact operations (list access, list building)
- Works within current loop-focused architecture
- Uses runtime calls to manage complexity
- Provides measurable 2-3x speedups
- Maintains stability and correctness

**Estimated time**: 3-4 weeks
**Estimated speedup**: 2-3x on list-heavy code
**LOC**: ~500-700 new lines
**Complexity**: Medium (manageable with runtime calls)

This sets up a strong foundation for Phase 4, which can tackle more complex features like control flow and inlining.
