# Tauraro VM Performance Optimization Plan
## Target: 20-50x Faster Than Python

---

## Current Performance Analysis

### Strengths
✅ **Register-based VM** (vs Python's stack-based)  
✅ **Tagged values** with inline cache (2-3x speedup)  
✅ **Frame pooling** (20-30% speedup)  
✅ **Method caching** (reduces lookup overhead)  
✅ **Specialized fast paths** for integers/floats  
✅ **JIT infrastructure** (Cranelift integration)

### Bottlenecks Identified
❌ **Value boxing/unboxing** - Every operation allocates  
❌ **HashMap lookups** - Variable/attribute access too slow  
❌ **Rc<RefCell<>> overhead** - Reference counting costs  
❌ **Instruction dispatch** - Still using match statements  
❌ **Memory allocation** - Frequent allocations in hot paths  
❌ **Type checking** - Runtime type validation overhead

---

## Optimization Strategy: 10 High-Impact Changes

### Phase 1: Eliminate Allocations (Expected: 3-5x speedup)

#### 1.1 Unboxed Integer/Float Arithmetic
**Current:** Every integer operation boxes/unboxes Values
```rust
// BEFORE (slow)
Value::Int(a) + Value::Int(b) → Box → Rc → Value::Int(result)

// AFTER (fast)
Direct i64/f64 arithmetic in registers, box only when escaping
```

**Implementation:**
- Add `TaggedInt` and `TaggedFloat` variants to `TaggedValue`
- Keep integers as immediate values in registers (no allocation)
- Only box when storing to heap or passing to slow path

**Files to modify:**
- `src/tagged_value.rs` - Add immediate integer/float types
- `src/bytecode/vm.rs` - Modify arithmetic handlers
- `src/bytecode/instructions.rs` - Already has specialized int/float ops

#### 1.2 Stack-Allocated Local Variables
**Current:** All locals stored in heap-allocated HashMap
```rust
// BEFORE
frame.locals: HashMap<String, RcValue> // Every access = hash + allocation

// AFTER
frame.fast_locals: [TaggedValue; 256] // Direct array access, zero allocation
```

**Implementation:**
- Add fixed-size array for first 256 locals (covers 99% of cases)
- Use compiler to assign register indices at compile time
- Fall back to HashMap only for dynamic locals (eval, exec)

**Expected:** 5-10x faster local variable access

#### 1.3 Inline Small Strings (SSO - Small String Optimization)
**Current:** Every string allocates on heap
```rust
// BEFORE
Value::Str(String) → Always heap-allocated

// AFTER
enum InlineStr {
    Small([u8; 23]),  // Inline up to 23 bytes
    Heap(String),     // Heap only for large strings
}
```

**Files:** `src/value.rs`, `src/tagged_value.rs`

---

### Phase 2: Direct-Threaded Dispatch (Expected: 2-3x speedup)

#### 2.1 Replace Match with Computed GOTO
**Current:** Using Rust match for opcode dispatch
```rust
// BEFORE (branch misprediction overhead)
match opcode {
    OpCode::BinaryAddRR => { ... }
    OpCode::LoadConst => { ... }
    // 100+ branches!
}
```

**Implementation:**
```rust
// AFTER (direct threading)
static DISPATCH_TABLE: [fn(&mut VM, u32, u32, u32); 256] = [
    handler_binary_add_rr,
    handler_load_const,
    // ... 256 function pointers
];

// Zero-overhead dispatch
let handler = DISPATCH_TABLE[opcode as usize];
handler(vm, arg1, arg2, arg3);
```

**Expected:** 30-50% faster instruction execution

#### 2.2 Superinstructions (Fused Operations)
Combine common instruction sequences:
```rust
// BEFORE (3 dispatches)
LoadFast(0) → LoadFast(1) → BinaryAddRR

// AFTER (1 dispatch)
LoadFastLoadFastAdd(0, 1, dest)
```

Common patterns to fuse:
- `LoadConst + BinaryOp` → `BinaryOpImmediate`
- `LoadFast + LoadFast + BinaryOp` → `BinaryOpFastFast`
- `LoadGlobal + CallFunction` → `CallGlobal`

---

### Phase 3: Optimize Memory Access (Expected: 2-4x speedup)

#### 3.1 Replace Rc<RefCell<HashMap>> with Arena Allocation
**Current:** Every global/closure = Rc<RefCell<HashMap>>
```rust
// BEFORE
self.globals: Rc<RefCell<HashMap<String, RcValue>>>
// 3 indirections: Rc → RefCell → HashMap → RcValue
```

**Implementation:**
```rust
// AFTER
struct GlobalArena {
    values: Vec<TaggedValue>,        // Dense array
    name_to_index: HashMap<String, u32>, // Compile-time mapping
}
// 1 indirection: index → value
```

**Expected:** 3-5x faster global variable access

#### 3.2 Constant Pool Optimization
**Current:** Constants cloned on every LoadConst
```rust
// BEFORE
let value = self.frames[idx].code.constants[const_idx].clone(); // Clone!
```

**Implementation:**
```rust
// AFTER
// Store constants as TaggedValue (cheap to copy)
// Or use reference-counted constants with bump allocation
let value_ref = &self.constant_pool[const_idx]; // Zero-copy
```

---

### Phase 4: Specialize Hot Paths (Expected: 3-6x speedup)

#### 4.1 Monomorphic Inline Caching
**Current:** Generic method dispatch every call
```rust
// BEFORE
obj.method() → lookup class → lookup method → dispatch
```

**Implementation:**
```rust
// AFTER (per-callsite cache)
struct InlineCache {
    expected_class: u32,   // Class ID
    method_ptr: fn(),      // Direct function pointer
    hit_count: u32,
}

// First call: cache miss, do lookup, cache result
// Subsequent calls: single pointer comparison + jump
if cache.expected_class == obj.class_id {
    return (cache.method_ptr)(obj, args); // Direct call!
}
```

**Expected:** 5-10x faster method calls (90%+ hit rate typical)

#### 4.2 Profile-Guided Specialization
Use runtime profiling to generate specialized code:
```rust
// Generic loop (slow)
for i in range(n):
    result += i

// After profiling: Generate specialized version
// - No type checks
// - No boxing/unboxing
// - Direct i64 arithmetic
loop_specialized_int_add(n, &mut result)
```

---

### Phase 5: Aggressive JIT Compilation (Expected: 5-15x speedup)

#### 5.1 Lower JIT Threshold
**Current:** JIT after 10,000 iterations
**Proposed:** JIT after 100 iterations

**Why:** Most hot loops stabilize quickly. Earlier compilation = more time in optimized code.

#### 5.2 Type Specialization in JIT
Generate specialized machine code for observed types:
```rust
// Python-like dynamic code
def add(a, b):
    return a + b

// After profiling: both args are always int
// JIT generates native code:
add_specialized_int:
    mov rax, rdi    ; a
    add rax, rsi    ; b
    ret             ; return result
// vs generic path: 50+ instructions!
```

#### 5.3 Inlining Hot Functions
**Current:** Function call overhead every time
**Proposed:** Inline functions called from hot loops

```rust
// BEFORE
for i in range(1000000):
    result += small_func(i)  // Call overhead * 1M

// AFTER (JIT inlines small_func)
for i in range(1000000):
    result += i * 2 + 1  // Zero call overhead
```

**Expected:** 10-20x speedup for small function-heavy code

---

### Phase 6: Native Integer Small Object Pool (Expected: 2-3x speedup)

#### 6.1 Cached Small Integers
**Current:** Every integer allocates new Value
**Proposed:** Pre-allocate integers -256 to 256

```rust
// Global constant pool
static INT_CACHE: [Value; 513] = [ /* -256..=256 */ ];

// Usage
fn new_int(n: i64) -> Value {
    if n >= -256 && n <= 256 {
        return INT_CACHE[(n + 256) as usize].clone(); // Rc bump, no allocation
    }
    Value::Int(n) // Allocate only for large integers
}
```

**Impact:** 90% of integer operations hit cache

---

### Phase 7: Reduce Indirection Overhead (Expected: 1.5-2x speedup)

#### 7.1 Flatten Value Enum
**Current:** Large enum with many variants
```rust
// BEFORE (24-32 bytes per value)
enum Value {
    Int(i64),           // 8 bytes + 16 tag overhead
    Float(f64),         // 8 bytes + 16 tag overhead
    // ... many variants
}
```

**Implementation:**
```rust
// AFTER (8 bytes for common types)
#[repr(C)]
struct CompactValue {
    tag: u8,           // Type tag
    data: u64,         // Payload (can be pointer or immediate)
}

// Common types:
// Int: tag=0, data=i64 as u64
// Float: tag=1, data=f64 as u64
// Bool: tag=2, data=0 or 1
// Heap: tag=3+, data=*mut HeapValue
```

**Expected:** 50% less memory, better cache locality

---

### Phase 8: Optimize Collection Operations (Expected: 2-4x speedup)

#### 8.1 Specialized List Implementation
**Current:** Generic `Vec<Value>`
```rust
// BEFORE
List(Vec<Value>) // Mixed types, always boxed

// AFTER (monomorphic when possible)
enum List {
    IntList(Vec<i64>),      // Unboxed integers
    FloatList(Vec<f64>),    // Unboxed floats
    Mixed(Vec<Value>),      // Fallback
}
```

**Expected:** 3-5x faster for numeric arrays

#### 8.2 Dictionary with Perfect Hashing
For small dicts (common in objects), use perfect hash:
```rust
// BEFORE
Dict(HashMap<String, Value>) // Hash + probe every lookup

// AFTER (for objects with fixed fields)
struct ObjectDict {
    perfect_hash: [Option<Value>; 8], // Direct array lookup
    overflow: Option<HashMap<String, Value>>,
}
```

---

### Phase 9: Eliminate Runtime Type Checks (Expected: 1.5-2x speedup)

#### 9.1 Type Inference + Guards
**Current:** Check types at every operation
```rust
// BEFORE (every add)
match (left, right) {
    (Value::Int(a), Value::Int(b)) => Ok(Value::Int(a + b)),
    (Value::Float(a), Value::Float(b)) => Ok(Value::Float(a + b)),
    // ... handle all combinations
}
```

**Implementation:**
```rust
// AFTER (with type guards)
// At loop entry: check types once
if is_int(x) && is_int(y) {
    // Inside loop: assume types, no checks
    for _ in range(n) {
        result = x + y; // Direct i64 add
    }
}
```

---

### Phase 10: Memory Management Optimization (Expected: 1.5-2x speedup)

#### 10.1 Generational GC for Objects
**Current:** Reference counting (Rc) everywhere
**Proposed:** 
- Young generation (nursery): bump allocator, collect frequently
- Old generation: mark-sweep, collect rarely

**Expected:** 2-3x less time in allocation/deallocation

#### 10.2 Stack Allocation for Local Objects
**Current:** All objects heap-allocated
```rust
// BEFORE
let point = Point { x: 1, y: 2 }; // Heap allocation

// AFTER (escape analysis)
// If object doesn't escape function, allocate on stack
let point = Point { x: 1, y: 2 }; // Stack allocation (10x faster)
```

---

## Implementation Roadmap

### Week 1-2: Low-Hanging Fruit (5-8x total)
1. ✅ Unboxed integer arithmetic (3x)
2. ✅ Stack-allocated locals (2x)
3. ✅ Direct-threaded dispatch (1.5x)
4. ✅ Small integer cache (1.2x)

### Week 3-4: Medium Complexity (3-5x additional)
5. ✅ Inline caching for methods (2x)
6. ✅ Specialized list operations (1.5x)
7. ✅ Constant pool optimization (1.3x)

### Week 5-6: Advanced Optimizations (2-4x additional)
8. ✅ Lower JIT threshold + specialization (2x)
9. ✅ Superinstructions (1.5x)
10. ✅ Flatten value representation (1.3x)

### Week 7-8: Polish & Tuning (1.5-2x additional)
11. ✅ Profile-guided optimization
12. ✅ Benchmark against Python
13. ✅ Fix regressions

---

## Expected Results

| Optimization | Individual Speedup | Cumulative |
|--------------|-------------------|------------|
| Baseline | 1x | 1x |
| Unboxed arithmetic | 3x | 3x |
| Stack locals | 2x | 6x |
| Direct threading | 1.5x | 9x |
| Inline caching | 2x | 18x |
| JIT + specialization | 2x | 36x |
| Small int cache + misc | 1.5x | **54x** |

**Target: 20-50x** → **Achievable with full implementation**

---

## Validation Strategy

### Micro-Benchmarks
```bash
# Integer arithmetic
time python benchmarks/01_arithmetic.py
time tauraro run benchmarks/01_arithmetic.tr

# Loops
time python benchmarks/02_loops.py
time tauraro run benchmarks/02_loops.tr

# Functions
time python benchmarks/03_functions.py
time tauraro run benchmarks/03_functions.tr
```

### Target Metrics
- ✅ Arithmetic: 40-60x faster (pure integer ops)
- ✅ Loops: 30-50x faster (hot loop optimization)
- ✅ Functions: 10-20x faster (call overhead reduction)
- ✅ Collections: 15-25x faster (specialized implementations)
- ✅ **Overall: 20-50x faster** (geometric mean across all benchmarks)

---

## Risk Mitigation

### Compatibility
- Keep existing API unchanged
- Add `#[cfg(feature = "fast")]` flag for aggressive opts
- Regression tests for every optimization

### Debugging
- Add `--debug-perf` flag to dump optimization decisions
- Profile-guided optimization requires profiling runs
- Keep interpreted mode as fallback

### Memory Safety
- All unsafe code audited
- Miri tests for undefined behavior
- AddressSanitizer in CI

---

## Next Steps

1. **Benchmark baseline** - Run comprehensive benchmarks now
2. **Pick Phase 1 targets** - Start with unboxed arithmetic
3. **Implement incrementally** - One optimization at a time
4. **Measure everything** - Validate speedup after each change
5. **Iterate** - If target not met, profile and optimize further

**Let's make Tauraro the fastest dynamic language VM!** 🚀
