# Tauraro Language Documentation

**Tauraro** — compiled, statically-typed, Python-syntax. C performance. Compiler does the hard work.

See also: [Standard Library Documentation](../std/README.md) ·
[Developer & Contributor Documentation](../dev/README.md) (compiler internals,
building libraries with `taupkg`)

---

## Core Documentation

Every core doc covers: **What it is → When to use it → How it works → Common Mistakes → Best Practices**

| # | File | Topics |
|---|------|--------|
| 01 | [Introduction](01_intro.md) | Philosophy, pipeline, quick start, CLI flags |
| 02 | [Variables & Types](02_variables_and_types.md) | Mutability, const, type system, inference, literals |
| 03 | [Operators](03_operators.md) | Arithmetic, bitwise, logical, cast, precedence |
| 04 | [Control Flow](04_control_flow.md) | if/elif/else, while, for, break/continue, match |
| 05 | [Functions](05_functions.md) | def, params, return, throws, ?, generics, async, closures, decorators |
| 06 | [Strings & F-Strings](06_strings.md) | Literals, escape sequences, f-strings, string methods |
| 07 | [Collections](07_collections.md) | List[T], Dict, Set, Tuple, comprehensions, iteration |
| 08 | [Classes & Extend](08_classes.md) | class, extend, init, self, pub, static methods, dunders |
| 09 | [Enums](09_enums.md) | Algebraic data types, variants, pattern matching |
| 10 | [Interfaces](10_interfaces.md) | interface, implements, vtable dispatch, polymorphism |
| 11 | [Generics](11_generics.md) | Generic functions, generic classes, monomorphization |
| 12 | [Error Handling](12_error_handling.md) | try/except/finally, throws, Result[T,E], ? operator |
| 13 | [Memory & Ownership](13_memory_and_ownership.md) | Own/Borrow/Move/Shared, safety rules, scope exit |
| 14 | [Unsafe & Pointers](14_unsafe_and_pointers.md) | unsafe:, Pointer[T], alloc, dealloc, pointer arithmetic |
| 15 | [Modules](15_modules.md) | import, from, pub, export, module resolution, TAURARO_PATH |
| 16 | [Concurrency](16_concurrency.md) | async/await, spawn, task_group:, channels, Sendable |
| 17 | [Extern & FFI](17_extern_and_ffi.md) | extern "C", variadic functions, linking, ABI |
| 18 | [Parallelism & Inline Assembly](18_gpu_and_asm.md) | std.gpu.Gpu (OpenMP dispatch), asm(), memory barriers |
| 19 | [Compiler Error Reference](19_compiler_errors.md) | Every error code with cause, example, and fix |
| 20 | [Advanced Patterns](20_advanced_patterns.md) | Idioms, design patterns, performance, best practices |
| 21 | [Operator Overloading](21_operator_overloading.md) | Dunder methods: `__add__`, `__str__`, `__iter__`, `with` |

---

## Advanced Documentation

These topics are **optional** for everyday Tauraro development. Normal programs don't need them. They exist for systems programmers, library authors, and performance engineers.

| # | File | Topics |
|---|------|--------|
| A1 | [Lifetimes & Borrow Checking](advanced/01_lifetimes.md) | `ref`/`mut ref`, regions (`from`), `where … outlives`, the `[B-*]`/`[L-*]` checks |
| A2 | [Advanced Ownership](advanced/02_advanced_ownership.md) | Move semantics, Shared[T] deep dive, explicit borrow patterns |
| A3 | [Channel Select](advanced/03_channel_select.md) | `select:` blocks, timeout arms, fan-in/fan-out patterns |
| A4 | [Generators](advanced/04_generators.md) | Generator expressions `(x for x in ...)`, lazy evaluation |
| A5 | [Decorators](advanced/05_decorators.md) | `@inline`, `@hot`, `@property`, `@value_type`, custom decorators |
| A6 | [Sendable & Thread Safety](advanced/06_sendable.md) | Sendable interface, `[T-1]`/`[T-2]`/`[T-6]` checks, UnsafeSendable |
| A7 | [Concurrency Guide](advanced/07_concurrency_guide.md) | All concurrency models, primitives, decision matrix, best practices |
| A8 | [Zero-Copy Guide](advanced/08_zero_copy.md) | When zero-copy wins (StrView, borrowed payloads, dict borrows) vs parity |
| A9 | [Safety Specification](advanced/09_safety_spec.md) | **Normative**: ARC-floor invariants, what `--strict` proves/elides, how it's verified |

---

## Quick Reference

### Type Quick Reference

| Tauraro type | Size | Notes |
|---|---|---|
| `int` | 64-bit | 64-bit signed integer (default integer type) |
| `float` | 64-bit | 64-bit IEEE 754 double (default float type) |
| `bool` | 1 byte | boolean (`true`/`false`) |
| `char` | 1 byte | single ASCII byte |
| `str` | pointer | null-terminated UTF-8 string |
| `i8/i16/i32/i64` | 8/16/32/64-bit | fixed-width signed integers |
| `u8/u16/u32/u64` | 8/16/32/64-bit | fixed-width unsigned integers |
| `usize` / `isize` | 64-bit | platform word size |
| `f32/f64` | 32/64-bit | floating point |
| `List[T]` | heap | growable typed array (C-backed) |
| `Vec[T]` | heap | growable array (OOP class, `from std.core.vec`) |
| `Dict` | heap | string-keyed hash map |
| `Map[K,V]` | heap | typed hash map (`from std.core.map`) |
| `Set[T]` | heap | unordered unique collection |
| `Pointer[T]` | pointer | raw pointer (unsafe only) |
| `Result[T,E]` | struct | success (`Ok`) or error (`Err`) |
| `Option[T]` | struct | present (`Some`) or absent (`None`) |
| `Shared[T]` | heap | reference-counted, thread-safe |
| `Chan[T]` | heap | typed channel for concurrency |
| `Mutex[T]` | heap | mutex-protected value |
| `Atomic[T]` | heap | lock-free atomic value |

### Operator Precedence (high to low)

| Level | Operators |
|-------|----------|
| 1 (highest) | `**` (power) |
| 2 | unary `-`, `+`, `~`, `not` |
| 3 | `*`, `/`, `//`, `%` |
| 4 | `+`, `-` |
| 5 | `<<`, `>>` |
| 6 | `&` |
| 7 | `^` |
| 8 | `\|` |
| 9 | `==`, `!=`, `<`, `>`, `<=`, `>=`, `in`, `not in`, `is` |
| 10 | `and` |
| 11 | `or` |
| 12 | `if ... else ...` (ternary) |
| 13 (lowest) | `=`, `+=`, `-=`, etc. |

### Compiler Pipeline

```
source.tr
    ↓ Lexer (src/lexer.tr)       — tokenize, indent tracking, keyword recognition
    ↓ Parser (src/parser.tr)     — recursive descent → AST
    ↓ Sema (src/sema.tr)         — types, ownership inference, HIR
    ↓ Codegen (src/codegen/c.tr) — one .c file per module into build/
    ↓ GCC/Clang                  — one invocation for all .c files
    → native binary
```

---

## Learning Path

**Beginner:** 01 → 02 → 03 → 04 → 05 → 06 → 07  
**Intermediate:** 08 → 09 → 10 → 11 → 12 → 13 → 15  
**Systems programmer:** 14 → 16 → 17 → 18 → advanced/  
**Reference:** 19 (error codes), 20 (patterns), 21 (operator overloading)
