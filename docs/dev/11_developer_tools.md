# Developer Tools: `tauraroc fmt`, `tauraroc doc`, `tauraroc cov`

Three tools ship inside the compiler binary. They share one lossless lexeme layer
(`src/srclex.tr`), which keeps what the real lexer throws away: raw literal text,
whitespace and comments.

| Tool | Source | One-line summary |
|------|--------|------------------|
| `tauraroc fmt` | `src/tokfmt.tr` | idempotent, comment-preserving, self-verifying formatter |
| `tauraroc doc` | `src/docgen.tr` | API reference (Markdown + static HTML) from doc comments |
| `tauraroc cov` | `src/covrep.tr` + `--coverage` | line and function coverage (console, lcov) |

---

## 1. `tauraroc fmt`

```
tauraroc fmt [options] <file.tr | dir> ...
tauraroc fmt [options] -               # read stdin, write stdout

  -w, --write        rewrite files in place (only when they change)
  --check            write nothing; list files that would change; exit 1 if any
  --width <n>        wrap single-line statements longer than n columns (default 100)
  --sort-imports     sort runs of consecutive one-line import statements
  --ast              the legacy AST printer (single file; no safety nets, kept for compatibility)
```

With one file and no `-w`/`--check` the result goes to stdout. Directories are
searched recursively for `*.tr` (dot-directories and `build/` are skipped).
Exit status: `0` success, `1` `--check` found differences, `2` an error (unparsable
input, I/O, or a failed safety check — the file is then left untouched).

### What it does

* 4-space indentation per block level (tabs count as 4 columns on input).
* Spacing: one space around binary operators, `=`, `->`, `=>`; none inside brackets,
  before `,`/`;`/`:` or between a callee and `(`/`[`; `a, b`; `{k: v}`; unary
  operators hug their operand (`-x`, `*args`); keywords keep a space (`not (x)`,
  `return [1]`); function types are `def(int) -> int`.
* Blank lines: runs are capped (two at top level, one inside blocks); a blank line is
  inserted before each block-bodied `def`/`class`/`enum`/`interface`/`extend`
  (together with the comment/decorator lines attached directly above it) unless it
  directly follows its opening line.
* Comments: text is byte-identical; standalone comments are re-indented to the block
  they belong to, trailing comments get exactly two spaces before the `#`.
* Over-long statements (single physical line, longer than `--width`) are wrapped at
  their brackets — the last bracket group whose head fits, one argument per line when
  the body does not fit, closing bracket on its own line. A statement is wrapped only
  if the wrapped form fits; an unbreakable string or expression keeps the flat line.
* Existing line breaks inside brackets and method chains (`.a()` on the next line)
  are kept and re-indented by bracket depth; they are never joined.
* `--sort-imports` sorts a run of adjacent one-line `import`/`from … import` statements
  (no blank line or comment between them) alphabetically.

### What it never does

It is token/layout based, not AST based: nothing is re-printed from a parse tree.
String, char, f-string and number literals are copied byte for byte; statement order
and comment text never change; no trailing commas or parentheses are added.
Logical lines that contain an indentation-sensitive block expression inside brackets
(`x = (do:` … `)`) are kept verbatim and shifted as a unit.

### Safety nets (run on every file, before anything is written)

1. the original must parse with the real parser;
2. the output must re-lex to **exactly** the same token stream as the input (compared
   with the real lexer, ignoring only a missing final newline);
3. the output must parse;
4. formatting the output again must reproduce it byte for byte.

If a check fails the file is not touched and the failure is reported (`--debug` prints
the offending text). Besides this, `scripts/fmt_guarantee.sh` formats a copy of
`std/`, `tests/`, `examples/` and `src/`, requires a fixed point, and requires that
the C generated for every test program is byte-identical before and after formatting.

### CI

`scripts/check_fmt.sh [paths…]` runs `tauraroc fmt --check` over the given files or
directories (default: the files this commit changed relative to `origin/master`) and
exits non-zero if any would change. It is not wired into the GitHub workflow yet:
the existing tree has not been reformatted, so enabling it on `std/` would fail today.

### Limitations

* Indentation is derived from the input's own block structure; a mis-aligned file does
  not parse and is refused.
* Wrapping only breaks at brackets: long boolean chains (`a and b and c …`) or long
  strings are left as they are.
* Lines inside a bracketed continuation are not re-wrapped even if still too long.
* Column alignment of consecutive assignments or trailing comments is not preserved.
