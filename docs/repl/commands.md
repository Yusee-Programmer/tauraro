# REPL Special Commands

The Tauraro REPL includes special commands for enhanced productivity.

## Built-in Commands

### help()

Get help on objects:

```python
>>> help(math.sqrt)
# Shows documentation for sqrt function

>>> help(list)
# Shows documentation for list class
```

### dir()

List attributes and methods:

```python
>>> import math
>>> dir(math)
# Shows all math module functions

>>> dir([])
# Shows all list methods
```

### type()

Check object type:

```python
>>> type(42)
<class 'int'>

>>> type("hello")
<class 'str'>

>>> type([1, 2, 3])
<class 'list'>
```

### exit() / quit()

Exit the REPL:

```python
>>> exit()
# Or use Ctrl+D
```

## Introspection

```python
>>> x = [1, 2, 3]
>>> dir(x)  # See available methods
>>> help(x.append)  # Get help on specific method
>>> x.__class__  # Get class
>>> isinstance(x, list)  # Check type
True
```

## Tips

- Use `_` to reference the last result
- Use tab completion (if available)
- Multi-line code uses `...` continuation prompt
- Ctrl+C to cancel current input
- Ctrl+D to exit

## Next Steps

- [Interactive Mode](interactive.md) - Using the REPL
- [REPL Features](features.md) - Advanced features
