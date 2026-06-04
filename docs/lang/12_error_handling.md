# 12 — Error Handling

---

## Two Complementary Models

Tauraro provides two error handling styles that complement each other:

| Style | Mechanism | C implementation | Overhead |
|-------|-----------|-----------------|---------|
| **Exception-style** | `try / except / finally` + `raise` | `setjmp` / `longjmp` | Small per `try`, none on happy path |
| **Result-type** | `throws` + `?` + `Result[T, E]` | Struct return | Zero — plain C struct |

The models are orthogonal: you choose based on where you are in the call stack and what you need to communicate. They also interoperate freely — you can call a `throws` function from inside a `try` block.

---

## Exception-Style: try / except / finally

### When to Use

Use `try / except / finally` when:

- You are at a **boundary** — the point where errors are caught and a recovery decision is made.
- You need `finally` for cleanup that must always run regardless of success or failure.
- You want to catch errors from a **block of multiple operations** in one place.
- You are writing top-level recovery code (e.g., in `main()`).

### How It Works

**Basic try / except:**

```python
try:
    mut n = risky_parse(input)
    process(n)
except e:
    print(f"error: {e}")
```

If code inside `try:` calls `raise`, execution immediately jumps to the matching `except` clause. The variable `e` receives the error value passed to `raise`.

**Raising an exception:**

```python
def validate_age(age: int) -> void:
    if age < 0:
        raise("age cannot be negative")
    if age > 150:
        raise("age is unreasonably large: " + str(age))
```

`raise(msg)` takes a `str` and jumps to the nearest enclosing `try` block. If there is no enclosing `try`, the runtime prints the error and calls `abort()` — correct behavior for unrecoverable errors.

**Multiple except clauses:**

```python
try:
    connect(host, port)
    send_data(payload)
except ConnectionError as e:
    print(f"connection failed: {e}")
    retry()
except TimeoutError as e:
    print(f"timed out: {e}")
except Exception as e:
    print(f"unexpected error: {e}")
```

Clauses are checked top-to-bottom. The first matching clause executes. `except Exception as e:` is the catch-all base case.

**Note on type matching:** `except ConnectionError as e:` is a string-prefix convention — it matches errors whose message starts with the type name. It is lightweight, not a full exception hierarchy.

**finally — always runs:**

```python
def process_resource(path: str) -> void:
    mut handle = open_resource(path)
    try:
        do_work(handle)
    except e:
        print(f"work failed: {e}")
    finally:
        close_resource(handle)    # runs on both success and exception paths
```

**assert — contract checking:**

```python
assert len(input) > 0, "input must not be empty"
```

`assert` aborts with the message if the condition is false. Use it for invariants and programmer contracts, not user input validation.

### Common Mistakes

```python
# WRONG: Using try/except for expected, routine control flow
try:
    mut item = list[i]
except e:
    break    # use bounds checking instead — try/except is for exceptional conditions

# WRONG: Bare except with no variable — silently swallows the error
try:
    risky()
except:
    pass    # you never see what went wrong

# WRONG: Putting heavy computation inside try — setjmp stores all registers
try:
    big_computation()    # fine, but setjmp overhead is paid on every entry
    another_big_thing()
except e:
    ...
# BETTER: narrow the try block to just the risky call
```

### Best Practices

- Keep `try` blocks narrow — wrap only the operations that can actually raise.
- Always inspect `e` in the `except` clause; never silently swallow errors.
- Use `finally` for resource cleanup so cleanup runs even if an exception is raised mid-block.
- Reserve `try / except` for boundaries; use `throws + ?` for deep chains (see below).
- Functions containing `try/except` are **never inlined** by the C backend — `setjmp` prevents it. Keep `try/except` at boundary call sites, not deep in hot loops.

---

## Result-Type: throws + ? + Result[T, E]

### When to Use

Use `throws` + `?` when:

- You are building a **deep call chain** where every level can fail.
- Performance on the error path matters (zero overhead vs. `setjmp`).
- You want the failure mode to be **visible in the function signature**.
- You are writing a library where callers need to know a function can fail.

### How It Works

**Declaring a throws function:**

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
- `return value` wraps the value as a success result.
- `raise(err)` wraps the error and returns immediately.

`Result[T, E]` is a plain C struct — no heap allocation, no `setjmp`, zero overhead.

**The `?` propagation operator:**

```python
def doubled(s: str) throws str -> int:
    mut n = parse_int(s)?      # if parse_int fails, return its error immediately
    return n * 2

def tripled(s: str) throws str -> int:
    mut n = doubled(s)?        # if doubled fails, return its error
    return n * 3
```

On the happy path, `?` is a single conditional branch. On error, it propagates immediately without unwinding a stack.

**Handling a Result value:**

```python
mut r = parse_int("42")

if r.is_ok():
    print(f"parsed: {r.unwrap()}")

if r.is_err():
    print(f"error: {r.unwrap_or(0)}")
```

| Method | Meaning |
|--------|---------|
| `r.is_ok()` | Returns `true` if the result holds a success value |
| `r.is_err()` | Returns `true` if the result holds an error |
| `r.unwrap()` | Returns the success value — panics if error |
| `r.unwrap_or(default)` | Returns success value or `default` if error |

**Explicit `Result[T, E]` variables:**

```python
mut r1: Result[int, str] = tripled("7")
if r1.is_ok():
    print(f"tripled('7') = {r1.unwrap()}")    # 21

mut r2: Result[int, str] = tripled("x")
if r2.is_err():
    print(f"error: {r2.unwrap_or(0)}")
```

**Option[T] — nullable values:**

For values that may or may not exist (without an error reason), use `Option[T]`:

```python
def find_user(id: int) -> Option[User]:
    if id == 0:
        return None
    return Some(User.init(id, "alice"))

mut result = find_user(42)
if result.is_some():
    mut u = result.unwrap()
    print(u.name)

# Or with a default:
mut u2 = find_user(0).unwrap_or(User.init(-1, "guest"))
```

| Method | Meaning |
|--------|---------|
| `opt.is_some()` | Returns `true` if the option holds a value |
| `opt.is_none()` | Returns `true` if the option is `None` |
| `opt.unwrap()` | Returns the value — panics if `None` |
| `opt.unwrap_or(default)` | Returns value or `default` if `None` |

### Common Mistakes

```python
# WRONG: Calling a throws function and ignoring the result
parse_int(s)    # ERROR [T-4]: unhandled Result — assign, use ?, or discard with _

# WRONG: Using ? outside a throws function
def not_throws(s: str) -> int:
    mut n = parse_int(s)?    # ERROR: ? can only appear in a throws function

# WRONG: Mismatched error types with ?
def f() throws str -> int: ...
def g() throws int -> float:
    mut n = f()?    # ERROR: f() throws str but g() throws int — incompatible

# WRONG: Calling unwrap() without checking is_ok() first
mut r = parse_int("bad")
mut val = r.unwrap()    # panics at runtime — always check first
```

**Fix for T-4 (unhandled Result):**

```python
parse_int(s)?             # propagate with ?
mut r = parse_int(s)      # assign and inspect manually
_ = parse_int(s)          # explicitly discard (suppresses the warning)
```

### Best Practices

- Use `throws str` for simple error messages; use a custom class for structured errors with extra fields.
- Always annotate every function that can fail with `throws` — this makes failure visible in the call graph.
- Use `?` to propagate errors up the chain rather than writing `if r.is_err(): raise(r.unwrap_or(...))` manually.
- Match error types across `throws` boundaries — mismatched types require an explicit conversion.
- Never call `unwrap()` without a preceding `is_ok()` / `is_some()` check unless you have a strict invariant that the value is present.

---

## Custom Error Types

### When to Use

Use a custom error class when the caller needs structured information beyond a plain string — position, code, context.

### How It Works

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
    return 0
```

### Common Mistakes

```python
# WRONG: Using a class error with throws str — the types must match
def f() throws str -> int:
    raise(ParseError.init("oops", 0, ""))    # ERROR: throws str expects a str, not ParseError
```

### Best Practices

- Use `throws str` during prototyping; switch to a class when callers need to branch on error fields.
- Keep error classes small — message, code, context. Avoid embedding large objects.
- Implement a `describe()` method on every error class for consistent printing.

---

## Choosing Between try/except and throws

```
throws + ?          — deep chains, library code, performance-critical paths
try / except        — boundary recovery, multi-operation blocks, top-level main()
```

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

The two models interoperate freely:

```python
def safe_parse(s: str) -> int:
    mut r: Result[int, str] = parse_int(s)   # call a throws fn without ?
    if r.is_ok(): return r.unwrap()
    return 0    # default on error

def resilient_run() -> void:
    try:
        mut n = safe_parse(user_input())
        print(f"parsed: {n}")
    except e:
        print(f"caught: {e}")
```

---

## Error Propagation Across Module Boundaries

`throws` errors propagate naturally across module boundaries. The caller's function must also declare `throws` with a compatible error type:

```python
# In math.tr:
pub def safe_sqrt(x: float) throws str -> float:
    if x < 0.0:
        raise("cannot take sqrt of negative number")
    return 1.0

# In main.tr:
from math import safe_sqrt

def compute(x: float) throws str -> float:
    mut root = safe_sqrt(x)?    # propagates the str error
    return root * 2.0
```

---

## Compiler Rule T-4: Handle Throws Results

If a `throws` function is called as a statement (not assigned to a variable and not using `?`), the compiler emits a warning:

```python
def dangerous() throws str -> int:
    return 42

dangerous()    # WARNING [T-4]: 'dangerous()' returns a Result — its error must be handled
```

**Fix options:**
```python
dangerous()?             # propagate with ?
mut r = dangerous()      # assign and inspect
_ = dangerous()          # explicitly discard (suppresses the warning)
```

---

## Summary

| Feature | Keyword | Notes |
|---------|---------|-------|
| Exception block | `try` | `setjmp`-based |
| Catch exception | `except` | Multiple clauses allowed |
| Always-run cleanup | `finally` | Runs on success and error |
| Raise exception | `raise` | Jumps to nearest `try` |
| Contract check | `assert` | Aborts on false |
| Fallible function | `throws` | Changes return to `Result[T,E]` |
| Error propagation | `?` | Propagates or unwraps |
| Nullable value | `Option[T]` | `Some(v)` / `None` |
| Success-or-error | `Result[T, E]` | `Ok(v)` / `Err(e)` |

---

Next: [Memory & Ownership →](13_memory_and_ownership.md)
