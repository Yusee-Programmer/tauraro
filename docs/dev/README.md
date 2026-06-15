# Tauraro Developer & Contributor Documentation

This section is for people who want to **work on the Tauraro compiler itself**,
**contribute fixes to `src/`/`std/`/`runtime/`**, or **build and publish their
own libraries** with `taupkg`. If you just want to *use* the language, see the
[Language Documentation](../lang/README.md) and [Standard Library
Documentation](../std/README.md) instead.

---

## Contents

| # | File | Topics |
|---|------|--------|
| 01 | [How the Compiler Works](01_architecture.md) | Lexer → Parser → AST → Resolver → Sema/HIR → CGenerator pipeline, ownership/escape analysis, generic monomorphization, how to add a new keyword/statement |
| 02 | [Building and Contributing](02_contributing.md) | Self-hosting bootstrap (gen1→gen2→gen3), "never patch generated C", runtime header sync, blessing checklist, commit conventions |
| 03 | [Memory Model Internals](03_memory_model_internals.md) | `TrStr` refcounting, auto-drop/escape analysis, wrap-hoist, `List_TrStr`/`Dict_free_strval`, best practices for stdlib authors |
| 04 | [Codegen Guide](04_codegen_guide.md) | How specific features lower to C: generics, enums/match, closures, interfaces/vtables, Sendable/concurrency, channels/select, `ETypeArg` |
| 05 | [Building Libraries](05_building_libraries.md) | `taupkg` manifests, package layout conventions, `local:` deps, `TAURARO_PATH`, worked example, publishing |
| 06 | [Best Practices & Pitfalls](06_best_practices_pitfalls.md) | Curated list of hard-won lessons: memory/string pitfalls, codegen correctness, parser/lexer traps, build/bootstrap traps |

---

## Suggested Reading Order

**New compiler contributor:** 01 → 02 → 03 → 04 → 06
**Library author (taupkg):** 05 → 03 (for memory rules) → 06
**Fixing a specific bug:** jump to 06 first to check if it's a known pitfall, then 01/03/04 for the relevant subsystem

---

Next: [How the Compiler Works →](01_architecture.md)
