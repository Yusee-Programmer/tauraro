# Tauraro

A compiled, statically-typed programming language with Python-style syntax, Rust-level performance, and full bilingual (English + Hausa) keyword support.

## Installation

Download the latest binary for your platform from the [Releases](https://github.com/Yusee-Programmer/tauraro/releases) page:

| Platform | File |
|----------|------|
| Windows (x64) | `tauraroc-windows-x64.zip` |
| Linux (x64) | `tauraroc-linux-x64.tar.gz` |
| macOS (x64/arm64) | `tauraroc-macos.tar.gz` |

Extract and place `tauraroc` (or `tauraroc.exe` on Windows) somewhere on your `PATH`.

**Requirements:** GCC or Clang must be installed — `tauraroc` compiles to C and calls the system C compiler to produce the final binary.

## Hello World

```tauraro
def main():
    print("Sannu duniya!")
```

```sh
tauraroc --run hello.tr
```

## CLI Reference

```
tauraroc <file.tr> [options]

  --run             Compile and immediately execute
  --check           Semantic analysis only, no output
  --emit c          Print generated C code
  --emit ast        Print AST and stop
  --emit mir        Print MIR and stop
  --verbose         Show all pipeline phases
  -o <path>         Output executable path
  -O0/-O1/-O2/-O3  Optimization level (default: -O2)
  -Os               Optimize for size
```

## Language Features

- **Classes** with method dispatch, inheritance, and interfaces
- **Enums** as tagged unions with pattern matching
- **Generics** monomorphized at compile time
- **F-strings** — `f"result = {value}"`
- **Ownership inference** — memory managed automatically, no GC
- **Result/Option types** for explicit error handling
- **Bilingual keywords** — English and Hausa equally supported

## Bilingual Keywords

Every keyword has an English and Hausa equivalent — programs can be written in either or mixed freely:

| English | Hausa | Meaning |
|---------|-------|---------|
| `def` | `aiki` | function |
| `class` | `aji` | class |
| `struct` | `tsari` | struct |
| `if` | `idan` | conditional |
| `elif` | `koidan` | else-if |
| `else` | `sai` | else |
| `for` | `ga` | for loop |
| `while` | `yayinda` | while loop |
| `return` | `dawo` | return |
| `break` | `tsaya` | break |
| `continue` | `ci_gaba` | continue |
| `match` | `duba` | pattern match |
| `case` | `hali` | match arm |
| `try` | `gwada` | try block |
| `except` | `kama` | except handler |
| `finally` | `karshe` | finally block |
| `raise` | `jefa` | raise exception |
| `async` | `ba_jira` | async function |
| `await` | `jira` | await expression |
| `import` | `shigo` | import module |
| `from` | `daga` | from import |
| `as` | `kamar` | alias |
| `in` | `a_cikin` | membership / loop |
| `true` | `gaskiya` | boolean true |
| `false` | `karya` | boolean false |
| `none` | `babu` | null/none |
| `and` | `da` | logical and |
| `or` | `ko` | logical or |
| `not` | `ba` | logical not |
| `print` | `buga` | print to stdout |

## Example

```tauraro
class Kirga:
    mut adadi: i64

    def init(n: i64) -> Kirga:
        mut k = Kirga()
        k.adadi = n
        return k

    def ƙara(self, n: i64):
        self.adadi = self.adadi + n

    def nuna(self):
        buga(f"adadi = {self.adadi}")

def main():
    mut k = Kirga.init(0)
    ga i in range(10):
        k.ƙara(i)
    k.nuna()
```

## Compiler Pipeline

```
.tr source  →  Lexer  →  Parser  →  Sema  →  HIR  →  C codegen  →  GCC/Clang  →  exe
```

All stages are written in Tauraro itself — the compiler is fully self-hosted.
