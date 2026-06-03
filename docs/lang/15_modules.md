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

When the compiler encounters `import foo.bar`, it tries two patterns inside each search path:

1. `<search_path>/foo/bar.tr`
2. `<search_path>/foo/bar/mod.tr`

The first match wins.

**Search paths checked (in order):**

| Priority | Path | Notes |
|----------|------|-------|
| 1 | `.` | Current working directory at compile time |
| 2 | Directory of the entry `.tr` file | Lets a project import its own siblings |
| 3 | Parent of the entry file's directory | Useful when entry lives in `src/` |
| 4 | Grandparent of the entry file's directory | Needed when entry lives in `src/cmd/` |
| 5 | `<compiler_bin>/` | Compiler installation root |
| 6 | `<compiler_bin>/std/` | Built-in standard library |
| 7 | `<compiler_bin>/packages/` | Globally installed third-party packages |
| 8 | `<compiler_bin>/packages/sites/` | Site-specific installs (e.g. pip-style) |
| 9 | `packages/` | Project-local packages (CWD-relative) |
| 10 | `packages/sites/` | Project-local site packages (CWD-relative) |

**Recursive resolution:** Each imported module is scanned for its own imports, which are resolved transitively. The compiler builds the full dependency graph before emitting any C.

**Error for missing module:**
```
ERROR: cannot find module 'math.geometry'
  searched: ./math/geometry.tr
            ./math/geometry/mod.tr
            (+ all other search paths)
  FIX: check the file path and ensure it exists in a search path
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

## How Modules Are Compiled

Each module gets its own `.c` file. The compiler:

1. Resolves all imports recursively from the main entry file
2. Builds a unified `HirProgram` (semantic analysis, type checking, ownership inference)
3. Emits **one `.c` file per module** into the `build/` directory:

```
build/
  tauraro_rt.h          — runtime header (copied from the compiler's runtime/)
  tauraro_types.h       — shared type definitions and all forward prototypes
  main.c                — entry-point module
  module_utils.c        — one file for each user/third-party module
  module_math.c
  include/std/io.c      — one file per standard-library module
  include/std/string/str.c
  …
```

4. Invokes the detected C compiler **once** with every `.c` file in `build/`:

```bash
gcc -O2 build/main.c build/module_utils.c build/include/std/io.c … -o program
```

**With `-o <output>`:** The `.c` files in `build/` are removed after a successful link. Only the executable survives.

**With `--emit c`:** No compilation occurs. The `.c` files are written to `build/` and kept. Use this to inspect generated C or integrate with an external build system.

**Why one invocation with separate files:**
- GCC and Clang can inline and constant-fold across module boundaries at the object level
- The `build/` directory gives you a per-module view of the generated code
- The linker step is handled by the compiler driver, so no separate `ld` invocation is needed

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

The standard library is installed alongside the compiler binary in `<bin>/std/`. It is always on the search path — no `-I` or path configuration needed.

| Module | Import | Contents |
|--------|--------|---------|
| `std.core.vec` | `from std.core.vec import Vec` | `Vec[T]` growable array |
| `std.core.map` | `from std.core.map import Map` | Hash map |
| `std.core.string` | `from std.core.string import StringBuilder` | String builder |
| `std.core.ptr` | `from std.core.ptr import Pointer` | Raw pointer wrapper |
| `std.string.str` | `from std.string.str import Str` | String utilities |
| `std.string.fmt` | `from std.string.fmt import Fmt` | Number formatting |
| `std.fs` | `from std.fs import File, Dir, Path` | File system |
| `std.net.http` | `from std.net.http import HttpClient` | HTTP client |
| `std.net.https` | `from std.net.https import HttpsClient` | HTTPS client (opt-in) |
| `std.net.tcp` | `from std.net.tcp import TcpStream, TcpListener` | TCP sockets |
| `std.net.http_server` | `from std.net.http_server import HttpServer` | HTTP server |
| `std.iter.range` | `from std.iter.range import Range` | Integer range iteration |
| `std.iter.transform` | `from std.iter.transform import Transform` | `Vec[int]` transforms |
| `std.iter.float_transform` | `from std.iter.float_transform import FloatTransform` | `Vec[float]` transforms |
| `std.regex` | `from std.regex import Regex` | Regular expressions |
| `std.crypto.hash` | `from std.crypto.hash import Hash` | SHA-256, MD5 |
| `std.crypto.hmac` | `from std.crypto.hmac import Hmac` | HMAC-SHA256 |
| `std.crypto.uuid` | `from std.crypto.uuid import UUID` | UUID v4 generation |
| `std.compress.zlib` | `from std.compress.zlib import Zlib` | zlib compress/decompress |
| `std.unicode` | `from std.unicode import Unicode` | Unicode utilities |
| `std.async.task` | `from std.async.task import Task, Pool` | Async task runtime |
| `std.sync` | `from std.sync import Mutex, Atomic` | Synchronization primitives |
| `std.time` | `from std.time import Time, Clock` | Time and clock |

For the full API of each module see `docs/std/`.

**Third-party packages** installed into `<bin>/packages/` or `packages/` are imported the same way — by their module path. No special syntax distinguishes stdlib from packages.

---

## Common Module Errors

### Module not found

```
ERROR: cannot find module 'utils.parser'
```
**Causes:**
- File is at `utils/parser.tr` but you're running from a different working directory
- File is named `Parser.tr` (wrong case — Tauraro module paths are case-sensitive on Linux)
- Third-party package is not installed in `packages/` or `<bin>/packages/`

**Fix:** Check the file path and working directory. For third-party packages, place them in `packages/<pkg>/mod.tr` (or the layout distributed by the package).

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
