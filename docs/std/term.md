# std.term — Terminal UI: Styling, Cursor Control, Size, Progress Widgets

```tauraro
from std.term.style import Term, Color, Style
from std.term.cursor import Cursor
from std.term.size import Size
from std.term.progress import ProgressBar, Spinner
```

ANSI SGR text styling, cursor movement/visibility, terminal size querying
(with a safe fallback), and a progress bar + spinner widget — everything a
CLI tool needs to render nice output, with no external dependencies. Pairs
naturally with [`std.cli`](cli.md): parse flags with `Cli`, then render
results with `std.term`.

```tauraro
from std.cli import Cli
from std.term.style import Term, Color
from std.term.progress import ProgressBar

def main():
    mut app = Cli.init("build", "builds the project")
    app.flag("quiet", "q", "suppress progress output")
    mut parsed = app.parse(Env.init().user_args())

    if not parsed.has_flag("quiet"):
        mut bar = ProgressBar.init(100)
        mut i = 0
        while i <= 100:
            bar.update(i)
            i = i + 10
        bar.finish()

    print(Term.color("build succeeded", Color.Green))
```

---

## `std.term.style` — ANSI SGR text styling

The 16 standard ANSI colors (8 base + 8 bright), text attributes
(bold/dim/italic/underline/reverse), a `reset`, and 256-color/truecolor as a
stretch goal. Color numbering matches
[`std.io.console`](io.md)'s existing `Console.RED`/`Console.BRIGHT_RED`/etc.
constants (30-37 standard, 90-97 bright) — the two modules never disagree
about what "red" means, they just build the escape differently:
`std.io.console.Console.set_color(...)` applies a code directly (via a
runtime helper that also degrades gracefully on legacy Windows consoles
without VT100), while `std.term.style` builds the raw `"\x1b[<code>m ...
\x1b[0m"` escape *string* itself, which is what composing several attributes
into one span (color + bold + underline together) needs.

### `Color`

```tauraro
pub enum Color:
    Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
    BrightBlack, BrightRed, BrightGreen, BrightYellow
    BrightBlue, BrightMagenta, BrightCyan, BrightWhite
    Default
```

| Method | Signature | Description |
|---|---|---|
| `.fg_code` | `(self) -> int` | SGR foreground code (30-37 / 90-97 / 39 for `Default`). |
| `.bg_code` | `(self) -> int` | SGR background code (40-47 / 100-107 / 49 for `Default`). |

### `Term` — one-shot styling helpers

| Method | Signature | Description |
|---|---|---|
| `Term.RESET` | `() -> str` | The plain reset sequence, `"\x1b[0m"`. |
| `.color` | `(text: str, c: Color) -> str` | Wrap `text` in a foreground color escape + reset. |
| `.bg_color` | `(text: str, c: Color) -> str` | Wrap `text` in a background color escape + reset. |
| `.bold` / `.dim` / `.italic` / `.underline` / `.reverse` | `(text: str) -> str` | Wrap `text` in the named SGR attribute + reset. |
| `.style` | `(text: str, c: Color, bold_: bool, dim_: bool, underline_: bool, reverse_: bool) -> str` | Combine a color with any subset of attributes in **one** escape sequence. |
| `.color256` | `(text: str, n: int) -> str` | 256-color foreground (`38;5;<n>`), `n` in `0..255`. |
| `.bg_color256` | `(text: str, n: int) -> str` | 256-color background (`48;5;<n>`). |
| `.rgb` | `(text: str, r: int, g: int, b: int) -> str` | Truecolor (24-bit) foreground (`38;2;r;g;b`). |
| `.bg_rgb` | `(text: str, r: int, g: int, b: int) -> str` | Truecolor (24-bit) background (`48;2;r;g;b`). |

```tauraro
print(Term.color("error", Color.Red))
print(Term.style("warning", Color.Yellow, true, false, false, false))   # bold yellow
print(Term.rgb("custom orange", 255, 128, 0))
```

### `Style` — a reusable attribute builder

For styling the same combination of attributes repeatedly without rebuilding
the escape sequence by hand each time:

| Method | Signature | Description |
|---|---|---|
| `Style.init` | `() -> Style` | A style with no attributes set (default foreground/background). |
| `.fg` | `(self, c: Color) -> Style` | Set the foreground color (chainable). |
| `.bg` | `(self, c: Color) -> Style` | Set the background color (chainable). |
| `.bold` / `.dim` / `.italic` / `.underline` / `.reverse` | `(self) -> Style` | Enable the attribute (chainable). |
| `.prefix` | `(self) -> str` | Just the escape prefix (`"\x1b[<codes>m"`) for the accumulated attributes. |
| `.apply` | `(self, text: str) -> str` | `prefix() + text + Term.RESET()`. |

```tauraro
mut s = Style.init().fg(Color.Green).bold().underline()
print(s.apply("all good"))
print(s.apply("also good"))   # reuse without rebuilding
```

---

## `std.term.cursor` — Cursor movement, visibility, clearing

Standard ANSI CSI (Control Sequence Introducer) sequences. Every operation
has two forms: `build_*` returns the raw escape sequence as a `str` (pure,
no I/O — what the test suite asserts against), and the bare name writes it
directly to stdout.

| Method (bare = writes to stdout) | `build_*` equivalent | Description |
|---|---|---|
| `.up` / `.down` / `.right` / `.left` | `build_up(n)` etc. | `(n: int)` — move the cursor `n` cells. |
| `.move_to` | `build_move_to(row, col)` | `(row: int, col: int)` — move to an absolute (1-based) position. |
| `.move_to_col` | `build_move_to_col(col)` | `(col: int)` — move to a column on the current row. |
| `.hide` / `.show` | `build_hide()` / `build_show()` | Hide/show the cursor. |
| `.save` / `.restore` | `build_save()` / `build_restore()` | Save/restore cursor position. |
| `.clear_line` | `build_clear_line()` | Clear the entire current line. |
| `.clear_line_after` / `.clear_line_before` | `build_clear_line_after()` / `build_clear_line_before()` | Clear from cursor to end/start of line. |
| `.clear_screen` | `build_clear_screen()` | Clear the entire screen (cursor position unchanged). |
| `.clear_screen_after` / `.clear_screen_before` | `build_clear_screen_after()` / `build_clear_screen_before()` | Clear from cursor to end/start of screen. |
| `.carriage_return` | `build_carriage_return()` | A bare `"\r"` — move to column 1 without clearing. |

```tauraro
Cursor.hide()
Cursor.move_to(1, 1)
Cursor.clear_screen()
# ... draw a frame ...
Cursor.show()
```

`std.term.progress` builds on `Cursor.carriage_return()` + `Cursor.clear_line()`
for its redraw-in-place behavior.

---

## `std.term.size` — Terminal width/height

| Method | Signature | Description |
|---|---|---|
| `Size.get` | `() -> TermSize` | Query the terminal size. |
| `Size.width` / `Size.height` | `() -> int` | Convenience accessors equivalent to `Size.get().width` / `.height`. |
| `Size.is_tty` | `() -> bool` | Whether stdout looks like a real interactive terminal (vs. redirected to a file/pipe). |
| `DEFAULT_WIDTH` / `DEFAULT_HEIGHT` | `() -> int` | The fallback values (`80` / `24`). |

`TermSize` fields: `.width: int`, `.height: int`, `.is_tty: bool`.

```tauraro
mut sz = Size.get()
print(f"{sz.width}x{sz.height}")
if not sz.is_tty:
    print("(not a real terminal — output may be redirected)")
```

TTY detection reuses the same cross-platform check the C runtime uses for
its own ANSI color auto-detection (POSIX: `isatty(stdout)`; Windows: a real
console check via `GetConsoleMode`, which also best-effort enables VT100
processing so plain ANSI escapes render correctly on Windows 10+ consoles —
no separate setup call needed). When `is_tty()` is true, the actual
column/row count comes from the `COLUMNS`/`LINES` environment variables (set
by most terminal emulators, shells, and CI runners); when it's false, or
those variables aren't set, `Size.get()` returns the documented 80x24
fallback rather than a stale or garbage value.

> **Known gap:** a true `ioctl(TIOCGWINSZ)` / `GetConsoleScreenBufferInfo`
> pixel-exact size query is not implemented — see `bug2.txt` for the
> underlying compiler/FFI limitation (Tauraro has no conditional-compilation
> directive, and re-declaring a Win32 API that `<windows.h>` already
> prototypes conflicts at compile time). The `COLUMNS`/`LINES` fallback
> covers the common case (most terminals and all CI runners set them); a
> resized terminal whose shell doesn't re-export them won't be reflected
> until that's addressed at the runtime level.

---

## `std.term.progress` — Progress bar and spinner

Renders a `[####------] 42%` bar (or an indeterminate spinner) via
`\r` + `Cursor.clear_line()` redraw — no external dependencies.

### `render_bar` — pure rendering (used by tests)

```tauraro
pub def render_bar(current: int, total: int, bar_width: int) -> str
```

Renders just the bar's fixed-width core text, e.g. `render_bar(33, 100, 20)`
→ `"[######--------------] 33%"`. Clamps `current` into `[0, total]` and
guards `total <= 0` (renders `0%` instead of dividing by zero), so it's safe
to call with any input.

### `ProgressBar`

| Method | Signature | Description |
|---|---|---|
| `ProgressBar.init` | `(total: int) -> ProgressBar` | A bar for counting up to `total`. |
| `.with_width` | `(self, w: int) -> ProgressBar` | Set the bar's character width (default 30, chainable). |
| `.with_label` | `(self, label: str) -> ProgressBar` | Prefix the bar with a label (chainable). |
| `.update` | `(self, current: int)` | Redraw the bar in place for `current`. |
| `.inc` | `(self, delta: int)` | Advance by `delta` (shorthand for `update(current + delta)`). |
| `.finish` | `(self)` | Snap to 100% and move to a fresh line. |

Fields: `.total: int`, `.current: int`, `.bar_width: int`, `.label: str`, `.finished: bool`.

```tauraro
mut bar = ProgressBar.init(100).with_width(40).with_label("downloading:")
mut i = 0
while i <= 100:
    bar.update(i)
    i = i + 5
bar.finish()
```

**What this looks like animated** (a markdown doc can't show motion, so here
is the sequence of redraws): each `.update(i)` call emits `\r` + a clear-line
escape + the label and bar text, so in a real terminal the SAME line is
overwritten in place — `downloading: [##------------------------------------] 5%`
grows into `downloading: [########--------------------------------------] 20%`
and so on up to `downloading: [########################################] 100%`,
all on one line with no scrolling, then `.finish()` leaves the cursor on a
fresh line below it.

### `Spinner`

| Method | Signature | Description |
|---|---|---|
| `Spinner.init` | `() -> Spinner` | A new spinner, starting on frame `\|`. |
| `.with_label` | `(self, label: str) -> Spinner` | Trailing label text (chainable). |
| `.current_frame` | `(self) -> str` | The frame the next `.tick()` will draw (pure — usable in tests). |
| `.tick` | `(self)` | Advance to the next frame and redraw it in place. |
| `.stop` | `(self)` | Clear the spinner line and move to a fresh line. |

Frames cycle `| / - \` in order, wrapping back to `|`. Fields: `.frame_index: int`, `.label: str`, `.stopped: bool`.

```tauraro
mut sp = Spinner.init().with_label("connecting...")
while not connected:
    sp.tick()
sp.stop()
```

**What this looks like animated:** each `.tick()` overwrites the same line
in place (same `\r` + clear-line technique as `ProgressBar`), cycling
`| connecting...` → `/ connecting...` → `- connecting...` → `\ connecting...`
→ back to `| connecting...`, giving the classic spinning-cursor effect until
`.stop()` clears the line.

---

See `examples/term_demo.tr` for a runnable demo combining all four modules
(colored/styled text, terminal size, an animating progress bar, and a
ticking spinner), and `tests/lang/45_term.tr` for exact byte-level
assertions on every escape sequence this package produces.
