# std.io — File I/O, Buffered I/O, Directories, Paths, Console

```tauraro
from std.io.file    import File
from std.io.bufio   import BufReader, BufWriter
from std.io.dir     import make_dir, remove_dir, dir_exists, list_dir
from std.io.path    import path_join, path_basename, path_dirname, path_extension
from std.io.console import print_line, eprint, read_line
```

---

## std.io.file — File I/O

**When**: You need to read or write a file, check whether it exists, or get its size.
**Why**: Wraps raw C `fopen`/`fread`/`fwrite` with a clean Tauraro class API; handles open, read, write, seek, and close in one place.

### File class — instance API

Open a file explicitly, operate on it multiple times, then close it.

```tauraro
mut f = File.init("data.txt", "rb")
mut content = f.read()
f.close()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(path: str, mode: str) -> File` | `File` | Open `path` in mode (`"rb"` read, `"wb"` write, `"ab"` append). |
| `is_open` | `() -> bool` | `bool` | `true` when the underlying file handle is valid. |
| `read` | `() -> str` | `str` | Read the entire file; returns `""` if not open. |
| `readlines` | `() -> Vec[str]` | `Vec[str]` | Read all lines split on `"\n"`. |
| `write` | `(data: str)` | `void` | Write `data` through the open handle (overwrites from current position). |
| `writeln` | `(line: str)` | `void` | Write `line` followed by a newline. |
| `append` | `(data: str)` | `void` | Write `data` at the current file position. |
| `seek` | `(offset: int, whence: int)` | `void` | Move file pointer. `whence`: `0`=start, `1`=current, `2`=end. |
| `tell` | `() -> int` | `int` | Current byte position. Returns `-1` if not open. |
| `size` | `() -> int` | `int` | File size in bytes (seeks to end internally, then restores position). |
| `exists` | `() -> bool` | `bool` | `true` if `self.path` can be opened for reading. |
| `close` | `()` | `void` | Close the file handle. |

### File class — static helpers

One-shot operations that open, act, and close automatically.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `File.read_text` | `(path: str) -> str` | `str` | Read entire file and return its contents. |
| `File.lines` | `(path: str) -> Vec[str]` | `Vec[str]` | Read all lines of a file directly. |
| `File.write_text` | `(path: str, data: str) -> bool` | `bool` | Write (overwrite) a file. Returns `true` on success. |
| `File.append_text` | `(path: str, data: str) -> bool` | `bool` | Append to a file. Returns `true` on success. |
| `File.file_exists` | `(path: str) -> bool` | `bool` | `true` if the path names a readable file. |
| `File.file_size` | `(path: str) -> int` | `int` | File size in bytes. Returns `-1` if not found. |

### Example

```tauraro
from std.io.file import File

# One-shot write and read
File.write_text("out.txt", "hello\nworld\n")
mut text  = File.read_text("out.txt")      # "hello\nworld\n"
mut lines = File.lines("out.txt")          # ["hello", "world", ""]
print(str(lines.len()))                    # 3

# Instance API with seek
mut f = File.init("out.txt", "rb")
mut sz = f.size()                          # file size in bytes
f.seek(0, 0)                               # rewind to start
mut first = f.read()
f.close()

# Static helpers
print(str(File.file_exists("out.txt")))    # true
print(str(File.file_size("out.txt")))      # byte count
```

---

## std.io.bufio — Buffered File I/O

**When**: You are processing files line-by-line or writing many small chunks; raw `File.read_text` would load the whole file into RAM.
**Why**: `BufReader` reads ahead into an internal buffer (reducing syscalls); `BufWriter` batches writes and flushes when the buffer is full.

### BufReader

Buffered sequential reader — ideal for large text files read line-by-line.

```tauraro
from std.io.bufio import BufReader

mut r = BufReader.open("big.log", 8192)
while True:
    mut line = r.readline()
    if line == "": break
    print(line)
r.close()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `open` | `(path: str, buf_size: int) -> BufReader` | `BufReader` | Open `path` for reading; `buf_size` is the internal buffer capacity in bytes (e.g. `4096`). |
| `read_all` | `() -> str` | `str` | Read the entire remaining file and return as a string. |
| `readlines` | `() -> Vec[str]` | `Vec[str]` | Read all lines split on `"\n"`. |
| `readline` | `() -> str` | `str` | Read one line without the trailing newline. Returns `""` at EOF. Handles `\r\n` and `\n`. |
| `close` | `()` | `void` | Close the file handle. |

Fields: `open: bool` — `true` when the file was opened successfully.

### BufWriter

Buffered sequential writer — ideal for writing many small strings efficiently.

```tauraro
from std.io.bufio import BufWriter

mut w = BufWriter.open("output.txt", 4096)
w.writeln("line one")
w.writeln("line two")
w.close()   # flushes remaining buffer
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `open` | `(path: str, buf_size: int) -> BufWriter` | `BufWriter` | Open `path` for writing (create/overwrite). |
| `open_append` | `(path: str, buf_size: int) -> BufWriter` | `BufWriter` | Open `path` for appending. |
| `write` | `(s: str)` | `void` | Write `s` to the buffer; auto-flushes when buffer is full. |
| `writeln` | `(s: str)` | `void` | Write `s` followed by `"\n"`. |
| `flush` | `()` | `void` | Write the buffer contents to the file immediately. |
| `close` | `()` | `void` | Flush remaining data and close the file. |

Fields: `open: bool` — `true` when the file was opened successfully.

### Example

```tauraro
from std.io.bufio import BufReader, BufWriter

# Write 3 lines buffered
mut w = BufWriter.open("notes.txt", 1024)
w.writeln("alpha")
w.writeln("beta")
w.writeln("gamma")
w.close()

# Read them back line by line
mut r = BufReader.open("notes.txt", 1024)
mut line1 = r.readline()   # "alpha"
mut line2 = r.readline()   # "beta"
mut line3 = r.readline()   # "gamma"
mut eof   = r.readline()   # ""  (end of file)
r.close()
print(line1 + " " + line2 + " " + line3)   # "alpha beta gamma"
```

---

## std.io.dir — Directory operations

**When**: You need to create, delete, or list directories.
**Why**: Cross-platform wrappers over `mkdir`/`rmdir`/`opendir` with `bool` return values for error checking.

### Functions

| Function | Signature | Returns | Description |
|---|---|---|---|
| `make_dir` | `(path: str) -> bool` | `bool` | Create a single directory. `true` on success. |
| `make_dir_all` | `(path: str) -> bool` | `bool` | Create directory and all missing parents. |
| `remove_dir` | `(path: str) -> bool` | `bool` | Remove an **empty** directory. |
| `remove_dir_all` | `(path: str) -> bool` | `bool` | Recursively remove a directory and all its contents. |
| `dir_exists` | `(path: str) -> bool` | `bool` | `true` if the path is an existing directory. |
| `list_dir` | `(path: str) -> Vec[str]` | `Vec[str]` | Entry names (not full paths) in `path`. |
| `dir_count` | `(path: str) -> int` | `int` | Number of entries in `path`. |
| `walk` | `(path: str) -> Vec[str]` | `Vec[str]` | Recursively collect all file paths under `path`. |

### Example

```tauraro
from std.io.dir  import make_dir, list_dir, remove_dir_all
from std.io.file import File
from std.io.path import path_join

make_dir("tmp")
File.write_text(path_join("tmp", "a.txt"), "hello")
mut entries = list_dir("tmp")   # ["a.txt"]
print(str(entries.len()))       # 1
remove_dir_all("tmp")
```

---

## std.io.path — Path manipulation

**When**: You need to join, split, inspect, or normalise file paths in a cross-platform way.
**Why**: Avoids manual string slicing; handles Windows `\` vs POSIX `/` transparently.

All functions treat `/` as the canonical separator and normalise `\` to `/`.

| Function | Signature | Returns | Description |
|---|---|---|---|
| `path_normalize` | `(p: str) -> str` | `str` | Replace `\` with `/`. |
| `path_join` | `(a: str, b: str) -> str` | `str` | Join two path segments with `/`. |
| `path_dirname` | `(p: str) -> str` | `str` | Directory portion including trailing `/`. |
| `path_basename` | `(p: str) -> str` | `str` | Final component (file name + extension). |
| `path_extension` | `(p: str) -> str` | `str` | Last extension including dot, e.g. `".gz"`. |
| `path_strip_extension` | `(p: str) -> str` | `str` | Remove the last extension. |
| `path_with_extension` | `(p: str, ext: str) -> str` | `str` | Replace the extension. |
| `path_is_absolute` | `(p: str) -> bool` | `bool` | `true` for paths starting with `/` or a drive letter. |
| `path_canonicalize` | `(p: str) -> str` | `str` | Resolve `.` and `..` components. |
| `path_split` | `(p: str) -> Vec[str]` | `Vec[str]` | Split the path into its components. |

### Example

```tauraro
from std.io.path import path_join, path_extension, path_dirname, path_basename

mut full = path_join("src", "main.tr")         # "src/main.tr"
mut ext  = path_extension("archive.tar.gz")   # ".gz"
mut dir  = path_dirname("a/b/c.txt")           # "a/b/"
mut base = path_basename("a/b/c.txt")          # "c.txt"
```

---

## std.io.console — Console I/O

**When**: You need to print to stdout/stderr or prompt the user for input.
**Why**: Provides newline-safe helpers and a read-with-prompt function cleaner than raw `print`.

| Function | Signature | Returns | Description |
|---|---|---|---|
| `print_line` | `(s: str)` | `void` | Print `s` followed by a newline to stdout. |
| `print_raw` | `(s: str)` | `void` | Print `s` with no trailing newline. |
| `eprint` | `(s: str)` | `void` | Print `s` to stderr (no newline). |
| `eprintln` | `(s: str)` | `void` | Print `s` + newline to stderr. |
| `read_line` | `(prompt: str) -> str` | `str` | Print `prompt`, then read one line from stdin (newline stripped). |
| `input_line` | `() -> str` | `str` | Read one line from stdin with no prompt. |

### Example

```tauraro
from std.io.console import print_line, read_line, eprintln

print_line("Enter your name:")
mut name = input_line()
print_line("Hello, " + name + "!")
eprintln("stderr message")
```

> **Tip** — The built-in `print(s)` maps directly to `printf` and is slightly faster for simple debug output. Use `print_line` when you need a guaranteed newline without appending `"\n"` manually.
