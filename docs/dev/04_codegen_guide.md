# 04 — Codegen Guide

A reference for compiler contributors: how major Tauraro language features
flow from source through `src/sema.tr` into `src/codegen/c.tr`, and the
approximate shape of the C they produce. Generated C samples below are
**illustrative** — they show the structure (naming conventions, struct
layout, dispatch mechanism), not byte-exact compiler output.

See also: [Compiler Pipeline](../lang/README.md#compiler-pipeline),
[11 — Generics](../lang/11_generics.md).

---

## 1. Generic Monomorphization

**Where:** `src/codegen/c.tr`

- `CGenerator.ensure_mono(cls, type_args)` (~line 592) — generates one
  concrete C struct + method set per `(class, concrete-type-args)`
  combination, deduplicated via `mono_done: Map[str, bool]` keyed as
  `"<ClassName>_<suffix>"` (e.g. `"Box_int"`).
- `CGenerator.ensure_mono_func(fname, targ)` (~line 738) — same idea for
  generic free functions; mono key is `"func_<fname>__MONO_<targ>"` and the
  emitted C function is named `<fname>__MONO_<targ>`.
- `type_args_suffix` / `type_suffix` / `synth_class_suffix` (~lines 479-588)
  build the suffix string from concrete C types (`int` -> `i64`,
  `float` -> `f64`, `str` -> `str`, etc.).
- Output for monomorphized classes/functions is redirected into
  `self.mono_buf` (a `StringBuilder`), flushed once at the end of codegen.

### Example

```python
class Box[T]:
    pub value: T

extend Box:
    pub def init[T](v: T) -> Box[T]:
        mut b = Box[T]()
        b.value = v
        return b

    pub def get[T](self) -> T:
        return self.value

def identity[T](x: T) -> T:
    return x

mut b = Box.init[int](42)
print(identity[int](7))
```

### Generated C shape (approximate)

```c
typedef struct Box_i64 Box_i64;
typedef struct Box_i64 {
    long long value;
} Box_i64;

Box_i64 Box_i64_init(long long v);
long long Box_i64_get(Box_i64* self);

Box_i64 Box_i64_init(long long v) {
    Box_i64 b; b.value = v; return b;
}
long long Box_i64_get(Box_i64* self) { return self->value; }

/* generic free function */
long long identity__MONO_i64(long long x) { return x; }
```

Only the `(Box, int)` and `(identity, int)` instantiations that are actually
called get emitted — no `Box_str`/`identity_str` unless used elsewhere.

---

## 2. Enums (Algebraic Data Types)

**Where:** `src/codegen/c.tr`

- `gen_enum_struct(e: HirEnum)` (~line 929) — emits a `<Name>_tag` C `enum`
  for variant tags, a tagged-union `struct <Name>` (`tag` field plus a
  `union data` of per-variant anonymous structs for variants that carry
  fields), and constructor helpers:
  - zero-field variants: `#define <Enum>_make_<Variant>() ((<Enum>){.tag=<Enum>_<Variant>})`
  - data-carrying variants: `static inline <Enum> <Enum>_ctor_<Variant>(...)`
    (str fields are `_tr_str_retain`'d on construction).
- `gen_match(expr, arms, indent)` (~line 5097) — lowers `match` to either a
  chain of `if (subj.tag == <Enum>_<Variant>) { ... }` (no guards) or a
  `do { ... } while(0)` with per-arm `if`/`break` (when any arm has a
  `match ... if <guard>` clause). `Pattern.PVariantBind*` arms bind the
  payload via `subj.data.<Variant>.<field>`.

### Example

```python
enum Color:
    Red
    Green(int)

def main():
    mut c = Color.Green(42)
    match c:
        case Color.Red:
            print("red")
        case Color.Green(v):
            print(v)
```

### Generated C shape (approximate)

```c
typedef enum { Color_Red, Color_Green } Color_tag;

typedef struct Color {
    Color_tag tag;
    union {
        struct { long long v; } Green;
    } data;
} Color;

#define Color_make_Red() ((Color){.tag=Color_Red})
static inline __attribute__((always_inline))
Color Color_ctor_Green(long long v) {
    Color _r = {.tag=Color_Green};
    _r.data.Green.v = v;
    return _r;
}

/* match */
__auto_type _t1 = Color_ctor_Green(42);
if (_t1.tag == Color_Red) {
    printf("red\n");
} else if (_t1.tag == Color_Green) {
    long long v = _t1.data.Green.v;
    printf("%lld\n", v);
}
```

---

## 3. Closures / First-Class Functions

**Where:** `src/codegen/c.tr`

Closures are **portable (GCC + Clang)**: each is OUTLINED to a top-level
function plus a heap capture environment of pointers (capture-by-reference).

- **Capture analysis** happens in sema (`lower_expr` for `Expr.EClosure`):
  after lowering the body it collects free variables (`collect_block_refs`)
  that resolve to an *enclosing local* (not the closure's own params/locals,
  not a global/function) and records them as `captures` (a `HirParam` each,
  with type). The old GCC path left `captures` empty.
- `gen_closure(params, ret_ty, body, captures)` — emits a top-level function
  `_closure_<N>(void* __envp, params)` into `closure_buf` (flushed once at
  file end via `flush_closures`). The env is an **inline anonymous struct**
  `{ void* __fn; <cty>* p_cap; … }`; the same layout is written at the def and
  the creation site and matched through the `void*` cast (no typedef, so no
  ordering problem). Inside the body a captured name lowers to
  `(*__env->p_<name>)` (see `gen_expr` `EIdent` and `SAssign`, gated on
  `closure_env_var != "" && closure_cap_set.contains(name)`).
- **Creation** returns a statement-expression that carries a *block-scope
  prototype* for `_closure_<N>` (so the def can live at file end), `malloc`s
  the env, sets `__fn` + `&capture` for each, and **tags the low bit** of the
  returned pointer.
- **Call site** (`gen_call`, the `lambda`/`void*`/`def` branch): branch on the
  tag — a tagged value is a CLOSURE (untag, read `__fn`, call with the env as
  the first arg), an untagged value is a bare NAMED-function pointer (call
  directly). This lets a closure and a function name be interchangeable as a
  `def(...)->R` value.
- The closure value is still `void*` — `type_to_c("lambda")`/`type_to_c("def")`
  return `"void*"`. `closure_buf`/`flush_closures` handle emission; the env is
  heap-allocated and **not yet freed** (a small per-creation leak — reclamation
  via scope-based drop is a follow-up).

### Example

```python
def main():
    mut counter = 0
    add = lambda x: counter + x   # captures `counter` by reference
    counter = 10
    print(add(5))    # 15
```

### Generated C shape (approximate)

```c
/* file scope (flushed at end): */
long long _closure_1(void* __envp, long long x) {
    struct { void* __fn; long long* p_counter; }* __env = __envp;
    return (*__env->p_counter) + x;     /* capture by reference */
}

int main(void) {
    long long counter = 0;
    void* add = ({ long long _closure_1(void*, long long);
                   struct { void* __fn; long long* p_counter; }* __c = malloc(sizeof(*__c));
                   __c->__fn = (void*)&_closure_1; __c->p_counter = &counter;
                   (void*)((uintptr_t)__c | 1); });   /* tagged env */
    counter = 10;
    void* __cl = add;                                 /* call: tag -> env call */
    void* __ce = (void*)((uintptr_t)__cl & ~(uintptr_t)1);
    printf("%lld\n", ((uintptr_t)__cl & 1)
        ? ((long long(*)(void*, long long))(*(void**)__ce))(__ce, 5)
        : ((long long(*)(long long))__cl)(5));
}
```

(GCC nested functions are non-portable to non-GCC/Clang-on-Windows targets —
worth flagging if portability work ever targets MSVC.)

---

## 4. Interfaces / Vtable Dispatch

**Where:** `src/codegen/c.tr`

- `gen_interface_vtable(iface: HirInterface)` (~line 1003) — for each
  `interface`, emits:
  - `typedef struct _<Iface>_vtable { <ret> (*<method>)(void* self, ...); ... } <Iface>_vtable;`
  - `typedef struct { <Iface>_vtable* vtable; void* data; } <Iface>_obj;`
- `gen_one_iface_wrap(cls_name, iface)` (~line 1026) — for each class that
  `implements <Iface>`, emits a `static inline <Iface>_obj <Class>_as_<Iface>(<Class>* self)`
  function that builds a `static const <Iface>_vtable` populated with
  function-pointer casts to `<Class>_<method>`, and returns an
  `<Iface>_obj { .vtable = &_vtbl_..., .data = (void*)self }`.
- For **generic** classes/interfaces, the monomorphized equivalent is
  generated inline inside `ensure_mono` (~lines 671-728), producing
  `<Iface>_<suffix>_vtable` / `<Class>_<suffix>_as_<Iface>_<suffix>`.
- Dispatch (~line 2321-2323, `gen_method_call`): a call on a value whose
  static type is an interface compiles to
  `obj.vtable-><method>(obj.data, <extra args>)`.

### Example

```python
interface Shape:
    def area(self) -> float

class Circle implements Shape:
    pub r: float

extend Circle:
    pub def area(self) -> float:
        return 3.14159 * self.r * self.r

def print_area(s: Shape):
    print(s.area())

print_area(Circle.as_Shape(&circle_instance))
```

### Generated C shape (approximate)

```c
typedef struct _Shape_vtable {
    double (*area)(void* self);
} Shape_vtable;
typedef struct { Shape_vtable* vtable; void* data; } Shape_obj;

static inline Shape_obj Circle_as_Shape(Circle* self) {
    static const Shape_vtable _vtbl_Circle_Shape = {
        .area = (double(*)(void*))(Circle_area),
    };
    return (Shape_obj){ .vtable = (Shape_vtable*)&_vtbl_Circle_Shape, .data = (void*)self };
}

void print_area(Shape_obj s) {
    printf("%f\n", s.vtable->area(s.data));
}
```

---

## 5. Sendable / Concurrency

**Where:** `src/sema.tr` (checks), `runtime/tauraro_rt.h` (helpers),
`src/codegen/c.tr` (type mapping)

### Sema checks (`src/sema.tr`)

- A helper around line 440-460 (thread-safety section) decides whether a
  type is "Sendable": `Shared`/`Weak` are always Sendable (ref-counted);
  user classes are Sendable only if they list `implements Sendable`
  (checked via `cls.iface_names`), otherwise sema raises **[T-1]**
  ("Type '...' is not Sendable and cannot be safely shared across threads.").
- **[T-2]** (~line 527): a class that declares `implements Sendable` but has
  a non-Sendable field (not wrapped in `Mutex[T]`/`RwLock[T]`/`Atomic[T]`)
  is an error.
- **[T-3]** (~line 529): a `Sendable` class with a bare primitive field
  (not `Atomic[T]`) is a *warning* about possible data races.
- `Thread.spawn(...)` / `ThreadPool.spawn(...)` / `await_all` argument lists
  are checked arg-by-arg for Sendability (~lines 3131-3146, 2900).

### Codegen type mapping (`src/codegen/c.tr`, ~lines 456-464)

| Tauraro type | C type |
|---|---|
| `Mutex[T]` | `_TrMutexBox*` |
| `RwLock[T]` | `_TrRWLBox*` |
| `Atomic[T]` | `_TrAtomic*` |
| `Chan[T]` | `_TrChan*` |
| `Shared[T]` | `_TrSharedBox*` |
| `Thread` | `_TrThreadObj*` |
| `ThreadPool` | `_TrThreadPool*` |

### Runtime helpers (`runtime/tauraro_rt.h`)

- `_TrMutexBox { _TrMutexH* mu; long long data; _Atomic int rc; volatile int _locked; }`
  with `_tr_mutexbox_new/_lock_get/_set_unlock/_unlock/_clone/_drop` (~line 1330+).
  `.get()` in source compiles to `_tr_mutexbox_lock_get`, paired with an
  `__attribute__((cleanup))` auto-unlock guard emitted by codegen for RAII-style
  unlocking at scope exit.
- `_TrAtomic { _Atomic long long val; }` with `_tr_atomic_new/_load/_store/
  _add/_sub/_swap/_cas` (~line 1466+) — all lock-free `stdatomic.h` ops.
- `spawn`/`Thread.spawn(fn, args...)`: `emit_spawn_wrapper_for_expr` (~line
  3745) generates a `static void* _tr_spawn_wrap_<fn>(void* _vp)` trampoline
  per spawned function — 0-arg, 1-arg (cast `_vp` directly), and N-arg
  (heap `long long[N+1]` array, `[0]` = return slot) variants — passed to
  `_tr_thread_start(fn_ptr, arg)`.
- `task_group:` blocks (`HirStmt.STaskGroup`, sema.tr ~3806/4601, c.tr
  tracked via `in_task_group` depth counter) join all spawned tasks before
  the block exits.

### Example

```python
class Counter implements Sendable:
    pub n: Atomic[int]

def main():
    mut c = Counter()
    c.n = Atomic[int].init(0)
    task_group:
        spawn c.n.add(1)
        spawn c.n.add(1)
    print(c.n.get())   # 2
```

### Generated C shape (approximate)

```c
typedef struct Counter { _TrAtomic* n; } Counter;

static void* _tr_spawn_wrap_inc(void* _vp) {
    _TrAtomic* n = (_TrAtomic*)_vp;
    _tr_atomic_add(n, 1);
    return NULL;
}

void main(void) {
    Counter c; c.n = _tr_atomic_new(0);
    _TrThread t1 = _tr_thread_start(_tr_spawn_wrap_inc, (void*)c.n);
    _TrThread t2 = _tr_thread_start(_tr_spawn_wrap_inc, (void*)c.n);
    /* task_group join */
    _tr_thread_join(t1); _tr_thread_join(t2);
    printf("%lld\n", _tr_atomic_load(c.n));
}
```

(approximate/needs verification — the exact spawn-wrapper signature for
method-call spawns like `spawn c.n.add(1)` vs. free-function spawns may
differ; verify against `emit_spawn_wrapper_for_expr` for method-call
callees.)

---

## 6. Generator Expressions

**Where:** `src/codegen/c.tr`, `src/sema.tr`, AST: `src/ast.tr`

- `HirExpr.EGeneratorExpr(element, generators, _)` (ast.tr line 181) exists as
  an AST/HIR node and, in `c.tr` (~line 1159), is dispatched to the same
  `gen_list_comp(element, generators)` as `EListComp` (~line 3682) — i.e. *if*
  this node were ever produced, it would lower eagerly to a fully-materialized
  `List`, exactly like a list comprehension, with no lazy state-machine /
  coroutine codegen.
- **However**, `(x for x in ...)` syntax is rejected by `src/parser.tr` today
  ("unexpected ')' in expression") — the parser never actually constructs
  `EGeneratorExpr`, so this is dead codegen reachable only if parser support is
  added later. This matches `docs/lang/advanced/04_generators.md`'s "Status:
  not currently supported" — generator expressions are not a usable feature
  yet. If/when parser support lands, expect the eager list-comp lowering
  described above (not true laziness) unless the codegen is revisited too.

### Example

```python
mut squares = (x * x for x in range(5))
for s in squares:
    print(s)
```

### Generated C shape (approximate)

```c
/* identical shape to [x*x for x in range(5)] */
List_i64* squares = List_i64_new();
for (long long x = 0; x < 5; x++) {
    List_i64_append(squares, x * x);
}
for (size_t _i = 0; _i < squares->len; _i++) {
    long long s = squares->data[_i];
    printf("%lld\n", s);
}
```

---

## 7. Channels / Select

**Where:** `src/codegen/c.tr`, `runtime/tauraro_rt.h`

- `type_to_c`: `Chan[T]` -> `_TrChan*` (~line 456).
- Method calls on a `Chan` value (~lines 2462-2481) map directly to runtime
  functions:

| Tauraro | C |
|---|---|
| `Chan[T].init(cap)` / `Chan(cap)` | `_tr_chan_new(cap)` |
| `ch.send(v)` | `_tr_chan_send(ch, v)` |
| `ch.recv()` | `_tr_chan_recv(ch)` |
| `ch.try_send(v)` | `_tr_chan_try_send(ch, v)` |
| `ch.try_recv()` | `_tr_chan_try_recv_val(ch)` |
| `ch.send_timeout(v, ms)` | `_tr_chan_send_timeout(ch, v, ms)` |
| `ch.recv_timeout(ms)` | `_tr_chan_recv_timeout_val(ch, ms)` |
| `ch.close()` / `is_closed()` / `len()` / `cap()` / `free()` | `_tr_chan_close/_is_closed/_len/_cap/_free` |
| `for x in ch:` | `_tr_chan_recv_ok(ch, &ok)` (~line 4751) loop |

- `_TrChan` struct (`runtime/tauraro_rt.h` ~line 702) is a fixed-capacity
  ring buffer of `long long` slots guarded by a `CRITICAL_SECTION`/mutex
  (non-int payloads are passed through as bit-cast `long long`s / pointers).
- `gen_chan_select(arms, indent)` (~line 4965) compiles a `select:` block to
  a **polling loop**: each arm's channel is probed with
  `_tr_chan_try_recv_val` (recv arms, ~line 5032) or `_tr_chan_try_send`
  (send arms, ~line 5052) inside a retry loop, executing the first arm whose
  non-blocking op succeeds (with a `default:`/timeout arm breaking out).

### Example

```python
def main():
    mut ch1 = Chan[int].init(1)
    mut ch2 = Chan[int].init(1)
    ch1.send(10)
    select:
        case v := <-ch1:
            print(v)
        case v := <-ch2:
            print(v)
```

### Generated C shape (approximate)

```c
_TrChan* ch1 = _tr_chan_new(1);
_TrChan* ch2 = _tr_chan_new(1);
_tr_chan_send(ch1, 10);

for (;;) {
    {
        bool _ok1;
        long long _crv_t1_0 = _tr_chan_try_recv_val(ch1); /* + ok-flag check */
        if (_ok1) { long long v = _crv_t1_0; printf("%lld\n", v); break; }
    }
    {
        bool _ok2;
        long long _crv_t1_1 = _tr_chan_try_recv_val(ch2);
        if (_ok2) { long long v = _crv_t1_1; printf("%lld\n", v); break; }
    }
}
```

(approximate — the real loop includes additional bookkeeping for timeout and
`default:` arms; see `gen_chan_select` for exact emitted statements.)

---

## 8. `Vec[def(...) -> R]` Function-Pointer Type Arguments (`ETypeArg`)

**Where:** `src/ast.tr`, `src/parser.tr`, `src/sema.tr`, `src/codegen/c.tr`

- AST node: `Expr.ETypeArg(ty: Pointer[AstType])` (`src/ast.tr` line 189).
- Parser (`src/parser.tr` ~line 1320, in `parse_postfix`'s `[...]` handling):
  when the token after `[` is `KwDef`, the bracket contents cannot be a value
  expression (a `def(...)->R` type can't start an expression), so the parser
  calls `parse_type()` and wraps the result in `Expr.ETypeArg(...)` instead
  of treating `[...]` as an index/subscript.
- Sema (`src/sema.tr` ~line 3300, inside `EIndex` lowering): when the index
  expression is `Expr.ETypeArg(targ_ty)`, sema sets `is_generic = true`,
  `generic_arg_ty = targ_ty` (the `def(...)->R` `AstType`, whose `.name` is
  `"def"`). Since the outer object (`Vec`) is a recognized container type
  (~line 3333 `obj_is_type` check), this becomes a type application: a
  `Vec` `AstType` with `args = [def(...)->R]`, returned as
  `HirExpr.EIdent("Vec", container_ty, false)` (~line 3350).
- Codegen (`src/codegen/c.tr`):
  - `type_to_c` for `Vec`/`List` (~lines 423-437) reads `ty.args.get(0)`
    (the `def(...)->R` type) and calls `list_elem_suffix(elem.name)`
    (~line 501) with `elem.name == "def"`.
  - `list_elem_suffix` has no special case for `"def"`, so it falls through
    to the default `return "ptr"` (~line 523) -> `type_to_c` returns
    **`List_ptr*`** — the generic `void**`-backed list
    (`runtime/tauraro_rt.h` ~line 2975: `typedef struct { void** data;
    size_t len; size_t capacity; } List_ptr;`).
  - Each `.push(add1)` / `.push(mul2)` pushes the bare function's address
    (`(void*)&add1`) — same `void*`-as-function-pointer representation used
    for closures/`lambda` (`type_to_c("def") == "void*"`, ~line 316). Calling
    `fns.get(0)(10)` casts the retrieved `void*` back to
    `long long(*)(long long)` and invokes it (same call-site cast path as
    closures, ~line 2109).

### Example

```python
def add1(x: int) -> int:
    return x + 1

def mul2(x: int) -> int:
    return x * 2

def main():
    mut fns = Vec[def(int) -> int].init(2)
    fns.push(add1)
    fns.push(mul2)
    print(fns.get(0)(10))    # 11
    print(fns.get(1)(10))    # 20
```

### Generated C shape (approximate)

```c
List_ptr* fns = List_ptr_new();      /* capacity hint 2 */
List_ptr_append(fns, (void*)&add1);
List_ptr_append(fns, (void*)&mul2);

printf("%lld\n", ((long long(*)(long long))fns->data[0])(10));  /* 11 */
printf("%lld\n", ((long long(*)(long long))fns->data[1])(10));  /* 20 */
```

No closure/env allocation occurs — `add1`/`mul2` are plain top-level C
functions, and `List_ptr` is the same generic pointer-list backing used for
other "unknown element type" `Vec`/`List` instantiations.

---

## Summary Table

| Feature | Primary file(s) | Key entry point | Confidence |
|---|---|---|---|
| Generic monomorphization | `src/codegen/c.tr` | `ensure_mono`, `ensure_mono_func` | High |
| Enums / match | `src/codegen/c.tr` | `gen_enum_struct`, `gen_match` | High |
| Closures | `src/codegen/c.tr` | `gen_closure` | High |
| Interfaces / vtables | `src/codegen/c.tr` | `gen_interface_vtable`, `gen_one_iface_wrap` | High |
| Sendable / concurrency | `src/sema.tr`, `runtime/tauraro_rt.h` | T-1/T-2/T-3 checks, `_tr_mutexbox_*`, `_tr_atomic_*` | High (codegen detail for spawn approximate) |
| Generator expressions | `src/codegen/c.tr` | `gen_list_comp` (shared with list comp) | High (eager-eval confirmed) |
| Channels / select | `src/codegen/c.tr`, `runtime/tauraro_rt.h` | `gen_chan_select`, `_tr_chan_*` | Medium (select loop detail approximate) |
| `Vec[def(...)->R]` / `ETypeArg` | `src/ast.tr`, `src/parser.tr`, `src/sema.tr`, `src/codegen/c.tr` | `ETypeArg`, `list_elem_suffix` -> `List_ptr` | High |

---

Previous: [Memory Model Internals ←](03_memory_model_internals.md) · Next: [Building Libraries →](05_building_libraries.md)
