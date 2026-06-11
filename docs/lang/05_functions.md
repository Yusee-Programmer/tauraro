# 05 — Functions

Functions are the fundamental unit of reusable logic in Tauraro. They are statically typed, compiled directly to C functions (no runtime dispatch overhead), and support generics, error propagation, closures, and async/await.

---

## Table of Contents

1. [Defining Functions](#defining-functions)
1a. [Local (Nested) Declarations in `main()`](#local-nested-declarations-in-main)
2. [Parameters and Return Types](#parameters-and-return-types)
3. [Decorators](#decorators)
4. [Closures and Lambdas](#closures-and-lambdas)
5. [Generic Functions](#generic-functions)
6. [Error Propagation with `throws`](#error-propagation-with-throws)
7. [Async Functions](#async-functions)
8. [Variadic Functions](#variadic-functions)
9. [Visibility: `pub` and `export`](#visibility-pub-and-export)
10. [Function Rules Quick Reference](#function-rules-quick-reference)

---

## Defining Functions

### When to use

Define a function whenever a block of logic needs a name, needs to be called more than once, or needs to be tested in isolation. If you are writing the same three-line pattern in two places, extract it into a function.

### How it works

Use the `def` keyword followed by the function name, parameter list, optional return type, and an indented body:

```python
def add(a: int, b: int) -> int:
    return a + b

def greet(name: str) -> void:
    print(f"Hello, {name}!")

def log(msg: str):           # void return type may be omitted
    print(msg)
```

### Common Mistakes

**Missing return type on non-void function:**
```python
def area(w: int, h: int):    # ERROR: return type missing for non-void function
    return w * h
```
Fix: `def area(w: int, h: int) -> int:`

**Omitting `return` on a branch:**
```python
def sign(n: int) -> str:
    if n > 0: return "positive"
    elif n < 0: return "negative"
    # ERROR [F-3]: missing return on the else path
```
Fix: add `else: return "zero"` or a final `return "zero"` after the chain.

**Using default parameter values:**
```python
def connect(host: str, port: int = 8080) -> void:   # ERROR: default parameters not supported
    ...
```
Fix: Use overloaded static methods in a class, or require the caller to pass the value explicitly.

### Best Practices

- Name functions as verb phrases: `parse_input`, `build_config`, `find_user`, `render_frame`.
- Annotate return types explicitly — even `-> void` — to make the contract readable and searchable.
- One function, one responsibility. If you are adding a third `elif` inside a function, consider splitting it.
- Return values rather than mutating through side effects. The ownership system manages returned values automatically.

---

## Local (Nested) Declarations in `main()`

### When to use

When a `class`, `enum`, `interface`, `extend`, or helper `def` is only meaningful
for a small script's `main()` function, you can declare it directly inside
`main()` instead of cluttering module scope — similar to local classes in Java.

### How it works

`class`, `enum`, `interface`, `extend`, and `def` statements are allowed inside
the body of `main()`:

```python
def main():
    class Vec2:
        pub x: int
        pub y: int

    extend Vec2:
        pub def init(x: int, y: int) -> Vec2:
            mut v = Vec2()
            v.x = x
            v.y = y
            return v

        pub def length_sq(self) -> int:
            return self.x * self.x + self.y * self.y

    enum Color:
        Red
        Green
        Blue

    def describe(c: Color) -> str:
        match c:
            case Color.Red: return "red"
            case Color.Green: return "green"
            case Color.Blue: return "blue"
        return ""

    mut v = Vec2.init(3, 4)
    print(v.length_sq())          # 25
    print(describe(Color.Green))  # green
```

The compiler hoists these declarations to module scope behind the scenes and
generates ordinary top-level C definitions for them — there is no extra runtime
cost. The names just remain scoped to `main()` for readability.

### Common Mistakes

**Declaring a nested type/function outside `main()`:**
```python
def helper():
    class Foo:    # ERROR [E-2]: nested declarations are main()-only
        pub x: int
```
Fix: move `Foo` to module (top-level) scope, or move the whole helper's logic
into `main()`.

### Best Practices

- Reserve nested declarations for small scripts, examples, and one-off tools
  where the type genuinely has no meaning outside `main()`.
- For anything reused across functions or modules, declare it at module scope.

---

## Parameters and Return Types

### When to use

Every function that operates on external data needs parameters. Every function that produces a value needs a return type annotation. Tauraro has no default parameters and no keyword arguments, so the parameter list is the complete contract.

### How it works

**Basic parameters:**

```python
def compute(x: int, y: float, label: str) -> void:
    print(f"{label}: {x as float + y}")
```

All parameter types must be annotated. Omitting a type is a parse error (`[F-1]`).

**Return values:**

```python
def max_of(a: int, b: int) -> int:
    if a > b: return a
    return b
```

**Multiple return paths — all must return:**

```python
def classify(n: int) -> str:
    if n > 0:   return "positive"
    elif n < 0: return "negative"
    else:       return "zero"     # all paths covered — [F-3] satisfied
```

**Early return:**

```python
def find_first(items: List[int], target: int) -> int:
    mut i = 0
    while i < len(items):
        if items[i] == target: return i    # exit as soon as found
        i = i + 1
    return -1    # not found
```

**Compiler rule [F-1]:** All parameters must have type annotations. Omitting a type is a parse error.

**Compiler rule [F-2]:** Parameters may not be shadowed by local variables of the same name.

**Compiler rule [F-3]:** A non-void function must have a `return` on every reachable code path:

```
ERROR [F-3]: Function 'max_of' returns 'int' but is missing a return statement
             on at least one code path. FIX: Add a return at the end, or ensure
             all if/elif/else branches return.
```

### Common Mistakes

**Shadowing a parameter name:**
```python
def scale(value: int, factor: int) -> int:
    mut value = value * factor    # ERROR [F-2]: shadows parameter 'value'
    return value
```
Fix: Use a different name: `mut result = value * factor`

**Non-void function with a missing return branch:**
```python
def parity(n: int) -> str:
    if n % 2 == 0: return "even"
    # falls off the end with no return — ERROR [F-3]
```
Fix: `else: return "odd"` or add `return "odd"` after the `if`.

### Best Practices

- Keep parameter lists short (3–4 parameters is a natural limit). If you need more, consider grouping them in a class or struct.
- Use explicit `-> void` rather than omitting the return type. The intent is clearer.
- For "optional" parameters, provide a distinct function or use a dedicated sentinel value explicitly passed by the caller.

---

## Decorators

### When to use

Decorators are compile-time annotations that change how the compiler treats a function. Use them when you need fine-grained control over inlining, calling convention, or method dispatch. Most code never needs a decorator.

### How it works

Place the decorator on the line immediately above `def`:

#### `@inline`

Forces inlining even when the compiler would not inline automatically:

```python
@inline
def clamp(v: int, lo: int, hi: int) -> int:
    if v < lo: return lo
    if v > hi: return hi
    return v
```

Use for hot, very small functions called in tight loops. The compiler already inlines most small functions automatically — `@inline` is a manual override.

Note: `@inline` is silently ignored on recursive functions and functions with `try/except` blocks, because those cannot be safely inlined.

#### `@noinline`

Prevents inlining even when the compiler would inline:

```python
@noinline
def log_error(msg: str) -> void:
    print(f"ERROR: {msg}")
```

Use when you want the function to appear as a distinct symbol in profiler output, or when inlining would bloat a hot loop with cold error-handling code.

#### `@hot` and `@cold`

```python
@hot
def inner_loop(buf: List[int], n: int) -> void:
    # tell GCC/Clang this is on the critical path
    ...

@cold
def handle_parse_error(msg: str) -> void:
    # tell GCC/Clang this is rarely called
    ...
```

`@hot` hints that the function is frequently executed; `@cold` hints that it is rarely called. The compiler uses these to influence branch prediction and code placement in the compiled binary.

#### `@staticmethod`

Marks a class method that has no `self` parameter. Called on the class name, not an instance:

```python
class MathUtils:
    @staticmethod
    def square(x: int) -> int:
        return x * x

    @staticmethod
    def cube(x: int) -> int:
        return x * x * x

mut s = MathUtils.square(5)    # 25, no instance needed
```

#### `@property`

Marks a zero-argument method as a readable property:

```python
class Circle:
    pub radius: float

    @property
    def area(self) -> float:
        return 3.14159 * self.radius * self.radius

def main():
    mut c = Circle.init(5.0)
    print(c.area)    # called without parentheses
```

#### `@packed` (class decorator)

Applied to a class (not a function), forces compact struct layout with no padding:

```python
@packed
class NetworkHeader:
    pub version: u8
    pub flags: u16
    pub length: u32
```

Use for hardware registers, network packets, or any struct where exact byte layout matters.

### Common Mistakes

**Expecting `@inline` to inline recursive functions:**
```python
@inline
def factorial(n: int) -> int:    # @inline silently ignored — recursion cannot be inlined
    if n <= 1: return 1
    return n * factorial(n - 1)
```

**Using `@staticmethod` with a `self` parameter:**
```python
class Foo:
    @staticmethod
    def bar(self, x: int) -> int:   # ERROR: static method must not have self
        return x
```

### Best Practices

- Do not add `@hot` or `@inline` speculatively. Profile first, annotate second.
- Never add `@inline` or `__attribute__((optimize(...)))` if you are dealing with GCC 15.x on MinGW — this can trigger an implicit LTO crash. The `@hot` decorator is safe.
- Use `@staticmethod` for utility and factory methods that logically belong to a class but do not operate on instance state.

---

## Closures and Lambdas

### When to use

Use a closure when you need a short anonymous function — especially one that captures a variable from the enclosing scope — for callbacks, transformations, or state machines. Closures are more expressive than passing plain function names because they carry their own captured state.

### How it works

Create a closure with `def (params) -> RetType: body` and assign it to a `mut` variable:

**Basic closure:**
```python
mut square = def (x: int) -> int:
    return x * x

print(square(7))    # 49
```

**Lambda shorthand** (single-expression body):
```python
mut double = lambda x: x * 2
mut add    = lambda x, y: x + y

print(double(5))     # 10
print(add(3, 4))     # 7
```

**Capturing outer variables:**
```python
def main():
    mut base: int = 100
    mut add_to_base = def (n: int) -> int:
        return base + n

    print(add_to_base(5))     # 105
    base = 200
    print(add_to_base(5))     # 205 — closure sees the updated value
```

Closures capture `mut` variables **by reference** — they see and can modify the current value of the outer variable:

```python
mut total: int = 0
mut accumulate = def (n: int) -> void:
    total = total + n

accumulate(5)
accumulate(10)
print(total)    # 15 — outer variable was modified
```

**Stateful counter:**
```python
def make_counter() -> lambda:
    mut count: int = 0
    return def () -> int:
        count = count + 1
        return count

mut counter = make_counter()
print(counter())    # 1
print(counter())    # 2
print(counter())    # 3
```

**Passing closures as function parameters:**

The type of any closure is `lambda`. Use it as a parameter type:

```python
def apply(f: lambda, x: int) -> int:
    return f(x)

mut triple = def (x: int) -> int: return x * 3
print(apply(triple, 7))    # 21
```

**Passing a closure to transform a list:**
```python
def map_list(items: List[int], f: lambda) -> List[int]:
    mut result: List[int] = []
    for x in items:
        result.append(f(x))
    return result

mut doubled = map_list([1, 2, 3, 4], lambda x: x * 2)
# doubled = [2, 4, 6, 8]
```

### Common Mistakes

**Forgetting `mut` on the closure variable:**
```python
square = def (x: int) -> int: return x * x    # ERROR: closure must be assigned to mut var
```
Fix: `mut square = def (x: int) -> int: return x * x`

**Expecting closures to have a concrete callable type other than `lambda`:**
```python
def run(f: def(int) -> int) -> void:    # ERROR: not valid syntax
    ...
```
Fix: Always use `lambda` as the parameter type: `def run(f: lambda) -> void:`

**Closing over a loop variable and expecting a snapshot:**
```python
mut fns: List[lambda] = []
for i in range(3):
    fns.append(lambda: i)    # all three closures share the same 'i' by reference
# After the loop, i = 2 — all closures return 2
```
Fix: Capture a copy by passing as a parameter or use an intermediate variable.

### Best Practices

- Prefer `lambda x: expr` for single-expression closures; use `def (params) -> T: body` when the body needs multiple statements.
- Use closures for short-lived logic (callbacks, filters, comparators). For complex reusable logic, define a named function.
- Be aware that captured variables are shared by reference. Modifying the outer variable after creating a closure affects what the closure sees.

See `examples/20_closures.tr` and `examples/21_closure_params.tr` for full working examples.

---

## Generic Functions

### When to use

Use a generic function when the logic is identical regardless of the type — identity functions, swap, min/max, map/filter operations. Generics avoid duplicating the same function for every type, with zero runtime cost.

### How it works

Declare type parameters in square brackets after the function name:

```python
def identity[T](x: T) -> T:
    return x

def swap_print[T](a: T, b: T) -> void:
    print(f"a={a}")
    print(f"b={b}")
```

**Explicit instantiation:**
```python
mut n = identity[int](42)        # specialized for int
mut s = identity[str]("hello")   # specialized for str
swap_print[int](1, 2)
swap_print[str]("x", "y")
```

**Type inference** — the compiler often infers the type argument from the argument value:
```python
mut n = identity(42)     # inferred as identity[int]
mut s = identity("hi")   # inferred as identity[str]
```

**Generic with constraint pattern:**
```python
def min_of[T](a: T, b: T) -> T:
    if a < b: return a
    return b

mut smaller = min_of(3, 7)          # int
mut earlier = min_of("alpha", "beta")   # str (lexicographic)
```

**How generics compile:** The compiler monomorphizes at each call site. `identity[int]` and `identity[str]` become two separate C functions. No boxing, no type erasure, no runtime overhead.

### Common Mistakes

**Forgetting the type argument when inference fails:**
```python
def wrap[T](x: T) -> List[T]:
    mut result: List[T] = []
    result.append(x)
    return result

mut w = wrap([])    # ERROR: cannot infer T from empty list
```
Fix: `mut w = wrap[int](42)`

**Using generic functions for unsafe pointer arithmetic:**
```python
def offset[T](ptr: T, n: int) -> T:
    return ptr + n    # may not monomorphize correctly for all T
```
Fix: Use concrete pointer types for unsafe/FFI code.

### Best Practices

- Let the compiler infer the type argument whenever possible — explicit `[T]` annotation is only needed when inference fails.
- Keep generic function bodies simple. Complex bodies with unsafe operations or raw pointer arithmetic should use concrete types.
- Generic functions work best for container-like patterns (wrap, unwrap, transform) and comparison patterns (min, max, clamp).

---

## Error Propagation with `throws`

### When to use

Use `throws` when a function can fail in a predictable way — parsing user input, reading a file, network operations, validating data. It is the Tauraro equivalent of returning `Result<T, E>` in Rust or raising a checked exception in Java.

### How it works

Add `throws ErrorType` between the parameter list and the return type:

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
- `return value` wraps as `Result { is_err: false, value: value }`
- `raise(err)` wraps as `Result { is_err: true, error: err }` and returns immediately

**The `?` propagation operator** unwraps the success value or propagates the error to the caller:

```python
def doubled(s: str) throws str -> int:
    mut n = parse_digit(s)?     # if parse_digit fails, return its error immediately
    return n * 2

def tripled(s: str) throws str -> int:
    mut n = doubled(s)?         # propagate through the chain
    return n * 3
```

**Handling a `throws` result at the call site:**
```python
def main() -> void:
    mut result = parse_digit("7")
    if result.is_err:
        print(f"error: {result.error}")
    else:
        print(f"digit: {result.value}")
```

**Compiler rule [T-4]:** The result of a `throws` call must be handled — either with `?`, with an `if result.is_err` check, or explicitly discarded. Silently ignoring it is a compile error.

### Common Mistakes

**Using `raise` in a non-`throws` function:**
```python
def parse(s: str) -> int:
    raise("bad input")    # ERROR: raise() only valid in throws functions
```
Fix: Add `throws str` to the signature.

**Not propagating `?` and reading `.value` blindly:**
```python
def process(s: str) -> int:
    mut r = parse_digit(s)
    return r.value    # ERROR [T-4]: unhandled Result — must check is_err first
```
Fix: Either `return parse_digit(s)?` (if this is also a `throws` function) or check `r.is_err` before using `r.value`.

**Mismatched error types in `?` chain:**
```python
def parse_digit(s: str) throws str -> int: ...
def read_line() throws IOError -> str: ...

def run(s: str) throws str -> int:
    mut line = read_line()?    # ERROR: read_line throws IOError, not str
    return parse_digit(line)?
```
Fix: The caller must handle `read_line`'s `IOError` explicitly rather than propagating with `?` if the error types differ.

### Best Practices

- Use `throws` for all I/O operations, parsing, and any operation that can fail due to external input.
- Use `?` to propagate errors through chains rather than checking each result manually — this keeps happy-path logic readable.
- Keep the error type consistent across a function chain so `?` can propagate without conversion.
- Document what conditions trigger `raise()` with a comment near the function signature.

See [Error Handling](12_error_handling.md) for the complete error handling guide.

---

## Async Functions

### When to use

Mark a function `async` when it represents logically asynchronous work — I/O operations, network calls, tasks that will eventually be suspended and resumed. Even though the current implementation executes synchronously, using `async`/`await` now makes the code forward-compatible with the full async runtime.

### How it works

```python
async def fetch(id: int) -> str:
    return f"item-{id}"

async def pipeline(n: int) -> int:
    mut data = await fetch(n)
    return len(data)

async def run() -> void:
    mut r1 = await pipeline(1)
    mut r2 = await pipeline(42)
    print(f"pipeline(1)={r1}")
    print(f"pipeline(42)={r2}")
```

**Current semantics:** `async`/`await` executes synchronously in the current compiler. `await fn()` is a direct function call — there is no scheduler, no event loop, no coroutine suspension. The syntax is intentionally forward-compatible: when a true async runtime is added, all `async`/`await` code will continue to work without changes.

**`spawn` and task groups** are available for concurrent task dispatch:

```python
async def main() -> void:
    task_group:
        spawn worker(1)
        spawn worker(2)
        spawn worker(3)
    print("all workers done")
```

See [Concurrency](16_concurrency.md) for `spawn`, `task_group`, `await_timeout`, and the full async I/O model.

### Common Mistakes

**Forgetting `await` on an async call:**
```python
async def main() -> void:
    mut data = fetch(1)    # ERROR: fetch returns a Future[str], not str — missing await
    print(data)
```
Fix: `mut data = await fetch(1)`

**Using `async def` for purely CPU-bound functions:**
```python
async def square(x: int) -> int:    # unnecessary — no I/O or suspension
    return x * x
```
`async` adds no overhead today, but it signals intent. Reserve it for I/O-bound or logically concurrent operations.

### Best Practices

- Prefix all I/O functions with `async` even when I/O is currently synchronous — this documents intent and enables future migration.
- `await` every call to an `async` function; never discard the future.
- Put `spawn` calls inside `task_group:` blocks so the compiler can enforce structured concurrency.

---

## Variadic Functions

Tauraro supports two flavors of trailing variadic parameters, both spelled
`name...` (a trailing param name followed by `...`):

1. **Regular functions** — `args...` collects the trailing call arguments
   into a `List[T]`.
2. **`extern "C"` declarations** — a trailing `args...` param maps to C's
   literal `...` variadic signature, for calling functions like `printf`.

### Regular Tauraro functions: `args...` -> `List[T]`

#### When to use

Use this when a function should accept any number of trailing arguments of
the same type — logging helpers, math reducers, constructors that take a
variable number of items, etc.

#### How it works

```python
def total(label: str, args: int...) -> int:
    mut sum = 0
    for v in args:
        sum = sum + v
    print(label + ": " + sum.to_str())
    return sum

def main():
    total("a", 1, 2, 3)       # args = [1, 2, 3]
    total("b", 10)            # args = [10]
    total("c")                # args = []
```

The element type comes from the annotation before `...` (`int` above; it
defaults to `int` if omitted). At each call site, the compiler collects all
arguments past the fixed parameters into a single `List[T]` literal — the
caller passes plain values, not a list.

### `extern "C"` declarations: `args...` -> C's `...`

#### When to use

Use a trailing `args...` in an `extern "C"` block when calling C library
functions that accept a variable number of arguments (`printf`, `sprintf`,
`ioctl`, etc.). You cannot define a function with literal C `...` in
Tauraro itself — only declare one via `extern "C"`.

#### How it works

```python
extern "C":
    def printf(fmt: str, args...) -> int
    def snprintf(buf: str, n: int, fmt: str, args...) -> int
```

A trailing `args...` in an `extern "C"` declaration emits C's `...`
variadic signature (`int printf(const char* fmt, ...)`). Call sites pass
arguments through unchanged:

```python
printf("value = %d\n", 42)
printf("x=%d y=%d\n", x, y)
snprintf(buf, 256, "result: %f", result)
```

### Common Mistakes

**Forgetting the trailing param name:**
```python
def log(msg: str, ...) -> void:    # ERROR: bare `...` is not valid syntax
    ...
```
Fix: name the trailing param, e.g. `def log(msg: str, args: str...) -> void:`.

### Best Practices

- Prefer `args...` -> `List[T]` for Tauraro-side variadic APIs — it is
  type-checked and iterable like any other `List[T]`.
- Prefer Tauraro's `f"..."` and `print()` for formatted output over
  `printf` — they are safer and type-checked. Use `extern "C"` variadics
  only when you need direct C interop.
- Always include the `-> int` return type on variadic C functions that
  return a value — the compiler uses it for correct call-site codegen.

---

## Visibility: `pub` and `export`

### When to use

Use `pub def` when you are writing a library module and want other Tauraro modules to import and call the function. Use `export def` when you need the function to be callable from C code (or another language) by a stable C symbol name.

### How it works

**`pub def` — importable from other Tauraro modules:**
```python
# math_utils.tr
pub def gcd(a: int, b: int) -> int:
    while b != 0:
        mut t = b
        b = a % b
        a = t
    return a
```

```python
# main.tr
import math_utils

mut d = math_utils.gcd(48, 18)    # 6
```

**`export def` — C-ABI stable symbol:**
```python
export def tauraro_add(a: int, b: int) -> int:
    return a + b
```

This generates a C function with external linkage and the exact name `tauraro_add`, usable from C, Python (via ctypes), or any language with C FFI.

### Common Mistakes

**Importing a non-`pub` function from another module:**
```python
# utils.tr
def helper() -> void:    # not pub
    ...

# main.tr
import utils
utils.helper()    # ERROR: 'helper' is not exported from utils
```
Fix: Add `pub` to `def helper()` in utils.tr.

### Best Practices

- Default to private (no prefix). Only add `pub` when the function is part of a deliberate public API.
- Use `export def` sparingly — it bypasses Tauraro's name mangling and type safety at the boundary. Always document the ABI contract.

---

## First-Class Functions (Callables)

A top-level function can be used as a **value** — passed to other functions, stored
in a variable, reassigned, and stored in a class field. A function value is a
**zero-cost function pointer** (a pointer to the top-level function; no heap, no
captured environment).

### The `def(...) -> R` type

A callable's type is written with `def`, mirroring how functions are declared:

```python
def add1(x: int) -> int:
    return x + 1

def main():
    mut h: def(int) -> int = add1   # a variable holding a function
    print(h(41))                    # 42 — call through the value
    h = mul2                        # reassign to another function
```

The type is `def(ParamTypes...) -> ReturnType`. Use `def() -> R` for no parameters,
and omit `-> R` for a function returning nothing.

### Passing functions as arguments

```python
def apply(f: def(int) -> int, v: int) -> int:
    return f(v)

apply(add1, 10)   # 11 — pass the function by name, call it inside
```

This is the idiomatic way to take a callback or handler.

### Storing callables in a struct (e.g. a router)

Put the callable in a **field**, then keep a normal collection of those structs:

```python
pub class Route:
    pub pattern: str
    pub handler: def(HttpConn) -> void

extend Route:
    pub def init(p: str, h: def(HttpConn) -> void) -> Route:
        mut r = Route(); r.pattern = p; r.handler = h; return r

def main():
    mut routes = Vec[Route].init(4)
    routes.push(Route.init("/", home))
    routes.push(Route.init("/about", about))
    mut rt = routes.get(0)
    rt.handler(conn)        # call the stored handler directly
```

> **Note:** a callable *element type* in a generic literal —
> `Vec[def(int) -> int].init()` — is not yet supported (in expression position
> the brackets parse as an index). Wrap the callable in a small struct field (as
> above) and use `Vec[YourStruct]`.

---

## Function Rules Quick Reference

| Rule | Description | Error |
|------|-------------|-------|
| F-1 | All parameters must have type annotations | `[F-1] Parameter type missing` |
| F-2 | Parameters may not be shadowed by local variables | `[F-2] Parameter name shadowed` |
| F-3 | Non-void function must return on all code paths | `[F-3] Missing return on code path` |
| T-4 | Result from a `throws` call must be handled | `[T-4] Unhandled Result from throws call` |
| E-1 | Method must exist on the receiver's type (or a base class) | `[E-1] No method 'x' found on type 'Y'` |
| E-2 | Nested class/def/enum/interface/extend declarations are `main()`-only | `[E-2] Nested declarations are only supported inside main()` |

---

Next: [Strings and F-Strings →](06_strings.md)
