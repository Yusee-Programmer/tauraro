# std.sys — System Operations, OS Info, Dates, and Time

```tauraro
from std.sys.env      import Env
from std.sys.fs       import delete_file, copy_file, file_size, rename_file, is_file
from std.sys.process  import Process
from std.sys.time     import Stopwatch, timestamp, sleep_ms
from std.sys.os       import OS
from std.sys.platform import Platform
from std.sys.datetime import DateTime, Date, Time, TimeDelta
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
| `init` | `() -> Env` | `Env` | Snapshot `argc`/`argv` at program start. |
| `get_var` | `(name: str) -> str` | `str` | Value of env variable `name`. Returns `""` if unset. |
| `get_var_or` | `(name: str, default: str) -> str` | `str` | Return `default` if the variable is unset. |
| `has_var` | `(name: str) -> bool` | `bool` | `true` if the variable is defined. |
| `set_var` | `(name: str, value: str)` | `void` | Set an environment variable for the current process. |
| `unset_var` | `(name: str)` | `void` | Remove an environment variable. |
| `get_arg_count` | `() -> int` | `int` | Number of program arguments (includes argv[0]). |
| `get_arg` | `(i: int) -> str` | `str` | Argument at index `i`. Returns `""` for out-of-range. |
| `get_arg_or` | `(i: int, default: str) -> str` | `str` | Return `default` for out-of-range. |
| `all_args` | `() -> Vec[str]` | `Vec[str]` | All arguments as a vector (index 0 = program name). |
| `user_args` | `() -> Vec[str]` | `Vec[str]` | Arguments from index 1 onward (skips program name). |
| `program_name` | `str` field | `str` | `argv[0]` — the program path. |

### Example

```tauraro
from std.sys.env import Env

mut env = Env.init()
print("Running: " + env.program_name)
print("Args: " + str(env.get_arg_count()))

mut home = env.get_var_or("HOME", "/tmp")
print("HOME = " + home)

env.set_var("MY_FLAG", "1")
print(str(env.has_var("MY_FLAG")))   # true
env.unset_var("MY_FLAG")
print(str(env.has_var("MY_FLAG")))   # false

mut args = env.user_args()           # everything after argv[0]
```

---

## std.sys.fs — File-system operations

**When**: Moving, copying, deleting, or sizing files without reading their contents.
**Why**: These complement `std.io.file`; they focus on file metadata and path-level operations.

| Function | Signature | Returns | Description |
|---|---|---|---|
| `delete_file` | `(path: str)` | `void` | Delete a file. No-op if absent. |
| `rename_file` | `(src: str, dst: str) -> bool` | `bool` | Move/rename a file. `true` on success. |
| `file_size` | `(path: str) -> int` | `int` | File size in bytes. Returns `0` if not found. |
| `copy_file` | `(src: str, dst: str) -> bool` | `bool` | Copy `src` to `dst`. `true` on success. |
| `is_file` | `(path: str) -> bool` | `bool` | `true` if the path exists and is a regular file. |

### Example

```tauraro
from std.sys.fs import copy_file, rename_file, file_size, delete_file

copy_file("a.txt", "b.txt")
rename_file("b.txt", "c.txt")
mut sz = file_size("c.txt")
print("Size: " + str(sz))
delete_file("c.txt")
```

---

## std.sys.process — Process control

**When**: You need to run shell commands, capture their output, or exit the program.
**Why**: Wraps `popen`/`system` with structured return types; `Process` class bundles exit code + stdout.

### Process class

```tauraro
mut p = Process.run("echo hello")
if p.succeeded():
    print(p.get_output())
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Process.run` | `(cmd: str) -> Process` | `Process` | Execute `cmd`, capture stdout and exit code. |
| `succeeded` | `() -> bool` | `bool` | `true` if exit code was 0. |
| `get_output` | `() -> str` | `str` | Captured stdout of the command. |
| `exit_code` | `int` field | `int` | Raw exit code. |

### Free functions

| Function | Signature | Returns | Description |
|---|---|---|---|
| `get_pid` | `() -> int` | `int` | Current process ID. |
| `exit` | `(code: int)` | `void` | Terminate the process with `code`. |
| `run` | `(cmd: str) -> int` | `int` | Execute a shell command; return its exit code. |
| `run_capture` | `(cmd: str) -> str` | `str` | Execute and capture stdout as a string. |
| `run_ok` | `(cmd: str) -> bool` | `bool` | `true` if exit code is 0. |

### Example

```tauraro
from std.sys.process import Process, get_pid, run_capture

print("PID: " + str(get_pid()))
mut out = run_capture("echo hello")
print(out)    # "hello\n"

mut p = Process.run("ls -la")
if p.succeeded():
    print(p.get_output())
```

---

## std.sys.time — Timing and sleep

**When**: You need to sleep, measure elapsed time, or get a Unix timestamp.
**Why**: `Stopwatch` is easier than calling `time_ms()` twice; `sleep_ms` maps directly to Windows/POSIX sleep.

### Free functions

| Function | Signature | Returns | Description |
|---|---|---|---|
| `sleep_ms` | `(ms: int)` | `void` | Sleep for `ms` milliseconds. |
| `sleep_s` | `(s: int)` | `void` | Sleep for `s` seconds. |
| `timestamp` | `() -> int` | `int` | Unix timestamp in seconds. |
| `time_ms` | `() -> int` | `int` | Milliseconds since an unspecified epoch (monotonic). |
| `time_ns` | `() -> int` | `int` | Nanoseconds since an unspecified epoch (monotonic). |
| `elapsed_ms` | `(start: int) -> int` | `int` | `time_ms() - start` — handy one-liner for measuring a duration. |
| `format_duration_ms` | `(ms: int) -> str` | `str` | Human-readable string like `"1h 3m 2s 400ms"`. |

### Stopwatch class

```tauraro
mut sw = Stopwatch.start()
# ... work ...
mut ms = sw.elapsed()
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `start` | `() -> Stopwatch` | `Stopwatch` | Create and immediately start. |
| `elapsed` | `() -> int` | `int` | Milliseconds since start (or last `reset`). |
| `stop` | `()` | `void` | Pause the stopwatch. |
| `reset` | `()` | `void` | Restart from zero. |
| `is_running` | `() -> bool` | `bool` | |

### Example

```tauraro
from std.sys.time import Stopwatch, elapsed_ms, time_ms, timestamp

mut t0 = time_ms()
sleep_ms(50)
mut dt = elapsed_ms(t0)   # >= 50
print("elapsed: " + str(dt) + "ms")

mut ts = timestamp()
print("unix time: " + str(ts))

mut sw = Stopwatch.start()
# ... work ...
print("duration: " + str(sw.elapsed()) + "ms")
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
