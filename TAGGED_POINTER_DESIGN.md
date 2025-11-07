# Tagged Pointer Value Design for Tauraro

## 🎯 Goal: 2-3x Performance Improvement

Replace the current large Value enum with a compact tagged pointer representation that:
- Stores small integers directly in the pointer (no allocation)
- Uses NaN-boxing for floats
- Reduces Value size from 16+ bytes to 8 bytes
- Eliminates enum discriminant checks

## 📊 Current vs Proposed

### Current Implementation (Slow)
```rust
pub enum Value {
    Int(i64),           // 16 bytes (8 for discriminant, 8 for value)
    Float(f64),         // 16 bytes
    Bool(bool),         // 16 bytes
    None,               // 8 bytes (just discriminant)
    Str(String),        // 40+ bytes
    List(HPList),       // 32+ bytes
    // ... many more variants
}

// Every operation requires enum match:
match value {
    Value::Int(n) => { /* do something */ }
    Value::Float(f) => { /* do something */ }
    _ => { /* slow path */ }
}
```

### Proposed Implementation (Fast)
```rust
// Single 64-bit value that encodes type in the bits
#[repr(transparent)]
pub struct TaggedValue(u64);

// Encoding scheme (NaN-boxing):
// 0x0000_0000_0000_0000 - 0x0000_7FFF_FFFF_FFFF : Small integers (47 bits)
// 0x0001_0000_0000_0000 - 0x7FF7_FFFF_FFFF_FFFF : Valid floats
// 0x7FF8_0000_0000_0000 - 0x7FFF_FFFF_FFFF_FFFF : NaN space (we use this!)
//   0x7FF8_0000_0000_0000 : None
//   0x7FF8_0000_0000_0001 : True
//   0x7FF8_0000_0000_0002 : False
//   0x7FF8_1xxx_xxxx_xxxx : Heap pointer (strings, lists, etc.)

// Operations become bit tests:
if value.0 & 0xFFFF_0000_0000_0000 == 0 {
    // It's a small integer - NO allocation!
    let n = value.0 as i64;
}
```

## 🔬 Detailed Design

### Memory Layout

```
64-bit value breakdown:
┌─────────────────┬──────────────────────────────────────────────┐
│  Sign + Exp     │              Significand                     │
│  (12 bits)      │              (52 bits)                       │
└─────────────────┴──────────────────────────────────────────────┘

Type Detection:
0x0000_xxxx_xxxx_xxxx    Small integer (sign bit = 0, exp < 0x7FF8)
0x7FF0_xxxx_xxxx_xxxx    Valid float (sign bit varies, exp = 0x7FF0-0x7FF7)
0x7FF8_xxxx_xxxx_xxxx    NaN space - OUR TAGGED VALUES!
0xFFFF_xxxx_xxxx_xxxx    Negative small integer

Breakdown of NaN space (0x7FF8_xxxx_xxxx_xxxx):
0x7FF8_0000_0000_0000    Reserved: None
0x7FF8_0000_0000_0001    Reserved: True
0x7FF8_0000_0000_0002    Reserved: False
0x7FF8_1000_0000_0000+   Heap pointers (48 bits for address)
```

### Type Tags

```rust
// Type tag in lower bits for heap-allocated objects
const TAG_MASK: u64       = 0xFFFF_0000_0000_0000;
const TAG_SMALL_INT: u64  = 0x0000_0000_0000_0000; // Any value < 0x7FF8
const TAG_FLOAT: u64      = 0x7FF0_0000_0000_0000; // 0x7FF0-0x7FF7
const TAG_NAN_BASE: u64   = 0x7FF8_0000_0000_0000;
const TAG_NONE: u64       = 0x7FF8_0000_0000_0000;
const TAG_TRUE: u64       = 0x7FF8_0000_0000_0001;
const TAG_FALSE: u64      = 0x7FF8_0000_0000_0002;
const TAG_HEAP: u64       = 0x7FF8_1000_0000_0000; // Base for heap pointers

// Heap object type tags (in lower bits of pointer)
const HEAP_TYPE_MASK: u64     = 0x0000_0000_0000_000F;
const HEAP_TYPE_STRING: u64   = 0x0000_0000_0000_0001;
const HEAP_TYPE_LIST: u64     = 0x0000_0000_0000_0002;
const HEAP_TYPE_DICT: u64     = 0x0000_0000_0000_0003;
const HEAP_TYPE_TUPLE: u64    = 0x0000_0000_0000_0004;
const HEAP_TYPE_CLOSURE: u64  = 0x0000_0000_0000_0005;
const HEAP_TYPE_OBJECT: u64   = 0x0000_0000_0000_0006;
const HEAP_TYPE_CLASS: u64    = 0x0000_0000_0000_0007;
// ... more types
```

## 💻 Implementation

### Core TaggedValue Structure

```rust
#[repr(transparent)]
#[derive(Copy, Clone)]
pub struct TaggedValue(u64);

impl TaggedValue {
    // Constants
    const TAG_MASK: u64 = 0xFFFF_0000_0000_0000;
    const TAG_NAN_BASE: u64 = 0x7FF8_0000_0000_0000;
    const SMALL_INT_MAX: i64 = (1i64 << 47) - 1;  // 47 bits for small int
    const SMALL_INT_MIN: i64 = -(1i64 << 47);

    // Constructors
    #[inline(always)]
    pub fn new_int(n: i64) -> Self {
        if n >= Self::SMALL_INT_MIN && n <= Self::SMALL_INT_MAX {
            // Small integer: store directly
            TaggedValue(n as u64)
        } else {
            // Large integer: allocate on heap
            Self::new_heap_int(n)
        }
    }

    #[inline(always)]
    pub fn new_float(f: f64) -> Self {
        let bits = f.to_bits();
        if (bits & Self::TAG_MASK) != Self::TAG_NAN_BASE {
            // Valid float: store directly
            TaggedValue(bits)
        } else {
            // NaN or Inf: allocate on heap to avoid collision
            Self::new_heap_float(f)
        }
    }

    #[inline(always)]
    pub fn new_bool(b: bool) -> Self {
        if b {
            TaggedValue(TAG_TRUE)
        } else {
            TaggedValue(TAG_FALSE)
        }
    }

    #[inline(always)]
    pub fn new_none() -> Self {
        TaggedValue(TAG_NONE)
    }

    // Type checks (ULTRA FAST - just bit tests!)
    #[inline(always)]
    pub fn is_small_int(&self) -> bool {
        // Any value with top bits != 0x7FF8 is a small int
        (self.0 & 0xFFFF_0000_0000_0000) < 0x7FF8_0000_0000_0000
    }

    #[inline(always)]
    pub fn is_float(&self) -> bool {
        let top = self.0 & 0xFFF0_0000_0000_0000;
        top >= 0x7FF0_0000_0000_0000 && top < 0x7FF8_0000_0000_0000
    }

    #[inline(always)]
    pub fn is_bool(&self) -> bool {
        self.0 == TAG_TRUE || self.0 == TAG_FALSE
    }

    #[inline(always)]
    pub fn is_none(&self) -> bool {
        self.0 == TAG_NONE
    }

    #[inline(always)]
    pub fn is_heap_object(&self) -> bool {
        (self.0 & 0xFFFF_0000_0000_0000) == TAG_HEAP
    }

    // Value extraction (ULTRA FAST)
    #[inline(always)]
    pub fn as_int(&self) -> Option<i64> {
        if self.is_small_int() {
            // Sign-extend from 47 bits
            let n = self.0 as i64;
            Some(n)
        } else if self.is_heap_object() {
            // Check if it's a heap-allocated large int
            self.as_heap_int()
        } else {
            None
        }
    }

    #[inline(always)]
    pub fn as_float(&self) -> Option<f64> {
        if self.is_float() {
            Some(f64::from_bits(self.0))
        } else if self.is_heap_object() {
            self.as_heap_float()
        } else {
            None
        }
    }

    #[inline(always)]
    pub fn as_bool(&self) -> Option<bool> {
        if self.0 == TAG_TRUE {
            Some(true)
        } else if self.0 == TAG_FALSE {
            Some(false)
        } else {
            None
        }
    }

    // Arithmetic operations (ULTRA FAST for small ints)
    #[inline(always)]
    pub fn add(&self, other: &TaggedValue) -> Option<TaggedValue> {
        if self.is_small_int() && other.is_small_int() {
            let a = self.0 as i64;
            let b = other.0 as i64;
            let result = a.wrapping_add(b);

            // Check if result fits in small int
            if result >= Self::SMALL_INT_MIN && result <= Self::SMALL_INT_MAX {
                Some(TaggedValue(result as u64))
            } else {
                // Overflow: allocate on heap
                Some(Self::new_heap_int(result))
            }
        } else {
            None // Fall back to slow path
        }
    }
}
```

### Heap-Allocated Objects

```rust
// For objects that don't fit in tagged pointer
#[repr(C)]
struct HeapObject {
    header: HeapHeader,
    data: HeapData,
}

#[repr(C)]
struct HeapHeader {
    type_tag: u8,
    ref_count: u32, // For garbage collection
    size: u32,
}

enum HeapData {
    LargeInt(i64),
    LargeFloat(f64),
    String(Box<str>),
    List(Vec<TaggedValue>),
    Dict(HashMap<TaggedValue, TaggedValue>),
    // ... more types
}
```

## 📈 Performance Benefits

### Memory Usage
```
Before (Value enum):
- Int: 16 bytes
- Float: 16 bytes
- Bool: 16 bytes
- Total for 1M integers: 16 MB

After (TaggedValue):
- Int: 8 bytes (stored in pointer!)
- Float: 8 bytes
- Bool: 8 bytes
- Total for 1M integers: 8 MB (50% reduction)
```

### CPU Performance
```
Before:
if let Value::Int(n) = value {    // Enum discriminant check
    result = n + 5;                // Then access value
}
// ~5-10 CPU cycles

After:
if value.is_small_int() {          // Single bit test
    result = value.0 + 5;          // Direct access
}
// ~2-3 CPU cycles (2-3x faster!)
```

### Cache Performance
- Smaller values = better cache locality
- Less pointer chasing
- More values fit in L1/L2 cache

## 🔧 Migration Strategy

### Phase 1: Add TaggedValue alongside Value (1 week)
- Implement TaggedValue struct
- Add conversion functions
- Test thoroughly

### Phase 2: Update VM operations (1 week)
- Update FastInt operations to use TaggedValue
- Update LoadFast/StoreFast
- Update arithmetic operations

### Phase 3: Replace Value in registers (3-4 days)
- Change Frame registers from Value to TaggedValue
- Update all register operations

### Phase 4: Gradually replace Value throughout (1 week)
- Update function calls
- Update classes
- Update collections

## 🧪 Testing Strategy

### Unit Tests
```rust
#[test]
fn test_small_int_encoding() {
    let v = TaggedValue::new_int(42);
    assert!(v.is_small_int());
    assert_eq!(v.as_int(), Some(42));
}

#[test]
fn test_float_encoding() {
    let v = TaggedValue::new_float(3.14);
    assert!(v.is_float());
    assert_eq!(v.as_float(), Some(3.14));
}

#[test]
fn test_fast_arithmetic() {
    let a = TaggedValue::new_int(10);
    let b = TaggedValue::new_int(32);
    let c = a.add(&b).unwrap();
    assert_eq!(c.as_int(), Some(42));
}
```

### Benchmarks
```python
# Simple integer loop
def bench_tagged_ints():
    total = 0
    for i in range(10000000):
        total = total + i
    return total

# Expected improvement: 2-3x faster
```

## ⚠️ Challenges & Solutions

### Challenge 1: Pointer Alignment
**Problem:** Heap pointers need to be aligned to use lower bits for tags
**Solution:** Use Box with custom allocator that guarantees 16-byte alignment

### Challenge 2: NaN Handling
**Problem:** Some floats are NaN and collide with our tag space
**Solution:** Detect NaN floats and allocate them on heap instead

### Challenge 3: Large Integers
**Problem:** Integers > 47 bits don't fit in tagged pointer
**Solution:** Allocate on heap with explicit tag

### Challenge 4: Garbage Collection
**Problem:** Need to track heap-allocated objects
**Solution:** Reference counting in HeapHeader (like current Rc<>)

## 📊 Expected Results

### Before Tagged Pointers
```
Arithmetic: 9.95s
Loops:      2.72s
Functions:  6.21s
```

### After Tagged Pointers (Projected)
```
Arithmetic: 3.3-5.0s  (2-3x faster) ✓
Loops:      0.9-1.4s  (2-3x faster) ✓
Functions:  2.1-3.1s  (2-3x faster) ✓
```

### vs Python (Projected)
```
Arithmetic: 0.42s vs 3.3-5.0s   (8-12x slower, was 24x)
Loops:      0.09s vs 0.9-1.4s   (10-16x slower, was 30x)
Functions:  0.1s vs 2.1-3.1s    (21-31x slower, was 62x)
```

**Major improvement towards Python-competitive performance!**

## 🎯 Success Criteria

1. **Memory:** 50% reduction in memory usage for integer-heavy workloads
2. **Speed:** 2-3x improvement on arithmetic benchmarks
3. **Compatibility:** All existing tests pass
4. **Code quality:** Clean, well-documented implementation

## 📚 References

- [LuaJIT NaN-Tagging](https://lua-users.org/lists/lua-l/2009-11/msg00089.html)
- [V8 Pointer Compression](https://v8.dev/blog/pointer-compression)
- [JavaScriptCore Value Representation](https://webkit.org/blog/10308/javascriptcore-value-representation/)
- [SpiderMonkey NunBox](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/Internals)

---

**Next Step:** Implement TaggedValue struct and basic operations
**Timeline:** 2-3 weeks for complete implementation
**Expected Gain:** 2-3x performance improvement!
