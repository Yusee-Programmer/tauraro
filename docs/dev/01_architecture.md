# 01 — How the Tauraro Compiler Works

---

## Overview

`tauraroc` is a self-hosted, ahead-of-time compiler: it reads `.tr` source files and
produces a native executable by generating C and handing it to GCC/Clang. The whole
compiler — lexer, parser, semantic analyzer, and code generator — is itself written
in Tauraro (`src/*.tr`).

```
source.tr
    │
    ▼
  Lexer            (src/lexer.tr)        text -> Vec[Token]
    │
    ▼
  Parser           (src/parser.tr)       Vec[Token] -> Program (AST)
    │
    ▼
  ModuleResolver   (src/resolver.tr)     follows imports -> all Decls across modules
    │
    ▼
  Sema             (src/sema.tr)         AST -> HirProgram (typed HIR)
    │                                    type checking, ownership/escape analysis,
    │                                    auto-drop insertion, monomorphization hints
    ▼
  CGenerator       (src/codegen/c.tr)    HirProgram -> one .c file per module
    │
    ▼
  GCC / Clang                            build/*.c -> native binary
```

Each stage's output type is the next stage's input type. Understanding those types —
`Token`, the `Expr`/`Stmt`/`Decl` AST, `HirExpr`/`HirStmt`/`HirProgram`, and the C
strings emitted by `CGenerator` — is the fastest way to understand the whole compiler.

---

## Stage 1: Lexer (`src/lexer.tr`, `src/token.tr`)

**Input:** raw source text (`str`)
**Output:** `Vec[Token]` + a parallel `Vec[int]` of line numbers

The `Token` enum is defined in `src/token.tr`. `Lexer` (in `src/lexer.tr`) is a
hand-written scanner over the raw byte buffer (`Pointer[char]`):

- Tracks `pos`/`line` and an `indent_stack: Vec[int]` for Python-style indentation —
  it emits synthetic `Indent`/`Dedent` tokens so the parser never has to look at
  whitespace.
- `keyword_to_token(s: str) -> Token` maps identifier text to keyword tokens,
  including **Hausa keyword aliases** (e.g. `aiki` = `def`, `aji` = `class`).
- Dedicated readers for each literal form: `read_int`, `read_string`,
  `read_triple_string`, `read_char`, `read_fstring`, `read_raw_string`,
  `read_byte_string`, `read_ident`.
- `tokenize(self) -> Vec[Token]` is the single entry point — it loops until EOF,
  calling `skip_spaces`/`skip_comment` between tokens and flushing any pending
  dedents at end-of-file.

---

## Stage 2: Parser (`src/parser.tr`)

**Input:** `Vec[Token]` + line numbers
**Output:** `Program` (a `Vec[Pointer[Decl]]`), defined in `src/ast.tr`

`Parser` is a classic recursive-descent parser with one method per grammar
production. Entry point:

```tr
pub def parse_program(self) -> Program   # loops calling parse_decl() until EOF
pub def parse_decl(self) -> Pointer[Decl]
```

Layered structure (each level calls the one below, building precedence from the
bottom up):

| Layer | Method | Handles |
|-------|--------|---------|
| Statements | `parse_stmt`, `parse_block` | `if`/`while`/`for`/`match`/`try`/`with`/`assert`/`spawn`/`task_group`/GPU/`asm`/chan-select, `let`/`mut`/`const`/`shared` bindings |
| Expressions (lowest→highest precedence) | `parse_ternary` → `parse_or_expr` → `parse_and_expr` → `parse_not_expr` → `parse_comparison` → `parse_bitor/xor/and_expr` → `parse_shift_expr` → `parse_additive` → `parse_multiplicative` → `parse_power` → `parse_unary` → `parse_postfix` → `parse_primary` | Full operator precedence table (see `docs/lang/03_operators.md`) |
| Declarations | `parse_decl` | `def`, `class`/`extend`, `enum`, `interface`, `import`/`from`, top-level `type` aliases |
| Types | `parse_type(self) -> AstType` | `int`, `List[T]`, `Pointer[T]`, `def(...)->R`, generic args |

Helper boxing functions `box_expr`, `box_stmt`, `box_decl`, `box_asttype` heap-allocate
AST nodes so they can be stored behind `Pointer[...]` in enum variants (Tauraro enums
with payloads are tagged unions; recursive variants need pointer indirection).

---

## The AST (`src/ast.tr`)

All node types the parser produces:

| Type | Kind | Notes |
|------|------|-------|
| `Expr` | enum | Every expression form: literals, `EBinOp`, `ECall`, `EMethodCall`, `EIndex`, `EFString`, `EListComp`, `EClosure`, `ETry`, `EAwait`, `ERange`, etc. |
| `Stmt` | enum | `SExpr`, `SLet`, `SAssign`, `SIf`, `SWhile`, `SFor`, `SMatch`, `STry`, `SWith`, `SAsm`, `SSpawn`, `SChanSelect`, `SLocalDecl` (nested decls), etc. |
| `Decl` | enum | Top-level: `DFunction`, `DClass`, `DEnum`, `DInterface`, `DExtend`, `DImport`, `DTypeAlias` |
| `AstType` | class | The type system's representation — name + generic `args: Vec[Pointer[AstType]]` (e.g. `List[int]` = `AstType{name:"List", args:[int]}`) |
| `FunctionDef`, `ClassDef`, `EnumDef`, `InterfaceDef`, `FieldDef`, `Param`, `VariantDef` | classes | Declaration bodies |
| `Pattern` | enum | `match`/`case` patterns |
| `Ownership` | enum | `Own`/`Borrow`/`Move`/`Shared` annotations parsed from `let`/params |
| `Block` | class | `Vec[Pointer[Stmt]]` — a statement list (function/loop/if body) |

---

## Stage 3: Module Resolution (`src/resolver.tr`)

**Input:** entry-file path
**Output:** a merged `Program` containing every `Decl` from every transitively
imported module, plus bookkeeping vectors

`ModuleResolver`:

- `resolve_main(main_path: str) -> Program` — entry point. Parses the main file,
  then recursively follows every `import`/`from X import Y` statement.
- `resolve_recursive` / `resolve_module_path` / `resolve_file` walk the search-path
  list (binary dir, `std/`, `packages/`, `TAURARO_PATH` entries — see
  `docs/lang/15_modules.md`) to turn a dotted module path (`std.net.tcp`) into a file
  on disk, parse it, and merge its declarations.
- Tracks `all_decls: Vec[Pointer[Decl]]` and a parallel `all_decl_modules: Vec[str]`
  (the dotted path each decl came from) — `main.tr` uses these to split generated C
  into per-module files.
- `parse_errors: int` — if non-zero after `resolve_main`, `main.tr` aborts before
  running Sema (a malformed parse can't be safely lowered).

---

## Stage 4: Semantic Analysis (`src/sema.tr` → HIR in `src/hir.tr`)

**Input:** `Program` (merged AST)
**Output:** `HirProgram` (`src/hir.tr`)

`Sema.analyze(self, prog: Program) -> HirProgram` is the entry point. This is the
largest and most intricate stage — it performs:

- **Type checking** — resolves every expression's `AstType`, checks assignment and
  call compatibility (rules T-1..T-4, documented per-error in
  `docs/lang/19_compiler_errors.md`).
- **Name resolution & scoping** — symbol table with block-scoped shadowing.
- **Lowering AST → HIR** — `Expr`/`Stmt`/`Decl` become `HirExpr`/`HirStmt`/
  `HirFunction`/`HirClass`/etc. (`src/hir.tr`). HIR is "AST with every type filled
  in" — every `HirExpr` variant carries an `AstType ty` field that codegen trusts
  without re-inferring.
- **Memory-safety / ownership analysis** — see next section.
- **Nested declarations** — `class`/`def`/`enum`/`interface` declared inside
  `main()` are hoisted into `sema.nested_classes` / `nested_functions` /
  `nested_enums` / `nested_interfaces`, consumed later by `generate_main_c`.

### Where ownership / escape analysis lives

This is where Tauraro's "no manual `free()`" promise is implemented (full rules in
`docs/lang/13_memory_and_ownership.md` and the safety-model spec docs). At a high
level, `Sema` tracks per-symbol flags as it walks each scope:

- **`decl_block_id`** — every `if`/`while`/`for`/etc. body gets a fresh monotonic
  block id (`next_block_id`) when opened. A local's `decl_block_id` records which
  block it was declared in, so Sema can decide exactly which C block should emit its
  `free()`/`SAutoDrop` when the local goes out of scope — including across
  divergent `if`/`else` branches.
- **`str_escaped`** — set when a `str` local is ever passed as a call/method
  argument. Escaped strings are excluded from auto-drop (the callee or a collection
  may now hold an alias), trading a potential leak for safety against
  use-after-free.
- **`coll_escaped`** — the same idea for `List`/`Vec`/`Dict`/`Map`/`Set` locals:
  if a collection is read in any non-receiver position (passed as an argument,
  assigned to another variable, returned, stored in a literal), it's marked escaped
  and excluded from auto-drop.
- **`SAutoDrop(name, class_name)`** — the HIR statement Sema injects at scope exit
  for every owned, unmoved, unborrowed, non-escaped local. `CGenerator` turns this
  into a `ClassName_free(&name)` call (or the appropriate `_tr_*_release` for `str`
  and collections).

If you're debugging a leak or a use-after-free/double-free, this is the file to
read — but the deep mechanics (per-type release helpers, `_tr_strz`, etc.) are
covered in the memory-management notes, not here.

---

## The Type System & Generic Monomorphization (`src/codegen/c.tr`)

`AstType` (from `src/ast.tr`) is the single type representation used from parsing
through codegen — it's a name (`"List"`, `"int"`, `"MyClass"`, generic type
parameter letters like `"T"`) plus `args: Vec[Pointer[AstType]]` for generic
arguments.

Monomorphization happens entirely in **codegen**, not Sema:

- `CGenerator.scan_mono_prog(prog)` (and its helpers `scan_mono_ty`,
  `scan_mono_block`, `scan_mono_expr`, `scan_mono_stmt`, `scan_mono_func`) walk the
  whole HIR *before* code generation to discover every concrete instantiation of
  every generic class (e.g. `Box[int]`, `Vec[str]`).
- `ensure_mono(cls: HirClass, type_args: Vec[Pointer[AstType]])` generates one
  specialized C struct + function set per unique `(class, type-args)` pair —
  keyed by a name like `Box_i64`. `mono_done: Map[str, bool]` deduplicates.
- `type_subst: Map[str, str]` holds the active `T -> "long long"`-style
  substitution while emitting a monomorphized body; `synth_class_suffix` derives
  the `_i64`/`_str`/etc. suffix from it.
- Results accumulate in `mono_buf: StringBuilder`, which is spliced into the
  generated module/header output.

There is no runtime generic dispatch, boxing, or type erasure — every generic
instantiation becomes its own concrete C code, same as Rust monomorphization or
C++ templates.

---

## Stage 5: C Code Generation (`src/codegen/c.tr`)

**Input:** `HirProgram`
**Output:** C source strings, written by `main.tr` into `build/`

`CGenerator` is a large class (~6500 lines) built around two core traversal
methods plus a family of `generate_*` entry points:

| Method | Role |
|--------|------|
| `gen_expr(self, e_ptr: Pointer[HirExpr]) -> str` | Emits a C expression for any `HirExpr` variant — literals, binops, calls, method calls, f-strings, comprehensions, etc. |
| `gen_stmt(self, s_ptr: Pointer[HirStmt], indent: int)` | Emits C statements (writes into the generator's output buffer) for any `HirStmt` variant, including the `SAutoDrop` release calls from Sema. |
| `register_program(prog)` | First pass: registers every class/enum/interface/function signature so forward references resolve. |
| `scan_mono_prog(prog)` | Second pass: discovers generic instantiations (see above). |
| `generate_types_header(prog) -> str` | Emits `tauraro_types.h` — struct definitions + function prototypes shared by all modules. |
| `generate_module_c(prog, class_set, fn_set, depth) -> str` | Emits one `.c` file for a single module's classes/functions. |
| `generate_main_c(prog, class_set, fn_set) -> str` | Emits `main.c` — the program entry point, plus any `main()`-nested classes/functions hoisted by Sema. |

**Output layout** (written by `main.tr`, one file per module):

```
build/tauraro_rt.h        — runtime header (copied from runtime/tauraro_rt.h)
build/tauraro_types.h      — shared struct defs + all function prototypes
build/include/<path>.c     — one file per std/core module (mirrors dotted path)
build/module_<name>.c       — one file per user/third-party module
build/main.c                — program entry point
```

Classes lower to C structs + `ClassName_method(...)` free functions (first param
`self*`); interfaces lower to vtable structs + wrapper functions; enums lower to
tagged unions; generics are expanded per the monomorphization pass above.

---

## Stage 6: Compilation (GCC/Clang)

`main.tr`'s `compile_all_c` invokes the detected C compiler **once** with every
`.c` file in `build/`, so GCC/Clang can inline across module boundaries at the
object level. With `-o`, intermediate `.c`/`.h` files are deleted after a
successful build (`cleanup_build`); with `--emit c`, compilation is skipped
entirely and the `build/` tree is left in place for inspection.

---

## Walkthrough: Adding a New Keyword/Statement

Suppose you want to add a new statement, e.g. `repeat N: <block>`. Touch files in
this order:

1. **`src/token.tr`** — add a `Token` variant (e.g. `KwRepeat`).
2. **`src/lexer.tr`** — add `"repeat" -> Token.KwRepeat` to `keyword_to_token`.
   (Remember Hausa bilingual aliases if relevant — see
   `docs/lang/01_intro.md`'s "Bilingual" note.)
3. **`src/ast.tr`** — add a new `Stmt` variant, e.g.
   `SRepeat(count: Pointer[Expr], body: Block)`.
4. **`src/parser.tr`** — add `parse_repeat_stmt(self) -> Pointer[Stmt]` and wire it
   into `parse_stmt`'s dispatch (the big `match`/`if` chain that checks
   `self.peek()`).
5. **`src/hir.tr`** — add the matching `HirStmt` variant, e.g.
   `SRepeat(count: Pointer[HirExpr], body: HirBlock)`.
6. **`src/sema.tr`** — in `analyze`'s statement-lowering match, handle `Stmt.SRepeat`:
   type-check `count`, recurse into `body` (new block scope = new `decl_block_id`,
   via `next_block_id`), and emit `HirStmt.SRepeat(...)`.
7. **`src/codegen/c.tr`** — in `gen_stmt`'s match, handle `HirStmt.SRepeat` by
   emitting the equivalent C (e.g. a `for` loop with a synthesized counter
   variable).
8. **Bootstrap & verify** — rebuild `tauraroc.exe` from the modified `src/*.tr` and
   confirm the self-host fixpoint still holds (see
   [`02_contributing.md`](02_contributing.md)). Add an example under `examples/`
   exercising the new syntax.

If the new construct also needs new ownership rules (e.g. it introduces a new
scope or can alias a collection), update the `decl_block_id`/`str_escaped`/
`coll_escaped` tracking in `sema.tr` as well — see the ownership section above.

---

Next: [Building and Contributing →](02_contributing.md)
