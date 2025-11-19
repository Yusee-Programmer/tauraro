# Quick Reference: Tauraro Dunder Method Status

## ✅ FULLY WORKING - Custom Objects

```python
class Vector:
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

v1 = Vector(1, 2)
v2 = Vector(3, 4)
v3 = v1 + v2  # WORKS ✅
print(v1 == v2)  # WORKS ✅
```

## 🔄 WORKING - Built-in Functions

```python
s = "hello"
print(len(s))      # WORKS ✅ - uses internal len()
print(s.upper())   # WORKS ✅ - direct method call
print([1,2,3].append(4))  # WORKS ✅ - direct method call
```

## ❌ NOT WORKING - Built-in Dunder Methods

```python
s = "hello"
print(s.__len__())  # FAILS ❌ - Hangs/crashes
print([1,2,3].__len__())  # FAILS ❌
```

## Why The Gap Exists

Tauraro has TWO method systems:

### System 1: Custom Objects (COMPLETE)
```rust
// In class_methods HashMap
"__add__" => Some(Closure {...})
// Called via: execute_closure_sync() with self as first arg
```

### System 2: Built-in Types (INCOMPLETE)
```rust
// In VM match statement
"upper" => Value::Str(s_clone.to_uppercase())
"__len__" => ???  // No self parameter, hangs
```

## What Needs to Happen

The built-in type dunders need to:
1. Accept `self` as implicit first argument
2. Not expect it in the explicit args Vec
3. Handle the implicit self correctly

## Files With the Issue

- `src/bytecode/vm.rs` line 8232+ (String __len__)
- `src/bytecode/vm.rs` line 7925+ (List __len__)
- Similar patterns needed for Dict, Tuple, Set

## Quick Test to Verify Fix

```tauraro
# This should work after fix
s = "hello"
len_value = s.__len__()
print(len_value)  # Should print 5
```

## Priority: **HIGH**
This is the last missing piece for 100% Python dunder compatibility.
