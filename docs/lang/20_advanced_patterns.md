# 20 — Advanced Patterns

This chapter collects idioms, design patterns, performance techniques, and architectural guidance that don't fit neatly into any single language feature chapter. It assumes familiarity with everything in chapters 01–19.

---

## Idioms

### Early Return for Guard Clauses

Prefer early returns to eliminate deep nesting. This is the single highest-leverage style change in any Tauraro codebase.

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

---

### Named Exit Flags for Nested Loops

Tauraro `break` only exits the innermost loop. For nested loops, use a flag:

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

For more complex early-exit logic, extract the inner loop into a function that `return`s:

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

### Accumulator Pattern

Build results by accumulating into a local, then return once:

```python
def words_longer_than(words: List[str], n: int) -> List[str]:
    mut result: List[str] = []
    for word in words:
        if len(word) > n:
            result.append(word)
    return result
```

Compiles to a simple loop with no allocation per iteration.

---

### Default Value with Conditional Assignment

```python
def greeting(name: str) -> str:
    mut display = name
    if len(name) == 0:
        display = "stranger"
    return "Hello, " + display + "!"
```

Or inline:

```python
def greeting(name: str) -> str:
    mut display = name if len(name) > 0 else "stranger"
    return "Hello, " + display + "!"
```

---

### Swap Without Temp

```python
mut a = 10
mut b = 20
# Tuple swap:
a, b = b, a
```

The compiler lowers this to a temp variable in C — same performance as explicit temp, better readability.

---

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

Dict values are untyped. Store counts as strings (`str(n)`) or parse on read.

---

## Design Patterns

### Builder Pattern

Use method chaining with a mutable builder class to construct complex objects step-by-step:

```python
class HttpRequest:
    pub url:    str
    pub method: str
    pub body:   str
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

Each `with_*` method mutates `self` and returns `self`, enabling the chain. The compiler generates simple field-set calls — no overhead compared to setting fields individually.

---

### Strategy Pattern

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

**Limitation:** Strategy functions must be top-level `def`, not closures — closures carry captured state that the calling convention cannot pass transparently.

---

### RAII-Style Resource Cleanup

Tauraro doesn't have destructors, but you can pair `alloc`/`dealloc` with try/finally for guaranteed cleanup:

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
1. Acquire resource before `try:`
2. All cleanup in `finally:`
3. `finally:` runs on both success and exception paths

---

### Observer Pattern via Function Lists

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

**How it works:** `List[lambda]` stores closures. Each `handler(event)` is an indirect call through the stored function.

---

### Generic Pool Pattern

Pre-allocate a fixed-size pool and reuse slots instead of allocating per item:

```python
const POOL_SIZE = 256

class IntPool:
    pub slots:  List[int]
    pub used:   List[bool]
    pub count:  int

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

def main():
    mut pool = IntPool.init()
    mut a = pool.acquire()
    mut b = pool.acquire()
    pool.set(a, 42)
    pool.set(b, 99)
    print(f"slot {a} = {pool.get(a)}")
    print(f"slot {b} = {pool.get(b)}")
    pool.release(a)
    pool.release(b)
```

---

### State Machine Pattern

Encode states as an enum and transitions as a function:

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
                # ... actual connect logic ...
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
    print(conn.status())     # idle
    conn.connect()           # connecting → connected
    print(conn.status())     # connected
    conn.disconnect("timeout")
    print(conn.status())     # disconnected: timeout
```

---

### Memoization

Cache expensive results in a Dict keyed by input:

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

def main():
    mut i = 0
    while i <= 20:
        print(f"fib({i}) = {fib(i)}")
        i = i + 1
```

**Caveat:** Dict values are untyped. Store as `str` and parse on read, or use a typed List for integer keys:

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

---

## Performance Techniques

### Avoid Repeated len() in Loop Conditions

`len(list)` is O(1) — it reads the `size` field of `_TrList`. But calling it in every iteration condition is still a redundant field dereference:

```python
# Slightly less efficient:
mut i = 0
while i < len(items):    # dereferences items->size each iteration
    process(items[i])
    i = i + 1

# Better for hot loops:
mut n = len(items)
mut i = 0
while i < n:
    process(items[i])
    i = i + 1
```

In practice, GCC with `-O2` will hoist `len(items)` automatically if it can prove the loop doesn't change `items`. Cache it explicitly in tight inner loops.

---

### Prefer for-range Over While for Simple Loops

```python
# Clear intent, compiles to identical C:
for i in range(n):
    process(i)

# vs.
mut i = 0
while i < n:
    process(i)
    i = i + 1
```

The `for i in range(n)` form compiles to a direct loop — no overhead, maximum clarity.

---

### String Building: Don't Concatenate in a Loop

Each `+` on strings allocates a new string. Concatenating in a loop is O(n²):

```python
# BAD: O(n²) allocations
mut result = ""
for word in words:
    result = result + word + " "

# GOOD: collect then join
mut parts: List[str] = []
for word in words:
    parts.append(word)
# Then join in one pass... or use StringBuilder
```

For maximum efficiency, use `StringBuilder` from `core.string`:

```python
from core.string import StringBuilder

def join_words(words: List[str]) -> str:
    mut sb = StringBuilder.init()
    for word in words:
        sb.append(word)
        sb.append(" ")
    return sb.build()
```

---

### Avoid Redundant Copies with Class Fields

Class instances are heap-allocated. Passing a class to a function passes its pointer (a borrow), not a copy. This is already efficient:

```python
def expensive_read(data: MyClass) -> int:
    return data.value    # data is MyClass* in C — no copy
```

Returning a class from a function transfers ownership (pointer transfer), also no copy. The only way to deep-copy a class is to manually create a new instance and copy fields.

---

### Use @inline for Small Hot Functions

```python
@inline
def clamp(x: int, lo: int, hi: int) -> int:
    if x < lo: return lo
    if x > hi: return hi
    return x
```

`@inline` forces the compiler to inline the function at every call site. Use it for:
- Functions called in tight loops
- Functions with 1–5 expressions
- Math helpers that would otherwise incur call overhead

Don't use `@inline` on:
- Functions containing `try/except` (prevents inlining)
- Functions called from only one place (GCC inlines these automatically at `-O2`)
- Large functions (code bloat hurts I-cache)

---

### Bit Manipulation for Flags

Flags packed in an integer are faster than a List[bool] for fixed sets:

```python
const FLAG_READ    = 1
const FLAG_WRITE   = 2
const FLAG_EXEC    = 4
const FLAG_HIDDEN  = 8

def make_flags(r: bool, w: bool, x: bool) -> int:
    mut f = 0
    if r: f = f | FLAG_READ
    if w: f = f | FLAG_WRITE
    if x: f = f | FLAG_EXEC
    return f

def has_flag(flags: int, flag: int) -> bool:
    return (flags & flag) != 0

def main():
    mut perms = make_flags(true, true, false)
    print(has_flag(perms, FLAG_READ))    # true
    print(has_flag(perms, FLAG_EXEC))    # false
    perms = perms | FLAG_EXEC            # grant exec
    perms = perms & ~FLAG_WRITE          # revoke write
```

`~` is bitwise NOT. `perms & ~FLAG_WRITE` clears the write bit without touching other flags.

---

### Prefer Stack Allocation for Small Buffers

When you need a fixed-size buffer for a short lifetime, allocate on the heap via `alloc` once at the start of the containing function rather than repeatedly:

```python
def process_many(inputs: List[str]) -> void:
    unsafe:
        mut buf: Pointer[char] = alloc[char](256)    # one allocation
        for input in inputs:
            # use buf for each input — no per-iteration alloc
            mut n = len(input)
            if n > 255: n = 255
            mut i = 0
            while i < n:
                buf.offset(i).write(input[i] as char)
                i = i + 1
            buf.offset(n).write('\0')
            process_buf(buf, n)
        dealloc(buf)    # one deallocation
```

---

### Parallelism with gpu:

For independent element-wise work over large arrays, `gpu:` gives free parallelism on multi-core machines:

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

Rules (see chapter 18):
1. Each iteration must be fully independent
2. No loop-carried dependencies
3. Compile with `-fopenmp` to enable actual parallelism

---

## Architectural Patterns

### Module Organization for Growing Projects

**Rule:** One concept per module. Don't create large "utils" catch-all modules — they grow without bound and create hidden coupling.

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

Dependency direction: `main` → everything; `logic` → `data.model`; `output` → `data.model`; `data.repo` → `data.model`. No cycles.

---

### Error Propagation Strategy

**Rule:** Pick one error model per layer and convert at boundaries.

```
Layer: I/O  →  throws str       (C errno → Tauraro error)
Layer: Logic → throws str       (business rule violations)
Layer: Top  → try/except        (convert to user-facing message)
```

```python
# I/O layer: propagate errors up
def read_config(path: str) throws str -> Config:
    mut fd = open(path, O_RDONLY, 0)
    if fd < 0: raise("cannot read config: " + path)
    # ...
    return config

# Logic layer: validate, propagate
def apply_config(cfg: Config) throws str -> void:
    if cfg.port < 1 or cfg.port > 65535:
        raise("invalid port: " + str(cfg.port))
    # ...

# Top layer: handle and report
def main():
    try:
        mut cfg = read_config("config.tr")?
        apply_config(cfg)?
        run(cfg)
    except e:
        print("startup failed: " + e)
```

Don't mix: don't `raise` in a function that doesn't declare `throws`, don't ignore `Result` values (T-4).

---

### Interface Design: Keep Them Narrow

Wide interfaces (many methods) are hard to implement and test. Narrow interfaces (1–3 methods) compose better:

```python
# BAD: fat interface
interface Storage:
    def read(key: str) -> str
    def write(key: str, value: str) -> void
    def delete(key: str) -> void
    def list_keys() -> List[str]
    def clear() -> void
    def size() -> int

# GOOD: narrow interfaces
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

Instead of global state, pass dependencies explicitly:

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

Functions that accept lambdas are testable (pass a no-op lambda) and composable.

---

### Layered Validation

Validate at system boundaries (user input, files, network) not deep in logic:

```python
class ParsedArgs:
    pub host: str
    pub port: int
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
    # ... parsing logic ...
    mut args = ParsedArgs()
    args.host    = "localhost"
    args.port    = 8080
    args.workers = 4
    args.validate()?    # validate immediately after construction
    return args
```

After `validate()` succeeds, logic code can trust the values — no need to re-check downstream.

---

## Tauraro-Specific Idioms

### Using Hausa Keywords

All keywords have Hausa equivalents. These are fully interchangeable — mix as you like:

```python
# English
def greet(name: str) -> void:
    if len(name) > 0:
        print("Hello, " + name)
    else:
        print("Hello, stranger")

# Hausa
aiki gaisuwa(suna: str) -> void:
    idan tsawon(suna) > 0:
        buga("Sannu, " + suna)
    sai:
        buga("Sannu, bako")
```

Bilingual source files work too — you can use `def` and `aiki` in the same file.

**All equivalents:**

| English | Hausa |
|---------|-------|
| `def` | `aiki` |
| `class` | `aji` |
| `struct` | `tsari` |
| `if` | `idan` |
| `elif` | `koidan` |
| `else` | `sai` |
| `for` | `ga` |
| `while` | `yayinda` |
| `return` | `dawo` |
| `break` | `tsaya` |
| `continue` | `ci_gaba` |
| `print` | `buga` |
| `match` | `duba` |
| `case` | `hali` |
| `try` | `gwada` |
| `except` | `kama` |
| `finally` | `karshe` |
| `raise` | `jefa` |
| `true` | `gaskiya` |
| `false` | `karya` |
| `none` | `babu` |
| `and` | `da` |
| `or` | `ko` |
| `not` | `ba` |
| `len` | `tsawon` |
| `range` | `zango` |

---

### Match as Expression

`match` can be used as an expression by returning from each arm:

```python
def day_name(n: int) -> str:
    match n:
        case 0: return "Sunday"
        case 1: return "Monday"
        case 2: return "Tuesday"
        case 3: return "Wednesday"
        case 4: return "Thursday"
        case 5: return "Friday"
        case 6: return "Saturday"
        case _: return "unknown"
```

Since each `case` is a statement block, this compiles to a C `switch` with `return` in each case — zero overhead compared to a C `switch`.

---

### Enumerate with Index

When you need both the index and value from a list:

```python
def print_indexed(items: List[str]) -> void:
    for i, item in enumerate(items):
        print(f"  [{i}] {item}")

def main():
    print_indexed(["apple", "banana", "cherry"])
    # [0] apple
    # [1] banana
    # [2] cherry
```

`enumerate(items)` compiles to a loop with an auto-incremented index variable alongside the element dereference.

---

### Tuple Unpacking

Unpack multiple return values cleanly:

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

Tuple returns are unpacked by the compiler with zero overhead — returned by value, unpacked at the call site.

---

### Generic Containers

Generic classes with `[T]` give type-safe containers with a single implementation:

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
    int_stack.push(3)
    print(int_stack.pop())    # 3
    print(int_stack.peek())   # 2

    mut str_stack = Stack[str].init()
    str_stack.push("hello")
    str_stack.push("world")
    print(str_stack.pop())    # world
```

The compiler generates `Stack_int` and `Stack_str` as separate C structs — fully type-safe, no runtime overhead.

---

### The `_` Discard Pattern

Use `_` to explicitly discard values you don't need:

```python
# Discard a Result (explicitly — suppresses T-4)
_ = parse_int(s)

# Discard loop index when you only want the value
for _, item in enumerate(items):
    process(item)

# Discard part of a tuple
_, hi = min_max(values)    # only want the max
```

`_` tells both the compiler and readers that the value is intentionally unused.

---

## Common Anti-Patterns to Avoid

### Don't Store Everything in Dict

`Dict` has `str` keys and untyped values. It's convenient but hides type errors at compile time:

```python
# BAD: Dict masquerading as a typed struct
mut config: Dict = {}
config["port"]    = 8080       # stored as int
config["host"]    = "localhost" # stored as str
config["workers"] = "4"         # stored as str — inconsistent!

# GOOD: proper typed class
class Config:
    pub port:    int
    pub host:    str
    pub workers: int
```

Use `Dict` for genuinely dynamic key-value data (user-supplied keys, JSON-like structures). Use classes for fixed-shape data.

---

### Don't Use Global Mutable State for Configuration

```python
# BAD: mutable global
mut debug_mode = false
mut log_level  = 1

def process(data: str) -> str:
    if debug_mode: print(f"input: {data}")
    ...

# GOOD: pass config explicitly
class Options:
    pub debug: bool
    pub log_level: int

def process(data: str, opts: Options) -> str:
    if opts.debug: print(f"input: {data}")
    ...
```

Global mutable state makes code hard to test (can't isolate), creates implicit coupling, and causes data races in concurrent code.

---

### Don't Swallow Errors

```python
# BAD: silently ignores all exceptions
try:
    risky_operation()
except e:
    pass    # error swallowed — no one knows what went wrong

# GOOD: at minimum log it
try:
    risky_operation()
except e:
    print(f"warning: risky_operation failed: {e}")

# BEST: handle or propagate
def run() throws str -> void:
    risky_operation()?    # propagate to caller
```

---

### Don't Over-Inline

`@inline` on large functions causes code bloat that hurts CPU instruction cache:

```python
# BAD: inlining a large function called in many places
@inline
def complex_algorithm(data: List[int]) -> int:
    # 50 lines of logic...
    return result

# GOOD: only inline small, frequently-called helpers
@inline
def max(a: int, b: int) -> int:
    if a > b: return a
    return b
```

GCC at `-O2` already inlines small functions automatically. Use `@inline` only when profiling shows a specific call site is bottlenecked by call overhead.

---

### Don't Ignore Ownership for Shared Threads

```python
# BAD: shared mutable state without protection
mut counter: int = 0

def increment() -> void:
    counter = counter + 1    # DATA RACE if called from multiple threads

task_group:
    spawn increment()
    spawn increment()

# GOOD: use atomic or mutex
# Option 1: design so each thread owns its own counter
def count_range(start: int, end_val: int) -> int:
    mut local = 0
    mut i = start
    while i < end_val:
        local = local + 1
        i = i + 1
    return local
```

See chapter 16 for the full thread safety matrix.

---

## Putting It All Together: A Complete Example

This example uses: modules, classes, generics, error handling, pattern matching, interfaces, concurrency, and FFI.

```python
# task_runner.tr — a concurrent task processing system

from core.string import StringBuilder

class Task:
    pub id:       int
    pub priority: int
    pub payload:  str

extend Task:
    pub def init(id: int, priority: int, payload: str) -> Task:
        mut t = Task()
        t.id       = id
        t.priority = priority
        t.payload  = payload
        return t

interface Processor:
    def process(task: Task) -> str

class LogProcessor:
    pub prefix: str

extend LogProcessor:
    pub def init(prefix: str) -> LogProcessor:
        mut p = LogProcessor()
        p.prefix = prefix
        return p

    pub def process(self, task: Task) -> str:
        return self.prefix + f"[{task.id}] " + task.payload.upper()

class TaskQueue:
    pub tasks: List[Task]

extend TaskQueue:
    pub def init() -> TaskQueue:
        mut q = TaskQueue()
        q.tasks = []
        return q

    pub def add(self, task: Task) -> void:
        # Insert sorted by priority (high first)
        mut inserted = false
        mut i = 0
        while i < len(self.tasks) and not inserted:
            if task.priority > self.tasks[i].priority:
                # shift right... simplified: just append for this example
                inserted = true
            i = i + 1
        self.tasks.append(task)

    pub def run_all(self, proc: Processor) -> List[str]:
        mut results: List[str] = []
        for task in self.tasks:
            mut r = proc.process(task)
            results.append(r)
        return results

def main():
    mut queue = TaskQueue.init()
    queue.add(Task.init(1, 5, "hello"))
    queue.add(Task.init(2, 9, "world"))
    queue.add(Task.init(3, 1, "test"))

    mut proc = LogProcessor.init(">> ")
    mut results = queue.run_all(proc)

    print("Results:")
    for i, result in enumerate(results):
        print(f"  {i}: {result}")
```

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

The most important performance insight: **measure before optimizing**. Use `--emit c` to see what the compiler generates, compile with `-O2`, and profile before reaching for `@inline`, `gpu:`, or manual memory management. Most programs don't need any of it.

---

*End of Tauraro Language Documentation*

← [Compiler Error Reference](19_compiler_errors.md) | [README](README.md)
