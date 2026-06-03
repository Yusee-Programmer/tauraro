# std.sys.env — Environment, Arguments, and Search Paths

```tauraro
from std.sys.env import Env
```

`Env` gives a running Tauraro program access to the OS environment: command-line arguments, environment variables, and the module search path.

---

## Quick start

```tauraro
from std.sys.env import Env

def main():
    mut env = Env.init()
    print(env.program_name)               # argv[0]
    print(str(env.arg_count))             # total argument count
    print(env.get_var("HOME"))            # environment variable
    print(str(env.path.len()))            # number of search paths
```

---

## Class: Env

### Fields

| Field | Type | Description |
|-------|------|-------------|
| `arg_count` | `int` | Total argument count (includes `argv[0]`) |
| `program_name` | `str` | `argv[0]` — path to the running binary |
| `path` | `Vec[str]` | Module search path (see below) |

### Constructor

| Method | Signature | Description |
|--------|-----------|-------------|
| `Env.init` | `() -> Env` | Capture argc/argv and populate `path` with standard directories |

---

## Environment variables

| Method | Signature | Returns | Description |
|--------|-----------|---------|-------------|
| `get_var` | `(self, name: str) -> str` | `str` | Value of env var `name`, or `""` if unset |
| `has_var` | `(self, name: str) -> bool` | `bool` | `true` if `name` is set |
| `set_var` | `(self, name: str, value: str) -> bool` | `bool` | Set env var; returns `true` on success |
| `unset_var` | `(self, name: str) -> bool` | `bool` | Remove env var; returns `true` on success |

```tauraro
mut env = Env.init()
if env.has_var("DEBUG"):
    print("debug mode: " + env.get_var("DEBUG"))
env.set_var("APP_PORT", "8080")
```

---

## Command-line arguments

| Method | Signature | Returns | Description |
|--------|-----------|---------|-------------|
| `get_arg_count` | `(self) -> int` | `int` | Same as `arg_count` field |
| `get_arg` | `(self, n: int) -> str` | `str` | `argv[n]`, or `""` if out of range |
| `all_args` | `(self) -> Vec[str]` | `Vec[str]` | All arguments including `argv[0]` |
| `user_args` | `(self) -> Vec[str]` | `Vec[str]` | Arguments from `argv[1]` onward |

```tauraro
mut env = Env.init()
mut args = env.user_args()
mut i = 0
while i < args.len():
    print(args.get(i))
    i = i + 1
```

---

## Env.path — the module search path

`Env.path` is a `Vec[str]` pre-populated by `Env.init()` with the standard directories the Tauraro compiler uses to locate modules:

| Index | Path | Notes |
|-------|------|-------|
| 0 | Current working directory | CWD at the time of the call |
| 1 | `<exe_dir>/` | Directory containing the running binary |
| 2 | `<exe_dir>/std/` | Built-in standard library |
| 3 | `<exe_dir>/packages/` | Globally installed packages |
| 4 | `<exe_dir>/packages/sites/` | Site-specific installs |
| 5 | `packages/` | Project-local packages (CWD-relative) |
| 6 | `packages/sites/` | Project-local site packages (CWD-relative) |

You can read, append, prepend, or remove entries:

```tauraro
mut env = Env.init()

# Add a project-specific library directory (checked last)
env.path_add("/opt/mylibs")

# Add a high-priority override (checked first)
env.path_insert("./vendor")

# Check membership
if env.path_contains("/opt/mylibs"):
    print("custom lib path active")

# Remove an entry
env.path_remove("packages/sites")

# Iterate all paths
mut i = 0
while i < env.path.len():
    print(env.path.get(i))
    i = i + 1
```

### Path helper methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `path_add` | `(self, dir: str) -> void` | Append `dir` to `path` (lowest priority) |
| `path_insert` | `(self, dir: str) -> void` | Prepend `dir` to `path` (highest priority) |
| `path_contains` | `(self, dir: str) -> bool` | `true` if `dir` is already in `path` |
| `path_remove` | `(self, dir: str) -> void` | Remove all occurrences of `dir` from `path` |

---

## How Env.path connects to the compiler: TAURARO_PATH

Tauraro resolves imports at **compile time**. `Env.path` is a runtime value and cannot directly affect the compiler's resolver. The bridge is the **`TAURARO_PATH` environment variable**.

When `tauraroc` starts, it reads `TAURARO_PATH` and appends every entry to its module resolver search path — exactly like Python's `PYTHONPATH` extends `sys.path`.

| Platform | Separator | Example |
|----------|-----------|---------|
| Linux / macOS | `:` | `TAURARO_PATH=/opt/mylibs:/home/user/pkgs` |
| Windows | `;` | `TAURARO_PATH=C:\mylibs;C:\Users\user\pkgs` |

```bash
# Linux / macOS
export TAURARO_PATH=/opt/mylibs:/home/user/pkgs
tauraroc --run myapp.tr

# Windows (PowerShell)
$env:TAURARO_PATH = "C:\mylibs;C:\Users\user\pkgs"
tauraroc --run myapp.tr
```

### Complete search order (compiler resolver)

When `tauraroc` resolves `import foo.bar`, it checks these directories in order:

1. Entry file's directory and its parent / grandparent
2. `<bin_dir>/` — compiler installation root
3. `<bin_dir>/std/` — standard library
4. `<bin_dir>/packages/` — globally installed packages
5. `<bin_dir>/packages/sites/` — site-specific installs
6. `packages/` — project-local packages (CWD-relative)
7. `packages/sites/` — project-local site packages
8. **Every entry in `TAURARO_PATH`** — user-specified extra paths

`Env.path` in a user program lists entries 2–7 (plus CWD), and can be used to build a `TAURARO_PATH` value when spawning a child `tauraroc` process:

```tauraro
from std.sys.env  import Env
from std.sys.os   import OS

def main():
    mut env = Env.init()
    env.path_add("/opt/company-libs")

    # Build TAURARO_PATH from env.path and pass it to a child tauraroc
    mut sep = ":"
    if OS.is_windows(): sep = ";"
    mut path_str = ""
    mut i = 0
    while i < env.path.len():
        if i > 0: path_str = path_str + sep
        path_str = path_str + env.path.get(i)
        i = i + 1

    env.set_var("TAURARO_PATH", path_str)
    OS.run("tauraroc --run child.tr")
```

---

## Full example

```tauraro
from std.sys.env import Env

def main():
    mut env = Env.init()

    # Print all CLI arguments
    print("program: " + env.program_name)
    mut args = env.user_args()
    mut i = 0
    while i < args.len():
        print("arg[" + str(i + 1) + "] = " + args.get(i))
        i = i + 1

    # Read an optional config path from the environment
    mut cfg = env.get_var("APP_CONFIG")
    if cfg == "": cfg = "config.json"
    print("config: " + cfg)

    # Show the search path
    print("search paths:")
    i = 0
    while i < env.path.len():
        print("  " + env.path.get(i))
        i = i + 1
```
