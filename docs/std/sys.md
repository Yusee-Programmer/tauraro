# std.sys — System Operations, OS Info, Dates, and Time

```tauraro
from std.sys.env      import Env
from std.sys.fs       import Fs
from std.sys.process  import Process
from std.sys.time     import Clock
from std.sys.os       import OS
from std.sys.platform import Platform
from std.sys.datetime import DateTime, Date, Time, TimeDelta
from std.sys.signal   import Signal
from std.sys.dotenv   import Dotenv
from std.sys.cron     import Cron, CronSchedule
from std.sys.mmap     import MemMap
from std.sys.shm      import Shm
```

---

## std.sys.env — Environment variables and program arguments

**When**: You need to read configuration from environment variables or parse command-line arguments.
**Why**: `Env` bundles argument counting, positional access, and env-var read/write in one object.

### Env class

```tauraro
mut env = Env.init()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `init` | `() -> Env` | `Env` | Snapshot `argc`/`argv` at program start. Pre-populates `Env.path`. |
| `get_var` | `(name: str) -> str` | `str` | Value of env variable `name`. Returns `""` if unset. |
| `has_var` | `(name: str) -> bool` | `bool` | `true` if the variable is defined. |
| `set_var` | `(name: str, value: str) -> bool` | `bool` | Set an environment variable for the current process. `true` on success. |
| `unset_var` | `(name: str) -> bool` | `bool` | Remove an environment variable. `true` on success. |
| `get_arg_count` | `() -> int` | `int` | Number of program arguments (includes argv[0]). |
| `get_arg` | `(i: int) -> str` | `str` | Argument at index `i`. Returns `""` for out-of-range. |
| `all_args` | `() -> Vec[str]` | `Vec[str]` | All arguments as a vector (index 0 = program name). |
| `user_args` | `() -> Vec[str]` | `Vec[str]` | Arguments from index 1 onward (skips program name). |
| `program_name` | `str` field | `str` | `argv[0]` — the program path. |
| `path` | `Vec[str]` field | `Vec[str]` | Module search path (like Python's `sys.path`), pre-populated with cwd, exe dir, `std`, and `packages` directories. |
| `path_insert` | `(dir: str)` | `void` | Prepend `dir` to `Env.path` (checked first). |
| `path_add` | `(dir: str)` | `void` | Append `dir` to `Env.path` (checked last). |
| `path_contains` | `(dir: str) -> bool` | `bool` | `true` if `dir` is already in `Env.path`. |
| `path_remove` | `(dir: str)` | `void` | Remove `dir` from `Env.path` (no-op if absent). |

### Example

```tauraro
from std.sys.env import Env

mut env = Env.init()
print("Running: " + env.program_name)
print("Args: " + str(env.get_arg_count()))

mut home = env.get_var("HOME")
print("HOME = " + home)

env.set_var("MY_FLAG", "1")
print(str(env.has_var("MY_FLAG")))   # true
env.unset_var("MY_FLAG")
print(str(env.has_var("MY_FLAG")))   # false

mut args = env.user_args()           # everything after argv[0]

# Module search path
env.path_insert("./local_modules")
print(str(env.path_contains("./local_modules")))  # true
```

---

## std.sys.fs — File-system operations

**When**: Moving, copying, deleting, or sizing files without reading their contents.
**Why**: `Fs` is a static-method class that complements `std.io.file`; it focuses on file metadata and path-level operations.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fs.delete` | `(path: str) -> bool` | `bool` | Delete a file. `true` on success. |
| `Fs.rename` | `(old_path: str, new_path: str) -> bool` | `bool` | Move/rename a file. `true` on success. |
| `Fs.size` | `(path: str) -> int` | `int` | File size in bytes. Returns `-1` if not found. |
| `Fs.copy` | `(src: str, dst: str) -> bool` | `bool` | Copy `src` to `dst`. `true` on success. |
| `Fs.is_file` | `(path: str) -> bool` | `bool` | `true` if the path exists and is a regular file. |

### Example

```tauraro
from std.sys.fs import Fs

Fs.copy("a.txt", "b.txt")
Fs.rename("b.txt", "c.txt")
mut sz = Fs.size("c.txt")
print("Size: " + str(sz))
Fs.delete("c.txt")
```

---

## std.sys.process — Process control

**When**: You need to run shell commands, capture their output, exit the program, or sleep.
**Why**: `Process` is a static-method class wrapping `popen`/`system`/`exit` for process lifecycle and child execution. Overlapping OS identity helpers (hostname, username) live in `std.sys.os`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Process.exit` | `(code: int)` | `void` | Terminate the current process. `0` = success, non-zero = failure. |
| `Process.get_pid` | `() -> int` | `int` | OS process identifier of the running program. |
| `Process.system` | `(cmd: str) -> int` | `int` | Run `cmd` in the system shell; return the exit code (`0` = success). |
| `Process.shell_output` | `(cmd: str) -> str` | `str` | Run `cmd` and capture its stdout as a string. |
| `Process.check_call` | `(cmd: str) -> bool` | `bool` | Run `cmd`; return `true` if the exit code is `0`. |
| `Process.sleep_ms` | `(ms: int)` | `void` | Sleep for at least `ms` milliseconds. |
| `Process.sleep` | `(s: int)` | `void` | Sleep for at least `s` seconds. |

### Example

```tauraro
from std.sys.process import Process

print("PID: " + str(Process.get_pid()))

mut out = Process.shell_output("echo hello")
print(out)    # "hello\n"

if Process.check_call("ls -la"):
    print("listing succeeded")

Process.sleep_ms(100)
Process.exit(0)
```

---

## std.sys.time — Timing via the Clock class

**When**: You need to sleep, measure elapsed time, or get a Unix timestamp.
**Why**: `Clock.init()` records a start point so `elapsed_ms()` is a one-liner; the static helpers map directly to Windows/POSIX sleep and time functions.

```tauraro
mut c = Clock.init()
# ... work ...
mut ms = c.elapsed_ms()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Clock.init` | `() -> Clock` | `Clock` | Create a `Clock`, recording the current time as its start point. |
| `Clock.sleep_ms` | `(ms: int)` | `void` | Pause the current thread for at least `ms` milliseconds. |
| `Clock.sleep_s` | `(s: int)` | `void` | Pause for at least `s` seconds. |
| `Clock.timestamp` | `() -> int` | `int` | Wall-clock time as seconds since the Unix epoch. |
| `Clock.now_ms` | `() -> int` | `int` | High-resolution wall-clock time in milliseconds (monotonic). |
| `elapsed_ms` | `(self) -> int` | `int` | Milliseconds elapsed since this `Clock` was created. |

Fields: `start_time: int`.

### Example

```tauraro
from std.sys.time import Clock

mut c = Clock.init()
Clock.sleep_ms(50)
mut dt = c.elapsed_ms()   # >= 50
print("elapsed: " + str(dt) + "ms")

mut ts = Clock.timestamp()
print("unix time: " + str(ts))
```

---

## std.sys.os — Operating system information

**When**: You need to know the platform, hostname, CPU count, working directory, or run shell commands.
**Why**: `OS` is a static-method class — all calls are `OS.method()`, no instance needed.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `OS.hostname` | `() -> str` | `str` | Network hostname of this machine. |
| `OS.username` | `() -> str` | `str` | Username running this process. |
| `OS.platform` | `() -> str` | `str` | `"windows"`, `"linux"`, or `"macos"`. |
| `OS.machine` | `() -> str` | `str` | CPU architecture: `"x86_64"`, `"arm64"`, `"arm"`, or `"unknown"`. |
| `OS.is_windows` | `() -> bool` | `bool` | `true` when running on Windows. |
| `OS.cpu_count` | `() -> int` | `int` | Number of logical CPU cores. |
| `OS.memory_mb` | `() -> int` | `int` | Total physical RAM in megabytes. |
| `OS.cwd` | `() -> str` | `str` | Current working directory path. |
| `OS.chdir` | `(path: str) -> bool` | `bool` | Change working directory. `true` on success. |
| `OS.system` | `(cmd: str) -> int` | `int` | Run `cmd` in the system shell; return exit code. |
| `OS.shell_output` | `(cmd: str) -> str` | `str` | Run `cmd` and capture its stdout. |
| `OS.getpid` | `() -> int` | `int` | Current process ID. |
| `OS.sep` | `() -> str` | `str` | Directory separator (`"/"` or `"\\"` on Windows). |
| `OS.linesep` | `() -> str` | `str` | Line ending (`"\r\n"` or `"\n"`). |

### Example

```tauraro
from std.sys.os import OS

print("OS: "    + OS.platform())
print("Host: "  + OS.hostname())
print("User: "  + OS.username())
print("CPUs: "  + str(OS.cpu_count()))
print("RAM MB: "+ str(OS.memory_mb()))
print("CWD: "   + OS.cwd())
print("Sep: "   + OS.sep())

if OS.is_windows():
    print("Running on Windows")
```

---

## std.sys.platform — Runtime target and capability detection

**When**: You need to branch code at runtime based on the target platform (e.g., skip networking on embedded, choose a filesystem path only when available).
**Why**: `Platform` wraps the compile-time macros (`TAURARO_BARE`, `TAURARO_WASM`, etc.) as runtime booleans so your Tauraro code stays portable across Linux, Windows, macOS, Android, iOS, WASM, and bare-metal.

| Method | Returns | Description |
|---|---|---|
| `Platform.name()` | `str` | `"windows"`, `"linux"`, `"macos"`, `"android"`, `"ios"`, `"wasm"`, or `"embedded"` |
| `Platform.arch()` | `str` | `"x86_64"`, `"arm64"`, `"arm"`, `"wasm32"`, or `"unknown"` |
| `Platform.has_filesystem()` | `bool` | `true` when a writable filesystem exists (false on bare WASM / embedded) |
| `Platform.has_networking()` | `bool` | `true` when sockets are available (false on bare / WASM targets) |
| `Platform.has_threads()` | `bool` | `true` when OS threads are available |
| `Platform.has_os_services()` | `bool` | `true` when env vars, process control, etc. are available |
| `Platform.is_windows()` | `bool` | Running on Windows |
| `Platform.is_linux()` | `bool` | Running on Linux |
| `Platform.is_macos()` | `bool` | Running on macOS |
| `Platform.is_android()` | `bool` | Running on Android |
| `Platform.is_ios()` | `bool` | Running on iOS |
| `Platform.is_wasm()` | `bool` | Running in a WASM environment (bare or WASI) |
| `Platform.is_embedded()` | `bool` | Running on a bare-metal target (no OS) |
| `Platform.is_posix()` | `bool` | Running on any POSIX system (Linux, macOS, Android, iOS, WASI) |
| `Platform.is_mobile()` | `bool` | Running on Android or iOS |

### Example

```tauraro
from std.sys.platform import Platform

def main():
    print("Platform: " + Platform.name())
    print("Arch:     " + Platform.arch())

    if Platform.has_filesystem():
        print("Filesystem available")
    else:
        print("No filesystem — skipping file operations")

    if Platform.has_networking():
        print("Networking available")
    else:
        print("No networking on this target")

    if Platform.is_mobile():
        print("Running on a mobile device")
    elif Platform.is_embedded():
        print("Running on embedded hardware")
```

---

## std.sys.datetime — DateTime, Date, Time, TimeDelta

**When**: You need to work with dates, times, or durations — logging timestamps, scheduling, age calculations, calendar displays.
**Why**: Modelled after Python's `datetime` module; backed by the C standard `time.h` so it works on all platforms.

### TimeDelta — duration between two moments

```tauraro
mut td = TimeDelta.from_hours(2)
print(td.to_string())   # "02:00:00"
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `TimeDelta.init` | `(days: int, seconds: int) -> TimeDelta` | `TimeDelta` | Create from explicit days and seconds. Normalised internally. |
| `TimeDelta.from_seconds` | `(s: int) -> TimeDelta` | `TimeDelta` | Create from a total seconds count. |
| `TimeDelta.from_minutes` | `(m: int) -> TimeDelta` | `TimeDelta` | |
| `TimeDelta.from_hours` | `(h: int) -> TimeDelta` | `TimeDelta` | |
| `TimeDelta.from_days` | `(d: int) -> TimeDelta` | `TimeDelta` | |
| `total_seconds` | `() -> int` | `int` | Total duration expressed purely in seconds. |
| `to_string` | `() -> str` | `str` | Format as `"D d HH:MM:SS"` (days part omitted when 0). |

Fields: `days: int`, `seconds: int`.

---

### DateTime — date and time together

```tauraro
mut now = DateTime.now()
print(now.to_string())   # "2026-05-24 14:30:00"
```

#### Construction

| Method | Signature | Returns | Description |
|---|---|---|---|
| `DateTime.now` | `() -> DateTime` | `DateTime` | Current local date and time. |
| `DateTime.init` | `(year: int, month: int, day: int, hour: int, minute: int, second: int) -> DateTime` | `DateTime` | Construct from explicit components. |
| `DateTime.from_timestamp` | `(ts: int) -> DateTime` | `DateTime` | Build from a Unix timestamp (seconds since epoch). |

#### Fields

`year`, `month`, `day`, `hour`, `minute`, `second` — all `int`.

#### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `timestamp` | `() -> int` | `int` | Unix timestamp (seconds since 1970-01-01 UTC). |
| `weekday` | `() -> int` | `int` | 0=Monday … 6=Sunday (Python convention). |
| `isoweekday` | `() -> int` | `int` | 0=Sunday … 6=Saturday (C convention). |
| `yearday` | `() -> int` | `int` | Day of year 1–366. |
| `weekday_name` | `() -> str` | `str` | Full name: `"Monday"` … `"Sunday"`. |
| `weekday_abbr` | `() -> str` | `str` | `"Mon"` … `"Sun"`. |
| `month_name` | `() -> str` | `str` | `"January"` … `"December"`. |
| `is_leap_year` | `() -> bool` | `bool` | `true` when `year` is a leap year. |
| `add` | `(delta: TimeDelta) -> DateTime` | `DateTime` | Return a new `DateTime` offset forward by `delta`. |
| `sub` | `(delta: TimeDelta) -> DateTime` | `DateTime` | Return a new `DateTime` offset backward by `delta`. |
| `diff` | `(other: DateTime) -> TimeDelta` | `TimeDelta` | `self - other` as a `TimeDelta`. |
| `before` | `(other: DateTime) -> bool` | `bool` | `true` if this moment is earlier. |
| `after` | `(other: DateTime) -> bool` | `bool` | `true` if this moment is later. |
| `eq` | `(other: DateTime) -> bool` | `bool` | `true` if both represent the same second. |
| `strftime` | `(fmt: str) -> str` | `str` | Format using `strftime`-style specifiers, e.g. `"%Y-%m-%d"`. |
| `to_string` | `() -> str` | `str` | ISO 8601: `"YYYY-MM-DD HH:MM:SS"`. |
| `date_str` | `() -> str` | `str` | Date-only: `"YYYY-MM-DD"`. |
| `time_str` | `() -> str` | `str` | Time-only: `"HH:MM:SS"`. |

#### Example

```tauraro
from std.sys.datetime import DateTime, TimeDelta

mut now  = DateTime.now()
print(now.to_string())           # "2026-05-24 14:30:00"
print(now.date_str())            # "2026-05-24"
print(now.weekday_name())        # "Sunday"
print(str(now.is_leap_year()))   # false (2026)

mut tomorrow = now.add(TimeDelta.from_days(1))
print(tomorrow.date_str())       # "2026-05-25"

mut past = DateTime.init(2000, 1, 1, 0, 0, 0)
mut age  = now.diff(past)
print(str(age.days) + " days since Y2K")
```

---

### Date — date only

```tauraro
mut today = Date.today()
print(today.to_string())   # "2026-05-24"
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Date.today` | `() -> Date` | `Date` | Current local date. |
| `Date.init` | `(year: int, month: int, day: int) -> Date` | `Date` | Create from explicit components. |
| `to_string` | `() -> str` | `str` | `"YYYY-MM-DD"` |
| `to_datetime` | `() -> DateTime` | `DateTime` | Midnight `DateTime` on this date. |
| `eq` | `(other: Date) -> bool` | `bool` | |
| `before` | `(other: Date) -> bool` | `bool` | |
| `after` | `(other: Date) -> bool` | `bool` | |

Fields: `year: int`, `month: int`, `day: int`.

---

### Time — time of day only

```tauraro
mut now = Time.now()
print(now.to_string())   # "14:30:00"
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Time.now` | `() -> Time` | `Time` | Current local time of day. |
| `Time.init` | `(hour: int, minute: int, second: int) -> Time` | `Time` | Create from explicit components. |
| `to_string` | `() -> str` | `str` | `"HH:MM:SS"` |
| `total_seconds` | `() -> int` | `int` | `hour*3600 + minute*60 + second` |
| `eq` | `(other: Time) -> bool` | `bool` | |
| `before` | `(other: Time) -> bool` | `bool` | |

Fields: `hour: int`, `minute: int`, `second: int`.

---

## std.sys.signal — Graceful shutdown (Ctrl+C / SIGINT, SIGTERM)

**When**: You're writing a long-running server, listener, or worker-pool loop and want it to exit cleanly on Ctrl+C or `SIGTERM` instead of being killed mid-request.
**Why**: `Signal` installs OS signal handlers once at startup that set an internal flag; your accept/work loop polls that flag each iteration and exits on its own terms (closing sockets, flushing logs, etc.).

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Signal.install_shutdown_handler` | `()` | `void` | Register handlers for `SIGINT`/`SIGTERM` that set a flag checked by `shutdown_requested()`. Call once at startup, before entering the accept/work loop. |
| `Signal.shutdown_requested` | `() -> bool` | `bool` | `true` once Ctrl+C (`SIGINT`) or `SIGTERM` has been received. |

### Example

```tauraro
from std.sys.signal import Signal
from std.sys.time   import Clock

Signal.install_shutdown_handler()

print("Server running. Press Ctrl+C to stop.")
while not Signal.shutdown_requested():
    # ... accept connections, process work, etc. ...
    Clock.sleep_ms(100)

print("Shutdown requested — cleaning up and exiting.")
```

---

## std.sys.dotenv — `.env` file loader

**When**: You keep local configuration (API keys, database URLs, feature flags) in a `.env` file instead of hard-coding it or requiring every developer to export shell variables by hand.
**Why**: `Dotenv` reuses `std.sys.env.Env` for the actual environment-variable-setting (no reimplementation) and separates pure parsing (`parse`, testable with no side effects) from the side-effecting `load`.

Supported format: lines of `KEY=value`, `#`-prefixed comments, blank lines skipped, and optionally single- or double-quoted values (`KEY="value with spaces"`) to preserve embedded spaces. No variable interpolation and no multi-line values — an unquoted value may itself contain `=` (only the first `=` on a line splits key from value), e.g. `URL=http://x.com?a=1` parses with value `http://x.com?a=1`.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Dotenv.parse` | `(content: str) -> Dict[str, str]` | `Dict[str, str]` | Pure parse of `.env`-format text into a dict. No environment variables are touched. |
| `Dotenv.load` | `(path: str) -> int` | `int` | Read `path` from disk, parse it, and set every entry as a real process environment variable via `Env.set_var`. Returns the number of variables set, or `-1` if the file could not be read. |

### Example

```tauraro
from std.sys.dotenv import Dotenv
from std.sys.env     import Env

# Pure parsing -- no side effects, easy to test:
mut parsed = Dotenv.parse("NAME=Alice\n# a comment\nURL=http://x.com?a=1\nQUOTED=\"hello world\"\n")
print(parsed["NAME"])     # "Alice"
print(parsed["URL"])      # "http://x.com?a=1"
print(parsed["QUOTED"])   # "hello world"  -- quotes stripped

# Loading a real .env file into the process environment:
mut n = Dotenv.load(".env")
if n >= 0:
    mut env = Env.init()
    print("Loaded " + str(n) + " variables")
    print(env.get_var("DATABASE_URL"))
else:
    print(".env file not found")
```

---

## std.sys.cron — Cron expressions: next / previous fire time

**When**: You need to know *when* a cron schedule fires (next run, last run, "does this minute match?") without running anything.
**Why**: A pure, deterministic calculator on Unix timestamps in UTC — no timers, threads or OS localtime. `std.async.sched.Scheduler.cron` runs jobs; `Cron` just does the calendar math.

Five fields — `minute hour day-of-month month day-of-week` — each a comma list of `*`, `?` (dom/dow), `N`, `N-M`, `*/S`, `N-M/S`, `N/S`. Months accept `jan..dec`, weekdays `sun..sat` (case-insensitive 3-letter names); day-of-week `0` and `7` are both Sunday. Macros: `@yearly`/`@annually`, `@monthly`, `@weekly`, `@daily`/`@midnight`, `@hourly`. (`@reboot` and a seconds field are not supported.)

When **both** day-of-month and day-of-week are restricted, a day matches if *either* does (Vixie/cronie semantics: `0 0 13 * fri` = every 13th and every Friday); if either field starts with `*`/`?`, both must match. (The older `std.async.sched` parser ANDs them.)

| Call | Returns | Description |
|---|---|---|
| `Cron.parse(expr) throws str` | `CronSchedule` | Parse; errors are `"cron: ..."` messages |
| `Cron.is_valid(expr)` | `bool` | Does it parse? |
| `Cron.next(expr, ts) throws str` / `Cron.prev(expr, ts) throws str` | `int` | One-shot parse + search |
| `sched.next_after(ts)` | `int` | First fire time **strictly after** `ts` (Unix seconds, UTC) |
| `sched.prev_before(ts)` | `int` | Last fire time **strictly before** `ts` |
| `sched.next_after_offset(ts, utc_offset)` / `prev_before_offset` / `matches_offset` | `int` / `int` / `bool` | Evaluate in wall-clock time `utc_offset` seconds east of UTC (fixed offset, no DST rules); result is still a UTC timestamp |
| `sched.matches(ts)` | `bool` | Is the minute containing `ts` a fire time? |
| `sched.next_n(ts, n)` | `List[int]` | The next `n` fire times |
| `sched.next_datetime(dt)` / `prev_datetime(dt)` | `DateTime` | UTC `DateTime` forms |

Fire times are whole minutes, so results are multiples of 60. **`-1` means "no fire time"** (an unsatisfiable schedule such as `0 0 31 2 *`; the search covers 12 years).

```tauraro
from std.sys.cron import Cron, CronSchedule
from std.sys.datetime import DateTime, TimeZone

mut s = Cron.parse("*/15 9-17 * * mon-fri")?
mut now = DateTime.in_zone(2026, 10, 11, 6, 7, 30, TimeZone.utc()).timestamp()   # a Sunday
print(DateTime.from_timestamp_utc(s.next_after(now)).to_string())     # 2026-10-12 09:00:00
print(DateTime.from_timestamp_utc(s.prev_before(now)).to_string())    # 2026-10-09 17:45:00
```

---

## std.sys.mmap — Memory-mapped files and anonymous shared memory

**When**: You want to treat a file as a byte array (large read-mostly data, binary formats, an index you patch in place), or you need zero-filled memory that a forked child can share.
**Why**: `MemMap` wraps `mmap`/`msync`/`munmap` (POSIX) and `CreateFileMapping`/`MapViewOfFile`/`FlushViewOfFile` (Windows). The mapping is **owned**: it is unmapped (and its file handle closed) when the `MemMap` goes out of scope, by the user-defined `free()`; `close()` does it early and is idempotent.

Constructors always return a `MemMap`; check `is_open()`. On failure `error` holds the OS message and `code` the `errno` / `GetLastError()`.

| Constructor | Description |
|---|---|
| `MemMap.open_read(path)` | read-only shared mapping of the whole file |
| `MemMap.open_readwrite(path)` | read-write; stores reach the file |
| `MemMap.open_private(path)` | copy-on-write: stores stay in this mapping, never written back |
| `MemMap.open_range(path, mode, offset, length)` | map a window (`mode` 0 read, 1 read-write, 2 private; `length <= 0` = to EOF). `offset` need not be page-aligned. Read-write with `offset+length` past EOF **grows the file** |
| `MemMap.create(path, size)` | create/truncate a file of `size` bytes and map it read-write |
| `MemMap.anonymous(size)` | zero-filled memory not backed by a file (`MAP_SHARED|MAP_ANONYMOUS`, so a `fork()`ed child shares it) |
| `MemMap.supported()` / `MemMap.page_size()` | capability probe / mapping granularity |

| Instance method | Description |
|---|---|
| `len()`, `is_open()`, `is_writable()`, `in_range(off, n)` | state and bounds |
| `get_u8/i8/u16/i16/u32/i32/i64(off)`, `get_f64(off)` | little-endian fixed-width reads (`i64` values ≥ 2^63 read back negative) |
| `set_u8/u16/u32/i64(off, v)`, `set_f64(off, v)` | writes; `false` if read-only or out of range |
| `read_str(off, n)` / `write_str(off, s)` | binary-safe byte strings (`s.len()` is the real length, NULs included) |
| `fill(off, n, byte)` | memset |
| `atomic_load/atomic_store/atomic_add/atomic_cas(off, ...)` | 64-bit atomics on 8-byte-aligned offsets (`atomic_add` returns the **previous** value; `atomic_cas` returns `bool`) — valid across processes on shared maps |
| `flush()` / `flush_async()` / `flush_range(off, n, wait)` | `msync` / `FlushViewOfFile` (+ `FlushFileBuffers` for `flush()`); a no-op `true` for anonymous memory |
| `close()` | unmap now |

Out-of-range reads return `0` / `""` and out-of-range or read-only writes return `false`: nothing outside the mapping is ever touched.

```tauraro
from std.sys.mmap import MemMap

mut m = MemMap.create("table.bin", 4096)
if m.is_open():
    m.set_u32(0, 0xCAFE)
    m.write_str(16, "hello")
    m.flush()
    print(m.read_str(16, 5))     # hello
# unmapped here
```

**Limitations**: a file truncated by another process while mapped faults on access (inherent to `mmap`); empty files cannot be mapped; bare-metal, kernel and WASI builds have no virtual memory — `supported()` is `false` and every constructor fails with `not supported on this platform`. The Windows backend is compile-checked with MinGW (zig) but is not exercised by a Windows run in the module author's environment — the Windows CI leg runs `tests/lang/140_mmap.tr`.

---

## std.sys.shm — Named shared memory and a cross-process lock

**When**: Two or more processes (or a process and its children) must share a block of memory and coordinate access.
**Why**: `Shm` wraps `shm_open` + `mmap` (POSIX) or a named pagefile-backed file mapping (`Local\tauraro_shm_<name>`, Windows). Each region is `size` user bytes behind a hidden 64-byte header (magic, size, lock word), so `len()` is the user size everywhere and `Shm.open(name)` needs no size.

| Call | Description |
|---|---|
| `Shm.create(name, size)` | new zeroed region; **fails if the name exists** |
| `Shm.open(name)` | attach to an existing region |
| `Shm.create_or_open(name, size)` | create, or attach (the size must match an existing region) |
| `Shm.unlink(name)` | POSIX: remove the name (live maps keep working). Windows: no-op `true` (the object dies with its last handle) |
| `Shm.supported()`, `Shm.valid_name(name)` | probes (names: 1–30 chars of `[A-Za-z0-9._-]`; 30 because macOS caps shm names at 31 with the `/`) |
| `get_*/set_*/read_str/write_str/fill/atomic_*` | same typed, bounds-checked access as `MemMap`, offsets start at 0 of the user area |
| `lock(timeout_ms)` | `0` acquired, `1` acquired after **recovering a dead owner's lock**, `-1` timeout (`< 0` waits forever, `0` tries once), `-2` not open |
| `try_lock()`, `unlock()`, `holds_lock()` | non-blocking acquire; release (`false` if the calling thread is not the owner) |
| `close()` | unmap this view; releases the lock first if this object still holds it. Also runs at scope exit |

The lock is a coarse mutex in the header: the owner id is `(pid << 32 | thread id)`, so it works between processes **and** threads, polls (spin then 1 ms sleeps) rather than using a futex, and is re-entrant for the same thread. If the owning *process* died, the next `lock()` takes it over and returns `1` so you know the protected data may be half-written. A dead *thread* in a live process, and PID reuse, cannot be detected.

```tauraro
from std.sys.shm import Shm

mut a = Shm.create("jobs", 4096)
a.set_i64(0, 0)
# ... in any process:
mut b = Shm.open("jobs")
if b.lock(1000) >= 0:
    b.set_i64(0, b.get_i64(0) + 1)
    b.unlock()
Shm.unlink("jobs")
```

Android (bionic has no `shm_open`), bare-metal and WASI report `not supported on this platform`. Tested by `tests/lang/141_shm.tr`, including `fork()`-based contention, stale-owner recovery and a locked 3-process counter.

