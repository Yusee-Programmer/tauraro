# 08 — Classes and Extend

Classes are the primary way to define custom data types in Tauraro. Unlike most OOP languages,
Tauraro separates **data layout** (`class`) from **behavior** (`extend`). This separation is
deliberate and has real advantages for large codebases.

---

## 1. class and extend — The Split Design

### When to use

Use `class` + `extend` whenever you need a named, reusable data structure with associated
operations. This applies to virtually any non-trivial type: domain models, data containers,
I/O handles, state machines, graph nodes, and so on.

### How it works

`class Foo:` declares **field names and types** — nothing else. The result maps directly to a
C struct with no indirection.

`extend Foo:` attaches **methods** to that struct. Multiple `extend` blocks can exist in the same
file or spread across different files; they all contribute to the same C namespace (`Foo_*`).

```python
# Data layout — maps 1:1 to a C struct
class Point:
    pub x: int
    pub y: int

# Behavior — generates C functions Point_init, Point_distance_sq, etc.
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

**Why the split matters:**

1. You can read the `class` block to understand memory layout without scrolling through methods.
2. Multiple `extend` blocks let you organise methods by concern (construction, I/O, serialisation, etc.).
3. The `class` block maps exactly to the C struct — no hidden fields, no vtable pointer, no surprises.

### Common Mistakes

**Declaring methods inside `class:`**
```python
# WRONG — class body is for fields only
class Point:
    pub x: int
    pub y: int
    pub def init(x: int, y: int) -> Point:  # ERROR: methods not allowed inside class
        ...
```
Fix: move all `def` declarations into an `extend Point:` block.

**Forgetting `mut` on class instances**
```python
p = Point.init(3, 4)
p.x = 10              # ERROR: cannot assign field of immutable binding 'p'
```
Fix: `mut p = Point.init(3, 4)`

### Best Practices

- Keep `class` blocks short — one screen of field declarations is ideal.
- Split `extend` blocks by concern: one for construction, one for mutation, one for I/O.
- Name the canonical constructor `init` for consistency with the standard library.
- Use `pub class` when the class will be imported from other modules.

---

## 2. Field Declarations

### When to use

Every piece of data the class owns must be declared as a field. Think of the `class` block as the
memory blueprint: it determines exactly how much space each instance occupies and what names exist
to access that space.

### How it works

Fields are **private by default**. Add `pub` to expose a field outside the module.

```python
class Config:
    pub host:    str       # accessible from any module
    pub port:    i32       # accessible from any module
    pub timeout: float     # accessible from any module
    debug:       bool      # private — only accessible in this file
    internal_id: int       # private

class Node:
    pub value: int
    pub next:  Node        # pointer to another Node (self-referential)
    pub items: List[int]   # embedded list
    pub label: str         # string pointer
```

Any Tauraro type is valid as a field type: primitives (`int`, `float`, `bool`, `str`, `i32`,
`i64`, `f32`, `f64`), other classes, `List[T]`, `Map[V]`, `Option[T]`, enums, and `Pointer[T]`
for manual memory management.

### Common Mistakes

**Accessing a private field from another module**
```python
# file_a.tr
class Config:
    secret: str    # private
    pub host: str

# file_b.tr
import file_a
mut cfg = ...
print(cfg.secret)    # ERROR: field 'secret' of 'Config' is private
```
Fix: add `pub` to the field, or expose a public accessor method in `extend Config:`.

**Using a class type before declaring it** — the compiler uses two-pass resolution, so forward
references within the same file are fine. Cross-file forward references require the file to be
imported.

### Best Practices

- Default to private fields; expose only what callers genuinely need.
- Group logically related fields together and add a blank line between groups.
- Use the most precise numeric type for fields (`i32` instead of `int` when overflow is impossible).
- Document non-obvious fields with an inline comment.

---

## 3. Instance Methods and `self`

### When to use

Add an instance method whenever the operation needs to read or mutate the fields of a specific
object. Instance methods are the primary interface callers use to interact with a class.

### How it works

The first parameter of an instance method must be `self`. The compiler infers its type from the
enclosing `extend` block — you do not annotate it. Inside the method, access fields and call
other methods via `self.field` and `self.method()`.

```python
extend Point:
    pub def distance_sq(self) -> int:
        return self.x * self.x + self.y * self.y

    pub def translate(self, dx: int, dy: int) -> void:
        self.x = self.x + dx
        self.y = self.y + dy

    pub def describe(self) -> void:
        print(f"Point({self.x}, {self.y})")
```

Calling an instance method:
```python
mut p = Point.init(3, 4)
p.describe()               # compiles to: Point_describe(p)
mut dsq = p.distance_sq()  # compiles to: Point_distance_sq(p)
p.translate(1, -1)         # compiles to: Point_translate(p, 1, -1)
```

All method dispatch is **static** — resolved at compile time based on the declared type of `p`.
There is no virtual dispatch unless you use an [Interface](10_interfaces.md).

### Common Mistakes

**Omitting `self` on an instance method** — the method becomes a static method. The compiler will
not warn; you will get a linker error when trying to call it on an instance.

**Calling a static method on an instance**
```python
p = Point.init(1, 2)
q = p.origin()       # ERROR: 'origin' is a static method — call as Point.origin()
```
Fix: `q = Point.origin()`

### Best Practices

- Keep instance methods focused on a single responsibility.
- Prefer returning a value over mutating `self` when possible — it makes data flow easier to follow.
- If a method is logically part of construction or teardown, keep it near `init`.

---

## 4. Static Methods

### When to use

Use a static method for operations that belong to the class conceptually but do not require an
existing instance: constructors, factory functions, constants, and utility helpers.

### How it works

A method in `extend` with **no `self` parameter** is a static method. Call it on the class name,
not on an instance. The `@staticmethod` decorator is optional but makes intent explicit.

```python
extend Point:
    pub def origin() -> Point:           # static — no self
        return Point.init(0, 0)

    @staticmethod
    pub def from_polar(r: float, theta: float) -> Point:
        mut p = Point()
        p.x = (r * 3.14159 * theta) as int
        p.y = r as int
        return p

mut p = Point.origin()              # Point_origin()
mut q = Point.from_polar(5.0, 0.0) # Point_from_polar(5.0, 0.0)
```

### Common Mistakes

**Calling a static method on an instance** — see above.

**Using `self` accidentally inside a static method** — `self` is not in scope; this is a compile
error.

### Best Practices

- Name the primary constructor `init` to match the standard library convention.
- Use descriptive names for alternate constructors: `from_str`, `from_file`, `zero`, `default`.
- Mark all static methods with `@staticmethod` for clarity, even though it is optional.

---

## 5. The Constructor Pattern

### When to use

Tauraro has no built-in `new` keyword or constructor syntax. The standard pattern is a static
method named `init` that allocates an instance, fills its fields, and returns it. Use this
pattern for every class that needs non-trivial initialisation.

### How it works

`ClassName()` allocates a new **zero-initialized** instance on the heap and returns a pointer to
it. Allocation failures abort the program immediately — you never check for null.

```python
extend Point:
    pub def init(x: int, y: int) -> Point:
        mut p = Point()    # heap-allocate a zero-initialized Point
        p.x = x
        p.y = y
        return p           # return the pointer
```

**Multiple constructors** — add more static methods with different names:

```python
extend Point:
    pub def init(x: int, y: int) -> Point:
        mut p = Point(); p.x = x; p.y = y; return p

    pub def from_float(x: float, y: float) -> Point:
        return Point.init(x as int, y as int)

    pub def zero() -> Point:
        return Point.init(0, 0)
```

**Calling the constructor:**
```python
mut p = Point.init(3, 4)
mut origin = Point.zero()
mut q = Point.from_float(1.5, 2.7)
```

### Common Mistakes

**Writing `mut p = ClassName` instead of `mut p = ClassName()`** — `ClassName` is a type name,
not an expression. You must use `ClassName()` (with parentheses) to allocate.

**Forgetting to assign fields after `ClassName()`** — instances start zero-initialized. A `str`
field will be an empty string, numeric fields will be `0`. Always set every field in `init`.

### Best Practices

- Always name the primary constructor `init`.
- Initialize every field in `init`, even if the zero value is acceptable — it makes the layout
  readable.
- Validate arguments at the top of `init` before touching any fields.

---

## 6. Visibility (`pub`)

### When to use

Use `pub` on fields and methods that form the public API of the class. Leave everything else
private. Tight visibility boundaries reduce coupling and make refactoring easier.

### How it works

| Declaration | Effect |
|-------------|--------|
| `pub class Foo` | `Foo` is importable from other modules |
| `pub field: Type` | Field is accessible from other modules |
| `pub def method` in `extend` | Method is callable from other modules |
| (no `pub`) | Only accessible within the same `.tr` file |

The compiler enforces visibility at compile time:
```
ERROR: field 'step' of 'Counter' is private — cannot access from outside its module
```

Full example:
```python
class Counter:
    pub  value: int       # public — readable/writable from anywhere
    step: int             # private — internal implementation detail

extend Counter:
    pub def init(start: int, s: int) -> Counter:
        mut c = Counter()
        c.value = start
        c.step = s         # OK: we are inside the same module
        return c

    pub def increment(self) -> void:
        self.value = self.value + self.step

    def _apply_boost(self) -> void:    # private method
        self.step = self.step * 2
```

### Common Mistakes

**Making every field public "for convenience"** — this breaks encapsulation. Callers can then
bypass the class's invariants.

**Forgetting `pub` on `init`** — if `init` is not `pub`, callers in other modules cannot
construct the class.

### Best Practices

- Make the minimal set of fields public that callers actually need to read.
- Prefer exposing mutation through methods rather than public fields.
- Always mark `init` and other primary constructors as `pub`.

---

## 7. Multiple `extend` Blocks

### When to use

Split `extend` blocks when a class has many methods that naturally fall into separate concerns:
construction, mutation, serialisation, I/O, and so on. This is especially useful for large classes
or when different concerns are maintained by different team members.

### How it works

Multiple `extend` blocks for the same class are all merged into the same C namespace. There is no
cost and no constraint — you can add as many as you need, in any order.

```python
class Buffer:
    pub data:     List[int]
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

### Common Mistakes

**Expecting ordering to matter** — all `extend` blocks are merged. A method in the second
`extend` block can call a method from the first without any special import.

### Best Practices

- Use one `extend` block per logical concern and add a short comment identifying that concern.
- In multi-file layouts, keep construction methods in the same file as `class`.

---

## 8. Inheritance via Base Classes (Struct Embedding)

### When to use

Use struct embedding (`class Dog(Animal):`) when you want a type to carry all of another type's
fields and you want to call the parent type's methods on it. This is suitable for "is-a"
relationships where you control both types.

For true runtime polymorphism across different types, use [Interfaces](10_interfaces.md) instead.

### How it works

`class Dog(Animal):` causes `Dog` to include all of `Animal`'s fields as a prefix. Methods
declared in `extend Animal:` can be called on a `Dog` by casting to `Animal`. This is C **struct
embedding**, not virtual dispatch.

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

class Dog(Animal):    # Dog struct embeds Animal's fields first
    pub breed: str

extend Dog:
    pub def init(n: str, b: str) -> Dog:
        mut d = Dog()
        d.name = n     # inherited field, directly accessible
        d.breed = b
        return d

    pub def bark(self) -> void:
        print(f"{self.name} ({self.breed}): Woof!")

def main():
    mut d = Dog.init("Rex", "Shepherd")
    d.bark()
    d.breathe()    # Animal method called on Dog via upcast
```

**`super.method()`** calls the parent's method directly. When there is ambiguity with multiple
base classes, use `super[BaseClass].method()` to disambiguate.

### Common Mistakes

**Expecting virtual dispatch through a base class pointer** — Tauraro does not generate vtables
for struct embedding. Assign to an interface type if you need dynamic dispatch.

**Accessing `super` outside of `extend`** — `super` is only valid inside a method body in an
`extend` block.

### Best Practices

- Limit inheritance depth to one or two levels; deep hierarchies are hard to reason about.
- Prefer composition (a field of type `Animal`) over inheritance when the "is-a" relationship is
  not essential.
- Use `super[Base].method()` explicitly when overriding a method that exists in a parent — it
  makes the call chain visible to readers.

---

## 9. Operator Overloading (Dunder Methods)

### When to use

Add dunder methods to a class when you want it to participate in built-in language syntax:
arithmetic operators, comparisons, iteration, string conversion, index access, and so on.
Dunders are most valuable for value-like types such as vectors, matrices, durations, and
custom numeric types.

### How it works

Methods whose names start and end with `__` are **dunder methods**. The compiler automatically
dispatches to them when the corresponding operation is applied to an instance of the class.

```python
class Vec2:
    pub x: float
    pub y: float

extend Vec2:
    pub def init(x: float, y: float) -> Vec2:
        mut v = Vec2(); v.x = x; v.y = y; return v

    pub def __add__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x + other.x, self.y + other.y)

    pub def __sub__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x - other.x, self.y - other.y)

    pub def __mul__(self, scalar: float) -> Vec2:
        return Vec2.init(self.x * scalar, self.y * scalar)

    pub def __eq__(self, other: Vec2) -> bool:
        return self.x == other.x and self.y == other.y

    pub def __str__(self) -> str:
        return "Vec2(" + str(self.x) + ", " + str(self.y) + ")"

    pub def __bool__(self) -> bool:
        return self.x != 0.0 or self.y != 0.0

    pub def __len__(self) -> int:
        return 2

def main():
    mut a = Vec2.init(1.0, 2.0)
    mut b = Vec2.init(3.0, 4.0)

    mut c = a + b          # calls __add__
    mut d = a * 2.0        # calls __mul__
    print(c)               # calls __str__
    if a:                  # calls __bool__
        print("non-zero")
```

**Complete list of supported dunders:**

| Dunder | Triggered by |
|--------|-------------|
| `__add__`, `__sub__`, `__mul__`, `__div__`, `__mod__` | `+`, `-`, `*`, `/`, `%` |
| `__neg__` | unary `-` |
| `__eq__`, `__ne__`, `__lt__`, `__le__`, `__gt__`, `__ge__` | `==`, `!=`, `<`, `<=`, `>`, `>=` |
| `__bool__` | `if obj:`, `not obj`, `while obj:` |
| `__len__` | `len(obj)` |
| `__str__`, `__repr__` | `str(obj)`, `print(obj)` |
| `__getitem__`, `__setitem__` | `obj[i]`, `obj[i] = v` |
| `__contains__` | `x in obj` |
| `__iter__`, `__next__` | `for x in obj:` |
| `__enter__`, `__exit__` | `with obj:` |
| `__call__` | `obj(args...)` |

See [21 — Operator Overloading](21_operator_overloading.md) for the complete reference.

### Common Mistakes

**Wrong return type on `__eq__`** — it must return `bool`, not `int`.

**Forgetting `__str__` causes `print(obj)` to print a raw pointer address** — always define
`__str__` on types you intend to print.

### Best Practices

- Only implement dunders that genuinely make semantic sense for the type.
- Implement `__str__` for any type you will print during debugging.
- Implement `__eq__` whenever you implement `__lt__` — code that sorts will also compare for equality.
- Keep dunder methods short; delegate complex logic to named helpers.

---

## 10. C Code Generation Reference

Understanding what Tauraro emits helps you reason about performance and debug C-level issues.

| Tauraro | Generated C |
|---------|-------------|
| `class Point:` | `typedef struct Point { ... } Point;` |
| `pub x: int` | `int64_t x;` in the struct |
| `extend Point:` | Functions prefixed `Point_` |
| `def init(...)` in `extend Point:` | `Point* Point_init(...)` |
| `def describe(self) -> void` | `void Point_describe(Point* self)` |
| `p.describe()` | `Point_describe(p)` |
| `p.x` | `p->x` |
| `Point()` | `_tr_alloc(sizeof(Point)); memset(0)` |

There is no vtable in the struct unless an interface is involved. All dispatch is a direct
function call — zero overhead compared to hand-written C.

---

Next: [Enums →](09_enums.md)
