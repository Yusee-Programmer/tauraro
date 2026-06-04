# 10 — Interfaces

Interfaces are Tauraro's mechanism for **runtime polymorphism** — writing code that works with
many different concrete types through a shared contract, resolved at call time via a vtable.
All other method dispatch in Tauraro is static. Interfaces are the one and only place where
dynamic dispatch occurs.

---

## 1. Declaring an Interface

### When to use

Declare an interface when you want to define a contract that multiple unrelated classes must
fulfil. A good interface captures a minimal set of operations that constitute a complete, coherent
role: `Drawable`, `Serializable`, `Comparable`, `Logger`.

Avoid creating interfaces prematurely. If you only have one class that will ever implement the
interface, use direct method calls — they have zero overhead and are easier to read.

### How it works

`interface IName:` contains **method signatures only** — return types and parameter types, no
implementations, no fields.

```python
interface IAnimal:
    def speak(self) -> void
    def name(self) -> str
    def legs(self) -> int

interface ILogger:
    def log(self, level: str, msg: str) -> void
    def flush(self) -> void

interface ISerializer:
    def serialize(self) -> str
    def byte_size(self) -> int
```

Interface names conventionally start with `I` to distinguish them from classes, but this is a
convention, not a language rule.

### Common Mistakes

**Adding field declarations to an interface** — interfaces are method-only contracts. Fields are
not allowed in an interface body.

**Adding method implementations to an interface** — Tauraro interfaces have no default
implementations. Every implementing class must provide the complete method body.

### Best Practices

- Keep interfaces small — 2 to 5 methods is ideal. Larger interfaces are harder to implement
  correctly and tend to violate the interface-segregation principle.
- Name interfaces after the role they represent: `IReader`, `IWriter`, `IHashable`, `ICloseable`.
- Document each interface method with a comment describing its contract (preconditions, return
  meaning, side effects).

---

## 2. Implementing an Interface

### When to use

A class implements an interface when it can fulfil all the method contracts that interface
requires. Add `implements` to a class that you want to be accepted wherever that interface type
is expected.

### How it works

Add `implements InterfaceName` to the class header. The class must then define every method
listed in the interface inside an `extend` block.

```python
class Dog implements IAnimal:
    pub label: str
    pub breed: str

extend Dog:
    pub def init(n: str, b: str) -> Dog:
        mut d = Dog()
        d.label = n
        d.breed = b
        return d

    pub def speak(self) -> void:
        print(f"{self.label}: Woof!")

    pub def name(self) -> str:
        return self.label

    pub def legs(self) -> int:
        return 4

class Cat implements IAnimal:
    pub label: str

extend Cat:
    pub def init(n: str) -> Cat:
        mut c = Cat()
        c.label = n
        return c

    pub def speak(self) -> void:
        print(f"{self.label}: Meow!")

    pub def name(self) -> str:
        return self.label

    pub def legs(self) -> int:
        return 4
```

**Multiple interfaces** — comma-separate them in the class header:

```python
class Employee implements IPrintable, ISerializable:
    pub name:   str
    pub salary: int
```

The compiler generates a separate vtable for each interface the class implements. Each call site
selects the correct vtable automatically.

### Common Mistakes

**Forgetting `implements` in the class header** — the vtable wrapper is never generated:
```python
class Bird:            # forgot "implements IAnimal"
    pub label: str

introduce(bird)    # ERROR: no Bird_as_IAnimal wrapper — linker error
```
Fix: `class Bird implements IAnimal:`

**Implementing the wrong method signature:**
```python
interface IAnimal:
    def speak(self) -> void

extend Dog:
    pub def speak(self) -> str:    # ERROR: should return void, not str
        return "Woof"
```
Fix: match the interface signature exactly, including return type and parameter types.

**Missing one interface method** — the compiler does not verify at compile time that all methods
are present. A missing method produces a **linker error** with a message like
`undefined reference to Dog_speak`.

### Best Practices

- Implement all interface methods in the same `extend` block. This makes it easy to verify
  completeness at a glance.
- Add a comment above the `extend` block that names which interface it implements.
- Verify coverage by searching for the interface name in your `extend` block before compiling.

---

## 3. Automatic Interface Conversion (Upcast)

### When to use

Wherever an interface type is expected (function parameter, variable type, list element), you
can simply pass a class instance. The compiler automatically generates the upcast. You never
write an explicit cast, wrapper function, or conversion call.

### How it works

When the compiler sees `class Dog implements IAnimal:`, it generates:
1. A vtable struct `Dog_IAnimal_vtable` — one function pointer per interface method.
2. A fat-pointer struct `IAnimal` — a pair `(void* obj, vtable* vt)`.
3. An inline wrapper at every call site that converts a `Dog*` into an `IAnimal` fat-pointer.

**Passing to a function:**
```python
def introduce(a: IAnimal) -> void:
    print(f"{a.name()} has {a.legs()} legs")
    a.speak()

mut dog = Dog.init("Rex", "Shepherd")
mut cat = Cat.init("Whiskers")

introduce(dog)    # Dog → IAnimal: auto-converted at call site
introduce(cat)    # Cat → IAnimal: auto-converted at call site
```

**Assigning to an interface-typed variable:**
```python
mut a: IAnimal = dog    # Dog → IAnimal: auto-converted
a.speak()               # dispatches through vtable → Dog_speak
```

**Call cost:** one pointer dereference (load vtable pointer) + one indirect function call.
Identical to C++ virtual dispatch. There is no extra heap allocation per call.

### Common Mistakes

**Assigning to an untyped variable and expecting interface dispatch:**
```python
mut a = dog         # a has type Dog — direct dispatch, not vtable
a.speak()           # calls Dog_speak directly — fine, but not polymorphic
```
Fix: declare the variable type explicitly: `mut a: IAnimal = dog`

**Storing a concrete type in a `List` and expecting interface dispatch on retrieval:**
```python
mut animals: List[Dog] = []    # List of Dog, not List[IAnimal]
animals.append(dog)
mut x = animals[0]
# x has type Dog, dispatch is static, not polymorphic
```
Fix: use `List[IAnimal]` and wrap each element before appending (see section 4).

### Best Practices

- Annotate interface-typed variables explicitly: `mut a: IAnimal = dog`. This is both
  documentation and required for correct dispatch.
- Pass interface types by value at function boundaries — the fat-pointer is already small
  (two words).

---

## 4. Interface-Typed Variables and Collections

### When to use

Use interface-typed variables and `List[IName]` when you need to hold or process a heterogeneous
collection of objects that share a common interface, such as a list of renderable objects, a
queue of runnable tasks, or a set of registered plugins.

### How it works

```python
# Single interface-typed variable
mut a_dog: IAnimal = dog
mut a_cat: IAnimal = cat

a_dog.speak()                              # vtable dispatch → Dog_speak
print(f"{a_dog.name()} has {a_dog.legs()} legs")

# Collection of interface values
mut animals: List[IAnimal] = []
mut a_dog: IAnimal = dog        # wrap once — fat-pointer stored in list
mut a_cat: IAnimal = cat
mut a_bird: IAnimal = bird
animals.append(a_dog)
animals.append(a_cat)
animals.append(a_bird)

mut i = 0
while i < len(animals):
    mut a = animals[i]
    a.speak()              # vtable dispatch to the correct implementation
    i = i + 1
```

Each element in `List[IAnimal]` is a fat-pointer (object pointer + vtable pointer). The list
stores them contiguously. There is no additional heap allocation per element beyond the list
storage itself.

### Common Mistakes

**Appending a concrete instance directly to `List[IAnimal]` without an intermediate typed variable:**
```python
mut animals: List[IAnimal] = []
animals.append(dog)    # may fail if compiler cannot infer the upcast target type
```
Fix: wrap first: `mut a_dog: IAnimal = dog; animals.append(a_dog)`

**Modifying the concrete object after wrapping** — the fat-pointer holds a raw pointer to the
original concrete object. Modifications to the original object are visible through the interface
value, which may be surprising.

### Best Practices

- Always wrap concrete instances into named interface-typed variables before appending to collections.
- Document collection element types clearly: `mut handlers: List[IEventHandler] = []`.

---

## 5. Multiple Interfaces per Class

### When to use

A class can implement multiple interfaces when it plays multiple roles in the system. For example,
an `Employee` might be both `IPrintable` (can be displayed) and `ISerializable` (can be saved to
disk). Each role is a separate interface; the class provides all methods for all roles.

### How it works

```python
interface IPrintable:
    def print_info(self) -> void

interface ISerializable:
    def serialize(self) -> str

class Employee implements IPrintable, ISerializable:
    pub name:   str
    pub salary: int

extend Employee:
    pub def init(n: str, s: int) -> Employee:
        mut e = Employee()
        e.name = n
        e.salary = s
        return e

    # IPrintable
    pub def print_info(self) -> void:
        print(f"  {self.name}: ${self.salary}")

    # ISerializable
    pub def serialize(self) -> str:
        return f"{self.name},{self.salary}"

def print_one(x: IPrintable) -> void:
    x.print_info()

def to_csv(x: ISerializable) -> str:
    return x.serialize()

def main():
    mut emp = Employee.init("Alice", 95000)

    print_one(emp)              # Employee → IPrintable, auto-converted
    mut row = to_csv(emp)       # Employee → ISerializable, auto-converted
    print(row)                  # "Alice,95000"
```

Each interface gets its own vtable. The auto-conversion at each call site picks the correct one.
The concrete `Employee` object is never duplicated.

### Common Mistakes

**Putting all methods into one god-interface** — this defeats the purpose of interfaces. Split
concerns into separate interfaces so each function can accept only what it needs.

### Best Practices

- Implement each interface's methods in a labeled comment section within the `extend` block.
- Functions should accept the narrowest interface type that satisfies their needs — not the
  concrete class, not a fat interface.

---

## 6. Generic Interfaces

### When to use

Use generic interfaces when the contract involves a type parameter that varies per implementation.
The canonical example is a container interface: `Container[T]` where `T` is the element type.

### How it works

Declare type parameters in brackets: `interface Container[T]:`. Implement with matching brackets:
`class Stack[T] implements Container[T]:`.

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
        mut s = Stack[T]()
        s.items = Vec[T].init(8)
        s.count = 0
        return s

    pub def push(self, item: T) -> void:
        self.items.push(item)
        self.count = self.count + 1

    pub def pop(self) -> T:
        self.count = self.count - 1
        return self.items.pop()

    pub def peek(self) -> T:
        return self.items.get(self.count - 1)

    pub def size(self) -> int:
        return self.count

    pub def is_empty(self) -> bool:
        return self.count == 0
```

The compiler generates a **monomorphized vtable** for each concrete instantiation:
`Container_i64_vtable` for `Container[int]`, `Container_str_vtable` for `Container[str]`, and so on.

Using the generic interface:
```python
def drain_all(c: Container[int]) -> void:
    while not c.is_empty():
        print(f"  popped: {c.pop()}")

def main():
    mut s = Stack[int].init()
    s.push(10)
    s.push(20)
    s.push(30)

    drain_all(s)                    # Stack[int] → Container[int], auto-converted

    mut c: Container[int] = s       # explicit interface variable
    print(f"  size={c.size()}")     # vtable dispatch
```

**Rules for generic interfaces:**
- Declare type parameters in brackets in the `interface` line: `interface Container[T]:`
- Use the same brackets in `implements`: `class Stack[T] implements Container[T]:`
- Each monomorphized usage generates its own vtable struct; there is no shared code path.

### Common Mistakes

**Omitting the type parameter when implementing:**
```python
class Stack implements Container:    # ERROR: Container requires a type parameter
class Stack[T] implements Container[T]:    # OK
```

**Using a generic interface variable without specifying the concrete type:**
```python
mut c: Container = s    # ERROR: Container requires a type argument
mut c: Container[int] = s    # OK
```

### Best Practices

- Document the type parameter's constraints in a comment: `# T must support ==` if your implementation
  relies on equality comparison.
- Test each monomorphized usage explicitly in your test suite — bugs in generic code can be
  type-specific.

---

## 7. Constraining Generics with Interfaces (`where` clause)

### When to use

Use the `where T: IName` clause on a generic function or generic class to restrict which types
may be used as the type argument. This lets you call interface methods on `T` inside the generic
body.

### How it works

```python
interface IComparable:
    def less_than(self, other: IComparable) -> bool

def find_min[T](items: List[T]) -> T where T: IComparable:
    mut best = items[0]
    mut i = 1
    while i < len(items):
        if items[i].less_than(best):
            best = items[i]
        i = i + 1
    return best
```

The compiler verifies at instantiation time that the type argument implements the required interface.

### Common Mistakes

**Calling interface methods on a type parameter without a `where` clause** — the compiler has no
proof the method exists.

### Best Practices

- Use `where` clauses to express the minimum interface contract needed, not more.

---

## 8. Interface Limitations

Understanding these limitations prevents surprises.

| Limitation | Detail |
|------------|--------|
| No default implementations | Every implementing class must provide every method. |
| No interface fields | Interfaces are method contracts only. |
| No runtime type recovery | You cannot downcast `IAnimal` back to `Dog` at runtime. |
| No generic method defaults | Each generic interface method must be fully implemented per class. |
| Concrete object lifetime | The concrete object must outlive any interface value wrapping it. The interface holds a raw pointer; the compiler does not enforce the lifetime relationship. |

**Recovering the concrete type** — if you need type-tagged dispatch with type recovery, use an
enum instead of an interface:

```python
# When you need to recover the concrete type at runtime, use enum:
enum AnimalKind:
    IsDog(d: Dog)
    IsCat(c: Cat)
    IsBird(b: Bird)

match kind:
    case AnimalKind.IsDog(d):  d.fetch()
    case AnimalKind.IsCat(c):  c.purr()
    case AnimalKind.IsBird(b): b.fly()
```

**Object lifetime** — keep concrete objects in a scope that outlives all interface values derived
from them:

```python
def bad_example() -> IAnimal:
    mut dog = Dog.init("Rex", "Lab")   # dog lives on the stack-equivalent heap
    mut a: IAnimal = dog
    return a    # DANGER: a holds a pointer to dog, which will be freed after this function
                # if dog is not kept alive by the caller

def good_example(dog: Dog) -> void:   # caller owns dog, passes it in
    mut a: IAnimal = dog
    a.speak()                         # safe: dog outlives a within this call
```

---

## 9. Common Errors Reference

| Error | Cause | Fix |
|-------|-------|-----|
| Linker: `undefined reference to Dog_speak` | `Dog` is missing the `speak` method | Add `speak` to `extend Dog:` |
| Linker: `undefined reference to Dog_as_IAnimal` | `class Dog` is missing `implements IAnimal` | Add `implements IAnimal` to class header |
| Compile: wrong return type | Method return type doesn't match interface signature | Match exactly |
| Compile: `Animal is an interface, not a class` | Tried to construct `IAnimal.init(...)` | Construct the concrete class instead |

```python
# ERROR: constructing an interface
mut a = IAnimal.init("Rex")   # ERROR: IAnimal is an interface, not a class

# CORRECT: construct the concrete class, then use it as the interface
mut dog = Dog.init("Rex", "Lab")
mut a: IAnimal = dog
```

---

## 10. Full Example — Shapes

This example demonstrates two classes implementing one interface, a function that accepts the
interface, and iteration over a mixed collection.

```python
interface IShape:
    def area(self) -> float
    def perimeter(self) -> float
    def describe(self) -> void

class Circle implements IShape:
    pub radius: float

extend Circle:
    pub def init(r: float) -> Circle:
        mut c = Circle()
        c.radius = r
        return c

    pub def area(self) -> float:
        return 3.14159 * self.radius * self.radius

    pub def perimeter(self) -> float:
        return 2.0 * 3.14159 * self.radius

    pub def describe(self) -> void:
        print(f"Circle(r={self.radius}, area={self.area():.2f})")

class Rectangle implements IShape:
    pub width:  float
    pub height: float

extend Rectangle:
    pub def init(w: float, h: float) -> Rectangle:
        mut r = Rectangle()
        r.width = w
        r.height = h
        return r

    pub def area(self) -> float:
        return self.width * self.height

    pub def perimeter(self) -> float:
        return 2.0 * (self.width + self.height)

    pub def describe(self) -> void:
        print(f"Rect({self.width}x{self.height}, area={self.area():.2f})")

def total_area(shapes: List[IShape]) -> float:
    mut total: float = 0.0
    mut i = 0
    while i < len(shapes):
        total = total + shapes[i].area()
        i = i + 1
    return total

def main():
    mut c = Circle.init(5.0)
    mut r = Rectangle.init(3.0, 4.0)

    # Single-dispatch — direct method call
    c.describe()    # Circle(r=5.0, area=78.54)
    r.describe()    # Rect(3.0x4.0, area=12.00)

    # Interface dispatch — polymorphic
    mut shapes: List[IShape] = []
    mut s_c: IShape = c
    mut s_r: IShape = r
    shapes.append(s_c)
    shapes.append(s_r)

    mut i = 0
    while i < len(shapes):
        shapes[i].describe()    # vtable dispatch
        i = i + 1

    print(f"Total area: {total_area(shapes):.2f}")    # 90.54
```

See `examples/09_interfaces.tr` for a runnable non-generic demonstration.
See `examples/19_generic_interfaces.tr` for a runnable generic interface demonstration.

---

## Hausa Keyword Alternatives

| Tauraro keyword | Hausa equivalent |
|-----------------|-----------------|
| `interface` | `yarjejeniya` (contract/agreement) |
| `implements` | `cika` (fulfil/implement) |
| `def` (in interface) | `alkawari` (promise/declaration) |
| `self` | `kaina` (myself) |

---

Next: [Generics →](11_generics.md)
