# 05 — Functions

---

## Defining Functions

```python
def add(a: int, b: int) -> int:
    return a + b

def greet(name: str) -> void:
    print(f"Hello, {name}!")

def log(msg: str):           # void return type is optional
    print(msg)
```

### Parameters

All parameter types must be annotated. There are no default parameters and no keyword arguments.

```python
def compute(x: int, y: float, label: str) -> void:
    print(f"{label}: {x as float + y}")
```

**Compiler rule [F-1]:** Functions must declare parameter types. Omitting a parameter type is a parse error.

**Compiler rule [F-2]:** Functions must not shadow parameter names with local variables of the same name.

### Return Values

```python
def max_of(a: int, b: int) -> int:
    if a > b: return a
    return b
```

**Compiler rule [F-3]:** A function that declares a non-void return type must have a `return` statement on every possible code path. If any path is missing a `return`, the compiler errors:

```
ERROR [F-3]: Function 'max_of' returns 'int' but is missing a return statement
             on at least one code path. FIX: Add a return at the end, or ensure
             all if/elif/else branches return.
```

**Fix:** Add a final `return` or complete the `if/elif/else` chain:
```python
def classify(n: int) -> str:
    if n > 0:   return "positive"
    elif n < 0: return "negative"
    else:       return "zero"    # all paths covered
```

### Early Return

Use `return` anywhere in the function body:

```python
def find_first(items: List[int], target: int) -> int:
    mut i = 0
    while i < len(items):
        if items[i] == target: return i    # early exit
        i = i + 1
    return -1    # not found
```

---

## Decorators

Decorators are compile-time hints applied above a function definition:

### `@inline`

Forces inlining even on functions that wouldn't normally be inlined:

```python
@inline
def clamp(v: int, lo: int, hi: int) -> int:
    if v < lo: return lo
    if v > hi: return hi
    return v
```

**When to use:** For hot, very small functions called in tight loops that the compiler might not automatically inline. The compiler already inlines most small functions automatically — use `@inline` sparingly.

**Compiler rule:** `@inline` is silently ignored on recursive functions and functions with `try/except`, because those cannot be inlined.

### `@staticmethod`

Marks a method that has no `self` parameter. Called on the class name:

```python
class MathUtils:
    @staticmethod
    def square(x: int) -> int:
        return x * x

    @staticmethod
    def cube(x: int) -> int:
        return x * x * x

mut s = MathUtils.square(5)    # 25
```

Static methods have no `self` parameter and are called on the class name, not an instance.

### `@packed`

Applied to a class (not function), makes the struct use packed layout (no padding):

```python
@packed
class NetworkHeader:
    pub version: u8
    pub flags: u16
    pub length: u32
```

Forces compact memory layout with no padding between fields. Use for hardware registers, network packets, or any struct where exact byte layout matters.

### `@hot` and `@cold`

```python
@hot
def critical_loop():    # tell GCC/Clang this is on the hot path
    ...

@cold
def error_handler():    # tell GCC/Clang this is rarely called
    ...
```

These hint to the compiler that a function is on the hot or cold path, influencing branch prediction and code placement decisions.

---

## Closures

A closure is an anonymous function that captures variables from its surrounding scope. Use `def (params) -> RetType: body` to create one:

```python
def main():
    mut count: int = 0
    mut counter = def () -> int:
        count = count + 1
        return count

    print(counter())    # 1
    print(counter())    # 2
    print(counter())    # 3
```

Closures can take parameters:

```python
mut add = def (a: int, b: int) -> int:
    return a + b
print(add(3, 4))    # 7
```

Closures can both capture outer variables AND take parameters:

```python
mut base: int = 100
mut add_to_base = def (n: int) -> int:
    return base + n
print(add_to_base(5))     # 105
base = 200
print(add_to_base(5))     # 205 — sees updated capture
```

### Capture Semantics

Closures capture outer `mut` variables **by reference** — they see and can modify the current value of the outer variable. Modifications inside the closure affect the outer variable:

```python
mut total: int = 0
mut accumulate = def (n: int):
    total = total + n
accumulate(5)
accumulate(10)
print(total)    # 15
```

### The lambda Type

The type of a closure is `lambda`. Use it in function parameters or variable annotations:

```python
def apply(f: lambda, x: int) -> int:
    return f(x)

mut square = def (x: int) -> int: return x * x
print(apply(square, 7))    # 49
```

See `examples/20_closures.tr` and `examples/21_closure_params.tr` for full working examples.

---

## Generic Functions

Generic functions work with any type. The compiler generates a concrete version for each type argument used:

```python
def identity[T](x: T) -> T:
    return x

def swap_print[T](a: T, b: T) -> void:
    print(f"a={a}")
    print(f"b={b}")

mut n = identity[int](42)        # generates identity specialized for int
mut s = identity[str]("hello")   # generates identity specialized for str
swap_print[int](1, 2)
swap_print[str]("x", "y")
```

**How generics compile:** The compiler monomorphizes at each call site — `identity[int]` and `identity[str]` become separate specialized functions. No boxing, no type erasure, no runtime cost.

**Type inference for generics:** The compiler often infers the type argument from the value:
```python
mut n = identity(42)     # inferred as identity[int]
mut s = identity("hi")   # inferred as identity[str]
```

**Limitation:** Complex generic bodies involving pointer arithmetic on `T` or unsafe operations may not always monomorphize correctly. Use concrete types for performance-critical unsafe code.

---

## Error Propagation with `throws`

The `throws` keyword declares that a function can fail and automatically changes its return type to `Result[ReturnType, ErrorType]`:

```python
def parse_digit(s: str) throws str -> int:
    if len(s) == 0:
        raise("empty string")
    mut code: int = s[0] as int
    if code < 48 or code > 57:
        raise("not a digit: " + s)
    return code - 48
```

Inside a `throws` function:
- `return value` → wraps as `Result { is_err: false, value: value }`
- `raise(err)` → wraps as `Result { is_err: true, error: err }` and returns immediately

### The `?` Propagation Operator

`?` after a `throws` call unwraps the success value or propagates the error to the caller:

```python
def doubled(s: str) throws str -> int:
    mut n = parse_digit(s)?     # if parse_digit fails, return its error
    return n * 2

def tripled(s: str) throws str -> int:
    mut n = doubled(s)?
    return n * 3
```

See [Error Handling](12_error_handling.md) for the full error handling guide.

---

## Async Functions

Mark a function `async` to declare it as a coroutine:

```python
async def fetch(id: int) -> str:
    return f"item-{id}"

async def pipeline(n: int) -> int:
    mut data = await fetch(n)
    return len(data)

async def run():
    mut r1 = await pipeline(1)
    mut r2 = await pipeline(42)
    print(f"pipeline(1)={r1}")
    print(f"pipeline(42)={r2}")
```

**Current semantics:** `async`/`await` executes **synchronously** in the current compiler. `await fn()` is a direct function call — there is no scheduler, no event loop, no suspension. The syntax is forward-compatible: when a true async runtime is added, all `async`/`await` code will continue to work.

**What `async` enables today:**
- Communicates that a function is I/O-bound or logically asynchronous
- Makes code self-documenting about intended concurrent behavior
- Type-checks correctly: `await f()` has the return type of `f`, not a future type

See [Concurrency](16_concurrency.md) for `spawn` and `task_group:`.

---

## Variadic Functions (FFI Only)

```python
extern "C":
    def printf(fmt: str, ...) -> int
    def snprintf(buf: str, n: int, fmt: str, ...) -> int
```

`...` in an `extern "C"` declaration allows variadic C functions to be called. Tauraro user-defined functions cannot be variadic.

---

## Function Rules Quick Reference

| Rule | Description | Error |
|------|-------------|-------|
| F-1 | All parameters must have type annotations | `[F-1] Parameter type missing` |
| F-2 | No shadowing of parameter names | `[F-2] Parameter name shadowed` |
| F-3 | Non-void function must return on all paths | `[F-3] Missing return on code path` |
| T-4 | Result from `throws` function must be handled | `[T-4] Unhandled Result from throws call` |

---

## Best Practices

**Keep functions small.** A function should do one thing. If you find yourself adding an `elif` for a third code path, consider splitting into two functions.

**Return values, not void+mutation.** Functions that build a value should return it, not take a mutable pointer. This is cleaner and enables the ownership system to manage memory automatically.

```python
# GOOD:
def build_report(data: List[int]) -> str:
    mut s = ""
    for x in data: s = s + f"{x}\n"
    return s

# AVOID (mutation via side effect):
def fill_report(data: List[int], out: str) -> void:
    # out cannot be mutated this way in Tauraro — str is by value
```

**Annotate return types explicitly.** Even though void is inferred, writing `-> void` makes the function's contract explicit and searchable.

**Name functions as verb phrases.** `parse_input`, `build_config`, `find_user`, `render_frame` — the name should describe what the function does.

---

Next: [Strings & F-Strings →](06_strings.md)
