# Performance Optimization Progress Report

## Benchmarks Run
- **Baseline (unoptimized)**: 29.34 seconds
- **After TaggedValue attempts**: 27.46 seconds (+6.4% speedup)
- **After integer cache**: 27.71 seconds (no improvement - actually slightly worse due to cache overhead)

## Current Performance vs Python
- **Python**: 1.22 seconds
- **Tauraro**: 27.71 seconds  
- **Ratio**: Python is **22.7x FASTER** than Tauraro

## Why Optimizations Didn't Work

### Problem 1: Value Enum is Too Large
The `Value` enum is 24-32 bytes and has many variants. Every match requires checking the discriminant.

### Problem 2: Rc<> Overhead
Every value is wrapped in `Rc<>` which means:
- 16 bytes overhead per value (8 byte pointer + 8 byte ref count)
- Reference counting on every clone
- Cache misses due to indirection

### Problem 3: TaggedValue Conversion Overhead
Converting `Value → TaggedValue → do operation → Value` adds overhead that cancels any gains.

### Problem 4: Integer Cache Miss Rate
In our benchmark:
- `result += i` where `i` goes from 0 to 1M
- Results quickly exceed cache range (-5 to 256)
- Cache hit rate: < 1% (only helps for small i values)

## Root Cause: Architectural Issue

The VM is fundamentally doing too much work per operation:

```rust
// Current (SLOW):
1. Get left value from register (Rc clone = 1 atomic op)
2. Get right value from register (Rc clone = 1 atomic op)  
3. Match on left.value (check discriminant)
4. Match on right.value (check discriminant)
5. Extract i64 from Value::Int
6. Do arithmetic (1 CPU instruction)
7. Create new Value::Int (allocate enum)
8. Wrap in Rc (allocate + set refcount)
9. Store in register

Total: ~50-100 CPU instructions for a single `+` operation!

// Python (FAST):
1. Check if both are small ints (2 pointer comparisons)
2. Extract integers (2 pointer dereferences)
3. Do arithmetic (1 CPU instruction)
4. Check if result in cache range
5. Return cached value (1 Rc bump)

Total: ~10-15 CPU instructions
```

## What We Need: Register-Level Optimization

### Solution 1: Unboxed Integers in Registers (Most Impact)
Change `Frame.registers` from `Vec<RcValue>` to:
```rust
enum RegisterValue {
    Int(i64),           // Unboxed - zero allocation!
    Float(f64),         // Unboxed
    Boxed(RcValue),     // For everything else
}
```

**Expected speedup**: 10-15x for integer-heavy code

### Solution 2: Specialize Hot Opcodes
Generate specialized versions at runtime:
```rust
// Instead of: BinaryAddRR (generic)
// Generate: BinaryAddRR_Int_Int (assumes both are Int)

fn binary_add_int_int(left_reg: usize, right_reg: usize, dest_reg: usize) {
    // Direct integer arithmetic, no type checks!
    registers[dest_reg] = RegisterValue::Int(
        registers[left_reg].as_int_unchecked() + 
        registers[right_reg].as_int_unchecked()
    );
}
```

**Expected speedup**: 5-10x additional

### Solution 3: Inline Hot Loops
Detect patterns like:
```python
for i in range(N):
    result += i
```

And JIT compile to:
```rust
for i in 0..N {
    result += i;  // Native machine code!
}
```

**Expected speedup**: 20-50x for numeric loops

## Recommended Next Steps

### Phase 1: Quick Win (2-3 hours)
1. Add `OpCode::BinaryAddIntInt` - specialized opcode that assumes int + int
2. Compiler detects when both operands are known to be Int
3. Emits specialized opcode instead of generic one

**Expected**: 3-5x speedup with minimal changes

### Phase 2: Medium Effort (1-2 days)
1. Change Frame.registers to use unboxed integers
2. Update all opcodes to handle RegisterValue
3. Box only when escaping to heap

**Expected**: 10-15x total speedup

### Phase 3: Advanced (1 week)
1. Lower JIT threshold to 100 iterations
2. Type specialization based on profiling
3. Inline hot functions

**Expected**: 20-50x total speedup (matches or beats Python!)

## Conclusion

**The current optimizations are hitting diminishing returns because we're optimizing the wrong layer.**

We need to:
1. Stop boxing/unboxing integers in registers
2. Specialize opcodes for common type combinations
3. Use JIT more aggressively

The good news: The infrastructure is there (JIT, profiling, type inference). We just need to use it!

**Recommendation**: Start with Phase 1 (specialized opcodes) as it's low-risk and high-reward.
