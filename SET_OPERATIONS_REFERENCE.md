# Set Operations Implementation Reference

## Value Enum Definition
```rust
pub enum Value {
    Set(Vec<Value>),  // Sets stored as vectors of Values
    // ... other variants
}
```

## Set Bitwise Operations

### Bitwise AND (`&`) - Set Intersection
**Operation**: `s1 & s2` returns elements in both sets
```rust
(Value::Set(a), Value::Set(b)) => {
    let result: Vec<Value> = a.iter()
        .filter(|item| b.contains(item))
        .cloned()
        .collect();
    Ok(Value::Set(result))
}
```

**Semantics**: Returns only items present in both `a` and `b`

### Bitwise OR (`|`) - Set Union
**Operation**: `s1 | s2` returns all elements from both sets (no duplicates)
```rust
(Value::Set(a), Value::Set(b)) => {
    let mut result = a.clone();
    for item in b.iter() {
        if !result.contains(item) {
            result.push(item.clone());
        }
    }
    Ok(Value::Set(result))
}
```

**Semantics**: 
- Starts with all items from `a`
- Adds items from `b` that aren't already in result
- Maintains no duplicate invariant

## Integer/Boolean Bitwise Operations
Both operations also support:
- `Int & Int` → `Int`
- `Bool & Bool` → `Bool`
- `Int & Bool` → `Int` (bool cast to i64)
- `Bool & Int` → `Int` (bool cast to i64)

Same pattern applies for OR operations.

## File Location
`src/bytecode/arithmetic.rs` - Lines containing `bitand_values` and `bitor_values` functions

## Compilation Check
All set operations verified to compile without errors. Test with:
```bash
cargo check --lib
cargo build --lib
```
