# Tauraro Self-Hosted Compiler

A complete, production-ready Tauraro compiler written entirely in Tauraro.
This is the **Phase 1.5** milestone: self-hosting the compiler so it can compile itself.

## Quick Start

```bash
# Build the Rust bootstrap compiler first
cargo build --release

# Compile a Tauraro source file
cargo run -- compiler/src/main.tr --emit c > bootstrap.c

# Compile the generated C to an executable
gcc -O3 bootstrap.c runtime/tauraro_rt.h -o tauraroc

# Now use the self-hosted compiler
./tauraroc examples/hello.tr --run
```

## CLI Reference

```
tauraroc <file.tr> [options]

Options:
  --emit c          Emit generated C code to stdout (or -o file)
  --emit ast        Print AST and stop
  --emit mir        Print MIR info and stop
  --run             Compile and immediately execute
  --check           Semantic analysis only, no codegen
  --verbose         Show all pipeline phases
  --backend llvm    Use LLVM IR backend (stub)
  -o <path>         Output file path
  -O0/-O1/-O2/-O3  GCC optimization level (default: -O2)
  -Os               Optimize for size
```

## Project Structure

```
compiler/
  src/
    main.tr          ← CLI driver, module resolver, linker invocation
    token.tr         ← Token enum + keyword table (bilingual)
    lexer.tr         ← Hand-written FSM lexer with indentation tracking
    ast.tr           ← All AST types (Program, Decl, Stmt, Expr, Type, Pattern)
    parser.tr        ← Recursive descent parser (1400+ lines)
    sema.tr          ← Semantic analysis, type checking, ownership inference
    hir.tr           ← HIR types + AST→HIR lowering
    resolver.tr      ← Unity-build module resolver
    codegen/
      c.tr           ← C transpiler backend (PRIMARY, production-ready)
      llvm.tr        ← LLVM IR backend (stub, Phase 2)
      mod.tr         ← Module declarations
  tests/
    lexer_test.tr    ← Lexer unit tests
    parser_test.tr   ← Parser unit tests
    sema_test.tr     ← Semantic analysis tests
    self_host_test.tr ← Integration & bootstrap tests
```

## Compiler Pipeline

```
.tr source
    │
    ▼ Lexer (lexer.tr)
Token stream with indent/dedent tokens
    │
    ▼ Parser (parser.tr)
Abstract Syntax Tree (ast.tr types)
    │
    ▼ Semantic Analysis (sema.tr)
Annotated HIR (hir.tr types)
  - Type checking
  - Ownership inference (Own/Borrow/Move/Shared/Stack)
  - Escape analysis
  - Free injection (HirStmt::SFree)
    │
    ▼ C Code Generator (codegen/c.tr)
Brutally optimized C code
  - Struct definitions for classes
  - Tagged unions for enums
  - Vtables for interfaces
  - range() → for(long long i = start; i < end; i++)
  - f-strings → snprintf()
    │
    ▼ GCC/Clang
Native executable
```

## Language Features Supported

- **Classes** → C structs with `ClassName_method(ClassName* self, ...)` mangling
- **Enums** → Tagged unions with constants and constructor helpers
- **Interfaces** → Vtable dispatch structs
- **Generics** → Monomorphized at codegen time
- **Pattern matching** → if/else chains with binding extraction
- **F-strings** → snprintf with format string synthesis
- **For loops** → Optimized C for loops (range → counter loops)
- **Ownership inference** → Automatic free injection
- **Bilingual keywords** → English + Hausa equally supported
- **Error handling** → Result types as plain structs

## Bilingual Support

Every keyword has an English and Hausa form:

| English | Hausa | Meaning |
|---------|-------|---------|
| `def` | `aiki` | function definition |
| `class` | `aji` | class definition |
| `if` | `idan` | conditional |
| `elif` | `koidan` | else-if |
| `else` | `sai` | else |
| `for` | `ga` | for loop |
| `while` | `yayinda` | while loop |
| `return` | `dawo` | return |
| `match` | `duba` | pattern match |
| `case` | `hali` | match arm |
| `true` | `gaskiya` | boolean true |
| `false` | `karya` | boolean false |
| `none` | `babu` | null/none |
| `and` | `da` | logical and |
| `or` | `ko` | logical or |
| `not` | `ba` | logical not |
| `print` | `buga` | print to stdout |
