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

---

## `std.cli.argparse` — declarative parsing

```tauraro
from std.cli.argparse import ArgParser, Arg

def main():
    mut p = ArgParser.init("tool", "Process some files.")
    p.version("1.4.0")
    p.add(Arg.count("verbose", "v", "More output (repeat for more)"))
    p.add(Arg.option("output", "o", "Write results to FILE").value_name("FILE").default_value("out.txt").from_env("TOOL_OUT"))
    p.add(Arg.option("jobs", "j", "Parallel jobs").of_int().default_value("4"))
    p.add(Arg.option("format", "f", "Output format").one_of(["json", "yaml", "text"]))
    p.add(Arg.option("tag", "t", "Add a tag").repeatable())
    p.add(Arg.flag("color", "", "Colored output").allow_negation())
    p.add(Arg.positional("inputs", "Files to process").count_range(1, -1))
    mut build = p.command("build", "Build the project").also("b")
    build.add(Arg.flag("release", "r", "Optimised build"))
    mut ns = p.parse_process_or_exit()
    if ns.command == "build" and ns.sub.flag("release"): print("release build")
    print(ns.get("output"), ns.get_int("jobs"), ns.list("inputs"))
```

**Arg builders**: `Arg.flag(name, short, help)`, `Arg.count(...)` (`-vvv`), `Arg.option(...)`, `Arg.positional(name, help)`;
chain `.value_name(m)`, `.default_value(v)`, `.mandatory()`, `.optional()`, `.repeatable()`, `.count_range(min, max)`
(positional arity, -1 = unlimited), `.of_int()` / `.of_float()` / `.of_bool()`, `.one_of(list)`, `.from_env(var)`,
`.allow_negation()` (`--no-x`), `.hide()`, `.in_group(title)`, `.also(long_alias)`. Long names are the name with
`_` turned into `-`.

**ArgParser**: `add(arg)`, `command(name, about)` (returns the sub-parser; `.also(alias)` adds names),
`require_command()`, `mutually_exclusive(names)`, `require_one_of(names)`, `version(v)`, `set_epilog(t)`,
`set_width(w)`, `response_files(true)` (`@file` expands to the file's shell-quoted words, `#` comments allowed),
`parse(list)?`, `parse_vec(vec)?`, `parse_or_exit(list)`, `parse_process_or_exit()`, `help_text()`, `usage()`,
`completion_script("bash" | "zsh" | "fish")?`.

**Namespace**: `get(name)` (last value), `get_int/get_float/get_bool`, `flag(name)`, `count(name)`, `list(name)`,
`has(name)` (any source), `given(name)` (command line), `source(name)`, `command`, `sub`, `command_path()`,
`exit_now`/`exit_text` (set by `--help`, `-h`, `--version`, `-V`, `help <command>`).

Syntax: `--opt value`, `--opt=value`, `-o value`, `-ovalue`, clustered `-vvn`, `--` ends options, negative numbers
are values, unknown options get "did you mean" suggestions, errors include the usage line. Precedence: command
line, environment variable, default.
