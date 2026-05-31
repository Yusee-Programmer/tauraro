# 15 — Modules

---

## What Is a Module

A **module** is a `.tr` source file. Every file is a module. Modules have:
- A name derived from their file path
- A public/private boundary enforced by `pub`
- An import system for referencing other modules

There are no explicit module declarations, no separate header files, no build manifests. The file system is the module system.

---

## Module Naming

A module's name is its file path relative to the project root, with path separators replaced by `.` and no `.tr` extension:

| File path | Module name | Import statement |
|-----------|-------------|-----------------|
| `utils.tr` | `utils` | `import utils` |
| `math/geometry.tr` | `math.geometry` | `import math.geometry` |
| `math/geometry/mod.tr` | `math.geometry` | `import math.geometry` |
| `std/vec.tr` | `std.vec` | `import std.vec` |
| `core/string.tr` | `core.string` | `import core.string` |

The `mod.tr` form: a directory with a `mod.tr` file inside. This lets you split a large module across sub-files while exposing a single import path.

---

## Importing Modules

### import

```python
import math.geometry

def main():
    mut area = math.geometry.circle_area(5.0)
    print(f"area = {area}")
```

### import ... as (alias)

```python
import math.geometry as geo
import core.string as cstr

def main():
    mut area = geo.circle_area(5.0)
    mut upper = cstr.to_upper("hello")
```

### from ... import (selective import)

```python
from math.geometry import circle_area, rect_area
from core.string import to_upper, to_lower

def main():
    mut a = circle_area(5.0)
    mut u = to_upper("hello")
```

### from ... import ... as (aliased selective import)

```python
from math.geometry import circle_area as area
from core.string import to_upper as upper_case

def main():
    print(area(5.0))
    print(upper_case("hello"))
```

**Compiler behavior:** `from ... import` registers the imported names directly in the current scope. A name collision between two imported symbols is a compile error.

---

## Module Resolution

When the compiler encounters `import foo.bar`, it searches for the module in this order:

1. `foo/bar.tr` in each search path
2. `foo/bar/mod.tr` in each search path

**Search paths checked (in order):**
1. `.` — current working directory at compile time
2. Directory containing the main source file
3. `std/` if it exists
4. `stdlib/` if it exists
5. Paths from `-I <dir>` flags

**Recursive resolution:** Each imported module is scanned for its own imports, which are resolved transitively. The compiler builds the full dependency graph and compiles all modules together.

**Error for missing module:**
```
ERROR: cannot find module 'math.geometry'
  searched: ./math/geometry.tr
            ./math/geometry/mod.tr
  FIX: check the file path and ensure it exists relative to the project root
```

---

## Visibility: pub

All declarations are **private by default**. Use `pub` to expose them to other modules.

### Public vs Private

```python
# geometry.tr

# Private: only usable inside geometry.tr
def _deg_to_rad(deg: float) -> float:
    return deg * 3.14159 / 180.0

# Public: importable from other modules
pub def circle_area(r: float) -> float:
    return 3.14159 * r * r

# Public class
pub class Circle:
    pub radius: float      # public field
    cached_area: float     # private field — only accessible inside geometry.tr

extend Circle:
    pub def area(self) -> float:       # public method
        return 3.14159 * self.radius * self.radius

    def _cache_area(self) -> void:     # private method
        self.cached_area = self.area()
```

**Visibility matrix:**

| Declaration | `pub` | Accessible from other modules? |
|-------------|-------|-------------------------------|
| `def f()` | No | No |
| `pub def f()` | Yes | Yes |
| `class C` | No | No — class cannot be used at all |
| `pub class C` | Yes | Yes |
| `pub class C` with field `x` | No `pub` on field | Field not accessible |
| `pub class C` with `pub x` | Yes `pub` on field | Field accessible |
| Method in `extend` | No `pub` | Not callable from other modules |
| Method in `extend` with `pub` | Yes | Callable from other modules |

**Compiler rule:** Accessing a private symbol from outside its module is a compile error:
```
ERROR: function '_deg_to_rad' in module 'math.geometry' is private
  FIX: add 'pub' to the declaration, or access it through a public interface
```

---

## Exporting for C Interop: export

The `export` keyword makes a function visible with a stable, unmangled C symbol name:

```python
pub export def add(a: int, b: int) -> int:
    return a + b

pub export def tauraro_init() -> void:
    print("library initialized")
```

**Without `export`:** A function `add` in module `utils` is accessible as `utils.add` — the module path is part of the name.

**With `export`:** The function is exported with its plain name `add` — no module prefix, with full shared library visibility.

**`export` implies `pub`:** You can write `export def f()` without `pub` — the function is still public.

**Use cases:**
- Building a `.so`/`.dll` library callable from C
- Building the Tauraro compiler in self-hosted mode (the runtime calls specific functions by exact C name)
- Interop with dynamic linkers that need known symbol names

---

## The Unity Build

All modules are compiled together into **one C compilation unit**. The compiler:

1. Resolves all imports recursively from the main entry file
2. Merges all classes, enums, interfaces, and functions into one `HirProgram`
3. Emits one or more `.c` files (one per module for the multi-module path, or one merged file)

**Why unity build:**
- GCC and Clang can inline and constant-fold across the entire program
- No link-time optimization flags needed
- Generated C is easy to inspect — one file per module

**Symbol naming:** Functions from module `math.geometry` are prefixed `math_geometry_` in C (dots → underscores). This prevents collisions between modules defining the same name:

| Tauraro | C symbol |
|---------|----------|
| `math.geometry.circle_area` | `math_geometry_circle_area` |
| `core.string.to_upper` | `core_string_to_upper` |
| `main.process` | `process` (main module has no prefix) |

**Don't hardcode C names** — module hierarchy refactors change them. Use `export` for stable names.

---

## Organizing a Project

### Small Project

```
project/
  main.tr          # entry point
  utils.tr         # helper functions
  types.tr         # shared data types
```

```python
# main.tr
from utils import format_output
from types import Config

def main():
    mut cfg = Config.init()
    print(format_output(cfg))
```

### Medium Project

```
project/
  main.tr
  config.tr
  math/
    vec2.tr        # import as: math.vec2
    matrix.tr      # import as: math.matrix
  network/
    http.tr        # import as: network.http
    tcp.tr         # import as: network.tcp
    mod.tr         # re-exports vec2 + matrix: import as network
```

```python
# main.tr
import math.vec2 as v2
import math.matrix as mat
from network.http import get, post
```

### `mod.tr` as Re-Export Hub

```python
# math/mod.tr
from math.vec2 import Vec2
from math.matrix import Matrix4

# Other files can now do:
# from math import Vec2, Matrix4
```

### Circular Import Warning

Circular imports (module A imports B, and B imports A) are not supported. The compiler will error with a cycle detection message. Restructure by extracting the shared types into a third module that both A and B import.

---

## The Standard Library Modules

The standard library lives in `tauraro/src/include/core/`:

| Module | Import | Contents |
|--------|--------|---------|
| `core.vec` | `from core.vec import Vec` | `Vec[T]` growable array |
| `core.map` | `from core.map import Map` | String-keyed hash map |
| `core.string` | `from core.string import StringBuilder` | String builder utility |
| `core.alloc` | `from core.alloc import alloc` | Typed heap allocation |
| `core.io` | `from core.io import write_file, read_file` | File I/O |

These are the modules used by the self-hosted compiler itself. For application code, the built-in `List[T]`, `Dict`, and string operations cover most needs without imports.

---

## Common Module Errors

### Module not found

```
ERROR: cannot find module 'utils.parser'
```
**Causes:**
- File is at `utils/parser.tr` but you're running from a different working directory
- File is named `Parser.tr` (wrong case — Tauraro module paths are case-sensitive on Linux)
- Missing `-I` flag to add the search path

**Fix:** Check the file path and working directory. Add `-I <dir>` if needed.

### Accessing private symbol

```
ERROR: class 'Config' in module 'config' is private
```
**Fix:** Add `pub` to `class Config:` in `config.tr`

### Name collision after `from ... import`

```python
from math.vec import Vec2
from graphics.vec import Vec2   # ERROR: 'Vec2' already imported

from graphics.vec import Vec2 as GVec2   # OK: aliased
```

### Circular import

```
ERROR: circular import: main → utils → main
```
**Fix:** Extract shared types into a new module that both import.

---

Next: [Concurrency →](16_concurrency.md)
