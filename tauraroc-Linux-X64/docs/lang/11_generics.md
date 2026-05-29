# 11 — Generics

---

## What Are Generics

Generics let you write functions and classes that work with **any type** while still being statically type-checked. The type parameter is resolved at compile time — the compiler generates a separate, concrete C function for each type combination actually used. This is called **monomorphization**.

The result: zero-overhead generics. `identity[int]` is as fast as a hand-written `identity_int`. No boxing, no type erasure, no runtime overhead.

---

## Generic Functions

```python
def identity[T](x: T) -> T:
    return x

def first[T](items: List[T]) -> T:
    return items[0]

def swap[T](a: T, b: T) -> T:
    return b    # returns the second argument (simplified swap example)

def clamp[T](v: T, lo: T, hi: T) -> T:
    if v < lo: return lo
    if v > hi: return hi
    return v
```

The `[T]` after the function name declares the type parameter. Multiple type parameters are also allowed:

```python
def map_pair[A, B](a: A, b: B) -> A:
    return a

def apply_both[A, B](f: lambda, a: A, b: B) -> void:
    f(a)
    f(b)
```

### Calling Generic Functions

```python
# Explicit type argument:
mut n = identity[int](42)
mut s = identity[str]("hello")

# Inferred from argument:
mut n2 = identity(42)       # inferred as identity[int]
mut s2 = identity("world")  # inferred as identity[str]
```

**How monomorphization works:**
- `identity[int]` → generates `long long identity_int(long long x) { return x; }`
- `identity[str]` → generates `char* identity_str(char* x) { return x; }`
- Only types actually used in calls are compiled — no code bloat for unused types

---

## Generic Classes

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

**How generic classes compile:** The compiler generates a separate C struct for each concrete type combination:

```c
typedef struct { long long value; } Box_int;
typedef struct { char* value;     } Box_str;
typedef struct { double value;    } Box_float;
```

Each is an independent C struct with no relationship at runtime.

---

## Generic Containers

The most common use of generics is building reusable container types. The built-in `List[T]` is itself a generic type.

### Example: Stack[T]

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

---

## Type Substitution During Monomorphization

When the compiler monomorphizes a generic function or class, it performs a textual substitution of the type parameter:

```python
def double[T](x: T) -> T:
    return x + x    # works for int, float, str (str + str = concatenation)
```

For `double[int]`:
```c
long long double_int(long long x) { return x + x; }
```

For `double[str]`:
```c
char* double_str(char* x) { return _tr_str_concat(x, x); }
```

The `+` operator uses the correct C operator for each type — integer addition for int, `_tr_str_concat` for str.

---

## Constraints (Informal)

Tauraro does not have formal trait bounds or interface constraints on type parameters. Any type can be substituted for `T`. If the substituted type doesn't support an operation used in the generic body, you get a C compile error.

For example:
```python
def sum_all[T](items: List[T]) -> T:
    mut total: T = 0    # ERROR if T is str — can't initialize str with 0
    for x in items:
        total = total + x
    return total
```

This works for numeric `T` but fails for `str` at C compile time. **Best practice:** Document the intended types in a comment and use concrete types for anything involving arithmetic:

```python
# Works for numeric types: int, float, i32, f32, etc.
def sum_all[T](items: List[T]) -> T: ...
```

For the occasional need for constrained generics, use an interface:

```python
interface Summable:
    def add(self, other: Summable) -> Summable

def sum_summable[T](items: List[T]) -> T: ...  # informal constraint via comment
```

---

## Limitations

### No Higher-Kinded Types

`List[T]` works, but `F[T]` as a type parameter (where F itself is generic) is not supported. You cannot write:

```python
def apply_to_container[F, T](container: F[T], f: lambda) -> F[T]: ...  # not supported
```

### No Associated Types

Interface methods cannot return `Self` type generically:

```python
interface Builder:
    def build(self) -> Self    # not supported
```

### Recursive Generics

Mutually recursive generic types (like a generic tree where each node contains a `List[Node[T]]`) must use `Pointer[T]` for the recursive references:

```python
class TreeNode[T]:
    pub value: T
    pub children: List[Pointer[TreeNode[T]]]   # not yet fully supported
```

Use concrete types for complex recursive data structures.

---

## Generic Functions with Multiple Bounds (Pattern)

Since there are no formal constraints, use method dispatch to handle multiple types:

```python
# Pattern: pass a "comparer" object instead of relying on T having < built in
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

This is the **strategy pattern** — pass the type-specific behavior as an object. It is more explicit than a type constraint and maps cleanly to C.

---

## Summary

| Feature | Status | Notes |
|---------|--------|-------|
| Generic functions `def f[T](x: T)` | ✓ Working | Monomorphized per call site |
| Generic classes `class Box[T]` | ✓ Working | Generates one C struct per type |
| Type inference for generics | ✓ Working | Often inferred from arguments |
| Multiple type params `[A, B]` | ✓ Working | |
| Formal trait bounds | ✗ Not yet | Use comments + interface pattern |
| Higher-kinded types | ✗ Not yet | |
| Recursive generic types | ~ Partial | Use `Pointer[T]` for recursion |

---

Next: [Error Handling →](12_error_handling.md)
