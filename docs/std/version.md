# std.version — Semantic Versioning and Version Requirements

```tauraro
from std.version import Version, VersionReq
```

`Version` is a [SemVer 2.0.0](https://semver.org) version: parsing, validation,
precedence, bumps. `VersionReq` is an npm/cargo-style requirement (range) such as
`^1.2.3`, `>=1.0, <2.0 || ~3.1` or `1.2 - 1.4`.

---

## Quick start

```tauraro
from std.version import Version, VersionReq

def pick() throws str -> str:
    mut v = Version.parse("1.2.3-rc.1+build.5")?
    print(v.major)                       # 1
    print(v.pre_str())                   # rc.1
    print(v.bump_minor().to_str())       # 1.3.0
    mut req = VersionReq.parse("^1.2")?
    print(req.matches(v))                # false: pre-releases need an opt-in, see below
    return req.max_satisfying(["1.1.0", "1.4.2", "1.10.0", "2.0.0"])   # "1.10.0"
```

---

## Class: Version

| Field | Type | Description |
|---|---|---|
| `major`, `minor`, `patch` | `int` | Numeric parts |
| `pre` | `List[str]` | Pre-release identifiers (`[]` for a release) |
| `build` | `List[str]` | Build-metadata identifiers |

### Parsing

| Function | Description |
|---|---|
| `Version.parse(s) throws str` | Exact SemVer 2.0.0 grammar: `MAJOR.MINOR.PATCH`, no leading zeros, optional `-pre.release` and `+build` with `[0-9A-Za-z-]` identifiers, numeric pre-release identifiers without leading zeros. Parts are limited to 18 digits. |
| `Version.parse_loose(s) throws str` | Also accepts surrounding spaces, a leading `v`/`V`/`=`, missing minor/patch (`"1.2"` gives 1.2.0) and leading zeros. |
| `Version.is_valid(s)` | `parse` succeeded |
| `Version.init(s)` | Lenient and never fails (unparsable text gives 0.0.0). Same as taupkg's private `semver.Version.init`. |
| `Version.new(major, minor, patch)` | Build a release version |

Errors name the part and the input: `version: minor version '02' has a leading zero in '1.02.3'`.

### Formatting and comparison

| Method | Description |
|---|---|
| `to_str()` / `to_string()` | `1.2.3-rc.1+build.5` |
| `to_str_no_build()` | `1.2.3-rc.1` |
| `pre_str()`, `build_str()` | Identifiers joined with `.` |
| `is_prerelease()` | Has pre-release identifiers |
| `compare(other)` | -1 / 0 / 1 by SemVer precedence. Build metadata is ignored |
| `eq` `ne` `lt` `lte` `gt` `gte` | Comparisons built on `compare` |
| `compare_build(other)` | Precedence first, then build metadata as a tie-break (a total order, like npm's `compareBuild`) |
| `identical(other)` | Equal, including build metadata |
| `Version.sort(List[Version])` | Stable ascending sort (uses `compare_build`) |
| `Version.sort_strs(List[str])` | Parses loosely, drops unparsable entries, sorts, returns canonical strings |

Precedence follows semver.org §11. Major, minor and patch compare as numbers. A
pre-release is lower than its release. Identifiers compare left to right: numeric
ones as numbers (any length), alphanumeric ones in ASCII order, numeric below
alphanumeric, and the shorter list is lower when the rest is equal:
`1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0`.

### Bumps

The rules are npm's `semver.inc`. Build metadata is dropped.

| Method | Examples |
|---|---|
| `bump_major()` | 1.2.3 → 2.0.0; 2.0.0-rc.1 → 2.0.0 |
| `bump_minor()` | 1.2.3 → 1.3.0; 1.3.0-rc.1 → 1.3.0 |
| `bump_patch()` | 1.2.3 → 1.2.4; 1.2.4-rc.1 → 1.2.4 |
| `bump_prerelease(tag)` | 1.2.3 (`"rc"`) → 1.2.4-rc.0; 1.2.4-rc.0 → 1.2.4-rc.1; 1.2.4-beta.3 (`"rc"`) → 1.2.4-rc.0; `""` for numeric-only (1.2.3 → 1.2.4-0) |
| `with_pre(s) throws str`, `with_build(s) throws str` | Copy with new (validated) identifiers; `""` clears them |
| `clone()` | Deep copy |

---

## Class: VersionReq

| Function / method | Description |
|---|---|
| `VersionReq.parse(s) throws str` | npm dialect (a bare version is exact) |
| `VersionReq.parse_cargo(s) throws str` | Cargo dialect: a bare `1.2.3` means `^1.2.3` |
| `VersionReq.parse_opts(s, include_prerelease) throws str` | npm `includePrerelease` |
| `VersionReq.is_valid(s)`, `VersionReq.any()` | |
| `matches(v)`, `matches_str(s)` | `matches_str` parses loosely; text that does not parse never matches |
| `max_satisfying(List[str])`, `min_satisfying(List[str])` | Matching version string, or `""` if none matches. Entries that do not parse are skipped |
| `filter(List[str])` | All matching entries, in their original order |
| `to_str()` | Normalized comparator form, the same as node-semver's `Range.range`: `^1.2.3 \|\| 2.x` gives `>=1.2.3 <2.0.0-0\|\|>=2.0.0 <3.0.0-0` |
| `comps`, `nsets` | The desugared `Comparator`s (`op`, `ver`, `grp` = OR-set index) |

### Operators and their dialect

| Syntax | Meaning | Dialect |
|---|---|---|
| `1.2.3`, `=1.2.3`, `v1.2.3` | exactly 1.2.3 (build metadata ignored) | npm. `parse_cargo` reads a bare version as caret |
| `>` `>=` `<` `<=` | comparators; `>1.2` = `>=1.3.0`, `<=1.2` = `<1.3.0-0`, `<1` = `<1.0.0-0` | npm and cargo |
| `^1.2.3` | `>=1.2.3 <2.0.0-0`; `^0.2.3` → `<0.3.0-0`; `^0.0.3` → `<0.0.4-0`; `^1.2` → `<2.0.0-0`; `^0.0` → `<0.1.0-0` | npm and cargo |
| `~1.2.3`, `~>1.2.3` | `>=1.2.3 <1.3.0-0`; `~1` → `>=1.0.0 <2.0.0-0` | npm (cargo has the same `~`) |
| `1.2.x`, `1.2.*`, `1.2`, `1` | X-ranges / partial versions | npm (cargo writes `1.2.*`) |
| `*`, `x`, `X`, empty | any release | npm |
| `A - B` | `>=A <=B`; a partial upper bound excludes the next minor or major: `1.2 - 1.4` = `>=1.2.0 <1.5.0-0` | npm |
| spaces or `,` | AND | spaces are npm's form, commas are cargo's; both work |
| `\|\|` | OR | npm |

Spaces between an operator and its version are allowed (`>= 1.0.0`, `^ 1`).
Desugaring and normalization follow node-semver 7: comparators that
repeat are dropped, `>=0.0.0` becomes `*`, a set holding `<0.0.0-0` matches
nothing, and a range with a lone `*` set becomes `*`.

### Pre-release versions

npm and cargo share this rule. A version with a pre-release tag matches a
comparator set only when one of that set's comparators has a pre-release on
the **same** `major.minor.patch`. So `>=1.2.3-beta` accepts `1.2.3-rc.1` but not
`1.3.0-alpha`, and `^1.2.3` does not accept `1.2.4-rc.1`. With
`parse_opts(s, true)`, pre-releases match like any other version, and desugared
lower bounds become `X.Y.Z-0`, as with npm's `includePrerelease`.

---

## taupkg compatibility

`std.version` covers everything in taupkg's private `src/semver.tr`:
`Version.init(s)`, `to_string()`, `compare`/`eq`/`lt`/`gt`/`lte`/`gte`,
`satisfies(constraint)` and `Version.best_match(Vec[str], constraint)` behave the
same way for the forms taupkg supports. It behaves differently in these cases:

- `^0.x.y` follows npm and cargo: `^0.2.3` is `<0.3.0`. taupkg keeps only the major, so it is `<1.0.0`.
- A pre-release suffix is parsed and compared. taupkg's `int("3-rc")` drops it.
- Pre-release versions only match under the rule above. taupkg ignored pre-release tags.
- A constraint can combine comparators (`>=1.0 <2.0`, `,`, `||`, hyphen ranges, x-ranges). taupkg accepts only one operator.
- `satisfies` returns false when a constraint does not parse. taupkg read an unknown prefix as an exact version.

---

## Tests

`tests/lang/131_version.tr` holds 501 checks:

- the semver.org valid and invalid lists
- the §11 precedence chain
- the npm bump rules
- about 300 range vectors taken from node-semver's `range-parse`, `range-include` and `range-exclude` fixtures (the non-`loose` ones), including the `includePrerelease` cases
