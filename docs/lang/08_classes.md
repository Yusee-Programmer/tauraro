# 08 — Classes and Extend

---

## Why class + extend Are Separate

Most languages combine field declarations and methods into one block. Tauraro separates them:

- `class Foo:` declares the **data layout** — field names and types only
- `extend Foo:` attaches **methods** to the class

**Benefits:**
1. You can read the `class` block to understand memory layout without scrolling through methods
2. Multiple `extend` blocks can live in different files, letting you organise methods by concern
3. The `class` block maps exactly to the C struct — no indirection, no surprises

---

## Declaring a Class

```python
class Point:
    pub x: int
    pub y: int

class Person:
    pub name: str
    pub age:  int
    active: bool    # private — not accessible outside this module
```

Fields are **private by default**. Add `pub` to expose them.

### Field Types

Any Tauraro type is valid as a field type:

```python
class Node:
    pub value: int
    pub next:  Node         # pointer to another Node
    pub items: List[int]    # embedded list
    pub label: str          # string pointer

class Config:
    pub host:    str
    pub port:    i32
    pub timeout: float
    pub debug:   bool
    pub tags:    List[str]
```

---

## Adding Methods with extend

```python
extend Point:
    pub def init(px: int, py: int) -> Point:
        mut p = Point()
        p.x = px
        p.y = py
        return p

    pub def distance_sq(self) -> int:
        return self.x * self.x + self.y * self.y

    pub def translate(self, dx: int, dy: int) -> void:
        self.x = self.x + dx
        self.y = self.y + dy

    pub def describe(self) -> void:
        print(f"Point({self.x}, {self.y})")
```

### The `self` Parameter

The first parameter of an instance method must be `self`. It receives a pointer to the current instance. You don't need to annotate its type — the compiler infers it from the enclosing `extend` block.

You always write `.` to access fields and call methods on `self`.

### Methods Without `self` (Static Methods)

A method in `extend` that has no `self` parameter is a **static method** — called on the class name, not an instance:

```python
extend Point:
    pub def origin() -> Point:        # static method (no self)
        return Point.init(0, 0)

    pub def from_tuple(t: Tuple) -> Point:    # static method
        return Point.init(t.x, t.y)

mut p = Point.origin()    # calling a static method
```

Or more explicitly with the `@staticmethod` decorator:

```python
extend Point:
    @staticmethod
    pub def from_polar(r: float, theta: float) -> Point:
        mut p = Point()
        p.x = (r * 3.14159 * theta) as int    # simplified
        p.y = r as int
        return p
```

---

## The Constructor Pattern

Tauraro has no built-in constructor mechanism. The convention is a static method named `init` that allocates and returns an instance:

```python
extend Point:
    pub def init(x: int, y: int) -> Point:
        mut p = Point()    # allocate a zero-initialized Point on the heap
        p.x = x
        p.y = y
        return p
```

`Point()` allocates a new zero-initialized instance on the heap. Allocation failures abort immediately — you never need to check for null.

**Calling the constructor:**
```python
mut p = Point.init(3, 4)     # calls Point_init(3, 4) in C
p.describe()
```

**Multiple constructors:** Just add more static methods with different names:

```python
extend Point:
    pub def init(x: int, y: int) -> Point: ...
    pub def from_float(x: float, y: float) -> Point: ...
    pub def zero() -> Point:
        return Point.init(0, 0)
```

---

## Using a Class

```python
def main():
    mut p = Point.init(3, 4)

    # Method calls:
    p.describe()               # Point_describe(p) in C
    mut dsq = p.distance_sq()  # Point_distance_sq(p) in C
    p.translate(1, -1)         # Point_translate(p, 1, -1) in C

    # Field access:
    print(p.x)                 # p->x in C
    p.y = 10                   # p->y = 10 in C

    print(f"distance sq = {dsq}")
```

All method dispatch is **static** — resolved at compile time based on the declared type of `p`. There is no virtual dispatch unless you use an interface.

---

## Visibility (`pub`)

```python
class Counter:
    pub  value: int       # accessible from any module
    step: int             # private — only accessible in this file

extend Counter:
    pub def init(start: int, step: int) -> Counter:
        mut c = Counter()
        c.value = start
        c.step = step       # OK: within the same module
        return c

    pub def increment(self) -> void:     # public method
        self.value = self.value + self.step

    def _do_internal(self) -> void:      # private method
        self.step = self.step * 2
```

**Visibility rules:**
- `pub class` — class is importable from other modules
- `pub field` — field is accessible from other modules via an instance
- `pub def` in `extend` — method is callable from other modules
- A field without `pub` is accessible only within the same `.tr` file
- A method without `pub` is callable only within the same `.tr` file
- The compiler enforces visibility at compile time

**Compiler error for visibility violation:**
```
ERROR: field 'step' of 'Counter' is private — cannot access from outside its module
```

---

## Multiple extend Blocks

You can have multiple `extend` blocks for the same class in the same file:

```python
class Buffer:
    pub data: List[int]
    pub capacity: int

extend Buffer:    # construction and writing
    pub def init(cap: int) -> Buffer:
        mut b = Buffer()
        b.data = []
        b.capacity = cap
        return b

    pub def push(self, v: int) -> void:
        self.data.append(v)

extend Buffer:    # reading and inspection
    pub def get(self, i: int) -> int:
        return self.data[i]

    pub def len(self) -> int:
        return len(self.data)

    pub def describe(self) -> void:
        print(f"Buffer(len={len(self.data)}, cap={self.capacity})")
```

All `extend` blocks for the same class are merged — they share the same C namespace (`Buffer_*`).

---

## Inheritance via base classes

Tauraro does not have classical inheritance. Instead, you can embed one class inside another using `base_classes`:

```python
class Animal:
    pub name: str

extend Animal:
    pub def init(n: str) -> Animal:
        mut a = Animal()
        a.name = n
        return a
    pub def breathe(self) -> void:
        print(f"{self.name} breathes")

class Dog(Animal):    # Dog embeds Animal
    pub breed: str

extend Dog:
    pub def init(n: str, b: str) -> Dog:
        mut d = Dog()
        d.name = n     # inherited field
        d.breed = b
        return d

    pub def bark(self) -> void:
        print(f"{self.name} ({self.breed}): Woof!")
```

The `Dog(Animal)` syntax causes `Dog` to include all of `Animal`'s fields. Methods from `Animal` can be called on a `Dog` by casting to `Animal` type.

This is **struct embedding**, not dynamic dispatch. For polymorphism across types, use interfaces (see [Interfaces](10_interfaces.md)).

---

---

## Common Class Errors

### Missing `mut` on class instance

```python
p = Point.init(3, 4)
p.x = 10              # ERROR: cannot assign field of immutable binding 'p'
```
**Fix:** `mut p = Point.init(3, 4)`

### Accessing private field from another module

```python
# in file_a.tr:
class Config:
    secret: str    # private
    pub host: str

# in file_b.tr:
import file_a
cfg = ...
print(cfg.secret)    # ERROR: 'secret' is private
```
**Fix:** Add `pub` to the field, or add a public accessor method.

### Calling static method on instance

```python
p = Point.init(1, 2)
q = p.origin()       # ERROR: 'origin' is a static method — call as Point.origin()
```
**Fix:** `q = Point.origin()`

### Using class before extend

Classes can be declared and used before their `extend` block — the compiler resolves declarations in two passes. No issues there.

---

## Operator Overloading (Dunder Methods)

Classes can hook into operators and built-in functions by defining **dunder methods** — methods
whose names start and end with double underscores. The compiler automatically calls them when the
corresponding operation is used on a class instance.

```python
class Vec2:
    pub x: float
    pub y: float

extend Vec2:
    pub def __add__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x + other.x, self.y + other.y)

    pub def __str__(self) -> str:
        return "Vec2(" + str(self.x) + ", " + str(self.y) + ")"

    pub def __bool__(self) -> bool:
        return self.x != 0.0 or self.y != 0.0

mut a = Vec2.init(1.0, 2.0)
mut b = Vec2.init(3.0, 4.0)
mut c = a + b          # dispatches to __add__
print(c)               # dispatches to __str__
if a:                  # dispatches to __bool__
    ...
```

Supported dunders include `__add__`, `__sub__`, `__mul__`, `__neg__`, `__eq__`, `__ne__`,
`__lt__`, `__le__`, `__gt__`, `__ge__`, `__bool__`, `__len__`, `__str__`, `__repr__`,
`__getitem__`, `__setitem__`, `__contains__`, `__iter__`, `__next__`, `__enter__`, `__exit__`,
and `__call__`.

See [21 — Operator Overloading](21_operator_overloading.md) for the complete reference.

---

Next: [Enums →](09_enums.md)
