# Tauraro Self-Hosted Compiler — Architecture

## Overview

The self-hosted compiler is a complete Tauraro compiler written entirely in Tauraro.
It mirrors the Rust bootstrap compiler's pipeline but is designed to be both readable
and capable of compiling itself (bootstrap capability).

## Module Dependency Graph

```
main.tr
  ├── token.tr          (no dependencies)
  ├── lexer.tr          → token.tr
  ├── ast.tr            → token.tr
  ├── parser.tr         → lexer.tr, ast.tr, token.tr
  ├── hir.tr            → ast.tr
  ├── sema.tr           → ast.tr, hir.tr
  ├── resolver.tr       → (all of the above)
  └── codegen/
        ├── mod.tr      (declares submodules)
        ├── c.tr        → hir.tr, ast.tr
        └── llvm.tr     → hir.tr (stub)
```

Data flows in one direction: `token → ast → hir → C output`. No module imports
from a later stage.

## Phase 1 — Lexer (`src/lexer.tr`)

A hand-written finite state machine. No external dependencies.

**Indentation tracking**: The lexer maintains an `indent_stack: Vec[int]`. On each
newline it measures leading spaces/tabs, comparing against the top of the stack to
emit `Token.Indent` / `Token.Dedent` tokens. This converts Python-style significant
whitespace into explicit bracket-like tokens the parser can consume normally.

**Bilingual keywords**: A single `KEYWORDS` lookup table maps both English and Hausa
spellings to the same `Token` variant (e.g. both `"def"` and `"aiki"` → `Token.KwDef`).
The rest of the compiler never needs to know which spelling was used.

**Numeric literals**: Decimal integers, hex (`0x`/`0X`), octal (`0o`), binary (`0b`),
and floating-point (with optional `e`/`E` exponent) are all recognized in a single
scanning pass.

## Phase 2 — Parser (`src/parser.tr`)

Recursive descent, approximately 1 400 lines. Each grammar production is one method.

**Key design choices:**

- **No backtracking**: Every parse decision is made with at most one token of
  lookahead (`self.peek()` returns the current token without consuming it).
- **Error recovery**: On a parse error the parser emits a diagnostic and attempts
  to resync at the next statement boundary (next `Newline` or `Dedent`).
- **Decorator support**: Decorators are collected before the decorated declaration
  and attached as `Vec[Decorator]` to `FunctionDef`, `ClassDef`, etc.
- **Indentation handling**: `parse_block()` consumes the `Indent` token, delegates
  to `parse_stmt()` in a loop, then expects `Dedent`.

**Important grammar rules:**

```
program     ::= decl*
decl        ::= func_def | class_def | enum_def | interface_def
              | extend_def | import_stmt | from_import | extern_decl
              | decorator decl
func_def    ::= ('def'|'aiki') IDENT '(' params ')' ('->' type)? ':' block
class_def   ::= ('class'|'aji') IDENT (':' bases)? ':' class_body
enum_def    ::= 'enum' IDENT ':' INDENT (IDENT ('(' fields ')')? NEWLINE)+ DEDENT
block       ::= INDENT stmt+ DEDENT | simple_stmt NEWLINE
```

## Phase 3 — Semantic Analysis (`src/sema.tr`)

Single-pass visitor that simultaneously type-checks and lowers AST → HIR.

**Scope management**: `ScopeStack` is a `Vec[HashMap[str, HirType]]`. `push_scope()`
/ `pop_scope()` wrap every block. Variable lookup walks the stack from top to bottom.

**Ownership inference algorithm**:
1. All `mut` local variables start as `Own` if their type is heap-allocated
   (`Str`, `List`, custom classes) or `Stack` for primitives.
2. When a variable is passed to a function that takes ownership (heuristic:
   non-`mut` parameter of heap type), it is marked `Move`.
3. At `pop_scope()`, every `Own` variable that has not been moved gets a
   `HirStmt::SFree` injected at the end of its enclosing block.
4. `Borrow` is assigned when the address of a variable is taken or when a
   reference parameter is detected.

**Type checking**: Expression types are inferred bottom-up. Binary operators apply
standard widening rules (int + float → float). Mismatches emit entries into
`self.errors: Vec[str]`.

**Built-in prelude**: `print`/`buga`, `range`, `len`, `str`, `int`, `float`,
`Result[T,E]`, `Option[T]`, `List[T]`, `Dict[K,V]` are registered in a top-level
scope before user code is analyzed.

## Phase 4 — HIR (`src/hir.tr`)

A simplified, fully-typed intermediate representation. The HIR is what backends consume.

```
HirProgram
  functions  : Vec[HirFunction]
  classes    : Vec[HirClass]
  enums      : Vec[HirEnum]
  interfaces : Vec[HirInterface]

HirFunction
  name       : str
  params     : Vec[HirParam]
  ret        : HirType
  body       : HirBlock

HirStmt
  SExpr(e)            plain expression statement
  SLet(name, ty, e)   variable binding
  SAssign(lhs, rhs)   assignment
  SIf(cond, then, else_block)
  SWhile(cond, body)
  SFor(var, iter, body)
  SReturn(e?)
  SFree(name)         injected by sema — emits free()
  STry(body, catches, finally)
  SRaise(e)
  SAssert(e, msg?)
  SBreak / SContinue
  SPass

HirExpr
  ELit(Literal)
  EVar(name, ty)
  EBinop(op, lhs, rhs, ty)
  EUnop(op, e, ty)
  ECall(func, args, ty)
  EMethodCall(obj, method, args, ty)
  EField(obj, field, ty)
  EIndex(obj, idx, ty)
  EFString(parts, ty)
  EMatch(subj, arms, ty)
  ECast(e, ty)
  EList / EDict / ETuple
```

## Phase 5 — C Code Generator (`src/codegen/c.tr`)

The primary backend. Produces standard C11 that compiles with GCC or Clang.

### Seven-phase output structure

```
generate(prog):
  phase 1 — pragma optimize header (#pragma GCC optimize("O3,unroll-loops") …)
  phase 2 — runtime include (#include "tauraro_rt.h")
  phase 3 — forward declarations (one per function/method)
  phase 4 — struct definitions (classes, enums, interfaces — order matters for C)
  phase 5 — function prototypes (repeated for cross-referencing)
  phase 6 — method bodies (ClassName_method functions)
  phase 7 — free function bodies + C main wrapper
```

### OOP lowering

| Tauraro               | C output                                      |
|-----------------------|-----------------------------------------------|
| `class Foo { x: int }`| `typedef struct { long long x; } Foo;`        |
| `def bar(self)`       | `void Foo_bar(Foo* self, ...)`                |
| `interface IFoo`      | `typedef struct { void (*draw)(void*); } IFoo_vtable;` + `typedef struct { void* obj; IFoo_vtable* vtable; } IFoo_obj;` |
| `enum Color { Red }`  | `typedef enum { Color_Red } Color_tag;` + tagged union struct + constructor functions |

### Range loop optimization

```tauraro
for i in range(n):      →    for (long long i = 0; i < n; i++) {
    body                          body
                               }

for i in range(a, b):  →    for (long long i = a; i < b; i++) { … }
for i in range(a,b,s): →    for (long long i = a; i < b; i += s) { … }
```

Non-range iterators fall back to a generic iterator protocol loop.

### F-string lowering

```tauraro
f"hello {name}, age {age}"
```

Becomes a GCC statement-expression:

```c
({ char _fs_N[4096]; snprintf(_fs_N, sizeof(_fs_N), "hello %s, age %lld", name, age); _fs_N; })
```

Format specifiers are chosen by type: `%lld` for int, `%g` for float, `%s` for str/bool.
Bool values are wrapped: `(age ? "true" : "false")`.

### Pattern matching

`match subj: case Pat: body` is lowered to a chain of `if / else if` blocks.
Each pattern type generates a different condition:

| Pattern            | Generated condition                          |
|--------------------|----------------------------------------------|
| `Lit(42)`          | `subj == 42`                                 |
| `Wildcard`         | `1` (always true)                            |
| `Ident(x)`         | `1` (binding — `__auto_type x = subj;`)      |
| `EnumVariant(T.V)` | `subj.tag == T_V`                            |
| `VariantBind`      | `subj.tag == T_V` + `__auto_type f = subj.data.V.f;` |

### Ownership / free injection

`HirStmt::SFree("x")` emits:

```c
if (x) { free(x); x = NULL; }
```

The guarded pattern prevents double-free and is idempotent.

## Module Resolver (`src/resolver.tr`)

Unity-build strategy: all imported modules are merged into a single `HirProgram`
before code generation. This avoids separate compilation and linking complexity
while self-hosting.

Algorithm:
1. Start with the entry file's AST.
2. Walk all `import` / `from … import` declarations.
3. For each module path, convert dots to directory separators and append `.tr`
   (or look for `mod.tr` inside a directory of that name).
4. Recursively resolve dependencies (cycle detection via a `visited: HashMap[str,bool]`).
5. Merge resolved `HirProgram`s by appending their function/class/enum/interface lists.

## Safety Guarantees (enforced at compile time)

1. **No use-after-move**: variables marked `Move` cannot appear in subsequent expressions.
2. **No double-free**: `SFree` is emitted exactly once per `Own` variable per scope exit.
3. **No null dereference on owned pointers**: own pointers are initialized before use (checked by sema).
4. **Borrow outlives owner**: borrow references must not escape their owner's scope.
5. **No data races** (single-threaded model enforced; `spawn` is async-task, not OS thread).
6. **Exhaustive match**: wildcard or full variant coverage required in `match`.
7. **Immutable by default**: mutation requires `mut` keyword; assignments to non-`mut` are errors.
8. **Type-safe casts**: `as` only between numeric types or between compatible pointer types.
9. **No implicit coercion**: int/float/str conversions are always explicit.

## Testing Architecture

```
compiler/tests/
  lexer_test.tr      → tests Token stream correctness
  parser_test.tr     → tests AST shape (decl counts, field counts, variant names)
  sema_test.tr       → tests HIR shape (function/class/enum lowering)
  self_host_test.tr  → end-to-end: source string → C string, checks substrings
```

All tests use the same `assert_true` / `assert_eq_int` helpers and print
`PASS [label]` or `FAIL [label]: ...` to stdout. No test framework dependency.

## Known Limitations

- **LLVM backend** (`codegen/llvm.tr`): stub only. Generates no real LLVM IR.
- **Async/await**: parsed and stored in HIR but no runtime semantics in C backend.
- **GPU blocks**: parsed, lowered to a no-op comment in C.
- **Generics**: basic monomorphization for `List[T]`, `Dict[K,V]`, `Result[T,E]`;
  user-defined generic classes/functions are partially supported.
- **Error locations**: diagnostics lack line/column numbers (no span tracking yet).
- **Decorators**: stored in HIR, no codegen action taken.
