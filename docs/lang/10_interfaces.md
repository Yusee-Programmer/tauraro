# 10 — Interfaces

---

## Why Interfaces

Tauraro uses **interfaces** for polymorphism — the ability to write code that works with many
different concrete types through a shared contract.

Tauraro's model:
- One keyword to declare the contract: `interface`
- One keyword to fulfil it: `implements` (in the class header)
- The compiler handles the rest — vtable generation, wrapping, dispatch

**When to use interfaces:**
- A function needs to accept multiple unrelated classes through a uniform API
- You're building a plugin system, strategy pattern, or extensible dispatch
- You want to abstract over a collection of different types

**When NOT to use interfaces:**
- You only have one concrete type — use direct method calls (zero overhead)
- You need data sharing between types — use struct embedding or a shared class

---

## Declaring an Interface

```python
interface Animal:
    def speak(self) -> void
    def name(self) -> str
    def legs(self) -> int
```

An interface declares **method signatures** only — no implementations, no fields.

---

## Implementing an Interface

Put `implements InterfaceName` directly in the class header:

```python
class Dog implements Animal:
    pub label: str
    pub breed: str

extend Dog:
    pub def init(n: str, breed: str) -> Dog:
        mut d = Dog()
        d.label = n
        d.breed = breed
        return d

    pub def speak(self) -> void: print(f"  {d.label}: Woof!")
    pub def name(self) -> str:   return self.label
    pub def legs(self) -> int:   return 4
```

Multiple interfaces are comma-separated:

```python
class Employee implements Printable, Serializable:
    pub name:   str
    pub salary: int
```

**Compiler rule:** The compiler does not verify that all interface methods are implemented at
compile time — a missing method produces a linker error. Implement all methods listed in the
interface.

---

## Automatic Interface Conversion

Once a class declares `implements`, the compiler **automatically converts** class instances to
the interface type wherever one is needed. You never write any explicit cast or wrapper call.

### Passing to a function

```python
def introduce(a: Animal) -> void:
    print(f"  {a.name()} has {a.legs()} legs")
    a.speak()

mut dog = Dog.init("Rex", "Shepherd")
mut cat = Cat.init("Whiskers")

introduce(dog)    # Dog → Animal: auto-converted at the call site
introduce(cat)    # Cat → Animal: auto-converted at the call site
```

### Storing in an interface-typed variable

```python
mut a: Animal = dog    # Dog → Animal: auto-converted
a.speak()              # dispatches through vtable: Dog_speak
```

### Multiple concrete types, same interface

```python
def loudest_first(a: Animal, b: Animal) -> void:
    a.speak()
    b.speak()

mut dog = Dog.init("Rex", "Shepherd")
mut cat = Cat.init("Whiskers")
mut bird = Bird.init("Tweety", true)

loudest_first(dog, cat)     # both auto-converted
```

---

## How It Works

When the compiler sees `class Dog implements Animal:`, it generates a vtable for `Dog` (one function pointer per interface method) and a fat-pointer that pairs the object with its vtable. The conversion from `Dog` to `Animal` is automatically inserted at every call site — you never write it manually. Method calls on an interface variable go through the vtable: one pointer dereference, one function call — identical cost to C++ virtual dispatch.

---

## Interface-Typed Variables and Collections

When storing interface values in variables or collections, declare the type explicitly so the
compiler knows which wrapper to apply:

```python
# Declare the variable with the interface type — auto-wraps on assignment
mut a_dog: Animal = dog
mut a_cat: Animal = cat

# After the assignment, a_dog is an Animal — vtable dispatch from here on
a_dog.speak()
print(f"  {a_dog.name()} has {a_dog.legs()} legs")
```

For `List[Interface]`, convert to interface type first:

```python
mut animals: List[Animal] = []
mut a_dog: Animal = dog      # wrap once
mut a_cat: Animal = cat
animals.append(a_dog)
animals.append(a_cat)

mut i = 0
while i < len(animals):
    mut a = animals[i]
    a.speak()
    i = i + 1
```

---

## Multiple Interfaces per Class

Each call site independently selects the correct interface:

```python
interface Printable:
    def print_info(self) -> void

interface Serializable:
    def serialize(self) -> str

class Employee implements Printable, Serializable:
    pub name:   str
    pub salary: int

extend Employee:
    pub def init(n: str, s: int) -> Employee:
        mut e = Employee()
        e.name = n; e.salary = s
        return e

    pub def print_info(self) -> void:
        print(f"  {self.name}: ${self.salary}")

    pub def serialize(self) -> str:
        return f"{self.name},{self.salary}"

def print_one(x: Printable) -> void:   x.print_info()
def to_json(x: Serializable) -> str:   return x.serialize()

mut emp = Employee.init("Alice", 95000)

print_one(emp)              # Employee → Printable, auto-converted
mut s = to_json(emp)        # Employee → Serializable, auto-converted
```

---

## Interface Values Cannot Recover the Concrete Type

Interface values erase the concrete type — you cannot cast an `Animal` back to a `Dog` at runtime. If you need type-tagged dispatch, use an enum:

```python
# Use an enum when you need to recover the concrete type:
enum AnimalKind:
    IsDog(d: Dog)
    IsCat(c: Cat)

match kind:
    case AnimalKind.IsDog(d): d.fetch()
    case AnimalKind.IsCat(c): c.purr()
```

---

## Generic Interfaces

Interfaces can be parameterized with type parameters: `interface Container[T]:`.

A generic class implements a generic interface by declaring it in the class header:

```python
interface Container[T]:
    def push(self, item: T) -> void
    def pop(self) -> T
    def peek(self) -> T
    def size(self) -> int
    def is_empty(self) -> bool

class Stack[T] implements Container[T]:
    pub items: Vec[T]
    pub count: int

extend Stack:
    pub def init() -> Stack[T]:
        mut s = Stack[T](); s.items = Vec[T].init(8); s.count = 0; return s
    pub def push(self, item: T) -> void:
        self.items.push(item); self.count = self.count + 1
    pub def pop(self) -> T:
        self.count = self.count - 1; return self.items.pop()
    pub def peek(self) -> T: return self.items.get(self.count - 1)
    pub def size(self) -> int: return self.count
    pub def is_empty(self) -> bool: return self.count == 0
```

The compiler generates a **monomorphized vtable** for each concrete type used (e.g., `Container_i64_vtable`
for `Container[int]`). Auto-conversion and vtable dispatch work identically to non-generic interfaces:

```python
def drain_all(c: Container[int]) -> void:
    while not c.is_empty():
        print(f"  popped: {c.pop()}")

mut s = Stack[int].init()
s.push(10); s.push(20); s.push(30)

drain_all(s)             # Stack[int] → Container[int], auto-converted
mut c: Container[int] = s    # auto-converted at assignment
print(f"  top={c.peek()}")   # vtable dispatch, returns int correctly
```

**Rules for generic interfaces:**
- Declare type params in brackets: `interface Container[T]:`
- Use the same brackets in `implements`: `class Stack[T] implements Container[T]:`
- The type args after `implements` are consumed by the parser and used for validation only
- Each monomorphized usage (e.g., `Container[int]`, `Container[str]`) generates its own vtable struct

---

## Interface Limitations

1. **No default method implementations** — every implementing class must define all methods.

2. **No interface fields** — interfaces are method-only contracts.

3. **No default generic method implementations** — each generic interface method must be fully implemented by every implementing class.

4. **No runtime type recovery** — you cannot downcast an interface back to its concrete type.

5. **Concrete object lifetime** — the concrete `Dog` must outlive any `Animal` value wrapping it.
   The interface holds a raw pointer; the compiler does not enforce this relationship. Keep
   concrete objects in a scope that outlives all interface values derived from them.

---

## Common Errors

### Missing `implements` in the class header

```python
class Bird:          # forgot "implements Animal"
    pub label: str
    ...

introduce(bird)    # ERROR: no Dog_as_Animal wrapper generated — linker error
```
**Fix:** `class Bird implements Animal:`

### Implementing the wrong signature

```python
interface Animal:
    def speak(self) -> void

extend Dog:
    pub def speak(self) -> str:    # ERROR: should return void
        return "Woof"
```
**Fix:** Match the interface signature exactly.

### Constructing from the interface name

```python
mut a = Animal.init("Rex")    # ERROR: Animal is an interface, not a class
```
**Fix:** `mut dog = Dog.init("Rex")`

---

## Full Example

```python
interface Shape:
    def area(self) -> float
    def describe(self) -> void

class Circle implements Shape:
    pub radius: float

extend Circle:
    pub def init(r: float) -> Circle:
        mut c = Circle(); c.radius = r; return c

    pub def area(self) -> float:
        return 3.14159 * self.radius * self.radius

    pub def describe(self) -> void:
        print(f"Circle r={self.radius}, area={self.area()}")

class Rectangle implements Shape:
    pub width: float
    pub height: float

extend Rectangle:
    pub def init(w: float, h: float) -> Rectangle:
        mut r = Rectangle(); r.width = w; r.height = h; return r

    pub def area(self) -> float:
        return self.width * self.height

    pub def describe(self) -> void:
        print(f"Rect {self.width}x{self.height}, area={self.area()}")

def print_shape(s: Shape) -> void:
    s.describe()

def main():
    mut c = Circle.init(5.0)
    mut r = Rectangle.init(3.0, 4.0)

    print_shape(c)    # Circle → Shape, auto-converted
    print_shape(r)    # Rectangle → Shape, auto-converted

    mut total: float = c.area() + r.area()
    print(f"Total area: {total}")
```

See `examples/09_interfaces.tr` for a runnable demonstration of non-generic interfaces.

See `examples/19_generic_interfaces.tr` for a runnable demonstration of generic interfaces.

---

Next: [Generics →](11_generics.md)
