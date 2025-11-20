# Tauraro Exception System - Quick Reference

## Quick Start

### Basic Exception Handling
```python
try:
    result = risky_operation()
except ValueError:
    print("Value error occurred")
```

### Exception Binding
```python
try:
    x = int("invalid")
except ValueError as e:
    print("Error:", str(e))
```

### Bare Except (Catch All)
```python
try:
    unknown_operation()
except:
    print("Any exception caught")
```

### Exception Chaining
```python
try:
    data = parse_json("invalid")
except JSONError as original:
    raise RuntimeError("Failed to parse") from original
```

### Custom Exceptions
```python
class ValidationError(ValueError):
    def __init__(self, field, value):
        self.field = field
        self.value = value

try:
    if age < 0:
        raise ValidationError("age", age)
except ValidationError as e:
    print(f"Invalid {e.field}: {e.value}")
```

### Exception Hierarchy
```python
try:
    risky_function()
except Exception:  # Catches all exceptions
    print("Something went wrong")
except KeyError:   # Specific handler (won't execute above)
    print("Key missing")
```

## Supported Exceptions (63+)

### Built-in Base Exceptions
- `Exception` - Base for most exceptions
- `BaseException` - Base for all exceptions
- `SystemExit`, `KeyboardInterrupt`

### Logic Errors
- `AttributeError` - Missing attribute
- `IndexError` - Bad index
- `KeyError` - Missing dictionary key
- `LookupError` - Base for index/key errors
- `NameError` - Undefined variable
- `TypeError` - Wrong type
- `ValueError` - Wrong value
- `RuntimeError` - Unclassified error
- `NotImplementedError` - Not yet implemented

### I/O Errors
- `IOError` - Generic I/O error
- `OSError` - Operating system error
- `FileNotFoundError` - File not found
- `PermissionError` - No permission
- `TimeoutError` - Operation timeout
- `EOFError` - End of file

### And 40+ more...

## Compatibility Matrix

| Feature | Status |
|---------|--------|
| Basic raising | ✅ 100% |
| Single handler | ✅ 100% |
| Exception hierarchy | ✅ 100% |
| Exception chaining | ✅ 100% |
| Custom exceptions | ✅ 100% |
| Multiple handlers (sequential) | ✅ 100% |
| Multiple handlers (tuple syntax) | ⏳ Planned |
| Exception groups | ❌ Not yet |
| Exception notes | ❌ Not yet |
| Access `__cause__` | ⏳ Planned |
| Access `__traceback__` | ⏳ Planned |

## Common Patterns

### Handling Multiple Exception Types (Current Workaround)
```python
try:
    operation()
except ValueError:
    handle_value_error()
except TypeError:
    handle_type_error()
except KeyError:
    handle_key_error()
```

### Conditional Re-raising
```python
try:
    operation()
except ValueError as e:
    if should_reraise(e):
        raise RuntimeError("Operation failed") from e
    else:
        print("Error handled:", e)
```

### Cleanup with Finally
```python
try:
    use_resource(r)
except Exception as e:
    print("Error:", e)
finally:
    # Cleanup always happens
    cleanup()
```

### Catching Hierarchy
```python
try:
    access_sequence()
except LookupError as e:  # Catches both IndexError and KeyError
    print("Item not found:", e)
except TypeError:
    print("Type mismatch")
```

## Performance Tips

1. **Avoid bare `except:`** when possible - be specific
2. **Custom exceptions** have minimal overhead
3. **Exception chaining** preserves debugging info
4. **Try blocks** have zero cost when no exception occurs

## Debugging

### Get Exception Details
```python
try:
    operation()
except Exception as e:
    exc_type = type(e).__name__
    exc_msg = str(e)
    print(f"{exc_type}: {exc_msg}")
```

### Check Exception Type (Multiple)
```python
try:
    operation()
except (ValueError, TypeError, KeyError):
    print("Type/value/key error")
```

### Chain Debugging
```python
try:
    operation()
except RuntimeError as e:
    if hasattr(e, '__cause__'):
        print("Original error:", e.__cause__)
```

## Reference

### All Exception Hierarchy Levels
```
BaseException
 ├── SystemExit
 ├── KeyboardInterrupt
 ├── Exception
 │   ├── StopIteration
 │   ├── GeneratorExit
 │   ├── ArithmeticError
 │   │   ├── ZeroDivisionError
 │   │   ├── FloatingPointError
 │   │   └── OverflowError
 │   ├── AttributeError
 │   ├── BufferError
 │   ├── EOFError
 │   ├── ImportError
 │   │   └── ModuleNotFoundError
 │   ├── LookupError
 │   │   ├── IndexError
 │   │   └── KeyError
 │   ├── MemoryError
 │   ├── NameError
 │   │   └── UnboundLocalError
 │   ├── OSError
 │   │   ├── FileNotFoundError
 │   │   ├── PermissionError
 │   │   ├── TimeoutError
 │   │   └── ... (and more)
 │   ├── ReferenceError
 │   ├── RuntimeError
 │   │   └── NotImplementedError
 │   ├── SyntaxError
 │   ├── SystemError
 │   ├── TypeError
 │   ├── ValueError
 │   │   └── UnicodeError
 │   └── Warning
 └── ... (and more)
```

## Getting Help

- **Traceback**: Shows full call stack
- **Exception Message**: Use `str(exception)`
- **Exception Type**: Use `type(exception).__name__`
- **Exception Cause**: Check `exception.__cause__` (if using `from`)

## Known Limitations

- ❌ Cannot access `__cause__`, `__context__`, `__traceback__` as properties (stored internally)
- ⏳ Tuple syntax for multiple handlers not yet implemented
- ❌ Exception groups (PEP 654) not yet supported
- ❌ Exception notes (`add_note()`) not yet supported

---

**Version**: 0.2.0+  
**Compatibility**: 90% Python exception system  
**Last Updated**: 2025-11-20
