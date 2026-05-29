# 19 — Compiler Error Reference

Every error the Tauraro compiler emits includes a rule code `[X-N]`. This page documents every rule, its cause, an example that triggers it, and the fix.

---

## Type Rules (T-series)

### [T-1] No Implicit Type Coercion

**Message:** `cannot assign int to float without explicit cast`

**Cause:** You assigned or passed a value of one numeric type where a different numeric type was expected, without an explicit `as` cast.

```python
# WRONG:
mut x: int = 10
mut y: float = x         # T-1: cannot assign int to float

def set_float(f: float): ...
set_float(x)              # T-1: argument type mismatch
```

**Fix:**
```python
mut y: float = x as float
set_float(x as float)
```

**Why:** Implicit coercion silently loses precision (int→float for large values) or truncates (float→int). Every conversion is an intentional decision — make it explicit.

---

### [T-2] Type Mismatch

**Message:** `cannot add int and float` / `expected int, got str`

**Cause:** An operator or function received operands/arguments of incompatible types.

```python
# WRONG:
mut a: int = 5
mut b: float = 3.0
mut c = a + b             # T-2: cannot add int and float

mut items: List[int] = [1, 2, "three"]    # T-2: expected int, got str
```

**Fix:**
```python
mut c = a as float + b
mut items: List[int] = [1, 2, 3]
```

---

### [T-3] Return Type Mismatch

**Message:** `function 'f' declares return type 'int' but returns 'str'`

**Cause:** A `return` statement's value doesn't match the function's declared return type.

```python
def get_count() -> int:
    return "42"          # T-3: returning str but declared int
```

**Fix:**
```python
def get_count() -> int:
    return 42            # or: return int("42")
```

---

### [T-4] Unhandled Result from throws Function

**Message:** `'parse_int()' returns a Result and its error must be handled`

**Cause:** A `throws` function was called as a statement with no assignment and no `?`.

```python
parse_int(s)    # T-4: result discarded
```

**Fix:** One of three options:
```python
parse_int(s)?             # propagate with ?
mut r = parse_int(s)      # assign to variable
_ = parse_int(s)          # explicitly discard
```

---

### [T-5] Cannot Infer Type

**Message:** `cannot infer type of 'x' without initializer`

**Cause:** A variable was declared without a type annotation and without an initializer.

```python
mut x    # T-5: no type, no initial value
```

**Fix:**
```python
mut x: int = 0
mut x = 0           # inferred from value
```

---

## Memory Rules (M-series)

### [M-2] Use After Move

**Message:** `'data' was moved into 'send' and is no longer valid`

**Cause:** A variable was passed to a function that consumes it (takes ownership), then used again afterward.

```python
data = load_bytes()
send(data)          # data moved
print(len(data))    # M-2: data is no longer valid
```

**Fix:**
```python
print(len(data))    # use before moving
send(data)
```

---

### [M-4] Dangling Pointer / Lifetime Error

**Message:** `returning pointer to local variable 'p' which will be freed`

**Cause:** Returning a reference (pointer) to a local variable whose lifetime ends when the function returns.

```python
def get_ref() -> Point:
    mut p = Point.init(1, 2)
    return &p               # M-4: p freed at function exit
```

**Fix:**
```python
def get_ref() -> Point:
    return Point.init(1, 2)   # return by value — transfers ownership
```

---

### [M-5] Aliased Mutation

**Message:** `cannot reassign 'list' while 'view' borrows it`

**Cause:** Attempting to mutate or reassign a variable while a borrow of it is active.

```python
mut list = [1, 2, 3]
view = list
list = [4, 5, 6]    # M-5: view still borrows list
```

**Fix:** End the borrow before reassigning — use `view` completely before reassigning `list`.

---

### [M-6] Cannot Assign to Immutable Binding

**Message:** `cannot assign to immutable binding 'x'`

**Cause:** Assigning to a variable declared without `mut`.

```python
x = 10
x = 20    # M-6: x is immutable
```

**Fix:** Add `mut`: `mut x = 10`

---

### [M-7] None Assigned to Non-Optional Type

**Message:** `cannot assign 'none' to 'x' which has type 'int'`

**Cause:** `none` was assigned to a variable whose type cannot hold a null value.

```python
mut x: int = none    # M-7: int cannot be none
```

**Fix:**
```python
mut x: Option[int] = none    # OK: Option can hold none
mut x: int = 0               # OK: use zero as the "not set" value
```

---

## Function Rules (F-series)

### [F-1] Missing Parameter Type

**Message:** `parameter 'x' has no type annotation`

**Cause:** A function parameter was declared without a type.

```python
def add(a, b):    # F-1: a and b have no types
    return a + b
```

**Fix:**
```python
def add(a: int, b: int) -> int:
    return a + b
```

---

### [F-2] Parameter Name Shadowed

**Message:** `parameter 'n' is shadowed by local variable`

**Cause:** A local variable inside the function has the same name as a parameter.

```python
def double(n: int) -> int:
    mut n = n * 2    # F-2: re-declaring parameter 'n'
    return n
```

**Fix:** Use a different name:
```python
def double(n: int) -> int:
    mut result = n * 2
    return result
```

---

### [F-3] Missing Return on Code Path

**Message:** `Function 'f' returns 'int' but is missing a return statement on at least one code path`

**Cause:** A non-void function doesn't have a `return` on every possible execution path.

```python
def sign(n: int) -> int:
    if n > 0: return 1
    if n < 0: return -1
    # F-3: n == 0 path has no return
```

**Fix:**
```python
def sign(n: int) -> int:
    if n > 0:   return 1
    elif n < 0: return -1
    else:       return 0
```

Or add a final return:
```python
def sign(n: int) -> int:
    if n > 0: return 1
    if n < 0: return -1
    return 0    # catch-all
```

**Note:** F-3 is not checked for:
- `void` functions (return is optional)
- Functions named `init` or `new` (constructors)
- Interface method signatures (no body)
- `extern "C"` functions (no body)

---

## Name Rules (N-series)

### [N-1] Reserved Name Used as Function Name

**Message:** `'int' is a keyword and cannot be used as a function name`

**Cause:** A function was named with a language keyword or built-in type name.

```python
def int(x: str) -> int:    # N-1: 'int' is a reserved keyword
    return 0
```

**Fix:** Choose a non-reserved name:
```python
def to_int(x: str) -> int:
    return int(x)
```

---

## Parse Errors

Parse errors don't have rule codes — they are reported directly with the source location.

### Unexpected Indentation

```
ParseError at line 5: unexpected indentation
```

**Cause:** Inconsistent indentation (mixing tabs and spaces, or wrong depth).

```python
def foo():
    x = 1
  y = 2    # WRONG: 2 spaces instead of 4
```

**Fix:** Use exactly 4 spaces per level. No tabs.

### Unexpected Token

```
ParseError at line 8: unexpected token '='
```

**Cause:** Often a syntax error in an expression, or a keyword used in the wrong context.

### Missing Colon

```
ParseError at line 3: expected ':' after 'if' condition
```

**Fix:** Add `:` at the end of `if`, `while`, `for`, `def`, `class`, `extend` headers.

---

## Linker / C Compiler Errors

These appear after the Tauraro compiler succeeds but the C compiler (GCC/Clang) fails.

### undefined reference to `function_name`

**Cause:** A function declared in `extern "C"` is not in the linked libraries.

```
undefined reference to `curl_easy_init'
```

**Fix:** Add the library: `tauraroc main.tr -l curl --run`

### conflicting types for 'name'

**Cause:** Two declarations of the same C symbol with different types — usually from the compat block and the runtime header.

**Fix:** This typically indicates a mismatch between the Tauraro type annotation in `extern "C"` and the actual C function signature. Fix the `extern "C"` declaration to match exactly.

### assignment from incompatible pointer type

**Cause:** Assigning a `List_ptr*` (generic/unknown element type) to a `List_i64*` (typed list). See example 13 type mapping.

**Fix:** Ensure empty list literals `[]` are annotated with the correct type: `mut data: List[int] = []`

---

## Runtime Errors (Abort/Crash)

These don't produce compile errors — they crash at runtime.

### Null Pointer Dereference

**Symptom:** Segmentation fault or access violation.

**Cause:** Accessing a field or method through a null pointer.

```python
mut p: Point = none
p.x = 10    # CRASH: p is null
```

**Fix:** Always initialize before use, or check for null:
```python
if p as usize != 0 as usize:
    p.x = 10
```

### Out-of-Bounds List Access

**Symptom:** Crash or incorrect value.

**Cause:** Accessing `list[i]` where `i >= len(list)`.

**Fix:** Bounds-check manually: `if i >= 0 and i < len(list): ...`

### Stack Overflow

**Symptom:** Crash with a cryptic error on deep recursion.

**Cause:** Unbounded recursion, or very deep call stacks.

**Fix:** Convert recursive algorithms to iterative, or increase stack size via the OS.

---

## Diagnostic Tips

### Use --check for fast feedback

```bash
tauraroc --check program.tr    # type-checks without generating C
```

### Use --emit ast to see the parse tree

```bash
tauraroc --emit ast program.tr    # shows what the parser understood
```

### Use --emit c to inspect generated C

```bash
tauraroc --emit c program.tr    # see the C before GCC runs
```

If the compiler error is "from GCC", looking at the generated C and searching for the reported line number often reveals the type mismatch or undefined symbol instantly.

### Use --verbose for the full pipeline

```bash
tauraroc --verbose program.tr    # shows each phase: lex → parse → sema → codegen
```

---

Next: [Advanced Patterns →](20_advanced_patterns.md)
