# Phase 3: Collection Operations & Control Flow JIT Optimization

## Overview
Phase 3 focuses on optimizing the most commonly used operations in real-world Tauraro programs: collection indexing, built-in functions, and control flow. Based on benchmark analysis, these operations appear in nearly every hot loop.

## Current JIT Coverage Analysis

### ✅ Already Supported (Phases 1-2)
- **Arithmetic**: Integer/Float (+, -, *, /, //, %, **)
- **Bitwise**: &, |, ^, <<, >>
- **Comparisons**: ==, !=, <, <=, >, >= (Int/Float)
- **Unary**: -, ~, not
- **Variables**: Load/Store (Local, Global, Fast)
- **Conversions**: int() ↔ float()
- **Total**: ~55 opcodes

### ❌ Critical Gaps (From Benchmark Analysis)
```python
# These patterns appear in EVERY benchmark but aren't JIT-optimized:
total = total + lst[i]        # ❌ SubscrLoad not JIT-optimized
lst.append(i)                 # ❌ Method calls not JIT-optimized
for i in range(100000):       # ❌ ForIter/GetIter not JIT-optimized
if condition:                 # ❌ Jump operations not JIT-optimized
x = len(lst)                  # ❌ Built-in functions not JIT-optimized
```

## Phase 3 Scope

### Group 1: Subscript Operations (Highest Priority)
**Impact**: Used in 90% of loops for list/dict access

**Opcodes to Implement**:
1. `SubscrLoad` - Load from collection (list[i], dict[key])
2. `SubscrStore` - Store to collection (list[i] = value)
3. `Slice` - Slicing operations (list[start:end])

**JIT Implementation**:
- Fast path for integer indices on lists
- Fast path for string keys on dictionaries
- Bounds checking with inline code
- Direct memory access for contiguous arrays

**Expected Speedup**: 3-5x for list indexing in hot loops

---

### Group 2: Control Flow (High Priority)
**Impact**: Enables branch prediction and reduces interpreter overhead

**Opcodes to Implement**:
1. `Jump` - Unconditional jump
2. `JumpIfTrue` - Conditional jump (truthy)
3. `JumpIfFalse` - Conditional jump (falsy)
4. `FastBoolJump` - Optimized boolean jump
5. `FastIntCompareJump` - Fused compare+jump

**JIT Implementation**:
- Native x86-64 conditional branches (je, jne, jl, jg, etc.)
- Branch prediction hints
- Fall-through optimization for common case
- Eliminate interpreter dispatch overhead

**Expected Speedup**: 2-4x for code with conditionals

---

### Group 3: Iteration Operations (High Priority)
**Impact**: Core of every for loop

**Opcodes to Implement**:
1. `GetIter` - Get iterator from iterable
2. `ForIter` - Advance iterator and check for StopIteration
3. `FastRangeIter` - Optimized range iteration
4. `FastRangeLoop` - Ultra-fast range-based loops

**JIT Implementation**:
- Specialize for range() objects (most common case)
- Inline iterator logic for lists
- Eliminate boxing/unboxing in tight loops
- Direct counter increment for range()

**Expected Speedup**: 4-8x for range-based loops

---

### Group 4: Built-in Functions (Medium Priority)
**Impact**: Common operations that currently fall back to interpreter

**Functions to Optimize**:
1. `len()` - Get collection length
2. `range()` - Create range iterator
3. `str()` - Convert to string
4. `int()` - Convert to integer (already have IntToFloat)
5. `abs()` - Absolute value
6. `min()` / `max()` - Min/max of values

**JIT Implementation**:
- Recognize builtin function calls during compilation
- Inline simple builtins (len, abs)
- Emit specialized opcodes for range()
- Direct struct field access for len()

**Expected Speedup**: 2-3x for builtin-heavy code

---

### Group 5: Collection Methods (Medium Priority)
**Impact**: Critical for list building patterns

**Methods to Optimize**:
1. `list.append()` - Most common method in benchmarks
2. `list.extend()` - Batch append
3. `dict.get()` - Safe dictionary access
4. `dict.keys()` / `dict.values()` - Dictionary iteration

**JIT Implementation**:
- Fast path for list.append with capacity checks
- Inline method calls for small methods
- Direct struct manipulation
- Avoid allocations where possible

**Expected Speedup**: 3-5x for list building

---

### Group 6: String Operations (Lower Priority)
**Impact**: Common but less critical than above

**Operations to Optimize**:
1. String concatenation (+)
2. String indexing ([i])
3. String slicing ([start:end])
4. str.format() / f-strings

**JIT Implementation**:
- Detect string types at JIT time
- Use specialized string concat routines
- Direct UTF-8 indexing for ASCII strings
- Rope-based representation for large strings

**Expected Speedup**: 2-3x for string-heavy code

---

## Implementation Strategy

### Phase 3A: Foundation (Week 1)
- Implement control flow opcodes (Jump, JumpIfTrue, JumpIfFalse)
- Add basic block analysis to JIT compiler
- Implement branch target resolution
- Test with simple if/else patterns

**Deliverable**: Conditional branches work in JIT

### Phase 3B: Collections (Week 2)
- Implement SubscrLoad/SubscrStore for lists
- Add fast path for integer indices
- Implement bounds checking
- Add dictionary support
- Test with benchmark_list_operations

**Deliverable**: List/dict indexing 3x faster

### Phase 3C: Iteration (Week 3)
- Implement GetIter/ForIter
- Add FastRangeLoop specialization
- Optimize range() iterator
- Test with all loop benchmarks

**Deliverable**: range() loops 5x faster

### Phase 3D: Built-ins & Methods (Week 4)
- Implement inline len() and abs()
- Add list.append() fast path
- Optimize range() creation
- Test comprehensive suite

**Deliverable**: Built-in heavy code 2x faster

---

## New Opcodes Needed

None! All required opcodes already exist in the VM:
- SubscrLoad, SubscrStore, Slice
- Jump, JumpIfTrue, JumpIfFalse
- GetIter, ForIter, FastRangeIter, FastRangeLoop
- CallFunction (for built-ins)

**Task**: Add JIT implementations for existing opcodes

---

## Testing Plan

### Micro-Benchmarks
```python
# 1. List indexing
def test_list_indexing():
    lst = [i for i in range(10000)]
    total = 0
    for i in range(len(lst)):
        total = total + lst[i]  # ← JIT-optimized
    return total

# 2. Control flow
def test_control_flow():
    total = 0
    for i in range(10000):
        if i % 2 == 0:         # ← JIT-optimized
            total = total + i
    return total

# 3. List building
def test_list_append():
    lst = []
    for i in range(10000):
        lst.append(i)          # ← JIT-optimized
    return len(lst)

# 4. Range iteration
def test_range_loop():
    total = 0
    for i in range(10000):     # ← Ultra-optimized
        total = total + i
    return total
```

### Expected Improvements
| Benchmark | Before (ms) | After (ms) | Speedup |
|-----------|------------|-----------|---------|
| List indexing | 150 | 30 | 5.0x |
| Control flow | 120 | 40 | 3.0x |
| List append | 200 | 50 | 4.0x |
| Range loop | 100 | 15 | 6.7x |

---

## Technical Challenges

### 1. Branch Target Resolution
- Need to track jump targets during JIT compilation
- May need to emit placeholder jumps and patch later
- Handle forward and backward jumps

**Solution**: Two-pass JIT compilation with label tracking

### 2. Type Specialization
- SubscrLoad works differently for list vs dict vs string
- Need runtime type guards
- Fall back to interpreter for uncommon types

**Solution**: Emit type check + fast path + slow path fallback

### 3. Iterator State Management
- Iterators have internal state (position, end)
- Need to track state in JIT-compiled code
- Handle StopIteration exception

**Solution**: Specialize for range() first (no state), then generalize

### 4. Built-in Function Recognition
- Need to detect that `len` refers to the built-in, not a user variable
- Could be shadowed by local variable
- Need symbol table analysis

**Solution**: Check global namespace at JIT time, emit guard

---

## Success Metrics

### Primary Goals
- ✅ 3x speedup on list indexing benchmarks
- ✅ 4x speedup on range-based loops
- ✅ 2x speedup on control-flow heavy code
- ✅ All existing tests pass with JIT enabled

### Secondary Goals
- 5x speedup on list.append() patterns
- Support 90% of common iteration patterns
- JIT compile 80% of hot loops (vs 50% currently)

---

## Future Extensions (Phase 4+)

### Not in Phase 3, but natural next steps:
- **Function Inlining**: Inline small functions in hot loops
- **String Interning**: Optimize string comparisons
- **Type Inference**: Track types through multiple instructions
- **SIMD Operations**: Vectorize array operations
- **Escape Analysis**: Stack-allocate temporary objects
- **Deoptimization**: Safely fall back when assumptions fail

---

## Implementation Priority (Recommended Order)

1. **Start with Group 2** (Control Flow) - Foundation for everything
2. **Then Group 3** (Iteration) - Highest impact on benchmarks
3. **Then Group 1** (Subscript) - Common and measurable
4. **Then Group 4** (Built-ins) - Easier wins
5. **Then Group 5** (Methods) - Build on subscript work
6. **Finally Group 6** (Strings) - Nice to have

---

## Risk Assessment

### Low Risk
- Control flow opcodes (well-understood, native support)
- Range iteration (simple state machine)

### Medium Risk
- Subscript operations (multiple types to handle)
- Built-in function detection (namespace complexity)

### High Risk
- General iterator protocol (complex state, exceptions)
- Dictionary operations (hash collisions, resizing)

**Mitigation**: Start with low-risk items, add comprehensive tests, maintain interpreter fallback

---

## Conclusion

Phase 3 will transform the JIT compiler from "good for arithmetic" to "good for real programs". By optimizing the 5 most common operation categories, we'll see 3-5x speedups on realistic workloads while maintaining 100% compatibility with the interpreter.

**Estimated Total Implementation Time**: 4 weeks
**Estimated Speedup on Real Programs**: 3-5x overall, 10x+ on hot loops
**Lines of Code**: ~2000-3000 lines of new JIT code
**Complexity**: Medium (build on existing Phase 1-2 foundation)
