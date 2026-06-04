# 21 — Operator Overloading (Dunder Methods)

---

## Overview

Tauraro supports Python-style **dunder methods** (double-underscore methods) that let user-defined classes hook into built-in operators, functions, and language constructs. When the compiler sees an operator or built-in call on a class instance, it checks whether the class defines the corresponding dunder and, if so, generates a call to it instead of the default behavior.

All dunder methods are declared inside an `extend` block, like any other method.

**When to use:** Implement dunders when your class represents a value type where operators have a natural mathematical or domain meaning — vectors, matrices, currency amounts, date ranges, custom collections. Avoid dunders on classes where operators would have no clear meaning; it makes code harder to read.

---

## Arithmetic Operators

| Operator | Dunder    | Signature                   |
|----------|-----------|-----------------------------|
| `a + b`  | `__add__` | `(self, other: T) -> T`     |
| `a - b`  | `__sub__` | `(self, other: T) -> T`     |
| `a * b`  | `__mul__` | `(self, scalar: S) -> T`    |
| `a / b`  | `__div__` | `(self, other: T) -> T`     |
| `a % b`  | `__mod__` | `(self, other: T) -> T`     |
| `a ** b` | `__pow__` | `(self, exp: T) -> T`       |
| `-a`     | `__neg__` | `(self) -> T`               |
| `+a`     | `__pos__` | `(self) -> T`               |

**How it works:**

```python
class Vec2:
    pub x: float
    pub y: float

extend Vec2:
    pub def init(x: float, y: float) -> Vec2:
        mut v = Vec2()
        v.x = x
        v.y = y
        return v

    pub def __add__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x + other.x, self.y + other.y)

    pub def __sub__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x - other.x, self.y - other.y)

    pub def __mul__(self, scalar: float) -> Vec2:
        return Vec2.init(self.x * scalar, self.y * scalar)

    pub def __neg__(self) -> Vec2:
        return Vec2.init(-self.x, -self.y)

mut a = Vec2.init(1.0, 2.0)
mut b = Vec2.init(3.0, 4.0)
mut c = a + b       # calls Vec2___add__(a, b)
mut d = a * 2.0     # calls Vec2___mul__(a, 2.0)
mut n = -a          # calls Vec2___neg__(a)
```

**Common Mistakes:** Returning a new value from `__add__` but accidentally mutating `self`. Arithmetic dunders should always return a new instance.

**Best Practices:** If you implement `__add__`, implement `__sub__` as well — partial operator sets confuse callers.

---

## Comparison Operators

| Operator | Dunder   | Signature                  |
|----------|----------|----------------------------|
| `a == b` | `__eq__` | `(self, other: T) -> bool` |
| `a != b` | `__ne__` | `(self, other: T) -> bool` |
| `a < b`  | `__lt__` | `(self, other: T) -> bool` |
| `a <= b` | `__le__` | `(self, other: T) -> bool` |
| `a > b`  | `__gt__` | `(self, other: T) -> bool` |
| `a >= b` | `__ge__` | `(self, other: T) -> bool` |

**How it works:**

```python
extend Vec2:
    pub def __eq__(self, other: Vec2) -> bool:
        return self.x == other.x and self.y == other.y

    pub def __ne__(self, other: Vec2) -> bool:
        return not (self.x == other.x and self.y == other.y)

    pub def __lt__(self, other: Vec2) -> bool:
        # Compare by magnitude squared
        mut mag_self  = self.x * self.x + self.y * self.y
        mut mag_other = other.x * other.x + other.y * other.y
        return mag_self < mag_other

mut eq = a == b    # calls Vec2___eq__(a, b)
mut ne = a != b    # calls Vec2___ne__(a, b)
```

**Common Mistakes:** Implementing `__eq__` but not `__ne__` — they should be consistent. A simple `__ne__` is `return not self.__eq__(other)`.

**Best Practices:** If you implement any of `__lt__`, `__le__`, `__gt__`, `__ge__`, implement all four to support sorting and range comparisons correctly.

---

## Boolean and Length

| Operation   | Dunder     | Signature        |
|-------------|------------|------------------|
| `bool(obj)` | `__bool__` | `(self) -> bool` |
| `len(obj)`  | `__len__`  | `(self) -> int`  |

`__bool__` is also called implicitly by `if`, `while`, `and`, `or`, and `not` when the operand is a class instance.

**How it works:**

```python
extend Vec2:
    pub def __bool__(self) -> bool:
        return self.x != 0.0 or self.y != 0.0

    pub def __len__(self) -> int:
        return 2    # a Vec2 always has 2 components

if a:              # calls Vec2___bool__(a)
    print("non-zero vector")
mut l = len(a)     # calls Vec2___len__(a) → 2
```

**When to use `__bool__`:** When your class has a natural "empty" or "zero" state — an empty collection, a zero vector, an unset option. Without `__bool__`, using a class instance in a boolean context is a compile error.

---

## String Representation

| Function      | Dunder     | Signature           |
|---------------|------------|---------------------|
| `str(obj)`    | `__str__`  | `(self) -> str`     |
| `repr(obj)`   | `__repr__` | `(self) -> str`     |
| `print(obj)`  | `__str__`  | called automatically|
| `f"...{obj}"` | `__str__`  | called automatically|

`__repr__` falls back to `__str__` if only one of the two is defined.

**How it works:**

```python
extend Vec2:
    pub def __str__(self) -> str:
        return "(" + str(self.x) + ", " + str(self.y) + ")"

    pub def __repr__(self) -> str:
        return "Vec2(x=" + str(self.x) + ", y=" + str(self.y) + ")"

print(a)                 # calls Vec2___str__(a)  → "(1.0, 2.0)"
mut s: str = str(a)      # calls Vec2___str__(a)
mut r: str = repr(a)     # calls Vec2___repr__(a)
print(f"vec={a}")        # calls Vec2___str__(a) inside the f-string
```

**Best Practices:** `__str__` should be human-readable. `__repr__` should be unambiguous and, if possible, look like valid Tauraro code to construct the value.

---

## Container Protocol

| Operation    | Dunder         | Signature                     |
|--------------|----------------|-------------------------------|
| `obj[i]`     | `__getitem__`  | `(self, i: int) -> T`         |
| `obj[i] = v` | `__setitem__`  | `(self, i: int, val: T)`      |
| `x in obj`   | `__contains__` | `(self, val: T) -> bool`      |

**How it works:**

```python
class Bag:
    pub items: List[int]
    pub count: int

extend Bag:
    pub def init() -> Bag:
        mut b = Bag()
        b.items = []
        b.count = 0
        return b

    pub def add(self, val: int) -> void:
        self.items.append(val)
        self.count = self.count + 1

    pub def __getitem__(self, i: int) -> int:
        return self.items.get(i)

    pub def __setitem__(self, i: int, val: int) -> void:
        self.items.set(i, val)

    pub def __contains__(self, val: int) -> bool:
        mut i = 0
        while i < self.count:
            if self.items.get(i) == val: return true
            i = i + 1
        return false

mut bag = Bag.init()
bag.add(10)
bag.add(20)
mut v     = bag[0]         # calls Bag___getitem__(bag, 0)
bag[0]    = 99             # calls Bag___setitem__(bag, 0, 99)
mut found = 10 in bag      # calls Bag___contains__(bag, 10)
```

**Common Mistakes:** Forgetting to bounds-check in `__getitem__` — an out-of-bounds access crashes at runtime.

---

## Iterator Protocol

**When to use:** Making a class work with `for` loops.

`__iter__` is called once to set up iteration (returns `self` for stateful iterators). `__next__` is called each iteration — it must return `Option[T]`: `Option.some(val)` to yield a value, `Option.none()` to stop.

**How it works:**

```python
class Counter:
    pub current: int
    pub stop:    int

extend Counter:
    pub def init(stop: int) -> Counter:
        mut c = Counter()
        c.current = 0
        c.stop    = stop
        return c

    pub def __iter__(self) -> Counter:
        self.current = 0
        return self

    pub def __next__(self) -> Option[int]:
        if self.current >= self.stop: return Option.none()
        mut val = self.current
        self.current = self.current + 1
        return Option.some(val)

for n in Counter.init(5):
    print(f"n={n}")    # prints 0, 1, 2, 3, 4
```

**Common Mistakes:** Returning a copy of `self` from `__iter__` — the copy won't share state with the original. Return `self` (the same instance).

**Best Practices:** If your iterator cannot be restarted (e.g., a file reader), `__iter__` should just return `self` without resetting state.

---

## Context Manager Protocol

**When to use:** Any resource that needs guaranteed cleanup — file handles, locks, connections, temporary state.

`__enter__` is called when entering the `with` block and its return value is bound to the alias. `__exit__` is called when the block exits (on both success and exception). Return `false` from `__exit__` to propagate any exception; return `true` to suppress it.

**How it works:**

```python
class FileCtx:
    pub name:   str
    pub opened: bool

extend FileCtx:
    pub def init(name: str) -> FileCtx:
        mut fc = FileCtx()
        fc.name   = name
        fc.opened = false
        return fc

    pub def __enter__(self) -> FileCtx:
        self.opened = true
        print(f"[open]  {self.name}")
        return self

    pub def __exit__(self, exc_type: str) -> void:
        self.opened = false
        print(f"[close] {self.name}")

with FileCtx.init("data.txt") as fc:
    print(f"reading: {fc.name}")    # fc.opened is true here
# [open]  data.txt
# reading: data.txt
# [close] data.txt
```

The compiler expands `with obj as alias:` to:
1. Store the expression in a temporary
2. Call `__enter__()` — bind the result to `alias`
3. Execute the body
4. Call `__exit__()` in a `finally:` block — runs on both normal exit and exception

**Common Mistakes:** Forgetting that `__exit__` is called even on exceptions — don't put logic in `__exit__` that assumes success.

---

## Callable Objects

**When to use:** When a class represents a behavior that should be invocable — callbacks, command objects, partial function application.

**How it works:**

```python
class Multiplier:
    pub factor: int

extend Multiplier:
    pub def init(factor: int) -> Multiplier:
        mut m = Multiplier()
        m.factor = factor
        return m

    pub def __call__(self, x: int) -> int:
        return x * self.factor

mut triple = Multiplier.init(3)
mut r = triple(7)    # calls Multiplier___call__(triple, 7) → 21
print(r)             # 21
```

**Common Mistakes:** Using `__call__` as a replacement for a plain function when there is no state to carry. If there's no state, use a plain `def`.

---

## Complete Dunder Reference

| Dunder         | Triggered by                                         |
|----------------|------------------------------------------------------|
| `__add__`      | `a + b`                                              |
| `__sub__`      | `a - b`                                              |
| `__mul__`      | `a * b`                                              |
| `__div__`      | `a / b`                                              |
| `__mod__`      | `a % b`                                              |
| `__pow__`      | `a ** b`                                             |
| `__neg__`      | `-a`                                                 |
| `__pos__`      | `+a`                                                 |
| `__eq__`       | `a == b`                                             |
| `__ne__`       | `a != b`                                             |
| `__lt__`       | `a < b`                                              |
| `__le__`       | `a <= b`                                             |
| `__gt__`       | `a > b`                                              |
| `__ge__`       | `a >= b`                                             |
| `__bool__`     | `if a`, `while a`, `not a`, `bool(a)`                |
| `__len__`      | `len(a)`                                             |
| `__str__`      | `str(a)`, `print(a)`, f-string `{a}`                 |
| `__repr__`     | `repr(a)`                                            |
| `__getitem__`  | `a[i]`                                               |
| `__setitem__`  | `a[i] = v`                                           |
| `__contains__` | `x in a`                                             |
| `__iter__`     | `for x in a:` (setup)                                |
| `__next__`     | `for x in a:` (advance — returns `Option[T]`)        |
| `__enter__`    | `with a as x:` (setup)                               |
| `__exit__`     | `with a as x:` (teardown)                            |
| `__call__`     | `a(args...)`                                         |

---

## Notes

- Dunders not defined on a class are silently skipped — the compiler falls back to its default behavior for that operation.
- Dunder method names are mangled in the generated C as `ClassName___dunder__` (three underscores between class name and method name).
- The `__iter__` + `__next__` protocol takes priority over the `__len__` + `__getitem__` sequence protocol when both are defined.
- See `examples/18_dunder_methods.tr` for a runnable demonstration of all dunders.

---

Next: [Advanced Docs Index →](advanced/README.md)

← [Advanced Patterns](20_advanced_patterns.md) | [README](README.md)
