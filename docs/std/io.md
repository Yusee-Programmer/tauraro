# std.io — File I/O, Buffered I/O, Directories, Paths, Console, Async Polling

```tauraro
from std.io.file       import File
from std.io.bufio      import BufReader, BufWriter
from std.io.dir        import Dir
from std.io.path       import Path
from std.io.console    import Console
from std.io.poll       import IOPoll, IOEvent
from std.io.event_loop import EventLoop
from std.sys.fs        import Fs
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
**Why**: Cross-platform wrappers over `mkdir`/`rmdir`/`opendir` with `bool` return values for error checking, via the `Dir` class.

```tauraro
from std.io.dir import Dir

mut d = Dir.init("tmp")
d.make()
mut entries = d.list()
```

### Dir class

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(path: str) -> Dir` | `Dir` | Create a `Dir` bound to `path`. |
| `make` | `(self) -> bool` | `bool` | Create the directory. `true` on success or if it already exists. |
| `remove` | `(self) -> bool` | `bool` | Remove the directory (must be **empty**). `true` on success. |
| `exists` | `(self) -> bool` | `bool` | `true` if `self.path` is an existing directory. |
| `list` | `(self) -> Vec[str]` | `Vec[str]` | Entry names (not full paths), excluding `"."` and `".."`. |
| `count` | `(self) -> int` | `int` | Number of entries (excluding `"."` and `".."`). |

### Example

```tauraro
from std.io.dir  import Dir
from std.io.file import File
from std.io.path import Path

mut d = Dir.init("tmp")
d.make()
File.write_text(Path.init("tmp").join("a.txt"), "hello")
mut entries = d.list()           # ["a.txt"]
print(str(entries.len()))        # 1
d.remove()
```

---

## std.io.path — Path manipulation

**When**: You need to join, split, inspect, or normalise file paths in a cross-platform way.
**Why**: Avoids manual string slicing; handles Windows `\` vs POSIX `/` via the `Path` class.

`Path` wraps a string in `self.value`; `/` and `\` are both treated as separators by `dirname`/`basename`/`join`.

```tauraro
from std.io.path import Path

mut p = Path.init("a/b/c.txt")
print(p.basename())   # "c.txt"
```

### Path class

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `(p: str) -> Path` | `Path` | Create a `Path` wrapping `p`. |
| `normalize` | `(self) -> str` | `str` | Replace every `\` with `/`. |
| `join` | `(self, other: str) -> str` | `str` | Join `self.value` with `other`, inserting exactly one `/` between them (handles existing trailing/leading separators). |
| `dirname` | `(self) -> str` | `str` | Directory portion including trailing separator, e.g. `"a/b/c.txt"` → `"a/b/"`. Returns `"./"` if there is no separator. |
| `basename` | `(self) -> str` | `str` | Final component (file name + extension), e.g. `"a/b/c.txt"` → `"c.txt"`. |
| `extension` | `(self) -> str` | `str` | Last extension including dot, e.g. `".gz"`. Returns `""` if there is no extension. |
| `strip_extension` | `(self) -> str` | `str` | Remove the last extension, e.g. `"a.b.c"` → `"a.b"`. |
| `is_absolute` | `(self) -> bool` | `bool` | `true` for paths starting with `/` or a Windows drive letter (`X:`). |
| `to_str` | `(self) -> str` | `str` | Return the underlying string (`self.value`). |

### Example

```tauraro
from std.io.path import Path

mut full = Path.init("src").join("main.tr")          # "src/main.tr"
mut ext  = Path.init("archive.tar.gz").extension()    # ".gz"
mut dir  = Path.init("a/b/c.txt").dirname()           # "a/b/"
mut base = Path.init("a/b/c.txt").basename()          # "c.txt"
```

---

## std.io.console — Console I/O

**When**: You need to print to stdout/stderr, prompt the user for input, or use ANSI colors.
**Why**: Provides newline-safe helpers, typed input readers, and color/clear-screen helpers via the static-method `Console` class.

### Output

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Console.println` | `(s: str)` | `void` | Print `s` followed by a newline to stdout. |
| `Console.print` | `(s: str)` | `void` | Print `s` with no trailing newline. |
| `Console.eprint` | `(s: str)` | `void` | Print `s` to stderr (no newline). |
| `Console.eprintln` | `(s: str)` | `void` | Print `s` to stderr (no newline; same as `eprint`). |

### Input

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Console.read_line` | `(prompt: str) -> str` | `str` | Print `prompt`, then read one line from stdin (newline stripped). |
| `Console.input_line` | `() -> str` | `str` | Read one line from stdin with no prompt. |
| `Console.read_int` | `(prompt: str) -> int` | `int` | Print `prompt`, read a line, and parse it as an integer (returns `0` on parse failure). |
| `Console.read_float` | `(prompt: str) -> float` | `float` | Print `prompt`, read a line, and parse it as a float. |

### Screen control

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Console.clear` | `()` | `void` | Clear the terminal screen. |

### Colors (ANSI codes)

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Console.set_color` | `(code: int)` | `void` | Set foreground color to `code` (e.g. `Console.RED()`). |
| `Console.reset_color` | `()` | `void` | Reset colors to default. |
| `Console.print_colored` | `(s: str, code: int)` | `void` | Print `s` in color `code`, then reset. |
| `Console.println_colored` | `(s: str, code: int)` | `void` | Print `s` + newline in color `code`, then reset. |

Color constants (each a zero-arg static method returning the ANSI code): `Console.BLACK()`, `Console.RED()`, `Console.GREEN()`, `Console.YELLOW()`, `Console.BLUE()`, `Console.MAGENTA()`, `Console.CYAN()`, `Console.WHITE()`, and bright variants `Console.BRIGHT_RED()`, `Console.BRIGHT_GREEN()`, `Console.BRIGHT_YELLOW()`, `Console.BRIGHT_BLUE()`, `Console.BRIGHT_MAGENTA()`, `Console.BRIGHT_CYAN()`, `Console.BRIGHT_WHITE()`.

### Example

```tauraro
from std.io.console import Console

Console.println("Enter your name:")
mut name = Console.input_line()
Console.println("Hello, " + name + "!")
Console.eprintln("stderr message")

Console.print_colored("Warning!", Console.YELLOW())
Console.println("")
```

> **Tip** — The built-in `print(s)` maps directly to `printf` and is slightly faster for simple debug output. Use `Console.println` when you need a guaranteed newline without appending `"\n"` manually.

---

## std.io.poll — IOPoll: async I/O readiness

**When**: You are building an I/O-bound server or client that needs to wait on many sockets/file descriptors without blocking on each one individually.
**Why**: Wraps the `_TrIOPoll` C runtime abstraction (epoll on Linux, IOCP on Windows, kqueue on macOS/BSD, no-op stub under `TAURARO_KERNEL`) behind a single cross-platform interface.

```tauraro
from std.io.poll import IOPoll, IOEvent

mut poll = IOPoll.new()
poll.add(fd, IOPoll.POLLIN(), my_ctx_int)
mut events = poll.wait(100)   # 100 ms timeout
for ev in events:
    print("fd ready: events=" + str(ev.events) + " ctx=" + str(ev.userdata))
poll.destroy()
```

### Event flags

Combine with `|`:

| Constant | Value | Meaning |
|---|---|---|
| `IOPoll.POLLIN()`  | `0x01` | readable |
| `IOPoll.POLLOUT()` | `0x02` | writable |
| `IOPoll.POLLERR()` | `0x04` | error condition |
| `IOPoll.POLLHUP()` | `0x08` | connection closed / hangup |

### IOPoll class

| Method | Signature | Returns | Description |
|---|---|---|---|
| `new` | `() -> IOPoll` | `IOPoll` | Create a new poll instance (allocates the underlying OS handle). |
| `add` | `(self, fd: int, events: int, userdata: int) -> bool` | `bool` | Register `fd` for readiness events. `userdata` is an opaque int (index/id) returned in matching `IOEvent`s. |
| `modify` | `(self, fd: int, events: int, userdata: int) -> bool` | `bool` | Update the registered event mask/userdata for `fd`. |
| `remove` | `(self, fd: int) -> bool` | `bool` | Remove `fd` from the poll set. |
| `wait` | `(self, timeout_ms: int) -> Vec[IOEvent]` | `Vec[IOEvent]` | Wait up to `timeout_ms` for events (`-1` = block indefinitely). Returns ready descriptors (may be empty on timeout). |
| `destroy` | `(self)` | `void` | Release the underlying OS handle. |

### IOEvent class

| Field/Method | Signature | Returns | Description |
|---|---|---|---|
| `fd` | `int` | `int` | The file descriptor / handle that is ready. |
| `events` | `int` | `int` | Bitmask of ready events (see flags above). |
| `userdata` | `int` | `int` | The opaque value passed to `add`/`modify`. |
| `readable` | `(self) -> bool` | `bool` | `true` if `POLLIN` is set. |
| `writable` | `(self) -> bool` | `bool` | `true` if `POLLOUT` is set. |
| `error` | `(self) -> bool` | `bool` | `true` if `POLLERR` is set. |
| `hangup` | `(self) -> bool` | `bool` | `true` if `POLLHUP` is set. |

---

## std.io.event_loop — EventLoop: single-threaded async reactor

**When**: You're writing an I/O-bound server/client that needs to handle many connections on one thread without blocking ("Node.js model" — not M:N coroutines). For CPU-bound work, combine `EventLoop` on the I/O thread with a `ThreadPool` for workers.
**Why**: A small wrapper around `IOPoll` that tracks a running flag, iteration count, and elapsed time.

```tauraro
from std.io.event_loop import EventLoop
from std.io.poll import IOPoll

mut loop = EventLoop.new()
loop.add_fd(fd, IOPoll.POLLIN(), my_handler_id)
loop.run(100, 50)   # run 100 poll cycles, 50ms timeout each
loop.stop()
```

Full server pattern (manual loop using `poll_once`):

```tauraro
mut server = TcpListener.listen("0.0.0.0", 8080)
mut loop   = EventLoop.new()
loop.add_fd(server.fd, IOPoll.POLLIN(), 0)   # 0 = server sentinel

while loop.is_running():
    mut evs = loop.poll_once(50)             # wait up to 50ms
    for ev in evs:
        if ev.userdata == 0:                  # new connection
            mut client = server.accept_nb()
            loop.add_fd(client.fd, IOPoll.POLLIN(), client.fd)
        else:
            handle_client(ev.fd)              # read/write without blocking
```

### EventLoop class

| Method | Signature | Returns | Description |
|---|---|---|---|
| `new` | `() -> EventLoop` | `EventLoop` | Create a new event loop (creates its own `IOPoll`, sets `running = true`). |
| `add_fd` | `(self, fd: int, events: int, userdata: int) -> bool` | `bool` | Register `fd` for readiness notifications. `userdata` is caller-managed (e.g. connection id). |
| `modify_fd` | `(self, fd: int, events: int, userdata: int) -> bool` | `bool` | Update the event mask for a registered `fd`. |
| `remove_fd` | `(self, fd: int) -> bool` | `bool` | Remove `fd` from the loop. |
| `poll_once` | `(self, timeout_ms: int) -> Vec[IOEvent]` | `Vec[IOEvent]` | Run one poll cycle and return ready events. `-1` blocks forever, `0` is non-blocking, `>0` is a millisecond timeout. |
| `run` | `(self, max_iter: int, timeout_ms: int) -> int` | `int` | Run poll cycles until `stop()` is called or `max_iter` cycles complete (`0` = unlimited). Returns the total number of events seen across all cycles. |
| `stop` | `(self)` | `void` | Signal the loop to stop after the current cycle. |
| `is_running` | `(self) -> bool` | `bool` | `true` while the loop has not been stopped. |
| `elapsed_ms` | `(self) -> int` | `int` | Milliseconds elapsed since `EventLoop.new()`. |
| `destroy` | `(self)` | `void` | Stop the loop and destroy its underlying `IOPoll`. |

> **Note** — `run()` only counts and tallies events; it does not itself dispatch them to handlers. For real servers, drive your own loop with `poll_once()` as shown above, or process the `Vec[IOEvent]` returned by `run`'s underlying `poll_once` calls yourself.

---

## std.sys.fs — File-system operations

**When**: You need to delete, rename/move, copy, or check files outside the `File`/`Dir` instance APIs.
**Why**: Small static-method `Fs` class for common filesystem operations.

```tauraro
from std.sys.fs import Fs

Fs.copy("a.txt", "b.txt")
Fs.rename("b.txt", "c.txt")
Fs.delete("c.txt")
```

### Fs class — static methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fs.delete` | `(path: str) -> bool` | `bool` | Delete a file. `true` on success. |
| `Fs.rename` | `(old_path: str, new_path: str) -> bool` | `bool` | Rename (or move) a file. `true` on success. |
| `Fs.size` | `(path: str) -> int` | `int` | File size in bytes, or `-1` if the file does not exist. |
| `Fs.copy` | `(src: str, dst: str) -> bool` | `bool` | Copy `src` to `dst` by reading and rewriting the full content. `false` if `src` does not exist. |
| `Fs.is_file` | `(path: str) -> bool` | `bool` | `true` if `path` names a regular file that can be opened for reading. |
