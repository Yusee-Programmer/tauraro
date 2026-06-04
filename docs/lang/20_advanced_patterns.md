# 20 — Advanced Patterns

This chapter collects idioms, design patterns, performance techniques, and architectural guidance. It assumes familiarity with chapters 01–19.

---

## Idioms

### Early Return for Guard Clauses

**When to use:** Any function that has preconditions or validation at the top. Early returns flatten the nesting of your happy path.

**How it works:**
```python
# HARD TO READ: deeply nested happy path
def process(input: str) -> str:
    if len(input) > 0:
        if input != "skip":
            mut result = transform(input)
            if len(result) > 0:
                return result
    return ""

# EASY TO READ: guard clauses at the top
def process(input: str) -> str:
    if len(input) == 0: return ""
    if input == "skip":  return ""
    mut result = transform(input)
    if len(result) == 0: return ""
    return result
```

The generated C is identical — this is purely a readability pattern.

**Best Practices:** Put all precondition checks at the very top of the function, before any work begins. Each guard should be a single-line inline `if`.

---

### Accumulator Pattern

**When to use:** Building a collection from filtered or transformed input.

**How it works:**
```python
def words_longer_than(words: List[str], n: int) -> List[str]:
    mut result: List[str] = []
    for word in words:
        if len(word) > n:
            result.append(word)
    return result
```

Compiles to a simple loop with no allocation per iteration — the list grows only when an element is appended.

---

### Swap Without Temp

**When to use:** Swapping two variables.

**How it works:**
```python
mut a = 10
mut b = 20
a, b = b, a    # tuple swap — zero overhead
```

The compiler lowers this to a temp variable in C automatically.

---

### Named Exit Flags for Nested Loops

**When to use:** You need to break out of a nested loop based on a condition in the inner loop. Tauraro `break` only exits the innermost loop.

**How it works:**
```python
def find_pair(matrix: List[List[int]], target: int) -> int:
    mut found = false
    mut row = 0
    while row < len(matrix) and not found:
        mut col = 0
        while col < len(matrix[row]):
            if matrix[row][col] == target:
                found = true
                break
            col = col + 1
        row = row + 1
    return found as int
```

**Best Practices:** For more complex early-exit logic, extract the inner loop into a function that `return`s directly:

```python
def search_row(row: List[int], target: int) -> int:
    mut i = 0
    while i < len(row):
        if row[i] == target: return i
        i = i + 1
    return -1

def find_in_matrix(matrix: List[List[int]], target: int) -> bool:
    mut r = 0
    while r < len(matrix):
        if search_row(matrix[r], target) >= 0: return true
        r = r + 1
    return false
```

---

### The `_` Discard Pattern

**When to use:** Suppressing T-4 on purpose-discarded Results, or ignoring the index in `enumerate`.

**How it works:**
```python
_ = parse_int(s)                # explicitly discard a Result (suppresses T-4)

for _, item in enumerate(items):
    process(item)               # only care about the value

_, hi = min_max(values)         # only want the max
```

`_` tells both the compiler and readers that the value is intentionally unused.

---

## Design Patterns

### Builder Pattern

**When to use:** Constructing objects with many optional fields where not all need to be set every time.

**How it works:**

Each `with_*` method mutates `self` and returns `self`, enabling a call chain. The compiler generates simple field-set calls — identical performance to setting fields individually.

```python
class HttpRequest:
    pub url:     str
    pub method:  str
    pub body:    str
    pub timeout: int

extend HttpRequest:
    pub def init() -> HttpRequest:
        mut r = HttpRequest()
        r.url     = ""
        r.method  = "GET"
        r.body    = ""
        r.timeout = 30
        return r

    pub def with_url(self, url: str) -> HttpRequest:
        self.url = url
        return self

    pub def with_method(self, method: str) -> HttpRequest:
        self.method = method
        return self

    pub def with_body(self, body: str) -> HttpRequest:
        self.body = body
        return self

    pub def with_timeout(self, seconds: int) -> HttpRequest:
        self.timeout = seconds
        return self

def main():
    mut req = HttpRequest.init()
        .with_url("https://api.example.com/data")
        .with_method("POST")
        .with_body("{\"key\":\"value\"}")
        .with_timeout(60)
    print(f"POST {req.url} (timeout={req.timeout}s)")
```

**Common Mistakes:** Returning a copy instead of `self` — always return `self` (the same instance) so fields accumulate across the chain.

**Best Practices:** Provide a `build()` method that validates required fields before the object is used.

---

### Strategy Pattern

**When to use:** You need to swap an algorithm at call time — sorting order, comparison logic, filtering criteria.

**How it works:**

Pass behavior as a function pointer (lambda) to swap algorithms at runtime:

```python
def sort_ascending(a: int, b: int) -> int:
    if a < b: return -1
    if a > b: return 1
    return 0

def sort_descending(a: int, b: int) -> int:
    if a > b: return -1
    if a < b: return 1
    return 0

def insertion_sort(data: List[int], compare: lambda) -> void:
    mut i = 1
    while i < len(data):
        mut key = data[i]
        mut j = i - 1
        while j >= 0 and compare(data[j], key) > 0:
            data[j + 1] = data[j]
            j = j - 1
        data[j + 1] = key
        i = i + 1

def main():
    mut nums = [5, 2, 8, 1, 9, 3]
    insertion_sort(nums, sort_ascending)
    for n in nums: print(n)    # 1 2 3 5 8 9

    insertion_sort(nums, sort_descending)
    for n in nums: print(n)    # 9 8 5 3 2 1
```

**Common Mistakes:** Using closures as strategies — strategy functions must be top-level `def`. Closures carry captured state that the calling convention cannot pass transparently.

---

### RAII-Style Resource Cleanup

**When to use:** Any resource that must be released regardless of whether the code succeeds or throws — file handles, sockets, manual allocations.

**How it works:**

Pair `alloc`/`dealloc` with `try/finally` for guaranteed cleanup:

```python
def process_file(path: str) -> str:
    mut fd = open(path, O_RDONLY, 0)
    if fd < 0: raise("cannot open: " + path)

    unsafe:
        mut buf: Pointer[char] = alloc[char](4096)
        try:
            mut n = read(fd, buf as Pointer[void], 4095 as usize)
            buf.offset(n).write('\0')
            close(fd)
            return buf as str
        finally:
            dealloc(buf)    # always freed — even on exception
```

Pattern rules:
1. Acquire the resource before `try:`
2. All cleanup goes in `finally:`
3. `finally:` runs on both success and exception paths

**Best Practices:** For classes that hold resources, implement `__enter__` and `__exit__` so callers can use the `with` statement. See chapter 21.

---

### Observer Pattern via Function Lists

**When to use:** You need to notify multiple listeners when an event occurs, without the source knowing who is listening.

**How it works:**

```python
class EventBus:
    pub handlers: List[lambda]

extend EventBus:
    pub def init() -> EventBus:
        mut b = EventBus()
        b.handlers = []
        return b

    pub def subscribe(self, handler: lambda) -> void:
        self.handlers.append(handler)

    pub def emit(self, event: str) -> void:
        for handler in self.handlers:
            handler(event)

def on_login(event: str) -> void:
    print(f"login handler: {event}")

def on_audit(event: str) -> void:
    print(f"audit log: {event}")

def main():
    mut bus = EventBus.init()
    bus.subscribe(on_login)
    bus.subscribe(on_audit)
    bus.emit("user:logged_in")
```

`List[lambda]` stores function references. Each `handler(event)` is an indirect call through the stored function.

---

### State Machine Pattern

**When to use:** Objects that have a defined lifecycle with discrete stages and rules about which transitions are valid.

**How it works:**

Encode states as an enum and handle transitions in methods:

```python
enum ConnState:
    Idle
    Connecting
    Connected
    Disconnected(str)    # str = reason

class Connection:
    pub state: ConnState
    pub host:  str

extend Connection:
    pub def init(host: str) -> Connection:
        mut c = Connection()
        c.state = ConnState.Idle
        c.host  = host
        return c

    pub def connect(self) -> void:
        match self.state:
            case ConnState.Idle:
                self.state = ConnState.Connecting
                print(f"connecting to {self.host}")
                self.state = ConnState.Connected
                print("connected")
            case ConnState.Connected:
                print("already connected")
            case _:
                print("cannot connect in current state")

    pub def disconnect(self, reason: str) -> void:
        match self.state:
            case ConnState.Connected:
                self.state = ConnState.Disconnected(reason)
                print(f"disconnected: {reason}")
            case _:
                print("not connected")

    pub def status(self) -> str:
        match self.state:
            case ConnState.Idle:         return "idle"
            case ConnState.Connecting:   return "connecting"
            case ConnState.Connected:    return "connected"
            case ConnState.Disconnected(reason): return "disconnected: " + reason

def main():
    mut conn = Connection.init("example.com")
    print(conn.status())       # idle
    conn.connect()             # connecting → connected
    print(conn.status())       # connected
    conn.disconnect("timeout")
    print(conn.status())       # disconnected: timeout
```

**Best Practices:** Use the type-state pattern (generics) when you want invalid state transitions to be a compile error rather than a runtime check. See [Advanced — Ownership](advanced/02_advanced_ownership.md).

---

### Memoization

**When to use:** Pure functions with expensive repeated calls, especially recursive functions like Fibonacci.

**How it works:**

```python
mut _fib_cache: Dict = {}

def fib(n: int) -> int:
    if n <= 1: return n
    mut key = str(n)
    if key in _fib_cache:
        return int(str(_fib_cache[key]))
    mut result = fib(n - 1) + fib(n - 2)
    _fib_cache[key] = str(result)
    return result
```

For integer-keyed memos, a typed `List[int]` is faster than a `Dict`:

```python
mut _memo: List[int] = []
mut _computed: List[bool] = []

def init_memo(n: int) -> void:
    mut i = 0
    while i <= n:
        _memo.append(-1)
        _computed.append(false)
        i = i + 1

def fib_fast(n: int) -> int:
    if n <= 1: return n
    if _computed[n]: return _memo[n]
    mut result = fib_fast(n - 1) + fib_fast(n - 2)
    _memo[n] = result
    _computed[n] = true
    return result
```

**Common Mistakes:** Storing typed values in a `Dict` (which holds untyped values) and forgetting to `str()`/`int()` convert on read and write.

---

### Generic Containers

**When to use:** Building reusable data structures that work with multiple types at zero runtime overhead.

**How it works:**

```python
class Stack[T]:
    pub items: List[T]
    pub size:  int

extend Stack[T]:
    pub def init() -> Stack[T]:
        mut s = Stack[T]()
        s.items = []
        s.size  = 0
        return s

    pub def push(self, val: T) -> void:
        self.items.append(val)
        self.size = self.size + 1

    pub def pop(self) -> T:
        if self.size == 0: raise("pop from empty stack")
        mut val = self.items[self.size - 1]
        self.size = self.size - 1
        return val

    pub def peek(self) -> T:
        if self.size == 0: raise("peek at empty stack")
        return self.items[self.size - 1]

    pub def is_empty(self) -> bool:
        return self.size == 0

def main():
    mut int_stack = Stack[int].init()
    int_stack.push(1)
    int_stack.push(2)
    print(int_stack.pop())    # 2

    mut str_stack = Stack[str].init()
    str_stack.push("hello")
    print(str_stack.pop())    # hello
```

The compiler generates `Stack_int` and `Stack_str` as separate C structs — fully type-safe, no runtime overhead.

---

## Performance Techniques

### When to optimize — summary

Most programs don't need manual optimization. Profile first. Apply these techniques only when a specific bottleneck has been identified.

| Technique | When to reach for it |
|-----------|----------------------|
| Cache `len()` before loop | Very tight inner loops |
| `for i in range(n)` | Any simple counter loop |
| `StringBuilder` | String assembly inside loops |
| `@inline` | Small helpers called in measured hot loops |
| Pool pattern | High-frequency allocation of fixed-size objects |
| `gpu:` | Independent element-wise work over large arrays |
| Bit flags | Fixed-set boolean properties on hot objects |

---

### Avoid Repeated len() in Loop Conditions

`len(list)` is O(1) — it reads the `size` field. But GCC `-O2` may not always hoist it. Cache explicitly in tight inner loops:

```python
# Better for hot loops:
mut n = len(items)
mut i = 0
while i < n:
    process(items[i])
    i = i + 1
```

---

### String Building: Don't Concatenate in a Loop

Each `+` on strings allocates a new string. Concatenating in a loop is O(n²):

```python
# BAD: O(n²) allocations
mut result = ""
for word in words:
    result = result + word + " "

# GOOD: use StringBuilder
from core.string import StringBuilder

def join_words(words: List[str]) -> str:
    mut sb = StringBuilder.init()
    for word in words:
        sb.append(word)
        sb.append(" ")
    return sb.build()
```

---

### Use @inline for Small Hot Functions

**When to use:** Functions called in tight, measured loops with 1–5 expressions.

```python
@inline
def clamp(x: int, lo: int, hi: int) -> int:
    if x < lo: return lo
    if x > hi: return hi
    return x
```

**Common Mistakes:**
- `@inline` on large functions — causes code bloat that hurts CPU instruction cache
- `@inline` on functions containing `try/except` — prevents inlining
- `@inline` on functions called from one place — GCC already inlines these at `-O2`

---

### Bit Manipulation for Flags

**When to use:** Fixed-set boolean properties on hot objects. Faster than `List[bool]` for small sets.

```python
const FLAG_READ  = 1
const FLAG_WRITE = 2
const FLAG_EXEC  = 4

def has_flag(flags: int, flag: int) -> bool:
    return (flags & flag) != 0

def main():
    mut perms = FLAG_READ | FLAG_WRITE
    print(has_flag(perms, FLAG_READ))     # true
    print(has_flag(perms, FLAG_EXEC))     # false
    perms = perms | FLAG_EXEC             # grant exec
    perms = perms & ~FLAG_WRITE           # revoke write
```

`~` is bitwise NOT. `perms & ~FLAG_WRITE` clears the write bit without touching others.

---

### Generic Pool Pattern

**When to use:** High-frequency allocation of a fixed-size set of objects where heap churn is measurably hurting performance.

```python
const POOL_SIZE = 256

class IntPool:
    pub slots: List[int]
    pub used:  List[bool]
    pub count: int

extend IntPool:
    pub def init() -> IntPool:
        mut p = IntPool()
        p.slots = []
        p.used  = []
        p.count = 0
        mut i = 0
        while i < POOL_SIZE:
            p.slots.append(0)
            p.used.append(false)
            i = i + 1
        return p

    pub def acquire(self) -> int:
        mut i = 0
        while i < POOL_SIZE:
            if not self.used[i]:
                self.used[i] = true
                self.count = self.count + 1
                return i
            i = i + 1
        raise("pool exhausted")

    pub def release(self, slot: int) -> void:
        self.used[slot] = false
        self.count = self.count - 1

    pub def set(self, slot: int, value: int) -> void:
        self.slots[slot] = value

    pub def get(self, slot: int) -> int:
        return self.slots[slot]
```

---

### Parallelism with gpu:

**When to use:** Independent element-wise work over large arrays. Each iteration must be fully independent with no loop-carried dependencies.

```python
def normalize(values: List[float], scale: float) -> List[float]:
    mut n = len(values)
    mut result: List[float] = []
    mut i = 0
    while i < n:
        result.append(0.0)
        i = i + 1

    gpu:
        for i in range(n):
            result[i] = values[i] / scale

    return result
```

Compile with `-fopenmp` to enable actual parallelism. See chapter 18 for full `gpu:` rules.

---

## Architectural Patterns

### Module Organization for Growing Projects

**Rule:** One concept per module. Avoid "utils" catch-all modules.

```
project/
  main.tr              # entry point only — wires modules together
  config.tr            # Config class, parse_args, defaults
  data/
    model.tr           # data types shared across layers
    repo.tr            # data access (read/write Model instances)
  logic/
    processor.tr       # business logic — imports data.model, not data.repo
    validator.tr       # validation — imports data.model only
  output/
    formatter.tr       # renders data to strings
    writer.tr          # writes to file/stdout
```

Dependency direction: `main` → everything; `logic` → `data.model`; `output` → `data.model`. No cycles.

---

### Error Propagation Strategy

**Rule:** Pick one error model per layer and convert at boundaries.

```
Layer: I/O    →  throws str       (C errno → Tauraro error)
Layer: Logic  →  throws str       (business rule violations)
Layer: Top    →  try/except       (convert to user-facing message)
```

```python
# I/O layer: propagate errors up
def read_config(path: str) throws str -> Config:
    mut fd = open(path, O_RDONLY, 0)
    if fd < 0: raise("cannot read config: " + path)
    return config

# Logic layer: validate, propagate
def apply_config(cfg: Config) throws str -> void:
    if cfg.port < 1 or cfg.port > 65535:
        raise("invalid port: " + str(cfg.port))

# Top layer: handle and report
def main():
    try:
        mut cfg = read_config("config.tr")?
        apply_config(cfg)?
        run(cfg)
    except e:
        print("startup failed: " + e)
```

**Best Practices:** Don't `raise` in a function that doesn't declare `throws`. Don't ignore `Result` values (T-4).

---

### Interface Design: Keep Them Narrow

**When to use:** Anywhere you need polymorphism.

**How it works:**

Wide interfaces are hard to implement and test. Narrow interfaces compose:

```python
# BAD: fat interface
interface Storage:
    def read(key: str) -> str
    def write(key: str, value: str) -> void
    def delete(key: str) -> void
    def list_keys() -> List[str]
    def clear() -> void
    def size() -> int

# GOOD: narrow interfaces that compose
interface Reader:
    def read(key: str) -> str

interface Writer:
    def write(key: str, value: str) -> void

# A class can implement both
class MemStore:
    pub data: Dict

extend MemStore:
    pub def read(self, key: str) -> str:
        if key in self.data: return str(self.data[key])
        return ""
    pub def write(self, key: str, value: str) -> void:
        self.data[key] = value
```

---

### Dependency Injection via Function Parameters

**When to use:** Any code that needs to be testable or swappable without global state.

**How it works:**

```python
# BAD: hidden global state
mut _log_level = 1

def process(data: str) -> str:
    if _log_level > 0: print(f"processing: {data}")
    return transform(data)

# GOOD: explicit dependency
def process(data: str, log: lambda) -> str:
    log(f"processing: {data}")
    return transform(data)

def silent_log(msg: str) -> void: pass
def verbose_log(msg: str) -> void: print(msg)

def main():
    mut result = process("hello", verbose_log)
    mut result2 = process("hello", silent_log)
```

---

### Layered Validation

**When to use:** Systems that accept external input (user args, files, network). Validate at the boundary, trust inside.

```python
class ParsedArgs:
    pub host:    str
    pub port:    int
    pub workers: int

extend ParsedArgs:
    pub def validate(self) throws str -> void:
        if len(self.host) == 0:
            raise("host is required")
        if self.port < 1 or self.port > 65535:
            raise("port must be 1-65535, got " + str(self.port))
        if self.workers < 1 or self.workers > 64:
            raise("workers must be 1-64, got " + str(self.workers))

def parse_args(argv: List[str]) throws str -> ParsedArgs:
    mut args = ParsedArgs()
    args.host    = "localhost"
    args.port    = 8080
    args.workers = 4
    args.validate()?    # validate immediately after construction
    return args
```

After `validate()` succeeds, downstream code can trust the values — no re-checking needed.

---

## Tauraro-Specific Idioms

### Counter with Dict

```python
def count_words(text: str) -> Dict:
    mut counts: Dict = {}
    mut words = text.split(" ")
    for word in words:
        if word in counts:
            counts[word] = int(str(counts[word])) + 1
        else:
            counts[word] = 1
    return counts
```

Dict values are untyped. Store counts as strings (`str(n)`) or parse on read (`int(str(counts[key]))`).

---

### Enumerate with Index

```python
def print_indexed(items: List[str]) -> void:
    for i, item in enumerate(items):
        print(f"  [{i}] {item}")
```

`enumerate(items)` compiles to a loop with an auto-incremented index alongside the element dereference — no allocation.

---

### Tuple Unpacking

```python
def min_max(values: List[int]) -> (int, int):
    mut lo = values[0]
    mut hi = values[0]
    for v in values:
        if v < lo: lo = v
        if v > hi: hi = v
    return (lo, hi)

def main():
    mut lo, hi = min_max([3, 1, 4, 1, 5, 9, 2, 6])
    print(f"min={lo} max={hi}")    # min=1 max=9
```

Tuple returns are unpacked at the call site with zero overhead.

---

## Common Anti-Patterns to Avoid

### Don't Store Everything in Dict

`Dict` has `str` keys and untyped values — it hides type errors at compile time:

```python
# BAD: Dict masquerading as a typed struct
mut config: Dict = {}
config["port"]    = 8080
config["host"]    = "localhost"
config["workers"] = "4"    # inconsistent type!

# GOOD: proper typed class
class Config:
    pub port:    int
    pub host:    str
    pub workers: int
```

Use `Dict` for genuinely dynamic key-value data. Use classes for fixed-shape data.

---

### Don't Use Global Mutable State for Configuration

```python
# BAD: mutable global
mut debug_mode = false

def process(data: str) -> str:
    if debug_mode: print(f"input: {data}")
    ...

# GOOD: pass config explicitly
class Options:
    pub debug: bool

def process(data: str, opts: Options) -> str:
    if opts.debug: print(f"input: {data}")
    ...
```

Global mutable state makes code untestable, creates hidden coupling, and causes data races in concurrent code.

---

### Don't Swallow Errors

```python
# BAD: silently ignores all exceptions
try:
    risky_operation()
except e:
    pass    # no one knows what went wrong

# GOOD: at minimum log it
try:
    risky_operation()
except e:
    print(f"warning: risky_operation failed: {e}")

# BEST: handle or propagate
def run() throws str -> void:
    risky_operation()?
```

---

### Don't Ignore Ownership for Shared Threads

```python
# BAD: data race — shared mutable without protection
mut counter: int = 0

def increment() -> void:
    counter = counter + 1    # DATA RACE

task_group:
    spawn increment()
    spawn increment()

# GOOD: design so each thread owns its own data
def count_range(start: int, end_val: int) -> int:
    mut local = 0
    mut i = start
    while i < end_val:
        local = local + 1
        i = i + 1
    return local
```

See chapter 16 for the full thread safety model and [Advanced — Sendable](advanced/06_sendable.md) for compile-time thread safety enforcement.

---

## Summary

| Pattern | When to Use |
|---------|------------|
| Early return guards | Any function with preconditions |
| Builder | Constructing objects with many optional fields |
| Strategy (lambda) | Swappable algorithms at call sites |
| RAII via try/finally | Any resource that must be cleaned up |
| State machine (enum) | Objects with lifecycle stages |
| Memoization | Pure functions with expensive repeated calls |
| Narrow interfaces | Polymorphism with minimal coupling |
| Dependency injection | Any code that needs to be testable or swappable |
| Pool | High-frequency allocation of fixed-size objects |
| @inline + gpu: | Hot inner loops that are provably parallel |

The most important performance insight: **measure before optimizing**. Use `--emit c` to see what the compiler generates, compile with `-O2`, and profile before reaching for `@inline`, `gpu:`, or manual memory management.

---

Next: [Operator Overloading →](21_operator_overloading.md)

← [Compiler Error Reference](19_compiler_errors.md) | [README](README.md)
