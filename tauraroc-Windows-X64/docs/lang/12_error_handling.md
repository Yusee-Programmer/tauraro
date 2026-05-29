# 12 — Error Handling

---

## Two Complementary Models

Tauraro provides two error handling styles that complement each other:

| Style | Mechanism | C implementation | Overhead |
|-------|-----------|-----------------|---------|
| **Exception-style** | `try / except / finally` + `raise` | `setjmp` / `longjmp` | Small per `try`, none on happy path |
| **Result-type** | `throws` + `?` + `Result[T, E]` | Struct return | Zero — plain C struct |

The models are orthogonal: you choose based on where you are in the call stack and what you need to communicate.

---

## Exception-Style: try / except / finally

### Basic try/except

```python
try:
    mut n = risky_parse(input)
    process(n)
except e:
    print(f"error: {e}")
```

If code inside `try:` calls `raise(msg)`, execution immediately jumps to the `except e:` block. The variable `e` receives the string message passed to `raise`.

Hausa syntax:
```python
gwada:
    ...
kama e:
    buga(f"kuskure: {e}")
```

### raise

```python
def validate_age(age: int) -> void:
    if age < 0:
        raise("age cannot be negative")
    if age > 150:
        raise("age is unreasonably large: " + str(age))
    # valid
```

`raise(msg)` takes a `str`. It calls `_tr_exc_raise(msg)` in the runtime, which uses `longjmp` to jump to the nearest enclosing `try` block.

**What happens without a try/catch:** If `raise` is called and there is no enclosing `try`, the runtime prints the error message and calls `abort()` — the program terminates. This is the correct behavior for unrecoverable errors.

### Multiple except Clauses

```python
try:
    connect(host, port)
    send_data(payload)
except ConnectionError as e:
    print(f"connection failed: {e}")
    retry()
except TimeoutError as e:
    print(f"timed out: {e}")
except e:
    print(f"unexpected error: {e}")
```

Clauses are checked top-to-bottom. The first matching clause executes. The bare `except e:` is a catch-all.

**Note:** Exception type matching (e.g., `except ConnectionError`) is string-based — it checks whether the error message starts with the given type name. This is a lightweight convention, not a full exception hierarchy.

### finally

The `finally` block always runs, whether or not an exception was raised:

```python
def process_resource(path: str) -> void:
    mut handle = open_resource(path)
    try:
        do_work(handle)
    except e:
        print(f"work failed: {e}")
    finally:
        close_resource(handle)    # always runs — even if exception was raised
```

**How finally compiles:** The block is duplicated in C — once after the `catch` block (normal path) and once in the `catch` handler before re-raising. This is how `setjmp`/`longjmp`-based finally must be implemented.

### try/except Performance Notes

**Happy path:** A `setjmp` call — stores the CPU registers. On x86-64, this is roughly 20 instructions. On the happy path (no exception thrown), this is the only overhead.

**Exception thrown:** `longjmp` restores registers and jumps. This is fast but should not be used for flow control — only for truly exceptional conditions.

**Inlining:** Functions containing `try/except` are **never inlined** by the C backend. GCC/Clang cannot inline functions that use `setjmp`. Keep `try/except` at boundary functions.

---

## Result-Type: throws + ? + Result[T, E]

### Why Result Types

For deep call chains where every function can fail, `try/except` has two problems:
1. The `setjmp` overhead accumulates at every level
2. The control flow is implicit — you can't see from the call site whether a function can fail

The `throws` keyword solves both: explicit in the signature, zero overhead.

### Declaring a throws Function

```python
def parse_int(s: str) throws str -> int:
    if len(s) == 0:
        raise("empty input")
    mut i = 0
    mut result = 0
    while i < len(s):
        mut c: int = s[i] as int
        if c < 48 or c > 57:
            raise("not a digit at position " + str(i) + ": " + s)
        result = result * 10 + (c - 48)
        i = i + 1
    return result
```

The `throws ErrorType` annotation changes the return type to `Result[ReturnType, ErrorType]`. Inside a `throws` function:
- `return value` → emits `Result { is_err: false, data.value: value }`
- `raise(err)` → emits `Result { is_err: true, data.error: err }` and returns immediately

**Generated C type:**
```c
typedef struct {
    bool is_err;
    union {
        long long value;
        char*     error;
    } data;
} Result_i64_str;
```

A plain struct — no heap allocation, no exceptions, no overhead.

### The ? Propagation Operator

`?` after a throws call unwraps the success value or immediately propagates the error to the caller:

```python
def doubled(s: str) throws str -> int:
    mut n = parse_int(s)?      # if parse_int fails, return its error immediately
    return n * 2

def tripled(s: str) throws str -> int:
    mut n = doubled(s)?        # if doubled fails, return its error
    return n * 3
```

**How `?` compiles:**
```c
// mut n = parse_int(s)?
Result_i64_str _r1 = parse_int(s);
if (_r1.is_err) return _r1;        // propagate
long long n = _r1.data.value;      // unwrap success
```

A single conditional branch. On the happy path (no error), this is one branch prediction miss on the first call and correct prediction thereafter.

### Handling a Result Value

When you call a `throws` function without `?`, you get back a `Result[T, E]` value:

```python
mut r = parse_int("42")

# Check and use:
if r.is_ok:
    print(f"parsed: {r.ok}")      # the T value
if r.is_err:
    print(f"error: {r.err}")      # the E value

# Or access directly (unsafe if wrong):
mut val = r.ok                     # only valid if r.is_ok is true
```

| Property | Type | C field |
|----------|------|---------|
| `r.is_err` | `bool` | `r.is_err` |
| `r.is_ok` | `bool` | `!r.is_err` |
| `r.ok` | `T` | `r.data.value` |
| `r.err` | `E` | `r.data.error` |

### Explicit Result[T, E] Variables

```python
mut r1: Result[int, str] = tripled("7")
if r1.is_ok:
    print(f"tripled('7') = {r1.ok}")    # 21

mut r2: Result[int, str] = tripled("x")
if r2.is_err:
    print(f"error: {r2.err}")           # "not a digit at position 0: x"
```

---

## Compiler Rule T-4: Handle Throws Results

If a `throws` function is called as a statement (not assigned to a variable, not using `?`), the compiler warns:

```python
def dangerous() throws str -> int:
    return 42

dangerous()    # WARNING [T-4]: 'dangerous()' returns a Result — its error must be handled
               # FIX: use '? to propagate, assign to a variable, or '_ = dangerous()'
```

**Fix options:**
```python
dangerous()?                  # propagate with ?
mut r = dangerous()           # assign and inspect
_ = dangerous()               # explicitly discard (suppresses the warning)
```

---

## Choosing Between try/except and throws

### Use try/except when:
- You're at a **boundary** — the point where errors are caught and recovery happens
- You need `finally` for cleanup that must always run
- You want to catch errors from a **block of multiple operations**
- You're writing top-level error recovery (e.g., in `main()`)

```python
def main():
    try:
        mut cfg = load_config()      # may raise
        mut db  = connect_db(cfg)    # may raise
        run_server(db)               # may raise
    except e:
        print(f"startup failed: {e}")
```

### Use throws + ? when:
- You're building a **deep call chain** where every level can fail
- Performance on the error path matters
- You want the failure mode **visible in the function signature**
- You're building a library where callers need to know about failure

```python
# Deep chain — all use throws + ?
def parse_config(text: str) throws str -> Config: ...
def connect_from_config(cfg: Config) throws str -> Db: ...
def run_query(db: Db, sql: str) throws str -> List[Row]: ...

# Top-level — uses try/except for recovery
def main():
    try:
        mut cfg = parse_config(read_file("config.toml"))
        mut db  = connect_from_config(cfg)
        mut rows = run_query(db, "SELECT * FROM users")
        for row in rows: print_row(row)
    except e:
        print(f"error: {e}")
```

### Mixed Pattern

The two models interoperate freely. Call `throws` functions inside `try` blocks, and vice versa:

```python
def safe_parse(s: str) -> int:
    mut r: Result[int, str] = parse_int(s)   # call throws fn
    if r.is_ok: return r.ok
    return 0    # default on error

def resilient_run() -> void:
    try:
        mut n = safe_parse(user_input())
        print(f"parsed: {n}")
    except e:
        print(f"caught: {e}")
```

---

## Custom Error Types

Use a string (`throws str`) for simple errors. Use a class for richer errors:

```python
class ParseError:
    pub message:  str
    pub position: int
    pub context:  str

extend ParseError:
    pub def init(msg: str, pos: int, ctx: str) -> ParseError:
        mut e = ParseError()
        e.message  = msg
        e.position = pos
        e.context  = ctx
        return e

    pub def describe(self) -> void:
        print(f"ParseError at {self.position}: {self.message}")
        print(f"  context: {self.context}")

def parse_expr(s: str) throws ParseError -> int:
    if len(s) == 0:
        raise(ParseError.init("empty expression", 0, s))
    # ...
    return 0
```

---

## Error Propagation Across Module Boundaries

`throws` errors propagate naturally across module boundaries using `?`. The caller's function must also declare `throws` with a compatible error type:

```python
# In math.tr:
pub def safe_sqrt(x: float) throws str -> float:
    if x < 0.0:
        raise("cannot take sqrt of negative number")
    return 1.0   # simplified

# In main.tr:
from math import safe_sqrt

def compute(x: float) throws str -> float:
    mut root = safe_sqrt(x)?    # propagates the str error
    return root * 2.0
```

---

## Common Errors

### Calling throws without handling

```python
parse_int(s)    # ERROR [T-4]: unhandled Result — assign, use ?, or discard with _
```
**Fix:** `_ = parse_int(s)` or `mut r = parse_int(s)` or `parse_int(s)?`

### Using ? outside a throws function

```python
def not_throws(s: str) -> int:
    mut n = parse_int(s)?    # ERROR: ? can only be used in a throws function
    return n
```
**Fix:** Add `throws ErrorType` to the enclosing function, or handle the Result explicitly.

### Wrong error type in ?

```python
def f() throws str -> int: ...
def g() throws int -> float:
    mut n = f()?    # ERROR: f() throws str but g() throws int — incompatible
```
**Fix:** Match the error type, or convert: `mut r = f(); if r.is_err: raise(0)`

---

Next: [Memory & Ownership →](13_memory_and_ownership.md)
