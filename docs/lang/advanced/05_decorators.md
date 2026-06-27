# Advanced — Decorators

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

Decorators are compile-time annotations applied to functions or methods with the `@name` syntax. Built-in decorators instruct the compiler (and the C backend) to treat the function in a specific way. Custom decorators are functions that receive and transform a function at compile time.

All decorators are resolved and applied during the compilation phase — they have zero runtime overhead.

---

## When You Need This

- You want to give the compiler hints about inlining or hot path treatment for performance-critical code
- You are writing class methods and want to mark them as static methods or property getters
- You are building a library and want to apply cross-cutting behavior (logging, validation, timing) to multiple functions uniformly

---

## Syntax Reference

```python
@decorator_name
def function_name(...) -> ReturnType:
    ...

@decorator_name(argument)
def function_name(...) -> ReturnType:
    ...
```

Multiple decorators stack from bottom to top (the decorator closest to the `def` is applied first):

```python
@outer
@inner
def my_func():
    ...
# equivalent to: outer(inner(my_func))
```

---

## Built-in Decorators

### @inline

Hints to the C compiler to inline this function at every call site.

```python
@inline
def clamp(x: int, lo: int, hi: int) -> int:
    if x < lo: return lo
    if x > hi: return hi
    return x
```

**When to use:** Small functions called in tight, measured hot loops. Functions with 1–5 expressions.

**When NOT to use:**
- Functions containing `try/except` — prevents inlining
- Functions called from only one place — the C compiler already inlines these at `-O2`
- Large functions — code bloat hurts CPU instruction cache

---

### @noinline

Prevents the C compiler from inlining this function even when it would normally do so automatically.

```python
@noinline
def debug_dump(x: int) -> void:
    print(f"debug: x = {x}")
    # ... more diagnostic output
```

**When to use:**
- Debug or diagnostic functions that should stay as distinct call frames (visible in stack traces)
- Functions you are profiling and need to appear as a separate entry in the profile
- Very large functions you want to guarantee are never duplicated

---

### @hot

Marks a function as performance-critical. The compiler uses this to guide code layout and branch prediction hints.

```python
@hot
def process_packet(buf: Pointer[char], n: int) -> int:
    # inner loop for packet parsing
    mut i = 0
    while i < n:
        # ... 
        i = i + 1
    return i
```

**When to use:** Functions that appear in measured hot paths — functions your profiler shows consuming >5% of total execution time. `@hot` lowers the inlining threshold for functions called from this one and biases branch prediction toward the "work done" path.

---

### @staticmethod

Marks a method as a static class method — it has no `self` parameter and is called on the class name, not an instance.

```python
class MathUtils:
    pass

extend MathUtils:
    @staticmethod
    pub def gcd(a: int, b: int) -> int:
        while b != 0:
            mut t = b
            b = a % b
            a = t
        return a

mut g = MathUtils.gcd(48, 18)    # called on class, not instance
```

Note: A method without a `self` parameter is automatically treated as a static method. `@staticmethod` is the explicit form — use it when you want to be clear about intent.

---

### @property

Marks a method as a property getter. The method is accessed as a field (`obj.name`) rather than a call (`obj.name()`).

```python
class Circle:
    pub radius: float

extend Circle:
    pub def init(r: float) -> Circle:
        mut c = Circle()
        c.radius = r
        return c

    @property
    pub def area(self) -> float:
        return 3.14159265 * self.radius * self.radius

    @property
    pub def diameter(self) -> float:
        return self.radius * 2.0

mut c = Circle.init(5.0)
print(c.area)       # no () — accessed as a field
print(c.diameter)   # no ()
```

**When to use:** Computed properties where the computation is cheap and the value is logically a property of the object (not a method). Makes the API cleaner — callers don't need to know whether it is stored or computed.

**Common Mistakes:** Using `@property` on expensive computations. If the computation is slow, make it a regular method so callers see the `()` and know it is not free.

---

### @value_type

Applied to a **class** (not a function). Makes the class a **stack value type** —
like a C struct or Rust's `Copy` types (`&str`, `Point`, `NaiveDate`). Instead of
the default *reference* semantics (heap-allocated, reference-counted, shared by
pointer), a `@value_type` class is:

- **stack-allocated** — constructing one allocates nothing on the heap;
- **passed and returned by value** — a struct copy, no refcount traffic;
- **stored inline in collections** — `List[Point]` packs the structs in one
  buffer (like Rust's `Vec<Point>`), not an array of pointers;
- **not dropped** — it owns no heap and has no destructor.

```python
@value_type
pub class Point:
    pub x: int
    pub y: int

@value_type
pub class StrView:          # the stdlib zero-copy string view
    pub data: Pointer[char]  # borrowed
    pub len:  int
```

**When to use it.** A small, immutable, *value-semantics* type that is
constructed or copied frequently — points, colors, dates/times, ranges, borrowed
views. This is the lever that lets such types **match Rust's performance**:
`StrView` substring views went from ~19 ms to ~4 ms (Rust: ~3 ms) once marked
`@value_type`, because each view stopped heap-allocating a struct.

**Hard rules (the compiler does not yet check these — follow them):**

- **POD fields only.** Every field must be a primitive (`int`/`float`/`bool`/
  `char`), a `Pointer[T]`, or another `@value_type`. **No owned heap fields** —
  a `str`, `List`, `Dict`, `Set`, or reference-class field would **leak**, because
  a value type is never dropped (its fields are never released).
- **Treat it as immutable.** Methods receive `self` **by value**, so mutating
  `self.field` inside a method does **not** persist to the caller's copy. Model
  changes as methods that *return a new value* (`def with_x(self, x) -> Point`),
  not in-place mutation.
- **Value semantics on assignment.** `b = a` copies the struct; later changes to
  one do not affect the other. That's correct for points/dates/views, but wrong
  for anything you intend to share and mutate through aliases.

**Mutating methods are supported.** A method that writes `self.field` is given a
*pointer* `self` automatically and the call passes the receiver by reference, so
the write persists (the compiler detects self-mutation and rewires both sides):

```python
@value_type
pub class Counter:
    pub count: int

extend Counter:
    pub def bump(self):
        self.count = self.count + 1   # persists to the caller's value

mut c = Counter()
c.bump()                              # c.count is now 1
```

Note the value-semantics caveat still applies on **assignment/copy**: `b = a`
copies, so `b.bump()` does not affect `a`.

**Generic value types** (`Box[T]`) are supported — `List[Box[int]]` stores the
boxes inline. Use an explicit annotation at the binding site
(`mut b: Box[int] = Box[int].of(42)`), since bare inference can't yet substitute
the receiver's type args.

**Not yet supported (avoid for now):** using a value type as a `Dict`/`Map`
value or `Set` element. Reference (default) classes have none of these limits.

**Best practice.** Default to a normal (reference) class. Reach for
`@value_type` only for a small, immutable, copy-friendly type on a measured hot
path — that's where it turns ARC's refcount traffic and per-instance heap
allocation into free stack copies.

---

## Custom Decorators

Custom decorators are compile-time macros that inject C attributes into the generated code. They are declared with `decorator def` and must return a `str` naming a C compiler attribute:

```python
decorator def hot_fn() -> str:
    return "hot"

@hot_fn
def compute_sum(n: int) -> int:
    mut s = 0
    mut i = 0
    while i < n:
        s = s + i
        i = i + 1
    return s
```

The compiler reads the return value (`"hot"`) and emits `__attribute__((hot))` on the generated C function. This is equivalent to writing `@hot` directly, but lets you define reusable decorator names in Tauraro code.

### How it works

1. `decorator def name() -> str:` declares a decorator.
2. The body must be a single `return "attribute_string"` statement.
3. `@name` applied to a `def` inserts `__attribute__((attribute_string))` on the generated C function.

### Custom decorator with a C attribute string

```python
decorator def fast() -> str:
    return "optimize(\"O3\",\"unroll-loops\")"

@fast
def inner_loop(data: Pointer[int], n: int) -> void:
    mut i = 0
    while i < n:
        # ...
        i = i + 1
```

### Limitation

Custom decorators in Tauraro are C attribute injectors — they do not wrap function bodies or intercept calls at runtime. For runtime wrapping behavior (logging, retry, timing), write a higher-order function and call it explicitly rather than using a decorator.

---

## Examples

### Combining built-in decorators on a class method

```python
class Vec3:
    pub x: float
    pub y: float
    pub z: float

extend Vec3:
    @inline
    pub def dot(self, other: Vec3) -> float:
        return self.x * other.x + self.y * other.y + self.z * other.z

    @property
    pub def length_sq(self) -> float:
        return self.dot(self)

    @hot
    pub def normalize(self) -> Vec3:
        mut len_sq = self.length_sq
        if len_sq == 0.0: return self
        mut inv = 1.0 / (len_sq ** 0.5 as float)
        mut v = Vec3()
        v.x = self.x * inv
        v.y = self.y * inv
        v.z = self.z * inv
        return v
```

---

## Common Mistakes

**Applying `@inline` to everything.** Over-inlining bloats code and hurts I-cache performance. Use `@inline` only on small, hot functions identified by profiling.

**Using `@property` on slow computations.** Properties look like field accesses to callers, who assume they are cheap. If the computation is non-trivial, make it a regular method.

**Stacking decorators in the wrong order.** Decorators apply from innermost (closest to `def`) outward. `@outer @inner def f()` means `outer(inner(f))`. If order matters, be explicit.

**Expecting custom decorators to run at runtime.** Custom decorators transform the function at compile time. They cannot depend on runtime values, global state, or conditional logic that depends on program input.

---

## Best Practices

- **Use built-in decorators by measurement, not intuition.** Profile first, then apply `@inline` or `@hot` to the proven hot path.
- **Use `@property` for zero-argument computed values that are logically attributes.** `circle.area` reads as a property; `circle.compute_area()` reads as work. Match the syntax to the cost.
- **Keep custom decorators simple.** A decorator that adds logging, timing, or retry logic is reasonable. A decorator that rewrites the function's control flow is complex and hard to debug.
- **Document custom decorators.** Since they transform invisible behavior, a doc comment on the `decorator` declaration explaining what it adds is essential.

---

See also:
- [05 — Functions](../05_functions.md)
- [08 — Classes](../08_classes.md)
- [20 — Advanced Patterns](../20_advanced_patterns.md) (for `@inline` guidance)
