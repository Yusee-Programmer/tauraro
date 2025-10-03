# Built-in List Implementation in Tauraro

## Overview

Tauraro implements lists as a built-in data type that works exactly like Python lists but with enhanced performance. The list type is available globally without requiring any imports, just like in Python.

## Implementation Details

The list implementation in Tauraro is based on a high-performance wrapper around Rust's `Vec<Value>` called `HPList`. This implementation provides:

1. **Full Python Compatibility**: All Python list methods and operations are supported
2. **High Performance**: Uses Rust's efficient memory management and data structures
3. **Memory Efficiency**: Contiguous memory allocation for optimal cache performance
4. **Python-style Features**: Negative indexing, slicing, list comprehensions (if supported)

## Usage

### Basic Usage

```python
# Create an empty list
my_list = list()

# Create a list with initial values
numbers = [1, 2, 3, 4, 5]

# Add elements
my_list.append("Hello")
my_list.append("World")

# Access elements
first = my_list[0]
last = my_list[-1]

# List methods
my_list.insert(1, "Beautiful")
my_list.remove("Beautiful")
popped = my_list.pop()
```

### Supported Operations

- **Indexing**: `list[index]` with negative indexing support
- **Slicing**: `list[start:stop:step]` (if implemented in the parser)
- **Methods**: 
  - `append(item)` - Add item to end
  - `extend(iterable)` - Add all items from iterable
  - `insert(index, item)` - Insert item at index
  - `remove(item)` - Remove first occurrence of item
  - `pop([index])` - Remove and return item at index (default last)
  - `index(item)` - Find index of first occurrence
  - `count(item)` - Count occurrences of item
  - `sort()` - Sort list in place
  - `reverse()` - Reverse list in place
  - `copy()` - Create shallow copy
  - `clear()` - Remove all items

### Performance Characteristics

- **Append**: O(1) amortized
- **Insert/Remove**: O(n) worst case
- **Indexing**: O(1)
- **Sorting**: O(n log n)
- **Memory**: Contiguous allocation for cache efficiency

## Technical Implementation

The `HPList` struct wraps a `Vec<Value>` and provides Python-compatible methods:

```rust
pub struct HPList {
    data: Vec<Value>,
}
```

All list operations are implemented as methods on this struct, with proper error handling and Python semantics.

## Integration with Tauraro

The list type is integrated as a built-in data type through:

1. **Value enum**: `Value::List(HPList)` variant
2. **Built-in functions**: `builtin_list()` function in `builtins.rs`
3. **Global availability**: No imports required

This ensures that lists are available everywhere in Tauraro code, just like in Python.