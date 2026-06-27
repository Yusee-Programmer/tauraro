# Advanced — Macros (`macro def`)

> Compile-time code generation. A macro is a function that runs **during
> compilation**, receives a read-only reflection of the declaration it is
> attached to, and **returns Tauraro source** which the compiler parses and
> splices in. The generated code is then type- and borrow-checked like any other
> source.

---

## The model in one sentence

A `macro def name(item) -> code:` is a compile-time function that builds Tauraro
source with f-strings (using `item` to inspect the thing it's attached to), and
`@name` on a declaration runs it and inserts the result.

That's the whole system: no token streams, no `quote!`/`unquote`, no hygiene
rules, no separate build step. If you can write an f-string and a `for` loop, you
can write a macro.

---

## When you need this

- **Derive boilerplate** from a type's fields/variants — equality, hashing,
  `to_str`, serialization, builders.
- **Generate wrappers** around functions — FFI shims, logging/timing adapters,
  registration tables.
- Anywhere you'd otherwise hand-write repetitive code that follows mechanically
  from a declaration's shape.

If you only need a C attribute on a function (`hot`, `inline`), use a
[decorator](05_decorators.md) instead — that's the lighter tool.

---

## Syntax

Define a macro with `macro def`. It takes one parameter (the reflected item) and
returns `code` (Tauraro source, as a string built with f-strings):

```python
macro def name(item) -> code:
    return f"""
    ... generated Tauraro source, interpolating {item.name} etc ...
    """
```

Apply it with `@name` on a `class`, `def`, `enum`, or `interface`:

```python
@name
class Thing:
    ...
```

Use **triple-quoted f-strings** (`f"""..."""`) for multi-line output — they
preserve newlines and indentation, which is what you want when emitting blocks.

---

## The `item` reflection

`item` is a read-only record. Fields are plain values you read with `.field` and
splice with f-strings.

| Field | Available on | Meaning |
|---|---|---|
| `item.kind` | all | `"fn"`, `"class"`, `"enum"`, or `"interface"` |
| `item.name` | all | the declared name |
| `item.params` | fn | list of params — each has `.name`, `.type`, `.is_ref`, `.is_mut`, `.is_variadic` |
| `item.ret` | fn | return type (rendered string, e.g. `"int"`, `"List[str]"`) |
| `item.arglist` | fn | convenience: `"a, b, c"` (param names, for forwarding calls) |
| `item.is_pub` / `item.is_async` | fn, class | visibility / async flags |
| `item.throws` | fn | throws type, `""` if none |
| `item.fields` | class | list of fields — each has `.name`, `.type` |
| `item.methods` | class, interface | list of methods — each has `.name`, `.params`, `.ret` |
| `item.bases` / `item.interfaces` | class | base classes / implemented interfaces (lists of names) |
| `item.is_value_type` | class | `true` if marked `@value_type` |
| `item.variants` | enum | list of variants — each has `.name`, `.fields` (list of payload type strings) |
| `item.generics` | fn, class, enum | type parameter names |

Types are rendered as strings (`"Dict[str, Point]"`, `"ref str"`) so you splice
them directly.

---

## What you can write in a macro body

The macro body is interpreted at compile time over a small, predictable subset:

- `mut x = ...` locals and `x = ...` reassignment (strings, ints, bools, lists)
- `for x in <list>:` over a reflection list (e.g. `item.fields`, `item.params`)
- `if` / `elif` / `else`
- operators: `+` (string concat / int add), `==`, `!=`, `and`, `or`, `<`, `>`,
  `<=`, `>=`, `-`, `*`
- f-strings (single- and triple-quoted), attribute access (`f.name`),
  `.len()` on lists, `.to_str()`
- `macro_error("message")` — abort compilation with a clear diagnostic
- `return <source>` — the generated Tauraro source

Anything outside this subset is rejected with a `macro:` error — keep macro
bodies to mechanical template-building.

---

## Example — `@derive_eq`

Generate structural equality by iterating the fields:

```python
macro def derive_eq(c) -> code:
    mut checks = ""
    for f in c.fields:
        checks = checks + f" and self.{f.name} == other.{f.name}"
    return f"""extend {c.name}:
    pub def __eq__(self, other: {c.name}) -> bool:
        return true{checks}
"""

@derive_eq
class Point:
    pub x: int
    pub y: int

def main():
    mut a = Point()
    a.x = 1
    a.y = 2
    mut b = Point()
    b.x = 1
    b.y = 2
    print(a == b)    # true — uses the generated __eq__
```

The macro emits an `extend` block defining `__eq__`; the compiler attaches it and
`==` dispatches to it.

---

## Example — `@derive_clone`

Generate a `clone` that copies every field, by iterating `item.fields`:

```python
macro def derive_clone(c) -> code:
    mut copies = ""
    for f in c.fields:
        copies = copies + f"""        out.{f.name} = self.{f.name}
"""
    return f"""extend {c.name}:
    pub def clone(self) -> {c.name}:
        mut out = {c.name}()
{copies}        return out
"""

@derive_clone
class Vec3:
    pub x: int
    pub y: int
    pub z: int

def main():
    mut a = Vec3()
    a.x = 1
    mut b = a.clone()
    b.x = 99
    print(a.x)    # 1 — a is unchanged
    print(b.x)    # 99
```

Each line of the body is built with a triple-quoted f-string so the newline is
real and the indentation is preserved.

> **Emitting code: prefer triple-quoted f-strings.** Macro output is almost always
> multi-line, and `\n` / `\"` escapes inside a *single-line* f-string are **not**
> processed for generated source. Use `f"""..."""`: newlines are real, indentation
> is preserved, and a lone `"` inside is literal — exactly what you want when
> writing out Tauraro blocks.

---

## Validating input with `macro_error`

A macro should reject inputs it can't handle, with a useful message:

```python
macro def needs_fields(c) -> code:
    if c.fields.len() == 0:
        return macro_error(f"@needs_fields: '{c.name}' has no fields")
    ...
```

`macro_error` aborts compilation and prints `error: [MACRO] @needs_fields: ...`.

---

## How it works (the pipeline)

```
source.tr
    ↓ parse                  — `macro def`s and `@`-decorated decls become AST
    ↓ expand_macros          — run each @-matched macro over its item;
    │                          parse the returned source; splice the new decls;
    │                          drop the macro defs
    ↓ sema                   — generated code is type/borrow-checked like any source
    ↓ codegen → C → native
```

Because expansion happens **before** sema, generated code obeys the same `--strict`
rules as everything else — a macro cannot emit unsound or ill-typed code that
slips through. And because the macro defs are dropped before sema, their bodies
(which use the `item` reflection) never need to be valid runtime code.

---

## Limitations (by design)

- **Additive only.** Macros emit *new* declarations (`extend`, `def`, `export def`);
  they never rewrite the body of the thing they're attached to. This keeps macros
  predictable and greppable.
- **Declaration-attached only.** There are no function-like, call-site macros
  (`foo!(...)`). Macros apply to a `class`/`def`/`enum`/`interface` via `@`.
- **No hygiene.** Generated names are literal — prefix them (`__derive_`, `__py_`)
  to avoid clashes.
- **Interpreted subset.** Macro bodies run in a small evaluator; constructs
  outside the subset above are rejected.
- **String-template generation.** A typo in the template surfaces as an error in
  the *generated* code — use `--emit c` or read the error's source excerpt to
  debug.

These limits are deliberate: they're what keep the system small enough to learn in
five minutes.

---

## Best practices

- **Keep macro bodies mechanical** — build strings from `item`, nothing clever.
- **Prefix generated names** to avoid collisions (no hygiene).
- **Validate with `macro_error`** so misuse gives a clear message, not a confusing
  downstream type error.
- **Prefer a normal function or a [decorator](05_decorators.md)** when you don't
  actually need code generation — reach for a macro only when the code follows
  mechanically from a declaration's shape.
- **Reach for `@value_type`/built-in decorators first** for the things they
  already cover.

---

## See also

- [05 — Decorators](05_decorators.md) — `@inline`, `@hot`, `@property`, `@value_type`
- [09 — Safety Specification](09_safety_spec.md) — why generated code is still fully checked
- [08 — Classes](../08_classes.md) — `extend`, the usual macro output target
