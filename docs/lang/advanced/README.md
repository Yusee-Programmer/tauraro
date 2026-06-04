# Advanced Docs Index

This directory covers advanced Tauraro topics. Core Tauraro development — writing programs, using the standard library, building with modules, concurrency — does not require any of this. These docs exist for when you hit a wall and need a deeper mental model, or when you are building libraries and infrastructure rather than applications.

---

## Contents

| Doc | Topic | When You Need It |
|-----|-------|-----------------|
| [01 — Lifetimes](01_lifetimes.md) | The `from` keyword lifetime annotation | Returning pointers into caller-owned data |
| [02 — Advanced Ownership](02_advanced_ownership.md) | Move, borrow, Shared deep dive | Understanding M-2 errors; shared mutable state |
| [03 — Channel Select](03_channel_select.md) | `select:` for multiplexed channels | Fan-in, timeouts, non-blocking channel ops |
| [04 — Generators](04_generators.md) | Lazy generator expressions and list comprehensions | Memory-efficient iteration over large sequences |
| [05 — Decorators](05_decorators.md) | `@inline`, `@hot`, `@property`, custom decorators | Compile-time code annotation and transformation |
| [06 — Sendable](06_sendable.md) | Thread-safety enforcement via the `Sendable` interface | Passing types across threads without data races |

---

## Prerequisite Reading

Before reading these docs, make sure you are comfortable with:

- [13 — Memory and Ownership](../13_memory_and_ownership.md)
- [14 — Unsafe and Pointers](../14_unsafe_and_pointers.md)
- [16 — Concurrency](../16_concurrency.md)
- [19 — Compiler Errors](../19_compiler_errors.md)

---

## How These Topics Relate

```
Ownership model (ch 13)
    │
    ├── Advanced Ownership (02)  ← explains inference rules + Shared[T]
    │       │
    │       └── Lifetimes (01)   ← extends ownership to pointer return types
    │
Concurrency (ch 16)
    │
    ├── Sendable (06)            ← compile-time thread-safety enforcement
    │
    └── Channel Select (03)      ← advanced channel patterns

Language features (ch 21)
    │
    ├── Generators (04)          ← lazy iteration complement to list comprehensions
    │
    └── Decorators (05)          ← compile-time annotation system
```

---

← [Operator Overloading](../21_operator_overloading.md) | [Lang Docs Root](../README.md)
