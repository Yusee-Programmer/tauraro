# Tauraro Performance Analysis & Benchmark Results

**Date**: November 8, 2025
**Branch**: `claude/apply-jit-tauraro-features-011CUu1ZYCmen7xgDm4mEGiY`
**Status**: Performance issues identified, JIT work in progress

---

## Executive Summary

**Current Performance**: Tauraro is **40-70x SLOWER** than Python 3
**Root Cause**: Interpreter overhead, not JIT issues
**Phase 5.1 Status**: 70% complete (JIT integration with register issues)

---

## Benchmark Results

### Full Benchmark Suite (Python vs Tauraro)

```
================================================================================
COMPREHENSIVE PYTHON VS TAURARO BENCHMARKS
================================================================================

Benchmark                 Python (s)      Tauraro (s)     Speedup
--------------------------------------------------------------------------------
arithmetic                1.0817          44.9474         0.02x (41x SLOWER)
loops                     0.1930          13.9004         0.01x (72x SLOWER)
functions                 0.4365          28.3481         0.02x (65x SLOWER)
--------------------------------------------------------------------------------
TOTAL                     1.7112          87.1959         0.02x (51x SLOWER)
================================================================================

⚠️  TARGET NOT MET: 0.02x speedup (target: 20-50x FASTER)
   Need 1019x more optimization
```

### Breakdown

**Arithmetic Benchmarks**:
- Integer arithmetic: 13.3s vs 0.31s (42x slower)
- Float arithmetic: 17.4s vs 0.26s (67x slower)
- Mixed operations: 14.2s vs 0.51s (28x slower)

**Loop Benchmarks**:
- For loop: 3.97s vs 0.09s (44x slower)
- While loop: 8.62s vs 0.10s (86x slower)
- Nested loops: 1.31s vs 0.006s (218x slower!)

**Function Benchmarks**:
- Function calls (1M): 19.8s vs 0.33s (60x slower)
- Recursive fibonacci(25): 6.37s vs 0.05s (127x slower!)
- Iterative fibonacci (10k): 2.21s vs 0.05s (44x slower)

**Data Structures**:
- List comprehension: **PARSE ERROR** (not implemented)

---

## Root Cause Analysis

### 1. Interpreter Is Very Slow (Primary Issue)

The core interpreter has significant overhead:

**Evidence**:
- Simple 100k iteration loop takes 1.3s (interpreted, no JIT)
- Python does same loop in ~0.009s
- **144x slower** for basic integer arithmetic

**Causes**:
1. **Value boxing overhead**: Every operation boxes/unboxes RcValue
2. **No fast paths for common cases**: All operations go through full dispatch
3. **Memory allocation**: Frequent RcValue::new() calls
4. **Pattern matching overhead**: Large match statements for every opcode
5. **No inline caching**: Every operation redoes type checks

### 2. JIT Integration Incomplete (Secondary Issue)

Phase 5.1 implementation has issues:

**What Works**:
- ✅ JIT compilation triggers correctly (at 100 iterations)
- ✅ Loop control structure implemented
- ✅ Loop variable storage helper added
- ✅ Cranelift IR generation

**What Doesn't Work**:
- ⚠️  Register synchronization between interpreter and JIT
- ⚠️  Loop variable mapping incorrect
- ⚠️  JIT produces partial results (only first 100 iterations counted)

**Test Results** (with JIT enabled, threshold=100):
```
Test 1: Got 4851, Expected 49995000 (0.01% correct)
Test 2: Got 1980, Expected 20000 (9.9% correct)
Test 3: Got 4851, Expected 12497500 (0.04% correct)
```

The JIT runs but doesn't accumulate results correctly.

### 3. Missing Features

**List Comprehensions**: Parser error
```
ERROR: File "<main>", line 39, column 10
Unexpected token: expected Expected 'for' in comprehension
```

---

## Performance Bottlenecks (Detailed)

### Bottleneck 1: RcValue Boxing/Unboxing

**Current Code** (every operation):
```rust
let left = &registers[arg1].value;  // Dereference RcValue
let right = &registers[arg2].value; // Dereference RcValue
match (left, right) {
    (Value::Int(a), Value::Int(b)) => {
        // Do operation
        registers[arg3] = RcValue::new(Value::Int(result)); // Box result
    }
}
```

**Cost per operation**: ~20-30 CPU cycles
**Operations per second**: ~1M ops/sec
**Python performance**: ~50M ops/sec (50x faster)

### Bottleneck 2: OpCode Dispatch

**Current Code**:
```rust
match instruction.opcode {
    OpCode::BinaryAdd => { /* 50 lines */ }
    OpCode::BinarySub => { /* 50 lines */ }
    // ... 200+ opcodes
}
```

**Cost**: Branch misprediction + dispatch overhead = ~10 cycles/op
**Solution**: Jump table or threaded code

### Bottleneck 3: Type Checking

**Every arithmetic operation**:
```rust
match (&left.value, &right.value) {
    (Value::Int(a), Value::Int(b)) => { /* fast path */ }
    (Value::Float(a), Value::Float(b)) => { /* fast path */ }
    (Value::Int(a), Value::Float(b)) => { /* convert */ }
    // ... 10+ cases
}
```

**Cost**: ~15-20 cycles per operation
**Solution**: Inline caching, type speculation

### Bottleneck 4: Memory Allocation

**RcValue::new() called for every value**:
- Allocates on heap
- Initializes reference count
- Registers with drop glue

**Cost**: ~50-100 cycles
**Solution**: Value pool, unboxed integers

---

## Why JIT Isn't Helping (Yet)

### Issue 1: Register Synchronization

**Problem**: JIT stores loop variable in result_reg, but loop body instructions expect it elsewhere.

**Example**:
```python
for i in range(10000):
    total = total + i
```

**What happens**:
1. Interpreter executes iterations 0-99
2. JIT triggers, stores `i=100` in result_reg
3. JIT executes loop body: `total = total + i`
4. But loop body reads `i` from DIFFERENT register
5. JIT adds wrong value (or 0) to total
6. Result is incorrect

**Fix needed**: Map interpreter registers to JIT registers correctly

### Issue 2: Threshold Too High Initially

**Original threshold**: 10,000 iterations
- For `range(10000)` loop, triggers AFTER loop finishes
- JIT compiles but has nothing to execute
- Zero speedup

**Current threshold**: 100 iterations
- Better, but still wastes first 100 iterations
- Ideal: 10-50 iterations

### Issue 3: No Inline Optimization

**Current JIT**: Calls runtime helpers for everything
- List access: helper call (~10 cycles)
- Integer add: helper call (~10 cycles)
- Type check: helper call (~10 cycles)

**Total overhead**: ~30 cycles per operation
**Interpreter cost**: ~50 cycles per operation
**Speedup**: Only 1.6x, not 10-50x

**Needed**: Phase 5.2 inline optimizations

---

## Comparison to Other Interpreters

| Interpreter | Relative Speed | JIT | Optimization Level |
|-------------|----------------|-----|-------------------|
| **CPython** | 1.0x (baseline) | No | Moderate (opcode caching) |
| **Tauraro** | **0.02x (50x slower)** | Partial | Low (basic VM) |
| **PyPy** | 5-50x faster | Yes | Very High (tracing JIT) |
| **LuaJIT** | 10-100x faster | Yes | Extreme (DynASM) |
| **V8 (JS)** | 10-100x faster | Yes | Extreme (TurboFan) |

**Key Insight**: Even CPython (no JIT) is 50x faster than Tauraro

---

## Optimization Roadmap

### Phase 5.1: Fix JIT Integration (Current - 70% done)

**Remaining Work** (2-3 hours):
1. Fix register synchronization
2. Correct loop variable mapping
3. Test with benchmarks
4. Verify JIT produces correct results

**Expected Improvement**: 2-5x speedup (interpreter → JIT)

### Phase 5.2: Interpreter Fast Paths (High Priority)

**Optimizations**:
1. **Fast path for integer arithmetic** (no boxing)
   - Direct i64 operations
   - Only box at the end
   - **Expected**: 5-10x speedup on arithmetic

2. **Inline caching for operations**
   - Cache type combinations
   - Skip type checks for hot paths
   - **Expected**: 2-3x speedup

3. **Value pooling**
   - Reuse RcValue objects
   - Reduce allocations
   - **Expected**: 1.5-2x speedup

4. **Jump table dispatch**
   - Replace giant match with computed goto
   - **Expected**: 1.3-1.5x speedup

**Total Expected**: 10-30x speedup (combined)
**Timeline**: 1-2 weeks

### Phase 5.3: JIT Inline Optimizations

**Optimizations**:
1. Inline integer arithmetic (no helper calls)
2. Type guards and specialization
3. Constant folding
4. Range check elimination

**Expected**: 10-20x speedup on JIT code
**Timeline**: 2-3 weeks

---

## Immediate Action Items

### Priority 1: Fix JIT Register Issues (Blocking)

**Tasks**:
1. Debug register mapping in compile_loop_vm
2. Ensure loop variable is stored in correct register
3. Verify loop body instructions read from correct registers
4. Test with phase5_vm_integration.py

**Estimated Time**: 2-3 hours
**Impact**: Enable proper JIT testing

### Priority 2: Add Fast Path for Integer Add (Quick Win)

**Implementation**:
```rust
OpCode::BinaryAdd => {
    let left_rc = &registers[arg1];
    let right_rc = &registers[arg2];

    // Fast path: both integers
    if let (Value::Int(a), Value::Int(b)) = (&left_rc.value, &right_rc.value) {
        registers[arg3] = RcValue::new(Value::Int(a + b));
        continue;
    }

    // Slow path: other types
    // ... existing code ...
}
```

**Estimated Time**: 1 hour
**Impact**: 5-10x speedup on arithmetic benchmarks

### Priority 3: Lower JIT Threshold

**Change**: 100 → 10-20 iterations
**Rationale**: Waste less time in interpreter
**Estimated Time**: 5 minutes
**Impact**: Better JIT coverage

### Priority 4: Fix List Comprehensions (Correctness)

**Parser Issue**: Expected 'for' keyword not found
**Estimated Time**: 1-2 hours
**Impact**: Benchmark suite can run fully

---

## Performance Targets

### Short Term (Phase 5.1 + 5.2 complete)

**Target**: 10-20x faster than current
- Arithmetic: 2-3s (vs Python 1.1s)
- Loops: 1-2s (vs Python 0.2s)
- Functions: 2-3s (vs Python 0.4s)

**Overall**: Still 2-5x slower than Python, but acceptable

### Medium Term (Phase 5.3 complete)

**Target**: Match or beat Python
- Arithmetic: 0.5-1s
- Loops: 0.1-0.2s
- Functions: 0.2-0.4s

**Overall**: Competitive with CPython

### Long Term (Phase 6+)

**Target**: Match PyPy (5-10x faster than Python)
- Tracing JIT
- Advanced optimizations
- Type speculation

---

## Technical Debt

1. **Time module broken**: Returns incorrect large values
2. **List comprehensions**: Parser incomplete
3. **RcValue overhead**: Pervasive throughout codebase
4. **No profiling tools**: Can't identify hot spots easily
5. **Test suite slow**: Takes 1-2 minutes to run

---

## Recommendations

### For Next Session:

1. **Fix JIT register synchronization** (2-3 hours)
   - This is blocking all JIT performance testing
   - Once fixed, we can properly measure JIT impact

2. **Add integer fast path** (1 hour)
   - Quick win for arithmetic benchmarks
   - Easy to implement and test

3. **Lower JIT threshold to 20** (5 minutes)
   - More aggressive JIT compilation
   - Better performance on small-medium loops

### For Future Work:

1. **Refactor core interpreter** (1-2 weeks)
   - Remove RcValue from hot path
   - Add inline caching
   - Implement jump table dispatch

2. **Complete Phase 5.2-5.3** (2-3 weeks)
   - JIT inline optimizations
   - Reach performance parity with Python

3. **Implement list comprehensions** (1-2 hours)
   - Fix parser to handle comprehension syntax
   - Enable data structure benchmarks

---

## Conclusion

**Current State**:
- Tauraro is 50x slower than Python due to interpreter overhead
- JIT work is 70% complete but has register synchronization issues
- Core performance improvements are needed BEFORE JIT can show benefits

**Key Insight**:
The bottleneck is NOT the JIT - it's the base interpreter. Even with perfect JIT, we'd still be 10-20x slower than Python without interpreter optimizations.

**Path Forward**:
1. Fix JIT register issues (unblock testing)
2. Add interpreter fast paths (10-30x improvement)
3. Complete JIT inline opts (10-20x on JIT code)
4. Combined result: Match or beat Python performance

**Estimated Timeline**: 3-4 weeks to reach Python performance parity

---

**Last Updated**: November 8, 2025
**Author**: Claude (Performance Analysis Session)
**Status**: Analysis complete, optimization work in progress
