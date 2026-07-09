# 03 — Memory Model Internals: How Auto-Drop Is Codegen'd

---

## Overview

[`docs/lang/13_memory_and_ownership.md`](../lang/13_memory_and_ownership.md) describes
the **user-facing** guarantee: *"the compiler inserts every `free()` — you never write
one."* This document describes the **other side of that contract** — how `src/sema.tr`
and `src/codegen/c.tr` actually implement it in the generated C, for contributors
working on the compiler itself or on `std/` library code.

At a high level, three mechanisms cooperate:

1. **`TrStr`** — a small refcounted struct that is the C representation of every
   Tauraro `str` value. Plain heap allocations (classes, `List`/`Dict`/`Set`/etc.)
   remain raw, unrefcounted pointers freed exactly once via escape analysis.
2. **Auto-drop (RAII-lite)** — `src/sema.tr` decides, per local variable, whether it is
   safe to insert a release/free at scope exit (`HirStmt.SAutoDrop`), based on escape
   analysis (`str_escaped` / `coll_escaped`) and per-block visibility (`decl_block_id`).
3. **Wrap-hoisting** — `src/codegen/c.tr` detects *fresh, temporary* `TrStr` values
   (concat results, `.to_str()`, method-call results) that would otherwise leak or
   dangle if consumed inline, and hoists them into named temporaries with explicit
   `_tr_str_release` calls via `flush_wraps`/`gen_args`.

If you are debugging a leak or a heap-corruption crash involving strings, start with
the wrap-hoist section below — it is the single most common source of both classes of
bug (see "The fresh-expr-as-call-argument hazard").

---

## TrStr: the refcounted string type

### Struct layout

Defined in `runtime/tauraro_rt.h`:

```c
typedef struct {
    char* data;   /* NUL-terminated bytes */
    long* rc;     /* heap refcount, or NULL for literals/immortal strings */
} TrStr;
```

Every Tauraro `str` value is a `TrStr` by value (16 bytes on 64-bit: two pointers) —
not a `char*`. `data` and `rc` are **two separate heap allocations** (not one combined
block). This is deliberate: `_tr_strz(t)` returns `t.data` directly, and a lot of
`std/*.tr` code does `unsafe: _tr_c_free(x as Pointer[char])` on that `.data` pointer —
a single combined allocation would make that free corrupt the heap (freeing 8 bytes
past the real block start).

### Construction

| Helper | Use | `rc` |
|---|---|---|
| `_tr_str_lit(s)` | Wrap a `const char*` **string literal** | `NULL` (immortal — retain/release are no-ops) |
| `_tr_str_new(len)` | Allocate a fresh `len`-byte buffer, caller fills `.data` | `1` |
| `_tr_str_wrap(owned_char_ptr)` | Wrap an existing malloc'd `char*` (e.g. the result of a legacy `_tr_str_*` helper) into a refcounted `TrStr` | `1` (separate alloc for `rc`) |
| `_tr_str_box(s)` / `_tr_str_unbox(p)` | Box/unbox a `TrStr` (16 bytes) into the generic `void* val` slot used by `Option[T]`/`Result[T,E]`/collections | — |

`_tr_str_lit` and `_tr_str_wrap` are `_Generic`-dispatched macros: if you pass them an
expression that is *already* a `TrStr`, they pass it through unchanged instead of
double-wrapping. This matters because codegen sometimes can't statically know whether
a helper already returns `TrStr` or a legacy `char*`.

### Retain / release

```c
static inline TrStr _tr_str_retain(TrStr s) {
    if (s.rc) { (*s.rc)++; }
    return s;
}

static inline void _tr_str_release(TrStr s) {
    if (s.rc) {
        if (--(*s.rc) == 0) {
            _tr_free(s.data);
            _tr_free((void*)s.rc);
        }
    }
}
```

Because `rc == NULL` for string literals, retaining/releasing a literal is a no-op —
literals never need freeing. Everything else follows standard refcounting: retain on
every new alias that outlives the source expression, release exactly once per alias
when that binding goes out of scope.

### Copies vs. aliases — the key distinction for codegen

- **Aliases** (`_tr_str_retain` needed): an `EIdent` referring to an existing `str`
  local/parameter, assigned into a new binding. The new binding shares `data`/`rc`
  with the original — both must independently retain/release.
- **Fresh values** (no retain needed, but **must be released by someone**): the result
  of `+` concatenation, `.to_str()`, `.to_upper()`, any `_tr_str_wrap(...)`-producing
  helper, or any `ECall`/`EMethodCall`/`EBinOp` of type `str`. These have `rc == 1`
  already and are not aliased by anything else — codegen's job is to make sure exactly
  one `_tr_str_release` (or transfer-of-ownership) happens to them.
- **Borrowed reads** (no retain, no release): `EPropAccess` reads of a `str` field —
  these alias the owning struct's copy; the field's own lifetime governs it.

`src/codegen/c.tr`'s `str_retain_wrap(e, s, is_return)` implements the alias case: for
an `EIdent` referencing a tracked `str` local being copied into a new binding, it wraps
the generated expression in `_tr_str_retain(...)`. For `return x` where `x` is itself a
tracked local, no retain is emitted — ownership transfers to the caller (this matches
the `SAutoDrop` exclusion for returned locals).

---

## Auto-drop (RAII-lite)

### What gets dropped

`src/sema.tr`'s `is_droppable_sym(sym)` decides, for each local `Symbol`, whether the
compiler will emit an `HirStmt.SAutoDrop` for it at scope exit:

- **`str` locals**: get `_tr_str_release` at scope exit, *unless*:
  - `sym.str_escaped` is set (see below), or
  - the local was moved / maybe-moved / already freed, or
  - it has active borrows, or
  - it's not definitely initialized on every path.
- **`List`/`Vec`/`Dict`/`Map`/`Set` locals**: get the appropriate `*_free` /
  `Dict_free_strval` / `_tr_idict_free*` call at scope exit, *unless*:
  - `sym.coll_escaped` is set, or
  - moved/freed/active-borrows/not-definitely-initialized (same as above), or
  - the element/key/value types aren't "POD-droppable" (`_coll_elem_droppable`) — e.g.
    a `Dict[str, SomeClassWithoutFree]` is not auto-dropped because the runtime's
    `Dict_free` wouldn't know how to release the values.
- **User-defined class locals with a `free()` method**: dropped at scope exit unless
  moved/returned/borrowed-out/manually-freed, with a hardcoded exclusion list
  (`Vec`/`Map`/`List`/`Dict`/`Box`/`Mutex`/`RwLock`/`Atomic`/`Shared`/`Option`/`Result`/
  `Chan`/`StringBuilder`/`StringObj` — these either aren't refcounted the same way or
  may alias their own internal buffers).

### `decl_block_id` — per-block visibility

```python
pub decl_block_id: int    # id of the innermost if/while C block open at declaration
                          # within the current scope; 0 = scope top-level
```

Each `Symbol` records which `if`/`while` block (by a stack-based block id) was open
when it was declared. `compute_scope_drops` only emits an `SAutoDrop` for a symbol at
the point where its declaring block closes — and only if that block is still on the
"visible" block stack (`block_stack_contains`). This disambiguates **sibling**
`if`/`elif`/`else` or loop-iteration branches that declare same-named locals: each
branch gets its own drop, exactly once, at the end of *that* branch — not duplicated
into a sibling branch and not hoisted to the function's top-level scope.

### Escape analysis: `str_escaped` and `coll_escaped`

```python
pub str_escaped: bool    # a `str` local passed as a call/method-call ARGUMENT — may be
                          # aliased into a legacy List_str/Dict/Set (#52 not yet
                          # migrated); excluded from auto-drop to avoid UAF
pub coll_escaped: bool    # a List/Vec/Dict/Map/Set local that is ever read in a
                          # non-receiver position (call/method arg, assigned to
                          # another var, returned, stored in a literal) — raw
                          # unrefcounted pointer, so any alias must disable
                          # auto-free to avoid UAF
```

`mark_str_escaped` / `mark_coll_escaped` walk the HIR and flag any local that is used
in a way the compiler cannot prove is safe to auto-drop:

- Passed as an argument to a call or method call (`mark_escaped_coll_args` recursively
  walks binops, calls, method calls, casts, conditionals, collection literals, tuples,
  etc. looking for `EIdent` references to track).
- Returned from the function.
- Assigned into another variable (creates a second alias of the same raw pointer).
- Stored inside a list/dict/set/tuple literal.

When a local is escaped, the compiler simply **does not** auto-drop it — this trades a
potential leak for guaranteed safety (no use-after-free). This is a conservative,
correctness-first design: a leaked `str`/`List` is a known, bounded cost; a
use-after-free is a heap-corruption crash that can manifest anywhere.

> Note one important asymmetry: `mark_escaped_coll_args` is for `List`/`Vec`/`Dict`/
> `Map`/`Set` (raw pointers, must avoid double-free via aliasing). `str_escaped` is
> narrower — only call/method-call arguments — because `TrStr` is refcounted and
> *can* tolerate multiple aliases via `_tr_str_retain`; the exclusion exists
> specifically because legacy (`#52`-pre-migration) `List_str`/`Dict`/`Set` code paths
> may take a shallow, unretained alias of the `.data` pointer.

---

## The fresh-expr-as-call-argument hazard (and the wrap-hoist fix)

This is **the** pitfall to know about when touching string codegen or writing new
`std/` code that calls functions taking `str` parameters.

### The problem

Consider:

```python
resp.set_header("Cache-Control", "public, max-age=" + sm.max_age.to_str())
```

`"public, max-age=" + sm.max_age.to_str()` is a **fresh** `TrStr` (rc=1, brand new
`data`/`rc` allocations) — it is not bound to any variable. If codegen evaluated it
inline as a bare argument expression:

- Nothing retains it on the way into `set_header`.
- Nothing releases it after `set_header` returns (it's not a tracked local, so
  `SAutoDrop` never sees it) — that's a **leak**.
- Worse: in some code shapes, the temporary's lifetime/evaluation order interacted
  badly with how the callee stored the value (e.g. into a `Map[str,str]`), and the
  *previously stored* value for an unrelated key (`Cache-Control` vs `Connection`)
  came back as garbage — a heap-corruption / use-after-free **heisenbug**: it
  reproduced only in the full request-dispatch path, not in an isolated minimal
  repro, and adding `print()` statements anywhere nearby "fixed" it (a classic UAF
  signature). This is documented as the `#53`-class bug.

### The wrap-hoist fix (compiler-generated calls)

`src/codegen/c.tr` fixes this for codegen-emitted call sites via two cooperating
pieces:

**1. `_is_fresh_str_expr(e)`** — true for any `str`-typed `ECall`, `EMethodCall`, or
`EBinOp` (concatenation, `.to_str()`, nested calls, etc.) — anything that produces a
brand-new, independently-owned `TrStr`.

**2. `gen_args(args)`** — for each argument that is `str`-typed *and* a fresh
expression, hoists it into a named `TrStr` temporary and queues a release:

```python
pub def gen_args(self, args: Vec[Pointer[HirExpr]]) -> str:
    ...
    if _is_str_type(hir_expr_type(a).name) and self._is_fresh_str_expr(a):
        mut tmp = "_at" + self.next_temp()
        self.wrap_temp_decls.push("TrStr " + tmp + " = (" + a_s + ")")
        self.wrap_temp_names.push(tmp)
        a_s = tmp
```

**3. `flush_wraps(expr_s, is_void)`** — once the enclosing top-level expression or
statement is fully generated, wraps it in a GCC statement-expression that declares all
queued temporaries, evaluates the original expression (now referencing the temps),
releases every temporary, and yields the result:

```c
({ TrStr _at7 = (_tr_strx_concat("public, max-age=", _it8.data));
   __auto_type _wr = (set_header(resp, _tr_str_lit("Cache-Control"), _at7));
   _tr_str_release(_at7);
   _wr; })
```

So `resp.set_header("Cache-Control", "public, max-age=" + sm.max_age.to_str())`,
**when generated through `gen_args`/`flush_wraps`**, becomes something close to the
snippet above: the fresh concat result is bound to `_at7`, passed by value into
`set_header` (which may retain it if it stores it), and then released exactly once
afterward.

### `gen_binop`'s own concat-operand hoisting

A related case: `"a" + x.to_str() + "b"` is itself a binop whose *left* operand
(`"a" + x.to_str()`) is **also** a fresh `TrStr`. `gen_binop`'s `+`/concat handler
detects "owned" operands (`_tr_str_wrap(...)` results or `_is_fresh_str_expr` operands)
and consumes+releases them inline around `_tr_strx_concat`:

```python
if l_owned and r_owned:
    return "({ TrStr _cl = (" + ls + "); TrStr _cr = (" + rs + ");
               TrStr _cres = _tr_strx_concat(_cl.data, _cr.data);
               _tr_str_release(_cl); _tr_str_release(_cr); _cres; })"
```

This is what prevents nested concatenations and chained `.to_str()`/`.to_upper()` calls
from leaking their intermediate allocations — each fresh operand's `data`/`rc` pair is
released the instant `_tr_strx_concat` has copied its bytes into the new result.

### The remaining hazard: hand-written calls into APIs that don't get this treatment

The wrap-hoist machinery above is **codegen-side** — it only fires for argument
expressions that the C generator itself emits via `gen_args`. It does *not* retroactively
fix:

- Calls into runtime helpers reached via other codegen paths (e.g.
  `gen_args_strify`, direct `strz(...)` extraction for legacy `char*`-based APIs).
- The *semantics* of the callee: if a `std/` function takes a `str` parameter and
  stores it (e.g. into a `Map[str,str]` field) without itself retaining it, passing a
  fresh expression — even through `gen_args`'s hoist — only guarantees the temporary
  survives until `flush_wraps` releases it *after* the call returns. If the callee
  squirreled away `.data`/`.rc` from that temporary without its own `_tr_str_retain`,
  the post-call `_tr_str_release` can still invalidate the stored copy.

**The safe pattern for `.tr` authors**, regardless of which codegen path a given call
goes through, is the one already documented in
[`docs/lang/13_memory_and_ownership.md`](../lang/13_memory_and_ownership.md)'s spirit —
bind fresh string expressions to a `mut` local **before** passing them as an argument:

```python
# RISKY: fresh concat/.to_str() passed directly as an argument
resp.set_header("Cache-Control", "public, max-age=" + sm.max_age.to_str())

# SAFE: bind to a local first
mut cache_val = "public, max-age=" + sm.max_age.to_str()
resp.set_header("Cache-Control", cache_val)
```

Binding to a local gives the value a tracked lifetime (it participates in normal
auto-drop/`str_escaped` analysis as an `EIdent`) and ensures the callee, if it stores
the value, is working with a stable, retainable `TrStr` rather than a transient
statement-expression temporary.

---

## String-valued collections: `List_TrStr` and `Dict_free_strval`

Plain `List`/`Dict`/`Map`/`Set` are backed by raw, unrefcounted runtime structs
(`Dict`, `TrIDict`, etc. in `runtime/tauraro_rt.h`) whose `*_free` functions free
nodes/buckets/keys but historically had **no idea** that a stored *value* might itself
be a refcounted `TrStr` — freeing the container would leak every string value inside
it.

Two runtime additions close this gap:

- **`List_TrStr`** (`runtime/tauraro_rt.h`, the refcounted-string element container of the TrStr migration):
  a dedicated `{ TrStr* data; size_t len; size_t capacity; }` container used for
  `List[str]`/`Vec[str]`/`Set[str]` and `Dict.keys()`. `List_TrStr_append` retains each
  value on insert; `List_TrStr_free` releases every element before freeing the backing
  array.

- **`Dict_free_strval(d)` / `_tr_idict_free_strval(d)`**: like `Dict_free`/
  `_tr_idict_free`, but for `Dict[K,str]`/`Map[K,str]` whose values are
  `_tr_str_box(TrStr)`-allocated boxes. They unbox + `_tr_str_release` each value and
  free the box itself, *before* freeing the node/key/bucket/struct:

  ```c
  static void Dict_free_strval(Dict* d) {
      ...
      while (n) {
          _DictNode* nx = n->next;
          if (n->key) _tr_free(n->key);
          if (n->value) { _tr_str_release(*(TrStr*)n->value); _tr_free(n->value); }
          _tr_free(n);
          n = nx;
      }
      ...
  }
  ```

`src/codegen/c.tr` tracks, per local, whether a `Dict`/`Map`/`Set`/`List`/`Vec` is
"str-valued" (`coll_local_strval`) and whether it's backed by `TrIDict` vs `Dict`
(`coll_local_idict`), and `SAutoDrop` codegen (`HirStmt.SAutoDrop`) selects the correct
one of the four free variants (`Dict_free` / `Dict_free_strval` / `_tr_idict_free` /
`_tr_idict_free_strval`) accordingly. `src/sema.tr`'s `is_droppable_sym` correspondingly
allows `Dict[*, str]` to be auto-dropped *only because* this codegen-side
`Dict_free_strval` path exists — without it, auto-dropping a str-valued dict would leak
every value string.

`List[str]`/`Vec[str]`/`Set[str]` use `List_TrStr_free` (selected via the
`coll_local_sfx` map, suffix `"TrStr"`) the same way.

---

## Best practices for contributors writing new `std/` code

1. **Bind fresh string expressions to a local before passing them as call
   arguments**, especially for any function that *stores* its `str` parameter (header
   maps, config structs, builders). See "The remaining hazard" above. This is cheap
   and removes an entire class of heisenbugs.

   ```python
   # Prefer this:
   mut msg = "Error: " + err.to_str()
   logger.warn(msg)

   # over this, when `warn` stores its argument:
   logger.warn("Error: " + err.to_str())
   ```

2. **Never name a cleanup method `free()`** on a class whose constructor uses the
   `mut v = T(); ...; unsafe: raw.write(v); return raw` raw-box pattern (common in
   `std/encoding/json.tr`-style code). `is_droppable_sym` will auto-drop the local `v`
   inside the constructor itself — right after `raw.write(v)` — dangling `raw` for
   every instance ever constructed. Use `dispose()` instead (see
   `HttpResponse.dispose()` in `std/net/http_server.tr` for precedent).

3. **Don't manually `free()`/`_tr_str_release()` a tracked `str`/collection local in
   safe code** — `SAutoDrop` already does it, exactly once, at the correct scope-exit
   point for every control-flow path. Manual release races with `SAutoDrop` and causes
   double-frees.

4. **If you introduce a new collection-mutating runtime helper** (e.g. a new
   `List_X_append`/`Dict_X_free` variant), check whether `X` can be `str`/`TrStr` and
   whether the existing `Dict_free_strval`/`List_TrStr_free` pattern needs a sibling —
   `is_droppable_sym`'s `_coll_elem_droppable` gate exists specifically to keep
   auto-drop from firing on element/value types the runtime can't fully release.

5. **When debugging a string-related leak or crash, check for escape exclusions
   first**: a local with `str_escaped`/`coll_escaped` set will *not* be auto-dropped —
   that's an intentional leak-over-corruption tradeoff, not a bug, unless the local
   genuinely never escapes (in which case the escape analysis itself may be
   over-conservative and worth tightening, as has happened before with
   `mark_escaped_coll_args` and static-method calls).

---

Previous: [Building and Contributing ←](02_contributing.md) · Next: [Codegen Guide →](04_codegen_guide.md)
