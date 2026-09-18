# std.log — Leveled, Structured Logging

```tauraro
from std.log import Logger, LogLevel, Fields
```

Leveled, structured logging with text or JSON-lines output, to stdout
and/or a file, with per-logger minimum-level filtering.

```tauraro
mut log = Logger.init("http")            # target/subsystem tag
log.set_min_level(LogLevel.Info)         # below this, calls are no-ops
log.add_file_sink("app.log")             # also append to a file
log.set_format_json(true)                # JSON-lines instead of text

log.info_f("request handled", Fields.new().str("method", "GET").int("status", 200))
log.debug("suppressed at Info level")    # cheap no-op: level is checked first
```

---

## `LogLevel`

```tauraro
pub enum LogLevel:
    Trace
    Debug
    Info
    Warn
    Error
```

| Method | Signature | Description |
|---|---|---|
| `.rank` | `(self) -> int` | Numeric severity rank (higher = more severe); used for filtering. |
| `.name` | `(self) -> str` | Upper-case name (`"INFO"`, `"WARN"`, ...). |
| `.name_lower` | `(self) -> str` | Lower-case name (`"info"`, `"warn"`, ...). |
| `.at_least` | `(self, min: LogLevel) -> bool` | Whether this level meets or exceeds `min`. |
| `level_from_str` | `(s: str) -> LogLevel` | Parse a level from its name (case-insensitive), free function. |

---

## `Logger`

| Method | Signature | Description |
|---|---|---|
| `Logger.init` | `(target: str) -> Logger` | Create a logger tagged with a subsystem/target name. |
| `.set_min_level` | `(self, level: LogLevel)` | Calls below this level are cheap no-ops (checked before formatting). |
| `.get_min_level` | `(self) -> LogLevel` | Current minimum level. |
| `.set_format_json` | `(self, on: bool)` | JSON-lines output instead of the default text format. |
| `.set_stdout` | `(self, on: bool)` | Enable/disable the stdout sink (on by default). |
| `.add_file_sink` | `(self, path: str) -> bool` | Also append every log line to a file; returns `false` if the file couldn't be opened. |
| `.remove_file_sink` | `(self)` | Stop writing to the file sink. |
| `.enabled` | `(self, level: LogLevel) -> bool` | Whether a call at `level` would actually be emitted. |
| `.close` | `(self)` | Flush and close all sinks. |

### Plain-message logging

| Method | Signature |
|---|---|
| `.trace` / `.debug` / `.info` / `.warn` / `.error` | `(self, msg: str)` |

### Structured logging (with `Fields`)

| Method | Signature |
|---|---|
| `.trace_f` / `.debug_f` / `.info_f` / `.warn_f` / `.error_f` | `(self, msg: str, fields: Fields)` |

---

## `Fields` — structured key/value pairs

```tauraro
mut f = Fields.new().str("method", "GET").int("status", 200).float("latency_ms", 4.2).bool("cache_hit", true)
log.info_f("request handled", f)
```

| Method | Signature | Description |
|---|---|---|
| `Fields.new` | `() -> Fields` | Start a chain-buildable field set. |
| `Fields.empty` | `() -> Fields` | An empty field set (equivalent to `.new()`). |
| `.str` | `(self, key: str, val: str) -> Fields` | Add a string field; returns `self` for chaining. |
| `.int` | `(self, key: str, val: int) -> Fields` | Add an int field. |
| `.float` | `(self, key: str, val: float) -> Fields` | Add a float field. |
| `.bool` | `(self, key: str, val: bool) -> Fields` | Add a bool field. |
| `.len` | `(self) -> int` | Number of fields added. |

In text format, fields render as `key=value` pairs appended to the
message; in JSON format (`set_format_json(true)`), each log line is a
single JSON object with `level`/`target`/`msg`/timestamp plus one entry
per field.

---

## Sinks

`StdoutSink` and `FileSink` (both implementing `LogSink`) are managed
internally by `Logger` via `.set_stdout`/`.add_file_sink`/
`.remove_file_sink` — construct them directly only if you're building a
custom sink pipeline outside of `Logger`.
