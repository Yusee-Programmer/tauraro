# 21 — Operator Overloading (Dunder Methods)

---

## Overview

Tauraro supports Python-style **dunder methods** (double-underscore methods) that let user-defined
classes hook into built-in operators, functions, and language constructs.  When the compiler sees
an operator or a built-in call on a class instance, it checks whether the class defines the
corresponding dunder and, if so, generates a call to it instead of the default behaviour.

All dunder methods are declared inside an `extend` block, like any other method.

---

## Arithmetic Operators

| Operator | Dunder       | Signature                              |
|----------|--------------|----------------------------------------|
| `+`      | `__add__`    | `(self, other: T) -> T`                |
| `-`      | `__sub__`    | `(self, other: T) -> T`                |
| `*`      | `__mul__`    | `(self, scalar: S) -> T`               |
| `-x`     | `__neg__`    | `(self) -> T`                          |
| `+x`     | `__pos__`    | `(self) -> T`                          |

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

    pub def __neg__(self) -> Vec2:
        return Vec2.init(-self.x, -self.y)

mut a = Vec2.init(1.0, 2.0)
mut b = Vec2.init(3.0, 4.0)
mut c = a + b    # calls Vec2___add__(a, b)
mut n = -a       # calls Vec2___neg__(a)
```

---

## Comparison Operators

| Operator | Dunder    | Signature                         |
|----------|-----------|-----------------------------------|
| `==`     | `__eq__`  | `(self, other: T) -> bool`        |
| `!=`     | `__ne__`  | `(self, other: T) -> bool`        |
| `<`      | `__lt__`  | `(self, other: T) -> bool`        |
| `<=`     | `__le__`  | `(self, other: T) -> bool`        |
| `>`      | `__gt__`  | `(self, other: T) -> bool`        |
| `>=`     | `__ge__`  | `(self, other: T) -> bool`        |

```python
extend Vec2:
    pub def __eq__(self, other: Vec2) -> bool:
        return self.x == other.x and self.y == other.y

    pub def __ne__(self, other: Vec2) -> bool:
        return not (self.x == other.x and self.y == other.y)

mut eq = a == b   # calls Vec2___eq__(a, b)
mut ne = a != b   # calls Vec2___ne__(a, b)
```

---

## Boolean and Length

| Operation   | Dunder      | Signature               |
|-------------|-------------|-------------------------|
| `bool(obj)` | `__bool__`  | `(self) -> bool`        |
| `len(obj)`  | `__len__`   | `(self) -> int`         |

`__bool__` is also called implicitly by `if`, `while`, `and`, `or`, and `not` when the operand
is a class instance.

```python
extend Vec2:
    pub def __bool__(self) -> bool:
        return self.x != 0.0 or self.y != 0.0

    pub def __len__(self) -> int:
        return 2

if a:              # calls Vec2___bool__(a)
    ...
mut l = len(a)     # calls Vec2___len__(a)  → 2
```

---

## String Representation

| Function    | Dunder      | Signature           |
|-------------|-------------|---------------------|
| `str(obj)`  | `__str__`   | `(self) -> str`     |
| `repr(obj)` | `__repr__`  | `(self) -> str`     |
| `print(obj)`| `__str__`   | `(self) -> str`     |
| f-strings   | `__str__`   | `(self) -> str`     |

`__repr__` falls back to `__str__` if only one of the two is defined.

```python
extend Vec2:
    pub def __str__(self) -> str:
        return "Vec2(" + str(self.x) + ", " + str(self.y) + ")"

    pub def __repr__(self) -> str:
        return "Vec2(x=" + str(self.x) + ", y=" + str(self.y) + ")"

print(a)                 # calls Vec2___str__(a)
mut s: str = str(a)      # calls Vec2___str__(a)
mut r: str = repr(a)     # calls Vec2___repr__(a)
print(f"vec={a}")        # calls Vec2___str__(a) inside the f-string
```

---

## Container Protocol

| Operation       | Dunder         | Signature                       |
|-----------------|----------------|---------------------------------|
| `obj[i]`        | `__getitem__`  | `(self, i: int) -> T`           |
| `obj[i] = v`    | `__setitem__`  | `(self, i: int, val: T)`        |
| `x in obj`      | `__contains__` | `(self, val: T) -> bool`        |

```python
class Bag:
    pub items: List[int]
    pub count: int

extend Bag:
    pub def __getitem__(self, i: int) -> int:
        return self.items.get(i)

    pub def __setitem__(self, i: int, val: int):
        self.items.set(i, val)

    pub def __contains__(self, val: int) -> bool:
        mut i = 0
        while i < self.count:
            if self.items.get(i) == val: return true
            i += 1
        return false

mut bag = Bag.init()
bag.add(10)
mut v = bag[0]          # calls Bag___getitem__(bag, 0)
bag[0] = 99             # calls Bag___setitem__(bag, 0, 99)
mut found = 10 in bag   # calls Bag___contains__(bag, 10)
```

---

## Iterator Protocol

Implement `__iter__` and `__next__` to make a class work with `for` loops.

`__next__` must return `Option[T]` — `Option.some(val)` to yield a value, `Option.none()` to stop.

```python
class Counter:
    pub current: int
    pub stop: int

extend Counter:
    pub def init(stop: int) -> Counter:
        mut c = Counter()
        c.current = 0
        c.stop = stop
        return c

    pub def __iter__(self) -> Counter:
        self.current = 0
        return self

    pub def __next__(self) -> Option[int]:
        if self.current >= self.stop: return Option.none()
        mut val = self.current
        self.current += 1
        return Option.some(val)

for n in Counter.init(5):
    print(f"n={n}")    # prints 0..4
```

---

## Context Manager Protocol

Implement `__enter__` and `__exit__` to support the `with` statement.

`__exit__` receives three `str` arguments (`exc_type`, `exc_val`, `exc_tb`) and returns `bool`.
Return `false` to propagate any exception, `true` to suppress it.

```python
class FileCtx:
    pub name: str
    pub opened: bool

extend FileCtx:
    pub def init(name: str) -> FileCtx:
        mut fc = FileCtx()
        fc.name = name
        fc.opened = false
        return fc

    pub def __enter__(self) -> FileCtx:
        self.opened = true
        print(f"[open]  {self.name}")
        return self

    pub def __exit__(self, exc_type: str, exc_val: str, exc_tb: str) -> bool:
        self.opened = false
        print(f"[close] {self.name}")
        return false

with FileCtx.init("data.txt") as fc:
    print(f"opened: {fc.opened}")
# [open]  data.txt
# opened: true
# [close] data.txt
```

The compiler expands the `with` statement to:
1. Store the context expression in a temporary
2. Call `__enter__` and bind the result to the alias (`fc`)
3. Execute the body
4. Call `__exit__` with `NULL, NULL, NULL` in normal flow

---

## Callable Objects

Implement `__call__` to make instances callable like functions.

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
```

---

## Complete Dunder Reference

| Dunder         | Triggered by                          |
|----------------|---------------------------------------|
| `__add__`      | `a + b`                               |
| `__sub__`      | `a - b`                               |
| `__mul__`      | `a * b`                               |
| `__neg__`      | `-a`                                  |
| `__pos__`      | `+a`                                  |
| `__eq__`       | `a == b`                              |
| `__ne__`       | `a != b`                              |
| `__lt__`       | `a < b`                               |
| `__le__`       | `a <= b`                              |
| `__gt__`       | `a > b`                               |
| `__ge__`       | `a >= b`                              |
| `__bool__`     | `if a`, `while a`, `not a`, `bool(a)` |
| `__len__`      | `len(a)`                              |
| `__str__`      | `str(a)`, `print(a)`, f-string `{a}`  |
| `__repr__`     | `repr(a)`                             |
| `__getitem__`  | `a[i]`                                |
| `__setitem__`  | `a[i] = v`                            |
| `__contains__` | `x in a`                              |
| `__iter__`     | `for x in a:` (setup)                 |
| `__next__`     | `for x in a:` (advance, returns `Option[T]`) |
| `__enter__`    | `with a as x:` (setup)                |
| `__exit__`     | `with a as x:` (teardown)             |
| `__call__`     | `a(args...)`                          |

---

## Notes

- Dunders that are not defined on a class are silently skipped — the compiler falls back to its default behaviour for that operation.
- Dunder method names are mangled in the generated C as `ClassName___dunder__` (three underscores between class and method name).
- The `__iter__` + `__next__` protocol takes priority over the `__len__` + `__getitem__` sequence protocol when both are defined.
- See `examples/18_dunder_methods.tr` for a runnable demonstration of all dunders.
