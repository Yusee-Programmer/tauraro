# Phase 4: Runtime Helper Integration

## Overview

Integrate the 30 runtime helper functions into the JIT compiler to enable JIT compilation of collection operations (lists, strings, dicts, tuples, sets).

## Current Status

✅ **Phases 1-3 Complete**:
- 55 opcodes JIT-compiled (arithmetic, bitwise, comparisons)
- Comprehensive test suite (70+ tests)
- 30 runtime helpers implemented in `src/bytecode/jit_runtime.rs`

⏳ **Phase 4 Goal**: Call runtime helpers from JIT-compiled code

## Architecture

### Call Mechanism

```rust
// JIT compiler emits calls to runtime helpers
builder.call(helper_func, &[registers_ptr, arg1_reg, arg2_reg, result_reg]);

// Runtime helper executes with type safety
let result = tauraro_jit_list_index(registers_ptr, list_reg, index_reg, result_reg);

// Check for errors and deoptimize if needed
if result != 0 {
    // Fall back to interpreter
}
```

### Integration Points

1. **`src/bytecode/jit.rs`** - Add helper function declarations
2. **JIT Compiler** - Emit calls for collection opcodes
3. **Deoptimization** - Handle errors by falling back to interpreter

## Implementation Steps

### Step 1: Declare Runtime Helpers in JIT Module

Add function pointers to all 30 helpers so Cranelift can call them:

```rust
// In src/bytecode/jit.rs
use cranelift::prelude::*;
use cranelift_module::{Module, Linkage};

pub fn declare_runtime_helpers(module: &mut Module<impl Backend>) {
    // List operations
    declare_helper(module, "tauraro_jit_subscr_load_list", ...);
    declare_helper(module, "tauraro_jit_subscr_store_list", ...);
    declare_helper(module, "tauraro_jit_list_append", ...);
    declare_helper(module, "tauraro_jit_build_list", ...);

    // String operations
    declare_helper(module, "tauraro_jit_string_concat", ...);
    declare_helper(module, "tauraro_jit_string_index", ...);
    declare_helper(module, "tauraro_jit_string_slice", ...);
    declare_helper(module, "tauraro_jit_string_len", ...);

    // ... (27 more helpers)
}
```

### Step 2: Emit Calls in JIT Compiler

Update opcodes to call runtime helpers instead of skipping:

**Before**:
```rust
OpCode::SubscrLoad => {
    // Skip - not supported in JIT
    return None;
}
```

**After**:
```rust
OpCode::SubscrLoad => {
    let obj_reg = inst.arg1;
    let index_reg = inst.arg2;
    let result_reg = inst.arg3;

    // Get helper function
    let helper_func = self.get_helper("tauraro_jit_subscr_load");

    // Call helper: result = helper(registers, obj_reg, index_reg, result_reg)
    let result = builder.call(helper_func, &[
        registers_ptr,
        obj_reg_val,
        index_reg_val,
        result_reg_val
    ]);

    // Check for error and deoptimize if needed
    self.emit_error_check(builder, result);
}
```

### Step 3: Add Deoptimization Support

When a helper returns an error (-1), fall back to interpreter:

```rust
fn emit_error_check(&mut self, builder: &mut FunctionBuilder, result: Value) {
    // if (result != 0) goto deoptimize_block
    let zero = builder.ins().iconst(I32, 0);
    let cmp = builder.ins().icmp(IntCC::NotEqual, result, zero);

    let continue_block = builder.create_block();
    let deopt_block = builder.create_block();

    builder.ins().brif(cmp, deopt_block, &[], continue_block, &[]);

    // Deoptimization block: return to interpreter
    builder.switch_to_block(deopt_block);
    builder.ins().return_(&[error_val]);

    // Continue block: proceed with JIT code
    builder.switch_to_block(continue_block);
}
```

## Opcodes to Enable

### High Priority (Common Operations)
1. **SubscrLoad** - `list[index]`, `dict[key]`, `str[index]`
2. **SubscrStore** - `list[index] = value`, `dict[key] = value`
3. **ListAppend** - `list.append(item)`
4. **BuildList** - `[1, 2, 3]`
5. **BuildDict** - `{"a": 1, "b": 2}`
6. **GetIter** - `for item in iterable`
7. **ForIter** - Loop iteration

### Medium Priority
8. **BuildTuple** - `(1, 2, 3)`
9. **BuildSet** - `{1, 2, 3}`
10. **SetAdd** - `set.add(item)`

### String Operations
11. String concatenation (already supported via BinaryAdd)
12. String slicing
13. String length

## Performance Projections

### With Runtime Helpers
- **List operations**: 3-5x speedup vs interpreter
- **String operations**: 2-4x speedup
- **Dict operations**: 2-3x speedup
- **Iterator operations**: 5-8x speedup

### Bottleneck Analysis
- **Helper call overhead**: ~5-10 CPU cycles
- **Type checking**: ~2-3 cycles per operation
- **Still faster than interpreter**: ~50-100 cycles per operation

## Testing Strategy

### Test Cases
```python
# Test 1: List indexing in hot loop
def test_list_index():
    items = [1, 2, 3, 4, 5]
    total = 0
    for i in range(10000):
        total = total + items[i % 5]
    return total

# Test 2: List building
def test_list_build():
    for i in range(1000):
        items = [i, i+1, i+2]
    return items

# Test 3: Dictionary access
def test_dict_get():
    config = {"timeout": 30}
    total = 0
    for i in range(10000):
        total = total + config["timeout"]
    return total
```

### Success Criteria
- ✅ All collection operations work correctly
- ✅ Performance: 2-5x faster than interpreter
- ✅ Graceful deoptimization on type errors
- ✅ No memory leaks or crashes

## Timeline

**Week 1** (Current):
- Day 1-2: Declare runtime helpers in JIT module
- Day 3-4: Emit calls for SubscrLoad/SubscrStore
- Day 5-7: Add deoptimization support

**Week 2**:
- Day 1-3: Enable BuildList, BuildDict, BuildTuple
- Day 4-5: Enable GetIter, ForIter
- Day 6-7: Comprehensive testing and optimization

## Implementation Priority

1. **SubscrLoad** (list/dict/string indexing) - Most common
2. **ListAppend** - Very common in loops
3. **BuildList/BuildDict** - Moderate frequency
4. **GetIter/ForIter** - Critical for `for` loops
5. **String operations** - Common in text processing

## Risks and Mitigations

### Risk: Helper Call Overhead
**Impact**: Runtime helpers slower than inlined code
**Mitigation**:
- Inline common operations later (Phase 5)
- Still 2-5x faster than interpreter

### Risk: Deoptimization Too Frequent
**Impact**: Constant fallback to interpreter
**Mitigation**:
- Use type profiling to avoid deopt
- Add type guards for monomorphic code

### Risk: Memory Safety
**Impact**: Raw pointers could cause crashes
**Mitigation**:
- Comprehensive bounds checking in helpers
- Extensive testing with edge cases

## Next Steps

1. ✅ Create this plan document
2. ⏳ Declare first 5 helpers in JIT module
3. ⏳ Emit call for SubscrLoad opcode
4. ⏳ Test with simple list indexing
5. ⏳ Add remaining helpers incrementally

## Success Metrics

- **Coverage**: 90% of hot loops JIT-compilable (up from 50%)
- **Performance**: 5-10x speedup on real programs (vs 3-5x on arithmetic)
- **Reliability**: <1% deoptimization rate in production code
- **Compatibility**: 100% behavioral compatibility with interpreter

---

**Status**: 📋 Planning Complete - Ready to Begin Implementation
**Estimated Completion**: 2 weeks
**Risk Level**: Medium (raw pointers, but well-tested patterns)
