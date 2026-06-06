# Advanced — Generators

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Status: not currently supported

Generator functions (`yield`) and lazy generator expressions (`(x for x in seq)`)
are **not implemented** in the current compiler. Earlier drafts of this document
described them, but they do not exist in the language today — this note replaces
that content to keep the docs accurate.

What works instead:

- **List comprehensions** — eager, build a full `List[T]` immediately:

  ```python
  mut squares: List[int] = [x * x for x in numbers]
  mut evens:   List[int] = [x for x in numbers if x % 2 == 0]
  ```

  See [chapter 7 — Collections](../07_collections.md) for the full comprehension
  rules (single transform expression with an optional trailing `if` filter).

- **Manual iteration** — when you need lazy/once-through processing, write a
  plain `for` or `while` loop and process each element as you go, rather than
  materializing an intermediate collection:

  ```python
  mut total = 0
  for x in numbers:
      if x > 2:
          total = total + x * x
  ```

If generator support is added in the future, this document will describe it.
