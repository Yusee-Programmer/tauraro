# Phase 5: VM Integration Design

## Overview

This document outlines the design for integrating the Phase 4 Cranelift JIT compiler (`cranelift_jit.rs`) with the VM's hot loop detection system.

**Goal**: Replace the old `jit_compiler.rs` with the new `cranelift_jit.rs` implementation to enable runtime helper calls in JIT-compiled code.

---

## Current Architecture Analysis

### Existing Implementation (`jit_compiler.rs`)

**Signature**:
```rust
pub fn compile_loop(
    &mut self,
    function_name: &str,
    instructions: &[Instruction],
    constants: &[Value],
    loop_start: usize,
    loop_end: usize,
    result_reg: u32,
    start_value: i64,
    step: i64,
) -> Result<*const u8>
```

**Native Function Type**:
```rust
fn(*mut i64, *const i64, i64) -> i32
// Parameters: (registers_ptr, constants_ptr, iteration_count)
```

**Features**:
- Simple integer arithmetic compilation
- No runtime helper support
- Fixed loop iteration model (for range loops only)
- Returns raw function pointer

### New Implementation (`cranelift_jit.rs` - Phase 4)

**Signature**:
```rust
pub fn compile_loop(
    &mut self,
    function_name: &str,
    instructions: &[Instruction],
    constants: &[TauraroValue],
) -> Result<JitFunction>
```

**Native Function Type**:
```rust
type JitFunction = unsafe extern "C" fn(*mut RcValue, usize) -> i32
// Parameters: (registers_ptr, reg_count)
```

**Features**:
- Runtime helper integration (18 helpers)
- Automatic deoptimization
- Collection operations support (lists, dicts, strings, tuples, sets)
- Returns typed function pointer

---

## Integration Strategy

### Approach: Replace Old JIT Compiler

**Decision**: Replace `jit_compiler.rs` with `cranelift_jit.rs` entirely.

**Rationale**:
1. Phase 4 implementation is superior (runtime helpers, deopt, collection support)
2. Cleaner codebase (single JIT implementation)
3. Forward compatibility with Phase 5+ optimizations
4. Old implementation is a prototype, not production-ready

### Migration Steps

#### Step 1: Adapt CraneliftJIT Signature

**Problem**: VM expects specific `compile_loop` signature with loop_start, loop_end, etc.

**Solution**: Add wrapper method to CraneliftJIT that matches VM expectations:

```rust
impl CraneliftJIT {
    /// Compile a loop with VM-compatible signature
    pub fn compile_loop_vm(
        &mut self,
        function_name: &str,
        instructions: &[Instruction],
        constants: &[TauraroValue],
        loop_start: usize,
        loop_end: usize,
        result_reg: u32,
        start_value: i64,
        step: i64,
    ) -> Result<*const u8> {
        // Extract loop body instructions
        let loop_body = &instructions[loop_start..loop_end];

        // Compile with Cranelift
        let jit_fn = self.compile_loop(function_name, loop_body, constants)?;

        // Return raw pointer for VM
        Ok(jit_fn as *const u8)
    }
}
```

#### Step 2: Update VM JIT Compiler Type

**File**: `src/bytecode/vm.rs`

**Change**:
```rust
// Before:
#[cfg(feature = "jit")]
jit_compiler: Option<crate::bytecode::jit_compiler::JITCompiler>,

// After:
#[cfg(feature = "jit")]
jit_compiler: Option<crate::bytecode::cranelift_jit::CraneliftJIT>,
```

**Initialization**:
```rust
#[cfg(feature = "jit")]
jit_compiler: {
    match crate::bytecode::cranelift_jit::CraneliftJIT::new() {
        Ok(compiler) => Some(compiler),
        Err(e) => {
            eprintln!("Warning: Failed to initialize JIT compiler: {}", e);
            None
        }
    }
},
```

#### Step 3: Update VM Call Site

**File**: `src/bytecode/vm.rs` (around line 1678)

**Change**: Replace `compile_loop` call with `compile_loop_vm`:

```rust
// Current call:
match compiler.compile_loop(
    &function_name,
    &self.frames[frame_idx].code.instructions,
    &self.frames[frame_idx].code.constants,
    loop_start_pc,
    loop_end_pc,
    result_reg as u32,
    start_value,
    step_value,
) {
    Ok(native_fn_ptr) => {
        // ... handle compilation
    }
}

// No change needed if we add compile_loop_vm wrapper
```

#### Step 4: Update Native Code Execution

**Problem**: Native function signatures differ.

**Old signature**:
```rust
fn(*mut i64, *const i64, i64) -> i32
```

**New signature**:
```rust
fn(*mut RcValue, usize) -> i32
```

**Solution**: Adapt call site in VM to use RcValue array:

```rust
// Current execution (around line 1749):
let native_fn: NativeLoopFn = unsafe {
    std::mem::transmute(native_code_ptr)
};

let result = unsafe {
    native_fn(
        registers_ptr,
        constants_ptr,
        remaining,
    )
};

// New execution:
use crate::bytecode::cranelift_jit::JitFunction;

let native_fn: JitFunction = unsafe {
    std::mem::transmute(native_code_ptr)
};

// Get mutable pointer to registers
let registers_ptr = self.frames[frame_idx].registers.as_mut_ptr();
let reg_count = self.frames[frame_idx].registers.len();

let result = unsafe {
    native_fn(registers_ptr, reg_count)
};
```

#### Step 5: Deprecate Old JIT Compiler

**Action**: Remove `jit_compiler.rs` after successful integration.

**Timeline**: After Phase 5 VM integration tests pass.

---

## Technical Challenges

### Challenge 1: Loop Iteration Model

**Old Model**: JIT code receives iteration count and executes loop internally
```rust
fn loop_body(registers, constants, iteration_count) {
    for i in 0..iteration_count {
        // execute loop body
    }
}
```

**New Model**: JIT code executes ONE iteration, VM calls repeatedly
```rust
fn loop_body(registers, reg_count) -> i32 {
    // execute one iteration
    // return 0 (success) or -1 (error/deopt)
}
```

**Problem**: New model has higher call overhead but enables deoptimization.

**Solution**: Hybrid approach - JIT code executes multiple iterations in a batch:
```rust
fn loop_body_batched(registers, reg_count, batch_size) -> i32 {
    for _ in 0..batch_size {
        // execute one iteration
        // check for deopt condition
        if error { return -1; }
    }
    return 0;
}
```

### Challenge 2: Register Array Type Mismatch

**Old**: `*mut i64` (simple integers)
**New**: `*mut RcValue` (reference-counted values)

**Impact**: VM needs to pass RcValue array instead of i64 array.

**Solution**: VM already uses RcValue internally, so this is actually an improvement. No conversion needed.

### Challenge 3: Constants Handling

**Old**: Constants passed as separate array
**New**: Constants accessed via runtime helpers when needed

**Impact**: Simplifies JIT signature but requires helper calls for constant loads.

**Solution**: Phase 5.2 will inline constant loads to eliminate helper overhead.

---

## Implementation Plan

### Phase 5.1: VM Integration (Current)

**Tasks**:
1. Add `compile_loop_vm` wrapper to CraneliftJIT
2. Update VM to use `cranelift_jit::CraneliftJIT`
3. Adapt native code execution call site
4. Test with simple integer loop
5. Test with collection operations

**Timeline**: 1-2 days

**Success Criteria**:
- ✅ VM compiles hot loops using CraneliftJIT
- ✅ JIT-compiled loops execute correctly
- ✅ Deoptimization works on errors
- ✅ Performance: 3-5x speedup vs interpreter

### Phase 5.2: Inline Optimizations (Next)

**Tasks**:
1. Inline integer arithmetic (eliminate helper calls)
2. Inline constant loads
3. Add type guards for monomorphic code
4. Implement constant folding

**Timeline**: 1-2 weeks

**Success Criteria**:
- ✅ Integer loops execute without helper calls
- ✅ Performance: 10-20x speedup vs interpreter
- ✅ Code size remains reasonable (<2000 LOC)

### Phase 5.3: Advanced Optimizations (Future)

**Tasks**:
1. Range check elimination
2. Loop unrolling
3. Register allocation improvements
4. Instruction scheduling

**Timeline**: 2-3 weeks

---

## Performance Projections

### Current (Phase 4)

**Status**: CraneliftJIT implemented but not integrated with VM

**Projected Performance**:
- Integer arithmetic: 3-5x speedup (via helpers)
- Collection operations: 3-5x speedup (via helpers)
- Overall: 3-10x on real programs

**Bottleneck**: Runtime helper call overhead (~5-10 cycles per operation)

### Phase 5.1 (VM Integration)

**Status**: This document

**Projected Performance**:
- Same as Phase 4 (validation phase)
- Focus: Correctness and stability
- Deoptimization overhead: <1%

### Phase 5.2 (Inline Optimizations)

**Status**: Planned

**Projected Performance**:
- Integer arithmetic: 10-20x speedup (inlined ops)
- Collection operations: 3-5x speedup (still using helpers)
- Overall: 10-30x on numeric-heavy programs
- Overall: 5-15x on mixed programs

**Bottleneck**: Collection operations still use helpers

### Phase 5.3 (Advanced Optimizations)

**Status**: Future

**Projected Performance**:
- Integer arithmetic: 20-50x speedup
- Collection operations: 10-20x speedup (inlined checks)
- Overall: 20-50x on real programs

**Goal**: Match PyPy/LuaJIT performance levels

---

## Risk Assessment

### Low Risk ✅

1. **VM Integration** - Straightforward API changes
2. **Wrapper Method** - Simple adapter pattern
3. **Backward Compatibility** - Can keep old JIT temporarily

### Medium Risk ⚠️

1. **Performance Regression** - New model might be slower initially
   - **Mitigation**: Comprehensive benchmarks before/after

2. **Deoptimization Frequency** - Unclear how often deopt triggers
   - **Mitigation**: Add deopt statistics and tuning

3. **Register Array Handling** - Pointer type changes
   - **Mitigation**: Careful testing with valgrind/miri

### High Risk ❌

None identified.

---

## Testing Strategy

### Unit Tests

```python
# Test 1: Simple integer loop
def test_simple_loop():
    total = 0
    for i in range(10000):
        total = total + i
    return total
# Expected: 49995000

# Test 2: List operations
def test_list_loop():
    items = [1, 2, 3]
    total = 0
    for i in range(1000):
        total = total + items[i % 3]
    return total
# Expected: 2000

# Test 3: Nested data
def test_dict_loop():
    config = {"x": 10, "y": 20}
    total = 0
    for i in range(1000):
        total = total + config["x"]
    return total
# Expected: 10000
```

### Integration Tests

1. **Hot Loop Detection**: Verify loops trigger at 10,000 iterations
2. **Compilation Success**: Check successful JIT compilation
3. **Execution Correctness**: Compare JIT vs interpreter results
4. **Deoptimization**: Force errors and verify fallback
5. **Performance**: Measure speedup vs interpreter baseline

### Performance Benchmarks

```python
import time

# Benchmark 1: Numeric computation
def fib(n):
    a, b = 0, 1
    for i in range(n):
        a, b = b, a + b
    return a

# Run 100,000 iterations
start = time.time()
result = fib(100000)
end = time.time()
print(f"Time: {end - start:.3f}s")

# Expected:
# - Interpreter: ~2.0s
# - JIT Phase 5.1: ~0.5s (4x speedup)
# - JIT Phase 5.2: ~0.1s (20x speedup)
```

---

## Success Metrics

### Phase 5.1 Complete When:

- ✅ VM successfully uses CraneliftJIT instead of old JIT compiler
- ✅ All existing tests pass (70+ tests)
- ✅ New integration tests pass (10+ tests)
- ✅ Performance: 3-10x speedup vs interpreter
- ✅ Deoptimization works correctly
- ✅ No memory leaks or crashes
- ✅ Code quality: All warnings resolved

---

## File Changes Summary

### Files to Modify:

1. **`src/bytecode/cranelift_jit.rs`**
   - Add `compile_loop_vm` wrapper method
   - Keep existing `compile_loop` method

2. **`src/bytecode/vm.rs`**
   - Change `jit_compiler` type to `CraneliftJIT`
   - Update initialization
   - Update native code execution call site

3. **`src/bytecode/mod.rs`**
   - No changes needed (cranelift_jit already exported)

### Files to Remove (After Testing):

4. **`src/bytecode/jit_compiler.rs`** (deprecated)
   - Remove after Phase 5.1 is stable

---

## Next Steps

### Immediate (Phase 5.1):

1. ✅ Create this design document
2. ⏳ Implement `compile_loop_vm` wrapper
3. ⏳ Update VM integration points
4. ⏳ Test with simple loops
5. ⏳ Comprehensive testing

### Short Term (Phase 5.2):

1. Inline integer arithmetic operations
2. Add type guards
3. Constant folding optimization
4. Performance benchmarking

### Long Term (Phase 5.3):

1. Range check elimination
2. Loop unrolling
3. Instruction scheduling
4. Register allocation improvements

---

## Conclusion

**Phase 5.1 is a straightforward integration** that connects the Phase 4 Cranelift JIT implementation with the VM's hot loop detection system. The main work is adapting the function signatures and updating call sites.

**Key Benefits**:
- Runtime helper support (18 operations)
- Automatic deoptimization
- Collection operations in JIT code
- Foundation for Phase 5.2 inline optimizations

**Timeline**: 1-2 days for implementation and testing

**Risk Level**: Low - well-defined changes with clear testing strategy

---

**Status**: 📋 Design Complete - Ready for Implementation
**Next**: Implement `compile_loop_vm` wrapper method
