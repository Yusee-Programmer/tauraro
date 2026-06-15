# 11 — Generics

---

## What Are Generics

Generics let you write functions and classes that work with **any type** while still being statically type-checked. The type parameter is resolved at compile time — the compiler generates a separate, concrete C function or struct for each type combination actually used. This is called **monomorphization**.

The result: **zero-cost generics**. `identity[int]` compiles to the exact same C as a hand-written `identity_int`. No boxing, no type erasure, no vtable, no runtime overhead whatsoever.

---

## Generic Functions

### When to Use

Use generic functions when you need the same algorithm to work across multiple types and the logic is identical regardless of the type — sorting, searching, wrapping, transforming, swapping.

### How It Works

Declare a type parameter in square brackets after the function name:

```python
def identity[T](x: T) -> T:
    return x

def first[T](items: List[T]) -> T:
    return items[0]

def swap[T](a: T, b: T) -> T:
    return b    # returns the second argument

def clamp[T](v: T, lo: T, hi: T) -> T:
    if v < lo: return lo
    if v > hi: return hi
    return v
```

Multiple type parameters are allowed:

```python
def map_pair[A, B](a: A, b: B) -> A:
    return a

def zip[K, V](keys: List[K], vals: List[V]) -> Dict:
    mut d: Dict = {}
    mut i = 0
    while i < len(keys):
        d[keys[i]] = vals[i]
        i = i + 1
    return d
```

**Calling generic functions — explicit vs inferred:**

```python
# Explicit type argument:
mut n = identity[int](42)
mut s = identity[str]("hello")

# Inferred from argument (preferred when unambiguous):
mut n2 = identity(42)       # inferred as identity[int]
mut s2 = identity("world")  # inferred as identity[str]
```

**How monomorphization works:** The compiler scans every call site and generates a dedicated C function for each concrete type used. Only types actually used at call sites are compiled — there is no code bloat for unused instantiations.

### Common Mistakes

```python
# WRONG: Assuming T supports all arithmetic
def sum_all[T](items: List[T]) -> T:
    mut total: T = 0    # fails at C compile time if T is str
    for x in items:
        total = total + x
    return total

# WRONG: Using type() on a generic parameter
def check[T](x: T) -> bool:
    return type(x) == "int"    # ERROR: types are erased to C — type() on T is not valid
```

### Best Practices

- Document which types a generic function is intended for with a comment:
  ```python
  # Intended for numeric types: int, float, i32, f32
  def sum_all[T](items: List[T]) -> T: ...
  ```
- Prefer type inference at call sites unless the type would be ambiguous.
- If you need type-specific behavior, pass a strategy object rather than relying on `T` supporting an operator (see the Strategy Pattern section below).
- Never call `type()` on a generic parameter — types are erased to C and this check cannot work.

---

## Generic Classes

### When to Use

Use generic classes to build reusable container types, wrappers, or data structures where the contained type should be determined by the caller — `Box[T]`, `Stack[T]`, `Pair[A, B]`.

### How It Works

Declare the type parameter on the class and repeat it on each method in the `extend` block:

```python
class Box[T]:
    pub value: T

extend Box:
    pub def init[T](v: T) -> Box[T]:
        mut b = Box[T]()
        b.value = v
        return b

    pub def get[T](self) -> T:
        return self.value

    pub def set[T](self, v: T) -> void:
        self.value = v
```

Using a generic class:

```python
mut int_box   = Box.init[int](42)
mut str_box   = Box.init[str]("hello")
mut float_box = Box.init[float](3.14)

print(int_box.get())     # 42
print(str_box.get())     # hello
```

**How generic classes compile:** The compiler generates a completely independent C struct for each concrete type — `Box_int`, `Box_str`, `Box_float`. They share no code at runtime. Each has the correct field size for its type.

### Common Mistakes

```python
# WRONG: Forgetting [T] on init or methods — they will not be generic
extend Box:
    pub def init(v: int) -> Box[int]:    # only works for int now
        ...

# WRONG: Trying to use a class with a type param it was never called with
mut b = Box[MyClass].init(obj)    # OK only if MyClass.init is compatible
                                   # the compiler generates a new monomorphization
```

### Best Practices

- Always repeat the type parameter on every method in the `extend` block.
- Use concrete types (e.g., `Box[int]`) at call sites — explicit is clear.
- Build the generic class using `List[T]`, `Option[T]`, or other generic built-ins for fields rather than raw arrays.

---

## Generic Containers

### When to Use

Use generic containers when building reusable data structures — stacks, queues, pairs, trees — that should work uniformly for any element type.

### How It Works

The built-in `List[T]` is itself a generic type. You can build on top of it:

```python
class Stack[T]:
    pub items: List[T]

extend Stack:
    pub def init[T]() -> Stack[T]:
        mut s = Stack[T]()
        s.items = []
        return s

    pub def push[T](self, v: T) -> void:
        self.items.append(v)

    pub def pop[T](self) -> T:
        return self.items.pop()

    pub def peek[T](self) -> T:
        return self.items[len(self.items) - 1]

    pub def is_empty[T](self) -> bool:
        return len(self.items) == 0

    pub def size[T](self) -> int:
        return len(self.items)

def main():
    mut s = Stack.init[int]()
    s.push(1)
    s.push(2)
    s.push(3)
    while not s.is_empty():
        print(s.pop())    # 3, 2, 1
```

### Common Mistakes

```python
# WRONG: Calling pop() on an empty stack — no bounds check on Stack[T].pop()
# Always guard with is_empty() first:
if not s.is_empty():
    mut top = s.pop()
```

### Best Practices

- Gate destructive operations (`pop`, `peek`) behind an `is_empty()` guard.
- Return `Option[T]` from methods that may have no valid result instead of crashing.
- Test the container with at least two different concrete types to catch accidental type-specific assumptions.

---

## Built-in Generic Types

The standard library ships these generic types that you can use directly:

| Type | Purpose |
|------|---------|
| `List[T]` | Dynamic array |
| `Vec[T]` | Low-level growable vector (import from `std.core.vec`) |
| `Map[K, V]` | Hash map |
| `Option[T]` | Nullable value — `Some(v)` or `None` |
| `Result[T, E]` | Success or failure — `Ok(v)` or `Err(e)` |
| `Chan[T]` | Async channel |
| `Shared[T]` | Reference-counted shared ownership |
| `Mutex[T]` | Exclusive-access wrapper |
| `Atomic[T]` | Lock-free atomic value |
| `Pointer[T]` | Raw pointer (unsafe context) |

---

## Function-Pointer Type Arguments

### When to Use

Use this when you need a homogeneous collection of callables — a dispatch
table, a list of handlers, a pipeline of transform steps — where every
element has the same `def(...) -> R` signature.

### How It Works

A generic type argument can itself be a function-pointer type, written
`def(ParamTypes...) -> R`. This lets `Vec[def(...) -> R]`, `List[def(...) -> R]`,
and similar generic containers hold plain top-level functions directly:

```python
def add1(x: int) -> int:
    return x + 1

def mul2(x: int) -> int:
    return x * 2

def main():
    mut fns = Vec[def(int) -> int].init(2)
    fns.push(add1)
    fns.push(mul2)
    print(fns.get(0)(10))    # 11
    print(fns.get(1)(10))    # 20
```

The parser recognizes that `def` cannot begin a value expression inside
`[...]`, so `Vec[def(int) -> int]` is parsed as a type argument (`ETypeArg`)
rather than an index expression. Each element is a zero-cost function
pointer — pushing `add1`/`mul2` stores the function's address, not a closure.

### Common Mistakes

```python
# WRONG: mixing a function-pointer element type with a non-function value
mut fns = Vec[def(int) -> int].init(2)
fns.push(42)    # ERROR: 42 is not a def(int) -> int
```

### Best Practices

- Prefer this over the "wrap the callable in a struct field" workaround
  (see [Functions §First-Class Functions](05_functions.md#first-class-functions-callables))
  when every element really does share one signature and you don't need
  extra per-entry data (like a route pattern or name).
- When each entry needs additional metadata alongside the callable (a name,
  a pattern, a priority), keep using a small struct with a `def(...) -> R`
  field and `Vec[YourStruct]` instead.

---

## Type Constraints with `where`

### When to Use

Use `where` constraints when you need to restrict which types can be substituted — for example, requiring that `T` implements a specific interface.

### How It Works

```python
def max_of[T where T: Comparable](a: T, b: T) -> T:
    if a > b:
        return a
    return b
```

The `where T: Comparable` clause tells the compiler that `T` must satisfy the `Comparable` interface. If you call `max_of` with a type that does not implement `Comparable`, the compiler rejects it.

Without a `where` clause, any type can be substituted, and type mismatches only surface as C compile errors.

### Common Mistakes

```python
# WRONG: Omitting where when the body requires a specific operation
def max_of[T](a: T, b: T) -> T:
    if a > b: return a    # if T is a class without > defined, this is a C error
    return b
```

### Best Practices

- Use `where T: InterfaceName` whenever the generic body calls an interface method.
- Fall back to the strategy pattern (see below) when you need behavior for concrete types that do not share an interface.

---

## Strategy Pattern (Generic Functions with Multiple Types)

### When to Use

When you need type-specific behavior (like a custom comparator or serializer) but the types involved do not share a formal interface.

### How It Works

Pass the type-specific logic as an object — the **strategy pattern**:

```python
class IntComparer:
    pub def less_than(self, a: int, b: int) -> bool:
        return a < b

def generic_sort(items: List[int], cmp: IntComparer) -> void:
    mut i = 1
    while i < len(items):
        mut key = items[i]
        mut j = i - 1
        while j >= 0 and cmp.less_than(key, items[j]):
            items[j + 1] = items[j]
            j = j - 1
        items[j + 1] = key
        i = i + 1
```

This is explicit, maps cleanly to C, and avoids any runtime dispatch overhead.

### Common Mistakes

```python
# WRONG: Using a lambda field for the comparer instead of a class method
# Lambda fields do not compile to efficient C in all cases
```

### Best Practices

- Define one comparer class per type you intend to sort.
- Keep comparer classes simple — one method, no state — so they optimize away completely.

---

## Limitations

### No Higher-Kinded Types

`List[T]` works, but using a container type itself as a type parameter is not supported:

```python
def apply_to_container[F, T](container: F[T], f: lambda) -> F[T]: ...  # not supported
```

### No Associated Types

Interface methods cannot return `Self` generically:

```python
interface Builder:
    def build(self) -> Self    # not supported
```

### Recursive Generic Types

Mutually recursive generic types (e.g., a tree where each node holds a `List[Node[T]]`) require `Pointer[T]` for the recursive reference and are partially supported:

```python
class TreeNode[T]:
    pub value: T
    pub children: List[Pointer[TreeNode[T]]]   # partial support — use concrete types for complex cases
```

Use concrete types for complex recursive structures.

---

## Summary

| Feature | Status | Notes |
|---------|--------|-------|
| Generic functions `def f[T](x: T)` | Working | Monomorphized per call site |
| Generic classes `class Box[T]` | Working | Generates one C struct per concrete type |
| Type inference for generics | Working | Inferred from arguments at call sites |
| Multiple type params `[A, B]` | Working | |
| `where` constraints | Working | Requires interface definition |
| Higher-kinded types | Not yet | |
| Recursive generic types | Partial | Use `Pointer[T]` for self-referential fields |

---

Next: [Error Handling →](12_error_handling.md)
