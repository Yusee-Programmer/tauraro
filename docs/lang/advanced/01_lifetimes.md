# Advanced — Lifetimes & Borrow Checking

> Advanced topic. You can write production Tauraro without ever using any of
> this — the ARC memory model (see [chapter 13](../13_memory_and_ownership.md))
> keeps every program safe on its own. Lifetimes/borrows are an **opt-in** layer
> for people who want Rust-style zero-copy *with a compile-time guarantee*.

---

## The mental model: ARC floor + optional borrow checking

Tauraro has two layers, and understanding their relationship is the whole game:

1. **The ARC floor (always on).** Every heap value is reference-counted. The
   compiler inserts every retain/release/free. Nothing you do — or forget to do
   — can cause a use-after-free or a leak in safe code. This is the *default*,
   and it is complete by itself.

2. **The borrow layer (opt-in, `--strict`).** When you annotate borrows with
   `ref` / `mut ref`, regions with `from`, and bounds with `where … outlives …`,
   the compiler does Rust-style borrow checking *and* elides refcount traffic for
   borrows it can prove safe — giving you zero-copy. Compile **without** `--strict`
   and these annotations are still parsed but **only the ARC floor applies**: they
   never change the generated code, so your program behaves identically.

> **The guarantee:** a borrow is either *proven safe and zero-cost*, or a
> *compile error under `--strict`*. It is never silently unsound. Without
> `--strict` you simply fall back to ARC — correct, just with refcount traffic.

Everything that is erased for codegen: `ref`, `mut ref`, `from r`, `where a
outlives b`. They are a **checking concept layered over the ARC floor**, not a
representation change. A `ref str` is, in the generated C, just a `str`.

---

## When do you need this?

Reach for borrows/lifetimes only when **both** are true:

1. You want **zero-copy** — to avoid a copy or refcount churn on a hot path
   (parsing, tokenizing, tight loops over collections), **and**
2. You want the compiler to **prove** the borrow is safe (run with `--strict`).

If you are returning or storing **owned** values (the common case), or you are
fine with ARC's refcount traffic (almost always negligible), you need none of
this. Don't annotate for its own sake — annotate where it pays.

See the [Zero-Copy Guide](08_zero_copy.md) for *where it actually pays*.

---

## `ref` and `mut ref` — shared vs exclusive borrows

These are Tauraro's `&T` and `&mut T`.

```python
def longest(a: ref str, b: ref str) -> ref str from a, b:   # shared borrows
    if a.length() >= b.length():
        return a
    return b

def push_into(buf: mut ref List[int], x: int):              # exclusive borrow
    buf.append(x)
```

- `x: ref T` — **shared** borrow. Read-only. Many may coexist.
- `x: mut ref T` — **exclusive** borrow. Read/write. At most one, with no shared
  borrow active at the same time.

Under `--strict`:

- Mutating a shared `ref T` parameter is rejected — **`[B-3]`**. Declare it
  `mut ref T` if you need to mutate.
- A place may have *many* shared borrows **or** *one* exclusive borrow, never
  both overlapping — **`[B-1]`** (aliasing XOR mutability, exactly Rust's rule).

Both forms erase to the plain type. `ref str` is a `str`; the flags only drive
checking and zero-copy elision.

---

## Regions: `from` — naming where a borrow comes from

Tauraro lifetimes are **named regions tied to parameter/binding names** — there
are no separate `'a` tokens. The `from` clause names the source a returned borrow
points into.

```python
def first_token(s: ref str) -> ref str from s:   # returns a slice of `s`
    n = s.index_of(" ")
    if n < 0:
        return s
    return s.slice(0, n)
```

- Single source: `-> ref T from a`.
- Multiple sources: `-> ref T from a, b` — valid for the **shorter** of the named
  regions (their intersection).
- **Single non-primitive parameter is auto-inferred** — you may omit `from`:

```python
# `from data` inferred automatically
def get_first(data: ref List[int]) -> ref int:
    ...
```

A region name in `from` must be a parameter (or a region parameter of the
enclosing type, below). A typo is rejected as **`[L-2]`** under `--strict`.

---

## `where a outlives b` — outlives bounds

When a function is declared to return one region (`from b`) but actually returns
a *longer-lived* one (`a`), declare the relationship:

```python
def pick_first(a: ref str, b: ref str) -> ref str from b where a outlives b:
    return a    # `a` outlives `b`, so it's valid for b's scope — accepted
```

The clause is flexible about layout. All three are identical:

```python
def f(a: ref str, b: ref str) -> ref str from b where a outlives b:
    ...

# break to a continuation line
def f(a: ref str, b: ref str) -> ref str from b
    where a outlives b:
    ...

# parenthesised, multi-line, trailing comma allowed
def f(a: ref str, b: ref str, c: ref str) -> ref str from c
    where (
        a outlives c,
        b outlives c,
    ):
    ...
```

Each region in a bound must be a parameter, else **`[L-2]`**. A satisfied bound
relaxes the return-region check (**`[L-4]`**).

> Note: the bound is *trusted* inside the function (consistent with the ARC
> floor — ARC keeps every real borrow safe regardless). Full call-site
> verification of the bound is a future addition.

---

## Lifetimes on every declaration form

Regions are not just for functions — every declaration form can carry them, so
borrowed data can live in structs, enums, and interfaces.

```python
# Struct that borrows from region `src`
class Slice from src:
    pub body: str

# Enum with a borrowed (zero-copy) payload
enum Token from src:
    Word(ref str from src)     # a borrowed slice into the source, no copy
    Number(int)                # owned, no region
    End

# Interface whose implementors borrow from region `r`
interface View from r:
    def text(self) -> ref str from r

# Impl block re-declaring the region (Rust's `impl<'a>`)
extend Slice from src:
    pub def text(self) -> ref str from src:
        return self.body
```

A payload/field/return region must be declared on the type
(`enum E from r` / `class C from r` / `interface I from r`), else **`[L-2]`**.

---

## The borrow checker (`--strict` only)

NLL-precise (liveness-driven, path-aware), cross-block and cross-function:

| Code | Rule |
|---|---|
| **`[B-1]`** | Aliasing XOR mutability: a place can have many shared `ref` borrows **or** one exclusive `mut` borrow, never both overlapping. |
| **`[B-2]`** | A borrowed place can't be reassigned/moved/**mutated** while a borrow is live (includes mutating-method calls and reads of an exclusively-borrowed place). Works across blocks and across function calls. |
| **`[B-3]`** | A shared `ref T` parameter is read-only — mutating it requires `mut ref T`. |
| **`[L-2]`** | A region named in `from` / `where` / a payload must be a real parameter or region parameter. |
| **`[L-4]`** | A returned borrow's region must be the declared one (or proven longer-lived via `where … outlives …`). |

Method-mutability is **inferred**: a method counts as mutating only if it stores
into `self.<field>`. So getters like `counter.value()` are allowed on a shared
borrow; `counter.bump()` (which writes a field) is not. Zero false positives.

```python
# Under --strict:
mut counter = Counter()
cb: ref Counter = counter
counter.bump()        # [B-2] cannot mutate 'counter' while borrowed by 'cb'
```

---

## `--strict` vs default — what actually changes

- **Default build:** annotations are parsed and ignored for codegen. Pure ARC.
  Your program compiles and runs identically with or without the annotations.
- **`--strict` build:** the borrow checks above are enforced (a violation is a
  hard error, no binary), *and* the outlives engine elides refcount traffic for
  proven borrows (zero-copy). Crucially, **the elision is computed the same way in
  both modes** — `--strict` adds the *checks*, not the *speed*. The speed comes
  from the `ref` annotations themselves.

This means: **`--strict` has zero runtime cost.** Ship with it on for the safety
checks, off for faster compiles — the binary is the same either way.

---

## Best practices

- **Default to owned.** Reach for `ref` only on a measured hot path or when you
  want the compile-time guarantee. ARC's refcount ops are cheap; copying large
  data or churning allocations is what hurts — borrow *those*.
- **Develop with `--strict` on.** That's where you get the borrow errors that
  catch real aliasing bugs. Ship either way.
- **Keep borrows short-lived.** A borrow that doesn't escape and ends quickly is
  trivially proven safe and gets elided. Long-lived borrows stored in structs
  need explicit regions and are more likely to need `where` bounds.
- **Don't mutate a collection while borrowing from it.** `[B-2]` will stop you
  under `--strict`; without `--strict` the compiler keeps the safe retain, so it
  still won't corrupt — but it won't be zero-copy either.
- **Use `mut ref` to signal intent.** Even when ARC would handle it, declaring a
  parameter `mut ref` documents that the function mutates it and lets `[B-1]`
  enforce exclusivity at call sites.
- **Avoid `from` on public library APIs.** A returned borrow is an invisible
  lifetime contract the caller must honour. Public APIs should return owned
  values; keep borrows internal to a module or a tight loop.

---

## See also

- [Zero-Copy Guide](08_zero_copy.md) — when zero-copy actually wins, with numbers.
- [Memory and Ownership](../13_memory_and_ownership.md) — the ARC floor.
- [Advanced Ownership](02_advanced_ownership.md) — move/borrow/Shared.
- [Compiler Errors](../19_compiler_errors.md) — full `[B-*]` / `[L-*]` reference.
