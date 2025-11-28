# Reserved Keywords

Tauraro has a set of reserved keywords that cannot be used as variable names, function names, class names, or any other identifiers. Tauraro is a multilingual language that supports both **English** and **Hausa** keywords.

## Control Flow Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `if` | `idan` | Conditional statement |
| `elif` | `kokuma idan` | Else-if branch |
| `else` | `akasi` | Else branch |
| `for` | `duk` | For loop |
| `while` | `yayinda` | While loop |
| `break` | `tsaya` | Break out of loop |
| `continue` | `cigaba` | Continue to next iteration |
| `pass` | `wuce` | No-operation placeholder |
| `match` | `daidaita` | Pattern matching |
| `case` | `yanayi` | Match case |

## Function & Class Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `func` / `def` | `aiki` | Function definition |
| `class` | `iri` | Class definition |
| `return` | `maido` | Return from function |
| `yield` | `bayar` | Generator yield |
| `lambda` | `dan_aiki` | Anonymous function |

## Module Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `import` | `shigoda` | Import module |
| `from` | `daga` | Import from module |
| `as` | `dasunan` | Alias for import |
| `export` | `fitar` | Export symbol |
| `extern` | `waje` | External declaration |

## Exception Handling Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `try` | `gwada` | Try block |
| `except` / `catch` | `kama` | Exception handler |
| `finally` | `karshe` | Finally block |
| `raise` / `throw` | `jefa` | Raise exception |
| `assert` | `tabbatar` | Assertion |

## Async Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `async` | `marasa_jira` | Async function |
| `await` | `jira` | Await async result |

## Scope Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `global` | `duniya` | Global variable |
| `nonlocal` | `ba_gida` | Nonlocal variable |
| `del` | `share` | Delete variable |

## Other Keywords

| English | Hausa | Description |
|---------|-------|-------------|
| `with` | `tare` | Context manager |
| `in` | `acikin` | Membership test / for-in |
| `is` | `shine` | Identity test |

## Logical Operators (Keywords)

| English | Hausa | Description |
|---------|-------|-------------|
| `and` | `dakuma` | Logical AND |
| `or` | `ko` | Logical OR |
| `not` | `ba` | Logical NOT |

## Boolean & Null Constants

| English | Hausa | Description |
|---------|-------|-------------|
| `True` / `true` | `gaskiyane` | Boolean true |
| `False` / `false` | `karyane` | Boolean false |
| `None` / `none` / `null` | `babu` | Null value |

## Complete Keyword List

Here is the complete list of all reserved keywords in Tauraro:

### English Keywords
```
func, def, class, if, elif, else, for, while, break, continue, return,
pass, match, case, import, from, as, export, extern, try, except, catch,
finally, raise, throw, assert, async, await, global, nonlocal, del, with,
yield, lambda, in, is, and, or, not, True, true, False, false, None, none, null
```

### Hausa Keywords
```
aiki, iri, idan, kokuma idan, akasi, duk, yayinda, tsaya, cigaba, maido,
wuce, daidaita, yanayi, shigoda, daga, dasunan, fitar, waje, gwada, kama,
karshe, jefa, tabbatar, marasa_jira, jira, duniya, ba_gida, share, tare,
bayar, dan_aiki, acikin, shine, dakuma, ko, ba, gaskiyane, karyane, babu
```

## Usage Restrictions

Reserved keywords **cannot** be used as:

1. **Variable names**
   ```python
   # ❌ Invalid
   if = 10
   class = "hello"
   
   # ✅ Valid
   if_condition = 10
   class_name = "hello"
   ```

2. **Function names**
   ```python
   # ❌ Invalid
   def return():
       pass
   
   # ✅ Valid
   def get_return_value():
       pass
   ```

3. **Class names**
   ```python
   # ❌ Invalid
   class for:
       pass
   
   # ✅ Valid
   class ForLoop:
       pass
   ```

4. **Parameter names**
   ```python
   # ❌ Invalid
   def foo(while, break):
       pass
   
   # ✅ Valid
   def foo(while_condition, break_flag):
       pass
   ```

5. **Module aliases**
   ```python
   # ❌ Invalid
   import math as import
   
   # ✅ Valid
   import math as m
   ```

## Error Messages

When you try to use a reserved keyword as an identifier, Tauraro will report an error:

```
Error: 'if' is a reserved keyword and cannot be used as an identifier
  --> file.tr:5:1
   |
 5 | if = 10
   | ^^ reserved keyword
```

## See Also

- [Variables](variables.md) - Variable declarations and naming
- [Functions](functions.md) - Function definitions
- [Classes](classes.md) - Class definitions
- [Control Flow](control-flow.md) - Using control flow keywords
