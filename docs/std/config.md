# std.config — Layered Configuration

```tauraro
from std.config import Config, ConfigSpec, ConfigWatcher
```

A `Config` builds one view of an application's settings out of several layers.
Keys are dotted paths (`db.host`). A lookup returns the value from the highest
layer that set the key.

| Layer | Constant | Filled by | Source label (`source_of`) |
|---|---|---|---|
| 0 (lowest) | `LAYER_DEFAULT` | `set_default*` in code | `default` |
| 1 | `LAYER_FILE` | `load_file`, `load_file_optional`, `load_text` | `file app.toml` |
| 2 | `LAYER_ENV` | `load_env(prefix, sep)`, `load_env_map` | `env APP_DB__HOST` |
| 3 | `LAYER_ARGS` | `load_args(Namespace)`, `load_flags(argv)` | `flag --db.port` |
| 4 (highest) | `LAYER_OVERRIDE` | `set*` in code | `override` |

Within a layer, the latest write wins, so a second config file overrides the
first. The order of the `load_*` calls does not change which layer wins.

---

## Quick start

```tauraro
from std.config import Config

def setup(args: List[str]) throws str -> Config:
    mut cfg = Config.init()
    cfg.set_default("db.host", "localhost")
    cfg.set_default_int("db.port", 5432)
    cfg.set_default("http.timeout", "5s")
    cfg.load_file("config/app.toml")?
    mut local: bool = cfg.load_file_optional("config/app.local.yaml")?
    cfg.load_env("APP_", "__")            # APP_DB__PORT=6543  ->  db.port
    mut rest = cfg.load_flags(args)       # --db.port=7000  --debug  --no-color
    cfg.expect("db.password", "str").required().secret()
    cfg.expect("db.port", "int").range(1.0, 65535.0)
    cfg.check()?                          # every problem, one per line
    print(cfg.require_int("db.port")?)
    print(cfg.get_duration_ms("http.timeout", 1000))
    print(cfg.dump())                     # secrets masked, sources shown
    return cfg
```

---

## Loading

### Files

`load_file(path) throws str` picks the format from the file extension. A missing
file or an unknown extension raises an error. `load_file_optional(path) throws str -> bool`
skips a missing file. `load_text(text, format, source) throws str` loads text
directly. `Config.format_of(path)` returns the format name for a path.

| Extension | Parser | Mapping |
|---|---|---|
| `.toml` | `std.encoding.toml` | tables → dotted keys; values keep their TOML type |
| `.yaml` / `.yml` | `std.encoding.yaml` | mappings → dotted keys; values keep their type |
| `.json` | `std.encoding.json` | objects → dotted keys; the root must be an object |
| `.ini` / `.cfg` / `.conf` | `std.encoding.ini` | `[section] key` → `section.key`; values are text |
| `.env` | `std.sys.dotenv` | each name goes through the environment mapping below. The process environment is **not** modified |

An array of scalars becomes a list value. An array that holds tables or arrays
becomes indexed keys (`servers.0.host`, `servers.1.host`). Files are remembered
for `reload()` and `ConfigWatcher`.

### Environment

`load_env(prefix, sep)` reads every process environment variable that starts
with `prefix` (std.sys.env has no way to list variables, so a small C shim,
`std/config_native.c`, enumerates `environ`). The rest of the name is split on
`sep` and each segment is lowercased: `APP_DB__MAX_CONN` → `db.max_conn`. A name
with an empty segment is ignored. `load_env_map(Dict[str, str], prefix, sep)`
reads a map instead of the process environment. `env_mapping(prefix, sep)` sets
the mapping for `.env` files without reading the environment.

### Command line

- `load_args(ns)` takes an `std.cli.argparse` `Namespace`. It reads only the values given
  **on the command line**: argparse's own environment and default values are left to
  the lower layers. An argument named `db.host` (flag `--db.host`) or `db__host` maps to
  `db.host`. A repeated option becomes a list.
- `load_flags(args) -> List[str]` needs no parser. It reads `--key=value`, `--key` (true)
  and `--no-key` (false), and turns `-` into `_` in names (`--log-level=debug` → `log_level`).
  `--` ends the options. It returns the arguments that are not flags.

### Code

`set_default`, `set_default_int`, `set_default_float`, `set_default_bool`,
`set_default_list` write the defaults layer. `set`, `set_int`, `set_float`,
`set_bool`, `set_list` write the overrides layer. To write any layer with your
own label, use `set_layer(key, value, layer, source)`, `set_layer_kind(...)` or
`set_layer_list(...)`. `clear_layer(layer)` removes everything a layer set.

---

## Reading

| Method | Description |
|---|---|
| `get_str(key, fallback)` | Interpolated text |
| `get_int`, `get_float` | Spaces around the number are allowed. Floats accept an exponent |
| `get_bool(key, fallback)` | `true/false`, `yes/no`, `on/off`, `1/0`, in any case |
| `get_list(key) -> List[str]` | A list value. A scalar is split on `,`, with items trimmed and empty items dropped. Indexed keys `key.0`, `key.1` are also collected. Unset gives `[]` |
| `get_duration_ms(key, fallback)` | `250ms`, `5s`, `1.5s`, `2m`, `1h30m`, `1d`, `1w`. A bare number is milliseconds |
| `get_bytes(key, fallback)` | `512`, `10KB` / `MB` / `GB` / `TB` (powers of 1000), `512KiB` / `MiB` / `GiB` / `TiB` and `K` / `M` / `G` / `T` (powers of 1024), in any case |
| `require(key) throws str` and `require_int` / `_float` / `_bool` / `_duration_ms` / `_bytes` / `_list` | Raise errors that name the key, the value and the layer that set it |
| `has(key)` | The key has a value, or is a table with sub-keys |
| `source_of(key)`, `layer_of(key)` | Where the winning value came from (`""` / `-1` when unset) |
| `keys()`, `keys_under(prefix)` | Sorted key lists |
| `raw(key)` | The value without interpolation |
| `lookup(key) throws str`, `expand(text) throws str` | Interpolate a key's value or any text |
| `Config.parse_duration_ms(s)`, `Config.parse_bytes(s)`, `Config.parse_bool(s)` | Standalone parsers that raise on bad input |

The typed `get_*` getters never fail. A missing value, a value that does not
parse, or a broken reference gives the fallback.

Example errors:

```
config: required key 'db.host' is not set (checked defaults; files config/app.toml; environment APP_*; command line; overrides)
config: 'db.port' = 'abc' (set by env APP_DB__PORT) is not an integer
```

### Interpolation

| Syntax | Meaning |
|---|---|
| `${name}` | Config key `name` if it is set, otherwise the environment variable `name` |
| `${env:NAME}` | Always the environment variable |
| `${name:-fallback}` | Fallback when `name` is neither a key nor a variable |
| `$${` | A literal `${` |

Interpolation is lazy: values are resolved on each read. A reference therefore
sees overrides added later, and `raw` / `dump` show the text as written.
Reference cycles raise `config: interpolation cycle a -> b -> a`.

---

## Validation

- `expect(key, kind) -> ConfigSpec` declares a key. The kinds are `"str"`, `"int"`, `"float"`,
  `"bool"`, `"duration"`, `"bytes"`, `"list"` and `"any"`. A key `plugins.*` matches every
  sub-key. Calls chain: `.required()`, `.one_of([...])`, `.range(lo, hi)` and `.secret()`.
  `range` checks the number for numeric kinds, the item count for `list` and the length for `str`.
- `validate() -> List[str]` returns every problem: missing required keys, type, choice and
  range errors, and broken references in any key. `check() throws str` raises all of them
  together, one per line.
- `unknown_keys()` lists keys that are neither declared nor given a default. This catches
  typos such as `APP_DB__HSOT`.
- `to_json()` returns the resolved configuration as nested JSON. Values from typed sources
  (TOML, YAML, JSON, typed setters) keep their type. Text from env, INI or flags becomes a
  number or boolean when it parses as one, unless `expect` declared the key's kind.
- `validate_schema(schema) -> ValidationResult` runs a `std.validate.schema` `Schema` over
  `to_json()`.

### Secrets

`dump()` prints sorted `key = value  # source` lines with the raw values. A
value is masked as `******` (change it with `cfg.mask`) when one of these holds:

- the key was declared with `.secret()`
- the key was passed to `mark_secret(key)`
- the key's last segment contains `password`, `passwd`, `secret`, `token`,
  `apikey`, `api_key`, `private_key` or `credential`

`is_secret(key)` applies the same test.

---

## Reload and watching

`reload() throws str` drops the file and environment layers and reads them
again, in their original order. Defaults, flags and overrides stay.
`ConfigWatcher.init(cfg, debounce_ms)` uses `std.io.watch` to watch the
directories that hold the loaded files, so editors that save by renaming are
also seen. `poll(timeout_ms) throws str -> bool` reloads when one of those
files changed and returns true. `active` is false when nothing could be
watched. Call `destroy()` when done.

---

## Limitations

- Keys are case-sensitive. Environment segments are lowercased.
- `to_json` emits indexed keys (`servers.0.host`) as objects keyed `"0"`, not as arrays.
- The YAML, TOML and INI parsers are lenient: malformed input loads what they can
  read instead of failing. JSON must have an object at the root.
- `load_flags` has no `--key value` form, because it cannot tell flags from options
  without a declaration. Use `--key=value` or `load_args` with argparse.
