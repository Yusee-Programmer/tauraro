# Advanced — Lifetimes

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

Tauraro's ownership model (see [chapter 13](../13_memory_and_ownership.md)) tracks who owns each heap-allocated value. The lifetime system extends this to *pointers* and *borrows* — specifically, to functions that return a `Pointer[T]` or `ref T` into data they do not own.

The problem: if a function returns a pointer into one of its parameters, the compiler must know which parameter the pointer came from. Without this, it cannot verify that the caller still owns that data when they use the returned pointer.

The solution is the `from` keyword, which annotates the *lifetime source* of a returned pointer.

---

## When You Need This

You need `from` annotations only when **all three** of these are true:

1. Your function returns a `Pointer[T]`, `ref T`, or `mut_ref T`
2. The pointer points into a parameter's data (not heap memory you allocate yourself)
3. The function has two or more non-primitive parameters (single-param case is auto-inferred)

If you are returning owned values (the common case), you never need `from`.

---

## Syntax Reference

```
def function_name(params...) -> ReturnType from param_name:
    ...
```

The `from param_name` clause comes after the return type and before the colon. It tells the compiler: "the returned pointer's lifetime is bounded by `param_name`."

For `ref` and `mut_ref`:

```
def function_name(x: SomeType) -> ref SomeType from x:
    ...

def function_name(x: SomeType) -> mut_ref SomeType from x:
    ...
```

---

## Examples

### Single Parameter — Auto-Inferred (no annotation needed)

When a function has exactly one non-primitive parameter and returns a pointer, the compiler automatically infers that the pointer comes from that parameter:

```python
# No annotation needed — compiler infers 'from data'
def get_first(data: List[int]) -> Pointer[int]:
    return data.raw_ptr()

def main():
    mut nums = [10, 20, 30]
    mut p = get_first(nums)    # p is valid as long as nums is alive
    print(p.read())            # 10
    # nums is still in scope — p is safe to use
```

### Multiple Parameters — Must Annotate

When there are two or more non-primitive parameters, the compiler cannot infer which one the returned pointer comes from. You must annotate:

```python
# Must annotate — two non-primitive params
def pick(a: List[int], b: List[int], use_a: bool) -> Pointer[int] from a:
    if use_a: return a.raw_ptr()
    return a.raw_ptr()    # annotation says lifetime source is 'a'
                          # so we must always return from 'a'
```

If you need to return from either `a` or `b` depending on a condition, return an owned value instead:

```python
def pick_value(a: List[int], b: List[int], use_a: bool) -> int:
    if use_a: return a[0]
    return b[0]    # returns a copy — no lifetime concern
```

### Using ref and mut_ref

`ref T` is a read-only borrow. `mut_ref T` is a mutable borrow. Both use `from`:

```python
def find_max(data: List[int]) -> ref int from data:
    mut best = 0
    mut i = 1
    while i < len(data):
        if data[i] > data[best]: best = i
        i = i + 1
    return ref data[best]    # read-only reference into data

def increment_first(data: List[int]) -> mut_ref int from data:
    return mut_ref data[0]   # mutable reference into data
```

### The Error This Prevents

Without `from`, returning a pointer to a local is caught as [L-1]:

```python
# WRONG — triggers L-1:
def get_ref() -> Pointer[int]:
    mut x = 42
    return &x       # L-1: x is freed when function returns

# RIGHT — return by value:
def get_value() -> int:
    return 42
```

---

## Named Regions & Zero-Copy Borrow Elision

> This section documents the borrow engine that backs the `from` annotations. The
> headline: **borrow annotations are always optional.** Tauraro's ARC (automatic
> reference counting) is the safety *floor* — every value is correctly freed with or
> without annotations. Annotations only let the compiler *prove* a borrow is safe and
> then **elide the reference count entirely**, turning the borrow into a true
> zero-copy alias (a bare pointer move, no `retain`/`release`).

### `ref T` and `mut ref T` bindings

A local can borrow another value instead of owning it:

```python
mut owner = build_big_string()
mut view: ref str = owner        # read-only borrow — shares owner's buffer
print(len(view))
print(len(owner))                # owner still usable; view is just an alias
```

`ref T` is a shared (read-only) borrow; `mut ref T` (two words) is an exclusive
borrow. Both **erase to `T`** for codegen — a `ref str` is used exactly like a `str`.
The annotation never changes representation; it only drives checking and elision.

### Multi-source regions and region parameters

A return can borrow from *several* sources, and a class can carry a region:

```python
def longest(x: str, y: str) -> ref str from x, y:
    if len(x) >= len(y): return x
    return y

class Parser from src:               # the class borrows region `src`
    pub rest: ref str from src       # a borrowing field
```

### What "elision" buys you

When the compiler can **prove** the borrowed source outlives the borrow (the *outlives*
analysis), it emits the borrow as a pure alias:

```python
mut r = head(p, q)     # head() returns `ref str from a`
# emitted C:  TrStr r = head(p, q);   — no retain, no release. Zero-copy.
```

When it *cannot* prove it (the source might die first), it falls back to ARC
(retain + release) — still correct, just not zero-copy. **You never get a leak or a
dangling pointer either way**; proof only decides whether the refcount is skipped.

This works across function boundaries: a `-> ref` function returns its result without
retaining (a borrow), and callers that bind a proven borrow skip the release. Owned
contexts (storing into a field, returning from a non-`ref` function, pushing into a
collection) automatically take ownership (retain) so the borrow can safely outlive its
origin.

### Optional strict checking

By default, borrow violations are **not** fatal — ARC keeps the program safe. Compiling
with `--strict` turns the borrow checker on as hard errors (Rust-style), enforcing:

| Code | Meaning |
|------|---------|
| L-1 | returning a pointer/borrow to a function-local (it dies at return) |
| L-2 | `from` names a region that isn't a parameter/field in scope |
| L-3 | returning a freshly-owned value through a `ref` (borrow) return type |
| L-4 | returning a borrow from a region the signature didn't promise |
| L-5 | storing a freshly-owned value into a borrowing (`ref`) field |

Without `--strict`, these surface as warnings (or are simply absorbed by ARC), so code
from Rust users who annotate heavily still compiles and runs under the ARC floor.

---

## How the Compiler Enforces This

When the compiler sees a call site like:

```python
mut p = get_first(nums)
```

It knows `p`'s lifetime is bounded by `nums`. If you try to use `p` after `nums` has been moved or freed, the compiler emits [M-1]:

```python
mut nums = [10, 20, 30]
mut p = get_first(nums)
send_away(nums)           # moves nums — ownership transferred
print(p.read())           # M-1: nums was moved, p is now dangling
```

---

## Common Mistakes

**Annotating `from` with the wrong parameter.** If you annotate `from a` but sometimes return a pointer into `b`, the compiler will allow the code — but at runtime, callers who keep the returned pointer while allowing `b` to be freed will have a dangling pointer. Annotation must match reality.

**Using `from` instead of returning by value.** If you just need the data, return it by value. `from` is only useful when zero-copy access into the original buffer is a measured necessity.

**Assuming `from` works like Rust lifetimes.** In Rust, lifetime parameters appear on every usage site. In Tauraro, `from` is only on the return type of the function that returns the pointer. Callers don't need to annotate anything.

---

## Best Practices

- **Default to value semantics.** Return owned values. This eliminates all lifetime concerns at the cost of a copy. Copies of primitive values and small structs are free. Copies of large buffers may matter — measure first.
- **Use `from` only for proven hot paths** where zero-copy access is measurably important.
- **Never use `from` on public library APIs.** Callers must manage the lifetime constraint, which is an invisible contract. Return owned values in public APIs.
- **Single-parameter functions don't need annotation.** The auto-inference is intentional — don't add redundant `from x` when there's only one non-primitive param.

---

See also:
- [13 — Memory and Ownership](../13_memory_and_ownership.md)
- [14 — Unsafe and Pointers](../14_unsafe_and_pointers.md)
- [02 — Advanced Ownership](02_advanced_ownership.md)
- [Compiler Error M-4](../19_compiler_errors.md#m-4-dangling-pointer--lifetime-error)
