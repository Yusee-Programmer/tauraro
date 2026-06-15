# 06 — Best Practices & Known Pitfalls

A curated list of hard-won lessons from compiler and stdlib development. Each
entry says **what goes wrong**, **why**, and **the fix/pattern to use**. Most
of these were discovered the hard way (heisenbugs, self-host corruption,
silent miscompiles) — read this before touching the areas it covers.

---

## Memory & String Pitfalls (codegen / stdlib authors)

See [03 — Memory Model Internals](03_memory_model_internals.md) for the full
TrStr/auto-drop background. The entries below are the sharp edges.

### 1. Fresh string expressions as call arguments

**What goes wrong:** Passing a freshly-constructed string expression — a `+`
concat, a `.to_str()`/`.to_string()` result, or any `ECall`/`EMethodCall`/
`EBinOp` that produces a brand-new `TrStr` — **directly** as a call argument
can corrupt unrelated heap memory or segfault under real dispatch paths:

```python
# DANGEROUS — fresh concat expr passed directly as a call argument
resp.set_header("Cache-Control", "public, max-age=" + sm.max_age.to_str())
```

This corrupted an unrelated `Map[str,str]` entry (`Cache-Control` came back as
garbage or the *next* header's name) and crashed under concurrent dispatch.
Isolated minimal repros of `set_header`/`to_wire` did **not** reproduce it —
it only showed up through the real request/reactor dispatch path, and adding
`print()` nearby "fixed" it (classic heisenbug/UAF signature).

**Why:** The compiler's wrap-hoist pass (`flush_wraps` in `src/codegen/c.tr`)
handles most fresh-TrStr-as-argument cases, but not every call-site shape is
covered yet (tracked as the "#53 fresh TrStr operand lifetime" bug class).

**Fix — always bind fresh string expressions to a `mut` local first:**

```python
mut cache_val = "public, max-age=" + sm.max_age.to_str()
resp.set_header("Cache-Control", cache_val)
```

Apply this proactively to any `some_call(name, <expr>)` where `<expr>` is not
already a bare variable, anywhere in `std/` or application code that builds
headers/messages/keys from concatenation.

### 2. The `free`-named-method auto-drop trap

**What goes wrong:** If a class has a cleanup method literally named `free`,
and that class is constructed with a pattern like:

```python
extend Conn:
    pub def take_buffer(self) -> Buffer:
        mut raw = self.buf
        raw.write(v)
        return raw
```

sema's auto-drop sees the local `raw` has a `free()` method and a write
happened, and frees `raw` immediately at scope exit — **before** the `return`
takes effect — leaving the returned value dangling.

**Fix:** Name resource-cleanup methods `dispose()`, not `free()`, on classes
built via this "construct locally, mutate, then return" pattern. Reserve
`free()` for classes that truly own their resource for their entire lifetime
and are never returned past the scope that frees them.

### 3. String-valued collections need their own free path

`List[str]`, `Vec[str]`, `Set[str]`, `Dict[str,V]`/`Map[K,str]` are backed by
`List_TrStr` / `Dict_free_strval` / `_tr_idict_free_strval` (see
[03 — Memory Model Internals](03_memory_model_internals.md)). If you add a new
generic collection method that can hold `str` elements/values, make sure its
`.free()`/auto-drop path releases each boxed `TrStr`, not just the backing
array — a generic `free(buckets)` alone leaks every string the collection held.

### 4. `Map[K,V]`'s OOP class can't get new dispatched methods

`Map[K, V]` syntax always aliases the builtin `TrMap`/`Dict` runtime type —
`src/codegen/c.tr` hardcodes `class_name != "Map"` in several builtin-dispatch
checks. Adding a new method to `std/core/map.tr`'s `Map` class will not be
reachable via `m.your_method()` on a `Map[K,V]`-typed value. If you need new
Map behavior, either extend the builtin dispatch in `c.tr` directly, or expose
it as a free function taking the map.

---

## Codegen Correctness Pitfalls

### 5. Don't add `pragma GCC optimize(...)` or `__attribute__((optimize(...)))`

These trigger an **implicit LTO crash in GCC 15.2 (MinGW)** when emitted into
generated C. `__attribute__((hot))` alone is safe and already applied to hot
functions; don't go further. Bootstrap and normal builds use `-O2`; `-O3
-march=native -funroll-loops` is opt-in only (`src/main.tr`), never via
per-function pragmas in `c.tr`.

### 6. Method name vs. free-function name collisions

`src/codegen/c.tr` keeps a flat, bare-name map of free functions for dispatch.
A class method with the same name as a free function used to get clobbered by
the free function's entry. **Free functions now win the bare-name map** —
when adding a new class method, check whether a same-named free function
exists anywhere in scope; if so, qualify the call (`ClassName.method(...)` /
`obj.method(...)`) rather than relying on bare-name resolution.

### 7. Borrowed `hir_expr_type()` results

In the bootstrap sema, `hir_expr_type(expr)` returns can get marked `Own` and
then auto-freed if stored in a temp variable — even though the value is
borrowed from `expr`. **Inline the call** at its use site instead of binding
it to a `mut`/`let` temp:

```python
# WRONG — temp gets auto-dropped, corrupting the borrowed type
mut ty = hir_expr_type(e)
... use ty ...

# RIGHT — inline
... use hir_expr_type(e) ...
```

### 8. Static method dispatch recovery

`Task.init()`-style static calls can lower to `init(Task, ...)` with an empty/
void receiver type if `hir_expr_type` returns nothing useful for the class
name. The fix pattern: when `hir_expr_type(obj)` returns `void`/empty for a
static-call receiver, recover the class name from the literal identifier
(`obj_s`) instead of the inferred type.

---

## Parser / Lexer Pitfalls

### 9. Keyword tokens as method/field names after `.`

If a keyword (e.g. `async`, `await`, `yield`) is used as a method name after a
dot (`pool.spawn`, `task.async`), `consume_ident()` in `src/parser.tr` must
have an explicit `KwXxx -> "xxx"` case, or the parser silently breaks the
expression into two bad statements with a confusing downstream error. Any new
keyword added to the lexer should get a corresponding entry here.

### 10. Keyword tokens inside module import paths

The same issue applies to import paths: `from std.async.task import ...`
fails silently if `async` lexes as `KwAsync` and the import-path parser only
expects `Token.Ident`. Use `consume_module_ident()` (which already handles
this) for **all** module-path segment parsing, not just the first segment.

### 11. F-strings with escaped quotes

F-string lexing (`f"...\"...\"..."`) cannot be done with a generic logos
regex — it needs the lexer's custom callback path. If you touch f-string
lexing, verify `\"` inside `f"..."` still round-trips; also verify codegen's
`to_str` guard still prevents class methods from being intercepted by builtin
casts (this guard and the f-string fix were co-dependent).

### 12. Non-ASCII bytes in `.tr` source

Non-ASCII bytes in `.tr` comments break the lexer/parser. Keep all `.tr`
source (including comments) strictly ASCII. This matters most for CI: the
published bootstrap seed in `bootstrap/c/` can be **older** than your local
`.taupkg` build despite an identical version string — always verify by
behavior, not by version string, when debugging "it works locally but not in
CI".

### 13. Stage-4 inline-block parsing

A `def`/`if` body written as a single inline line with 2+ semicolons,
immediately followed by `elif`/`else` on the next line, fails to parse at
stage 4 of self-hosting:

```python
# FAILS at stage 4
if cond: a = 1; b = 2; c = 3
elif other: ...
```

Use an indented multi-line block instead when `elif`/`else` follows.

---

## Build / Bootstrap Pitfalls

### 14. Never patch generated C directly

Fixes belong in `.tr` source (`src/`, `std/`) or `runtime/tauraro_rt.h` —
never in `build/*.c`. Generated C is regenerated and overwritten on the next
`tauraroc` invocation; any direct edit is silently lost. See
[02 — Contributing](02_contributing.md).

### 15. Runtime header sync

`runtime/tauraro_rt.h` is the **single source of truth** for the C runtime.
After editing it, copy it to wherever the active build reads its runtime
header from (e.g. `src/build/` if a stale copy is cached there) — a stale
runtime header produces confusing "works after `cargo bootstrap` but not
`tauraroc.exe`" symptoms. `main.tr` reads `tauraro/runtime/` first; the
`sync_headers_to_runtime` step (if present) writes there too.

### 16. `_TrTaskState` and similar refcounted runtime structs

When a runtime struct is shared between the spawning thread and a callback
(e.g. `_TrTaskState` for `await_timeout`), set its initial refcount to the
number of independent owners (2: runtime + last caller) and have **the last
releaser** free it — not the first. Getting this backwards is a
use-after-free that often only manifests under timing pressure.

### 17. Owned-`str`-local auto-drop (Option B) — reverted, don't retry blindly

A compiler-enforced auto-free of every heap `str` local (even with
EBinOp-only scoping + transfer/escape-clearing) caused
`STATUS_HEAP_CORRUPTION` in the gen-2 self-host. This was reverted. The
shipped design instead uses **escape analysis** (`str_escaped`/`coll_escaped`,
see [03](03_memory_model_internals.md)) — locals that escape via return,
call-argument transfer, or collection storage are exempted from auto-drop.
If you're tempted to revisit "free every local unconditionally," check
`feedback_owned_str_autodrop_failed` history first — the escape-analysis
approach exists *because* the blunter approach failed self-host fixpoint.

---

## Validation Checklist (applies to all of the above)

Before considering any fix to `src/`, `std/`, or `runtime/tauraro_rt.h` done:

1. Rebuild: `tauraroc.exe src/main.tr -o tauraroc_new.exe`
2. Self-host fixpoint: `tauraroc_new.exe src/main.tr -o tauraroc_gen2.exe` →
   compare byte size to `tauraroc_new.exe` (and ideally one more generation).
3. Run the full example suite (`examples/*.tr`) — no parse errors, no
   crashes, expected output.
4. If the change touches memory management, run any relevant leak-check
   examples and compare before/after heap growth.
5. Only then copy the new binary over `tauraroc.exe` (and sync to
   `~/.taupkg/bin/tauraroc-windows-x64/` if that's part of your workflow).

---

Previous: [05 — Building Libraries](05_building_libraries.md) · Back to [docs/dev index](README.md)
