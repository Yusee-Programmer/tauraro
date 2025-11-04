# Critical Bugs Found in Tauraro Class System

## Summary
During the implementation of DUITK (Desktop UI Toolkit), I discovered several critical bugs in Tauraro's class system that prevent proper object-oriented programming.

## Bug 1: Class Instantiation from Class Methods Returns None

**Description**: When creating an instance of a class from within another class's method, the constructor returns `None` instead of the instance.

**Test Case**:
```python
class Inner:
    def __init__(self, value):
        print(f"Inner.__init__: value={value}")
        self.value = value
        print(f"Exiting Inner.__init__")

class Outer:
    def __init__(self):
        self.inners = []

    def create_inner(self, value):
        inner = Inner(value)  # <-- This returns None!
        return inner

outer = Outer()
inner1 = outer.create_inner(42)
print(inner1)  # Prints: None
```

**Expected**: `inner1` should be an `<Inner object>`
**Actual**: `inner1` is `None`

**Impact**: Cannot create instances of one class from methods of another class. This breaks the factory pattern and makes complex OOP designs impossible.

**Test File**: `test_nested_class.py`

## Bug 2: Bitwise OR Operations Return None

**Description**: Bitwise OR operations using the `|` operator return `None` instead of the computed value.

**Test Case**:
```python
window_style = 0x10000000 | 0x00CF0000
print(window_style)  # Prints: None
```

**Expected**: `window_style` should be `282001408` (0x10CF0000)
**Actual**: `window_style` is `None`

**Workaround**: Pre-calculate the value and use the literal:
```python
window_style = 282001408  # Works fine
```

**Impact**: Cannot use bitwise operations for creating flag combinations (common in Win32 API programming).

## Bug 3: Parameter Passing from Class Methods to Module Functions

**Description**: When calling a module-level function from within a class method, all parameters are received as `None`.

**Test Case**:
```python
def module_function(a, b, c):
    print(f"Received: a={a}, b={b}, c={c}")
    return a + b + c

class MyClass:
    def call_it(self):
        result = module_function(1, 2, 3)  # <-- All params become None!
        return result

obj = MyClass()
obj.call_it()  # Prints: "Received: a=None, b=None, c=None"
```

**Expected**: Function should receive `a=1, b=2, c=3`
**Actual**: Function receives `a=None, b=None, c=None`

**Impact**: Cannot call helper functions from class methods. Makes code modularization impossible.

## Workarounds Attempted

1. **Factory Functions**: Didn't work due to Bug #3 (parameter passing)
2. **Manual Instance Creation**: Can't work around Bug #1
3. **Inline Value Computation**: Works for Bug #2 (use pre-computed literals)

## Working Examples

These patterns DO work in Tauraro:

1. **Simple Class Instantiation (at module level)**:
```python
class MyClass:
    def __init__(self, value):
        self.value = value

obj = MyClass(42)  # Works fine
print(obj.value)  # 42
```

2. **Direct FFI Calls (no classes)**:
```python
# The demo_native_window.py approach works perfectly
load_library("user32.dll")
define_function("user32.dll", "CreateWindowExA", "pointer", [...])
hwnd = call_function("user32.dll", "CreateWindowExA", [...])  # Works!
```

## Impact on DUITK

These bugs make it impossible to implement DUITK as a class-based GUI framework. The current implementation:

- ✅ FFI system works perfectly
- ✅ Win32 API bindings work
- ✅ Direct window creation works (see `demo_native_window.py`)
- ❌ Cannot use Application/Window classes
- ❌ Cannot create windows from Application.create_window()
- ❌ Cannot use bitwise OR for window styles

## Recommendations

1. **Short Term**: Use the functional approach (like `demo_native_window.py`) instead of classes
2. **Medium Term**: File bug reports for all three issues with Tauraro team
3. **Long Term**: Once fixed, re-implement DUITK with proper OOP design

## Test Files

- `test_class_bug.py` - Shows simple classes work
- `test_nested_class.py` - Demonstrates Bug #1
- `test_duitk_minimal.py` - Minimal DUITK test
- `demo_native_window.py` - Working functional approach ✅

## Conclusion

The Tauraro FFI system and basic language features work well, but the class system has critical bugs that prevent object-oriented programming patterns. For now, GUI applications must use a functional/procedural approach.

**Status**: Bugs discovered and documented. Awaiting fixes from Tauraro maintainers.
