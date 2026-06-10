# 19 — Compiler Error Reference

Every error the Tauraro compiler emits includes a rule code `[X-N]`. This page documents every rule, organized by category, with its cause, a triggering example, and the fix.

---

## Quick Reference

| Code | Category | Short Description |
|------|----------|-------------------|
| [M-1] | Memory | Heap allocation not tracked |
| [M-2] | Memory | Use after move |
| [M-3] | Memory | Double free |
| [M-4] | Memory | Dangling pointer (borrow escapes) |
| [M-5] | Memory | Null dereference |
| [M-6] | Memory | Assign to immutable binding |
| [M-7] | Memory | Assign `none` to value type |
| [T-1] | Type | No implicit coercion — use `as` |
| [T-2] | Type | Type mismatch in operation |
| [T-3] | Type | Wrong argument type |
| [T-4] | Type | Return type mismatch / unhandled Result |
| [T-5] | Type | Cannot infer type |
| [N-1] | Name | Reserved name used as declaration |
| [F-1] | Function | Wrong number of arguments |
| [F-2] | Function | Parameter shadowed / undefined call |
| [F-3] | Function | Missing return on code path |
| [E-1] | Existence | Method does not exist on receiver's type |
| [E-2] | Existence | Nested declaration used outside `main()` |
| [U-1] | Unsafe | `alloc`/`dealloc` outside `unsafe:` (with `--strict`) |

---

## Memory Rules (M-series)

### [M-1] Heap Allocation Not Tracked

**Message:** `heap allocation in 'x' must be tracked — assign to a named binding`

**Cause:** An `alloc` result was used inline without being bound to a named variable, so the compiler cannot track the allocation's lifetime.

**When to use `unsafe:`:** All manual `alloc`/`dealloc` calls require an `unsafe:` block. This error fires when the compiler detects an allocation that it cannot pair with a corresponding free.

**How it works:**
```python
# WRONG:
unsafe:
    process(alloc[char](256))   # M-1: allocation not tracked

# RIGHT:
unsafe:
    mut buf: Pointer[char] = alloc[char](256)
    process(buf)
    dealloc(buf)
```

**Common Mistakes:**
- Passing `alloc(...)` directly as a function argument
- Allocating inside a loop without a binding, making it impossible for the compiler to detect the matching `dealloc`

**Best Practices:** Always bind allocations to a named variable immediately. Use `try/finally` to guarantee `dealloc` is reached even on error paths.

---

### [M-2] Use After Move

**Message:** `'data' was moved into 'send' and is no longer valid`

**Cause:** A variable was passed to a function that takes ownership (moves), then used again afterward.

**How it works:**
```python
# WRONG:
data = load_bytes()
send(data)           # data is moved here
print(len(data))     # M-2: data is no longer valid

# RIGHT: use before moving
print(len(data))
send(data)
```

**Common Mistakes:**
- Passing the same owned value to two different functions
- Storing an owned value in a container then continuing to use the original binding

**Best Practices:** Use `Shared[T]` when you need multiple owners. Use `clone(val)` for an explicit deep copy before the move.

---

### [M-3] Double Free

**Message:** `'buf' was already freed`

**Cause:** `dealloc` was called on a pointer that was already freed, or on a pointer that was not allocated by `alloc`.

**How it works:**
```python
# WRONG:
unsafe:
    mut buf: Pointer[char] = alloc[char](128)
    dealloc(buf)
    dealloc(buf)    # M-3: double free

# RIGHT:
unsafe:
    mut buf: Pointer[char] = alloc[char](128)
    dealloc(buf)
    # buf is no longer valid — don't use it
```

**Best Practices:** Structure code so every allocation has exactly one `dealloc`, in a `finally:` block if exceptions are possible.

---

### [M-4] Dangling Pointer / Lifetime Error

**Message:** `returning pointer to 'p' which will be freed when the function exits`

**Cause:** Returning a `Pointer[T]` or borrow that refers to a local variable — the memory becomes invalid when the function returns.

**How it works:**
```python
# WRONG:
def get_ref() -> Pointer[int]:
    mut x = 42
    return &x       # M-4: x freed at function exit

# RIGHT: return by value (transfers ownership)
def get_value() -> int:
    return 42
```

When you genuinely need to return a pointer to data owned by a parameter, use the `from` lifetime annotation:

```python
# Pointer lifetime bounded by caller's 'data'
def get_first(data: List[int]) -> Pointer[int] from data:
    return data.raw_ptr()
```

See [Advanced — Lifetimes](advanced/01_lifetimes.md) for the full `from` annotation system.

**Best Practices:** Prefer returning owned values over pointers. Only use `from` annotations when you have a genuine need for zero-copy access into caller-owned data.

---

### [M-5] Null Dereference

**Message:** `'p' may be null — check before dereferencing`

**Cause:** Accessing a field or calling a method on a variable that the compiler can determine may be `none`/null.

**How it works:**
```python
# WRONG:
mut p: Point = none
p.x = 10    # M-5: p is null

# RIGHT:
mut p: Point = Point.init(0, 0)
p.x = 10

# Or check first:
if p as usize != 0 as usize:
    p.x = 10
```

**Best Practices:** Initialize class variables at declaration. Use `Option[T]` for values that are intentionally nullable, and unwrap with a check.

---

### [M-6] Cannot Assign to Immutable Binding

**Message:** `cannot assign to immutable binding 'x'`

**Cause:** Assigning to a variable declared without `mut`.

**How it works:**
```python
# WRONG:
x = 10
x = 20    # M-6: x is immutable

# RIGHT:
mut x = 10
x = 20    # OK
```

**When to use:** Immutable bindings (`x = value`, no `mut`) are the default. They document intent: this value does not change. Only reach for `mut` when the value will genuinely need reassignment.

---

### [M-7] None Assigned to Non-Optional Type

**Message:** `cannot assign 'none' to 'x' which has type 'int'`

**Cause:** `none` was assigned to a variable whose type is not `Option[T]`.

**How it works:**
```python
# WRONG:
mut x: int = none    # M-7

# RIGHT:
mut x: Option[int] = none    # Option can hold none
mut x: int = 0               # Use zero for "not set" when appropriate
```

**Best Practices:** Use `Option[T]` for values that are genuinely optional. Avoid using `none` as a sentinel in non-optional types.

---

## Type Rules (T-series)

### [T-1] No Implicit Type Coercion

**Message:** `cannot assign int to float without explicit cast`

**Cause:** A value of one numeric type was used where a different numeric type was expected, without an explicit `as` cast.

**How it works:**
```python
# WRONG:
mut x: int = 10
mut y: float = x          # T-1: int → float without cast

def set_float(f: float): ...
set_float(x)              # T-1: wrong argument type

# RIGHT:
mut y: float = x as float
set_float(x as float)
```

**Why:** Implicit coercion silently loses precision (large `int` → `float`) or truncates (`float` → `int`). Every conversion must be an explicit, intentional decision.

**Thread safety:** [T-1] is also emitted when passing a non-`Sendable` type across a thread boundary. See [Advanced — Sendable](advanced/06_sendable.md).

---

### [T-2] Type Mismatch in Operation

**Message:** `cannot add int and float` / `expected int, got str`

**Cause:** An operator or function received operands/arguments of incompatible types.

**How it works:**
```python
# WRONG:
mut a: int = 5
mut b: float = 3.0
mut c = a + b              # T-2: cannot add int and float

mut items: List[int] = [1, 2, "three"]    # T-2: expected int, got str

# RIGHT:
mut c = a as float + b
mut items: List[int] = [1, 2, 3]
```

**Thread safety:** [T-2] is also emitted for detected data race conditions in concurrent code.

---

### [T-3] Wrong Argument Type

**Message:** `argument 1 of 'foo' expects 'int', got 'str'`

**Cause:** A function call passed an argument whose type does not match the parameter's declared type.

**How it works:**
```python
def greet(count: int) -> void:
    print(f"hello {count} times")

# WRONG:
greet("five")    # T-3: expected int, got str

# RIGHT:
greet(5)
```

**Thread safety:** [T-3] is also emitted for potential deadlock situations (holding a lock while awaiting another). See [Advanced — Sendable](advanced/06_sendable.md).

---

### [T-4] Unhandled Result from `throws` Function

**Message:** `'parse_int()' returns a Result and its error must be handled`

**Cause:** A `throws` function was called as a statement with no assignment and no `?` operator, discarding the error silently.

**How it works:**
```python
# WRONG:
parse_int(s)    # T-4: result discarded

# RIGHT (three options):
parse_int(s)?             # propagate with ?
mut r = parse_int(s)      # assign to variable
_ = parse_int(s)          # explicitly discard
```

---

### [T-5] Cannot Infer Type

**Message:** `cannot infer type of 'x' without initializer`

**Cause:** A variable was declared without a type annotation and without an initializer.

**How it works:**
```python
# WRONG:
mut x    # T-5: no type, no value

# RIGHT:
mut x: int = 0
mut x = 0      # type inferred from value
```

---

## Name Rules (N-series)

### [N-1] Reserved Name Used as Declaration

**Message:** `'int' is a keyword and cannot be used as a function name`

**Cause:** A function, class, or variable was given the name of a language keyword or built-in type.

**When to use:** This prevents accidentally shadowing built-in operations. Reserved names include all keywords (`if`, `while`, `def`, `class`, ...) and all built-in type names (`int`, `float`, `str`, `bool`, `char`, `List`, `Dict`, `Option`, `Result`, ...).

**How it works:**
```python
# WRONG:
def int(x: str) -> int:    # N-1: 'int' is reserved
    return 0

# RIGHT:
def to_int(x: str) -> int:
    return int(x)
```

---

## Function Rules (F-series)

### [F-1] Wrong Number of Arguments

**Message:** `'add' expects 2 arguments, got 3`

**Cause:** A function was called with a different number of arguments than it declares parameters.

**How it works:**
```python
def add(a: int, b: int) -> int:
    return a + b

# WRONG:
add(1, 2, 3)    # F-1: too many arguments
add(1)          # F-1: too few arguments

# RIGHT:
add(1, 2)
```

---

### [F-2] Parameter Shadowed / Undefined Function

**Message:** `parameter 'n' is shadowed by local variable` / `'foo' is not defined`

**Cause (shadowing):** A local variable inside the function re-declares a parameter name.

**How it works:**
```python
# WRONG:
def double(n: int) -> int:
    mut n = n * 2    # F-2: re-declaring parameter 'n'
    return n

# RIGHT:
def double(n: int) -> int:
    mut result = n * 2
    return result
```

**Cause (undefined):** Calling a function that has not been declared anywhere visible in the current scope or any imported module.

---

### [F-3] Missing Return on Code Path

**Message:** `function 'sign' returns 'int' but is missing a return on at least one code path`

**Cause:** A non-`void` function has at least one execution path that reaches the end without a `return`.

**How it works:**
```python
# WRONG:
def sign(n: int) -> int:
    if n > 0: return 1
    if n < 0: return -1
    # F-3: the n == 0 path has no return

# RIGHT:
def sign(n: int) -> int:
    if n > 0:   return 1
    elif n < 0: return -1
    else:       return 0
```

**Not checked for:**
- `void` functions (`return` is optional)
- Constructor functions named `init` or `new`
- Interface method signatures (no body)
- `extern "C"` declarations (no body)

---

## Existence Rules (E-series)

### [E-1] No Method Found on Type

**Message:** `No method 'foo' found on type 'Bar'.`

**Cause:** A method call `obj.foo(...)` was made on a value whose class `Bar`
(nor any of its base classes) declares a method named `foo`, and `foo` is not
one of the universal dunder/built-in methods (`init`, `to_str`, `__eq__`, etc.)
or a compiler-dispatched method on a built-in type (`Thread`, `Mutex`, `File`,
`OS`, ...).

**How it works:**
```python
pub class Foo:
    pub x: int

def main():
    mut f = Foo()
    f.nonexistent_method()   # E-1: no such method on 'Foo'
```

**Fix:** Define `pub def nonexistent_method(self, ...)` in `Foo`, or add it via
`extend Foo:` for `Foo` or one of its base classes.

---

### [E-2] Nested Declaration Outside `main()`

**Message:** `Nested class/def/enum/interface declarations are only supported inside main().`

**Cause:** A `class`, `def`, `enum`, `interface`, or `extend` statement appears
inside the body of a function other than `main()`. Local (nested) declarations,
Java-style, are only allowed in `main()` — see
[Local (Nested) Declarations in `main()`](05_functions.md#local-nested-declarations-in-main).

**How it works:**
```python
def helper():
    class Foo:    # E-2: not inside main()
        pub x: int

def main():
    class Foo:    # OK: declared inside main()
        pub x: int
```

**Fix:** Move the declaration to module (top-level) scope, or move the
surrounding logic into `main()`.

---

## Unsafe Rules (U-series)

### [U-1] Manual Memory Outside `unsafe:` Block

**Message:** `'alloc' may only be called inside an 'unsafe:' block (--strict mode)`

**Cause:** `alloc` or `dealloc` was called outside an `unsafe:` block when the compiler was invoked with `--strict`.

**When to use:** The `--strict` flag enables all safety checks. In strict mode, every manual memory operation must be explicitly wrapped in `unsafe:` to signal that you have audited the code.

**How it works:**
```python
# WRONG (with --strict):
mut buf: Pointer[char] = alloc[char](256)    # U-1

# RIGHT:
unsafe:
    mut buf: Pointer[char] = alloc[char](256)
    # ... use buf ...
    dealloc(buf)
```

**Best Practices:** Always compile with `--strict` for production code. The `unsafe:` annotation serves as documentation that this block has been manually reviewed.

---

## Parse Errors

Parse errors have no rule code — they are reported directly with the source location.

### Unexpected Indentation

```
ParseError at line 5: unexpected indentation
```

**Cause:** Inconsistent indentation — mixing tabs and spaces, or wrong depth.

```python
# WRONG:
def foo():
    x = 1
  y = 2    # 2 spaces instead of 4
```

**Fix:** Use exactly 4 spaces per indentation level. Never use tabs.

---

### Unexpected Token

```
ParseError at line 8: unexpected token '='
```

**Cause:** Often a syntax error in an expression, or a keyword used in the wrong context.

---

### Missing Colon

```
ParseError at line 3: expected ':' after 'if' condition
```

**Fix:** Add `:` at the end of every `if`, `elif`, `else`, `while`, `for`, `def`, `class`, and `extend` header.

---

## Linker / C Compiler Errors

These appear after the Tauraro compiler succeeds but the C compiler (GCC/Clang) fails.

### undefined reference to `function_name`

**Cause:** A function declared in `extern "C"` is not present in any linked library.

```
undefined reference to `curl_easy_init`
```

**Fix:** Pass the library: `tauraroc main.tr -l curl --run`

---

### conflicting types for 'name'

**Cause:** Two declarations of the same C symbol with different types — typically a mismatch between your `extern "C"` annotation and the actual C function signature.

**Fix:** Correct the `extern "C"` declaration to match the C header exactly.

---

### assignment from incompatible pointer type

**Cause:** Assigning a `List_ptr*` (generic list) to a `List_i64*` (typed list). This happens when an empty list literal `[]` has no type context.

**Fix:** Annotate empty list literals explicitly: `mut data: List[int] = []`

---

## Runtime Errors (Abort / Crash)

These are not compile errors — they crash at runtime.

### Null Pointer Dereference

**Symptom:** Segmentation fault or access violation.

**Cause:** Accessing a field or method through a null pointer.

```python
mut p: Point = none
p.x = 10    # CRASH: p is null
```

**Fix:** Initialize before use, or null-check:
```python
if p as usize != 0 as usize:
    p.x = 10
```

---

### Out-of-Bounds List Access

**Symptom:** Crash or incorrect value.

**Cause:** `list[i]` where `i >= len(list)`.

**Fix:** Bounds-check manually: `if i >= 0 and i < len(list): ...`

---

### Stack Overflow

**Symptom:** Crash on deep recursion.

**Cause:** Unbounded recursion or very deep call stacks.

**Fix:** Convert recursive algorithms to iterative form, or increase the OS stack size limit.

---

## Diagnostic Tips

### When to use — quick summary

| Goal | Tool |
|------|------|
| Fast type-check without compiling | `tauraroc --check program.tr` |
| See what the parser understood | `tauraroc --emit ast program.tr` |
| Inspect generated C before GCC runs | `tauraroc --emit c program.tr` |
| See all pipeline phases | `tauraroc --verbose program.tr` |

### Use --check for fast feedback

```bash
tauraroc --check program.tr    # type-checks without generating C
```

### Use --emit ast to see the parse tree

```bash
tauraroc --emit ast program.tr
```

### Use --emit c to inspect generated C

```bash
tauraroc --emit c program.tr
```

If an error comes from GCC, look at the generated C and search for the reported line number. Type mismatches and undefined symbols are usually obvious in the C output.

### Use --verbose for the full pipeline

```bash
tauraroc --verbose program.tr    # lex → parse → sema → codegen
```

---

Next: [Advanced Patterns →](20_advanced_patterns.md)
