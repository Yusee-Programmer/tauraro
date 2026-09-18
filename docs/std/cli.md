# std.cli — Command-Line Argument Parsing

```tauraro
from std.cli import Cli
from std.sys.env import Env
```

A clap-style CLI parser: flags, typed options, positionals, and
subcommands, with auto-generated `--help`/`-h` and `--version`.

```tauraro
def main():
    mut app = Cli.init("mytool", "does a thing")
    app.flag("verbose", "v", "enable verbose output")
    app.option("output", "o", "output path", "a.out")
    app.positional("input", "input file", true)

    mut parsed = app.parse(Env.init().user_args())
    if parsed.help_requested:
        app.print_help()
        return
    if not parsed.ok:
        print("error: " + parsed.error)
        return

    if parsed.has_flag("verbose"): print("verbose on")
    print(parsed.get_option("output"))
    print(parsed.get_positional("input"))
```

---

## `Cli` — building the parser

| Method | Signature | Description |
|---|---|---|
| `Cli.init` | `(name: str, about: str) -> Cli` | Create a new CLI app/subcommand. |
| `.version` | `(self, v: str)` | Set the version string reported by `--version`. |
| `.flag` | `(self, name: str, short: str, help: str)` | Add a boolean flag (`--name`/`-short`). |
| `.option` | `(self, name: str, short: str, help: str, default_: str)` | Add a string-valued option with a default. |
| `.option_required` | `(self, name: str, short: str, help: str)` | Add a required string-valued option (no default). |
| `.option_typed` | `(self, name: str, short: str, help: str, default_: str, kind: int)` | Add a typed option (`CLI_STR`/`CLI_INT`/`CLI_FLOAT`/`CLI_BOOL`); rejects a value that doesn't parse as that type. |
| `.option_typed_required` | `(self, name: str, short: str, help: str, kind: int)` | Required + typed. |
| `.positional` | `(self, name: str, help: str, required: bool)` | Add a positional argument, in declaration order. |
| `.subcommand` | `(self, name: str, about: str) -> Cli` | Add a subcommand and return its own `Cli` builder. |
| `.parse` | `(self, args: Vec[str]) -> CliArgs` | Parse an argument list (pass `Env.init().user_args()` for real `argv`, or a synthetic `Vec[str]` in tests). |
| `.help_text` | `(self) -> str` | Render the help text as a string. |
| `.print_help` | `(self)` | Print help text to stdout. |
| `.print_version` | `(self)` | Print the version string set via `.version`. |

Type kinds for `option_typed`/`option_typed_required`: `CLI_STR()`,
`CLI_INT()`, `CLI_FLOAT()`, `CLI_BOOL()` (each an `int`-returning
constant function, imported from `std.cli`).

---

## `CliArgs` — the parse result

`.parse(...)` always returns a `CliArgs`; check `.ok` before reading
values. On failure, `.error` names the bad flag/option/positional
(`--bogus` → error mentions `"bogus"`), and unknown-flag errors include a
"did you mean" suggestion when a close match exists.

| Field / Method | Signature | Description |
|---|---|---|
| `.ok` | `bool` | `true` if parsing succeeded. |
| `.error` | `str` | Error message when `.ok` is `false`. |
| `.help_requested` | `bool` | `true` if `--help`/`-h` was passed — check this before `.ok`. |
| `.has_flag` | `(self, name: str) -> bool` | Whether a boolean flag was set. |
| `.has_option` | `(self, name: str) -> bool` | Whether an option was explicitly provided (vs. using its default). |
| `.get_option` | `(self, name: str) -> str` | The option's string value (default if not provided). |
| `.get_option_int` | `(self, name: str) -> int` | Parsed as `int` (for `CLI_INT` options). |
| `.get_option_float` | `(self, name: str) -> float` | Parsed as `float` (for `CLI_FLOAT` options). |
| `.get_option_bool` | `(self, name: str) -> bool` | Parsed as `bool` (for `CLI_BOOL` options). |
| `.get_positional` | `(self, name: str) -> str` | A positional argument's value by name. |
| `.has_positional` | `(self, name: str) -> bool` | Whether a positional was provided. |
| `.has_subcommand` | `(self) -> bool` | Whether a subcommand was invoked. |
| `.subcommand` | `(self) -> CliArgs` | The invoked subcommand's own parsed `CliArgs`. |

```tauraro
mut app = Cli.init("mytool", "does a thing")
mut sub = app.subcommand("build", "build the project")
sub.flag("release", "r", "optimized build")

mut parsed = app.parse(args)
if parsed.has_subcommand():
    mut b = parsed.subcommand()
    if b.has_flag("release"): print("release build")
```

Short flags/options accept a single-letter alias (`-v` maps to the same
result as `--verbose`); an unrecognized `--flag`/`-f` fails with `.ok =
false` rather than being silently ignored.
