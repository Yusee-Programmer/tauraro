# Tauraro Language Documentation

**Tauraro** — compiled, statically-typed, Python-syntax. C performance. Compiler does the hard work.

---

## Document Map

| # | File | Topics |
|---|------|--------|
| 01 | [Introduction](01_intro.md) | Philosophy, pipeline, quick start, CLI flags |
| 02 | [Variables & Types](02_variables_and_types.md) | Mutability, const, type system, inference, literals |
| 03 | [Operators](03_operators.md) | Arithmetic, bitwise, logical, cast, precedence |
| 04 | [Control Flow](04_control_flow.md) | if/elif/else, while, for, break/continue, match |
| 05 | [Functions](05_functions.md) | def, params, return, throws, ?, generics, async, closures, decorators |
| 06 | [Strings & F-Strings](06_strings.md) | Literals, escape sequences, f-strings, string methods |
| 07 | [Collections](07_collections.md) | List[T], Dict, iteration, patterns |
| 08 | [Classes & Extend](08_classes.md) | class, extend, init, self, pub, static methods |
| 09 | [Enums](09_enums.md) | Algebraic data types, variants, pattern matching |
| 10 | [Interfaces](10_interfaces.md) | interface, implements, vtable dispatch, polymorphism |
| 11 | [Generics](11_generics.md) | Generic functions, generic classes, monomorphization |
| 12 | [Error Handling](12_error_handling.md) | try/except/finally, throws, Result[T,E], ? operator |
| 13 | [Memory & Ownership](13_memory_and_ownership.md) | Own/Borrow/Move/Shared, safety rules, scope exit |
| 14 | [Unsafe & Pointers](14_unsafe_and_pointers.md) | unsafe:, Pointer[T], alloc, dealloc, pointer arithmetic |
| 15 | [Modules](15_modules.md) | import, from, pub, export, module resolution |
| 16 | [Concurrency](16_concurrency.md) | async/await, spawn, task_group:, shared ownership |
| 17 | [Extern & FFI](17_extern_and_ffi.md) | extern "C", variadic functions, linking, ABI |
| 18 | [GPU & Inline Assembly](18_gpu_and_asm.md) | gpu: blocks, OpenMP, asm(), memory barriers |
| 19 | [Compiler Error Reference](19_compiler_errors.md) | Every error code with cause, example, and fix |
| 20 | [Advanced Patterns](20_advanced_patterns.md) | Idioms, design patterns, performance, best practices |
| 21 | [Operator Overloading](21_operator_overloading.md) | Dunder methods: `__add__`, `__str__`, `__iter__`, `with`, callable objects |

---

## Quick Reference

### Bilingual Keywords

Tauraro accepts both English and Hausa keyword variants:

| English | Hausa | Meaning |
|---------|-------|---------|
| `def` | `aiki` | define function |
| `class` | `aji` | define class |
| `struct` | `tsari` | define struct (alias for class) |
| `if` | `idan` | conditional |
| `elif` | `koidan` | else-if branch |
| `else` | `sai` | else branch |
| `for` | `ga` | for loop |
| `while` | `yayinda` | while loop |
| `return` | `dawo` | return from function |
| `break` | `tsaya` | break from loop |
| `continue` | `ci_gaba` | continue loop |
| `print` | `buga` | print to stdout |
| `match` | `duba` | pattern match |
| `case` | `hali` | match arm |
| `try` | `gwada` | try block |
| `except` | `kama` | catch exception |
| `finally` | `karshe` | always-run block |
| `raise` | `jefa` | throw exception |
| `true` | `gaskiya` | boolean true |
| `false` | `karya` | boolean false |
| `none` | `babu` | null value |
| `and` | `da` | logical and |
| `or` | `ko` | logical or |
| `not` | `ba` | logical not |
| `in` | `cikin` | membership test |

### Type Quick Reference

| Tauraro type | C type | Notes |
|---|---|---|
| `int` | `long long` | 64-bit signed integer |
| `float` | `double` | 64-bit IEEE 754 |
| `bool` | `bool` | 8-bit boolean |
| `char` | `char` | single byte |
| `str` | `char*` | C string pointer |
| `i8/i16/i32/i64` | `int8_t / short / int / long long` | fixed-width integers |
| `u8/u16/u32/u64` | `uint8_t / unsigned short / unsigned int / unsigned long long` | unsigned fixed-width |
| `usize` | `unsigned long long` | platform word size |
| `f32/f64` | `float / double` | floating point |
| `List[T]` | `List_T*` | growable typed array |
| `Dict` | `TrMap*` | string-keyed map |
| `Pointer[T]` | `T*` | raw pointer (unsafe) |
| `Result[T,E]` | `Result_T_E` | success/error struct |
| `Option[T]` | — | optional value |
