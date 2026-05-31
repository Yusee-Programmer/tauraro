# Tauraro Compiler Safety Rules — Comprehensive Proposal

**Philosophy**: *"The compiler carries all the complexity burden. The developer writes zero safety annotations."*  
Tauraro is Rust-without-Rust-complexity + Python syntax. Every rule below is enforced automatically — no lifetimes, no borrow annotations, no `unsafe` guards outside raw pointer ops.

---

## Category 1 — Memory Safety Rules

### Rule M-1: Automatic Ownership Inference ✅ (Implemented)

The compiler infers `Own`, `Borrow`, `Move`, or `Shared` for every variable.
No user annotation is ever required.

**Safe:**
```python
buf = read_data()       # compiler infers Own
process(buf)            # buf moved into process — compiler tracks this
# buf is now invalid — accessing it is a compile error
```

---

### Rule M-2: No Use-After-Move ✅ (Implemented)

Once a value is moved (passed to a function, assigned out), accessing the original
binding is a **compile error** — not a runtime crash.

**Error example:**
```python
data = load_bytes()
send_over_network(data)   # data is moved here
print(data.len)           # ERROR: 'data' was moved into 'send_over_network'
                          # FIX: Clone before sending or keep the result
```

**Correct:**
```python
data = load_bytes()
result = process(data)    # data moved, result is new
print(result.len)         # OK — using result, not the moved data
```

---

### Rule M-3: No Double-Free ✅ (Implemented)

The compiler lowers exactly one `Free` per owned variable at the point its scope exits
or it is moved. The second owner never receives a `Free`.

**Safe — compiler auto-manages:**
```python
def compute() -> List[int]:
    result = [1, 2, 3]    # Own — compiler frees at end of scope if not returned
    return result          # ownership transferred to caller — no free here

data = compute()           # caller now owns data
# data freed when this scope exits — exactly once
```

---

### Rule M-4: No Dangling Pointer ✅ (Implemented)

A borrowed reference cannot outlive its source. Cross-scope borrows are tracked.

**Error example:**
```python
def get_pointer() -> Pointer[Counter]:
    counter = Counter(0)      # counter lives on the stack of this function
    return &counter           # ERROR: returning pointer to local variable
                              # counter will be freed when function returns
```

**Correct:**
```python
def get_pointer() -> Pointer[Counter]:
    counter = alloc[Counter](1)   # heap allocation — survives function return
    unsafe:
        counter.write(Counter(0))
    return counter                 # OK — heap pointer outlives function
```

---

### Rule M-5: No Aliased Mutation ✅ (Implemented)

While a borrow to `x` is active, `x` cannot be mutated. Prevents use-after-borrow bugs.

**Error example:**
```python
mut data = [1, 2, 3]
view = data              # view borrows data
data = [4, 5, 6]         # ERROR: cannot reassign 'data' while 'view' holds a borrow
                         # FIX: drop view first, or clone data before borrowing
```

---

### Rule M-6: Bounds Checking — Static + Runtime ✅ Runtime / 🔲 Static Elision (Phase 2)

All list/array indexing includes a runtime bounds check. Phase 2 adds static elision:
if bounds are provably safe at compile time, the check is removed at zero cost.

**Always safe:**
```python
items = [10, 20, 30]
print(items[4])           # RUNTIME ERROR: index 4 is out of bounds (len=3)
                          # Clear message: "Index 4 out of bounds: list has 3 elements"
```

---

### Rule M-7: None / Option Safety ✅ (Implemented — Phase 1)

`none` is only valid for `Option[T]` variables. Assigning `none` to a non-Optional
type is a **compile error**.

**Error example:**
```python
mut count: int = none     # ERROR: cannot assign 'none' to 'int'
                          # FIX: use 'mut count: Option[int] = none'
                          #   or give count a real initial value: 'mut count: int = 0'

# Accessing Optional without checking is an error:
name: Option[str] = none
print(name + "!")         # ERROR: 'name' is Option[str], not str
                          # FIX: match name: case Some(n): print(n + "!")
```

**Correct:**
```python
mut user: Option[str] = none    # OK — Optional type
user = Some("Alice")
match user:
    case Some(name): print("Hello " + name)
    case None: print("No user set")
```

---

### Rule M-8: Stack Overflow Prevention (Note on Recursion) 🔲 (Phase 2)

Recursive functions without provable termination trigger a compiler note.

```python
def factorial(n: int) -> int:     # NOTE: recursive function — ensure base case
    if n <= 1: return 1
    return n * factorial(n - 1)   # tail-call optimized in Phase 2
```

---

### Rule M-9: No Uninitialized Variables ✅ (Implemented)

A variable declared with a type but no value, or assigned `none` to a non-optional
scalar type, is a **compile error**.

**Error examples:**
```python
mut x: int            # ERROR (if parser allows valueless let)
                      # FIX: mut x: int = 0

def maybe_set(flag: bool) -> int:
    mut result: int
    if flag:
        result = 42
    return result     # ERROR: 'result' may be uninitialized on the 'false' path
                      # FIX: mut result: int = 0
```

---

### Rule M-10: Integer Overflow — Defined Behavior 🔲 (Phase 2)

In safe mode: integer overflow is a runtime trap. In `unsafe:`: wrapping arithmetic
is explicitly allowed.

```python
x: i32 = 2147483647
y = x + 1             # RUNTIME ERROR: integer overflow on i32
                      # Use i64, or: unsafe: y = x.wrapping_add(1)
```

---

## Category 2 — Concurrency & Async Safety Rules

### Rule C-1: Spawn Captures Are Moved ✅ (Implemented)

Variables captured by `spawn` are **moved** — the caller loses access after spawn.
Prevents data races at the point of thread creation.

**Error example:**
```python
data = load_dataset()
spawn process_data(data)
print(data.len)            # ERROR: 'data' was moved into spawned thread
                           # FIX: use 'shared data = load_dataset()' for shared access
```

**Correct:**
```python
shared data = load_dataset()   # shared ownership — atomic refcount
spawn process_data(data)       # data cloned into thread
print(data.len)                # OK — original still valid via shared ref
```

---

### Rule C-2: Detached Spawn Note ✅ (Implemented)

`spawn` outside a `task_group:` creates a detached (fire-and-forget) thread.
The compiler emits a diagnostic note so the developer is aware.

```python
spawn background_cleanup()     # NOTE: detached thread — fire-and-forget
                               # Return value is unreachable
                               # Use task_group: if you need to wait for completion
```

---

### Rule C-3: task_group Completion Guarantee ✅ (Implemented)

Every `task_group:` block joins all spawned threads before exiting.
Variables freed at scope exit cannot be accessed by workers.

```python
results = Vec[int].init(4)
task_group:
    spawn compute_part(1, results)  # results captured — must outlive task_group
    spawn compute_part(2, results)
# All workers done here — results is valid and complete
print(results.len)
```

---

### Rule C-4: Async/Await Context Enforcement ✅ (Implemented)

`await` is only valid inside `async def` functions. Using it in a synchronous context
is a **compile error**.

**Error example:**
```python
def fetch_data() -> str:       # NOT async
    result = await http_get()  # ERROR: 'await' used outside an async function
                               # FIX: declare 'async def fetch_data() -> str:'
```

**Correct:**
```python
async def fetch_data() -> str:
    result = await http_get()  # OK — inside async function
    return result
```

---

### Rule C-5: Channel Send Moves Value 🔲 (Phase 2)

Sending a value into a channel moves it. The sender can no longer access it.
This makes channel-based concurrency race-free by construction.

```python
ch = Channel[int].new()
msg = 42
ch.send(msg)         # msg moved into channel
print(msg)           # ERROR: 'msg' was sent into channel and is no longer accessible
```

---

## Category 3 — Type Safety Rules

### Rule T-1: No Implicit Numeric Coercion ✅ (Enforced at codegen)

`int` and `float` do not implicitly coerce. Explicit `as` cast required.

```python
ratio: float = 3 / 2        # OK: integer division
precise: float = 3.0 / 2.0  # OK: float division
wrong: float = get_int()     # ERROR: cannot implicitly convert int to float
fixed: float = get_int() as float  # OK: explicit cast
```

---

### Rule T-2: Narrowing Cast Safety 🔲 (Phase 2)

Widening casts (`i32 as i64`) are zero-cost. Narrowing casts (`i64 as i32`) include
a runtime check in safe mode. Pointer casts require `unsafe:`.

```python
big: int = 9999999999
small: i32 = big as i32     # RUNTIME CHECK: value must fit in i32
unsafe:
    raw = Pointer[int](0) as Pointer[char]  # OK in unsafe: only
```

---

### Rule T-3: Exhaustive Match ✅ (Partial — enforced where detectable)

`match` on an enum must cover all variants. Missing arms are a **compile error**.

**Error example:**
```python
status: Option[str] = get_user()
match status:
    case Some(name): print(name)
    # ERROR: match on 'Option[str]' is missing case: None
    # FIX: add 'case None: handle_missing()'
```

**Correct:**
```python
match status:
    case Some(name): print(name)
    case None: print("No user found")
    # OR use wildcard:
    case _: pass
```

---

### Rule T-4: Result Must Be Handled ✅ (Implemented)

A function that returns `Result[T, E]` (declared with `throws`) cannot have its
return value silently discarded. Discarded errors become hard-to-debug silent failures.

**Error example:**
```python
def save_file(path: str, data: str) throws IOError -> bool: ...

save_file("output.txt", content)   # ERROR: Result returned by 'save_file' must be handled
                                    # Ignoring errors silently is not allowed
                                    # FIX: result = save_file(...); match result: ...
                                    # OR:  _ = save_file(...)  to explicitly discard
```

**Correct:**
```python
result = save_file("output.txt", content)
match result:
    case Ok(success): print("Saved!")
    case Err(e): print("Save failed: " + str(e))

# Alternative: propagate with ?
def run() throws IOError:
    save_file("output.txt", content)?   # propagates error to caller
```

---

### Rule T-5: Integer Must Not Be Used as Boolean ✅ (Implemented)

Using a numeric variable directly as an `if` condition (C-style truthy check)
is a **compile error**. Be explicit about what you check.

**Error example:**
```python
count = get_count()
if count:                  # ERROR: 'count' is int — use 'if count != 0:' to be explicit
    process()
```

**Correct:**
```python
if count != 0:             # OK — explicit boolean comparison
    process()

if count > 0:              # OK — explicit
    process()
```

---

## Category 4 — Resource Safety Rules

### Rule R-1: File Handle Auto-Close 🔲 (Phase 2)

File handles, sockets, and OS resources are tracked by the ownership system.
When the owner goes out of scope, the compiler injects `close()`.

```python
def read_config() -> str:
    f = open("config.txt")   # f is Own — compiler injects f.close() at scope exit
    data = f.read_all()
    return data              # f.close() injected here — always runs, even on early return
```

---

### Rule R-2: Mutex Lock-Unlock Pairing 🔲 (Phase 2)

Manual `mutex.lock()` without a matching `mutex.unlock()` on all paths
produces a compiler warning. Auto-unlock at scope exit in Phase 2.

```python
mx.lock()
do_work()
return early()    # WARNING: mutex 'mx' may not be unlocked on this path
mx.unlock()
```

---

## Category 5 — Control Flow Safety Rules

### Rule F-1: No Implicit Fallthrough ✅ (Structural — match arms are independent)

`match` arms never fall through. Each arm is fully independent.
This eliminates the C `switch` fallthrough bug class.

```python
match status_code:
    case 200: print("OK")
    case 404: print("Not found")
    case _: print("Unknown")
# No accidental fallthrough — each arm is isolated
```

---

### Rule F-2: Unreachable Code Warning 🔲 (Phase 2)

Code after `return`, `break`, or `raise` in the same block is flagged.

```python
def classify(x: int) -> str:
    if x > 0:
        return "positive"
    return "non-positive"
    print("done")   # WARNING: unreachable code after return
```

---

### Rule F-3: Definite Return Analysis ✅ (Implemented)

Every non-`void` function must return a value on **all** control flow paths.
Missing returns on any path are a **compile error**.

**Error example:**
```python
def abs_val(x: int) -> int:
    if x >= 0:
        return x
    # ERROR: function 'abs_val' returns 'int' but is missing a return statement
    #        on the path where condition is false
    # FIX: add 'return -x' in the else branch or after the if
```

**Correct:**
```python
def abs_val(x: int) -> int:
    if x >= 0:
        return x
    return -x          # All paths covered

def sign(x: int) -> str:
    if x > 0:
        return "positive"
    elif x < 0:
        return "negative"
    else:
        return "zero"  # All branches return — OK
```

---

## Category 6 — Reserved Name Protection Rules

### Rule N-1: No Reserved Name as Variable/Function Name ✅ (Implemented)

Built-in constants, type names, and constructors cannot be used as variable,
function, or class names. This prevents subtle shadowing bugs that are nearly
impossible to debug.

**Error examples:**
```python
# Built-in constants — ERRORS:
mut true = "yes"          # ERROR: 'true' is a built-in constant and cannot be redefined
mut false = 0             # ERROR: 'false' is a built-in constant and cannot be redefined
none = "nothing"          # ERROR: 'none' is a built-in constant and cannot be redefined

# Built-in constructors — ERRORS:
Some = 42                 # ERROR: 'Some' is a built-in Option constructor
Ok = "done"               # ERROR: 'Ok' is a built-in Result constructor

# Shadowing built-in functions — WARNINGS:
print = "hello"           # WARNING: 'print' shadows the built-in print function
                          # This will break all print() calls after this point
len = 5                   # WARNING: 'len' shadows the built-in len() function
```

**Correct:**
```python
# Use descriptive names that don't clash with builtins:
is_valid = true           # OK
result_ok = Ok(42)        # OK — using Ok as constructor, not redefining it
output_fn = get_printer() # OK — doesn't shadow print
item_count = len(items)   # OK — calling len(), not redefining it
```

**As function names:**
```python
def print(msg: str):      # WARNING: 'print' shadows built-in print function
    ...                   # This makes the original print unreachable

def compute() -> bool:    # OK — not a built-in name
    ...
```

---

## Category 7 — unsafe: Quarantine Rules

### Rule U-1: Permitted Only Inside unsafe: ✅ (Enforced at codegen)

- Raw pointer dereference (`ptr.read()`, `ptr.write()`)
- Address-of operator (`&x`)
- Pointer arithmetic (`ptr.offset(n)`)
- Inline assembly (`asm(...)`)
- Calls to `extern "C":` functions with raw pointers
- Manual allocation/free (`alloc[T](n)`, `_tr_free(p)`)

```python
mut p: Pointer[int] = alloc[int](4)
unsafe:
    p.write(42)           # OK inside unsafe:
    val = p.read()        # OK inside unsafe:

p.write(99)              # ERROR: pointer write requires unsafe: block
```

---

### Rule U-2: Ownership Still Enforced in unsafe: ✅ (Enforced)

`unsafe:` does NOT disable ownership checking. You still cannot use moved
variables, double-free, or alias mutable data.

```python
data = Vec[int].init(4)
process(data)               # data moved
unsafe:
    data.push(1)            # ERROR: 'data' was moved — even in unsafe: block
```

---

### Rule U-3: Minimal unsafe: Scope (Note) 🔲 (Phase 2)

Large `unsafe:` blocks trigger a compiler note to keep unsafe surface area small.

```python
unsafe:
    # ... 50 lines of unsafe code ...
    # NOTE: unsafe block is large (>10 statements)
    # Consider extracting to a named unsafe function for better isolation
```

---

## Implementation Status Summary

| Rule | Category | Status |
|---|---|---|
| M-1: Ownership inference | Memory | ✅ Implemented |
| M-2: No use-after-move | Memory | ✅ Implemented |
| M-3: No double-free | Memory | ✅ Implemented |
| M-4: No dangling pointer | Memory | ✅ Implemented |
| M-5: No aliased mutation | Memory | ✅ Implemented |
| M-6: Bounds checking | Memory | ✅ Runtime / Phase 2 static |
| M-7: None/Option safety | Memory | ✅ Implemented (Phase 1) |
| M-8: Stack overflow note | Memory | 🔲 Phase 2 |
| M-9: No uninitialized vars | Memory | ✅ Implemented |
| M-10: Integer overflow | Memory | 🔲 Phase 2 |
| C-1: Spawn captures moved | Concurrency | ✅ Implemented |
| C-2: Detached spawn note | Concurrency | ✅ Implemented |
| C-3: task_group guarantees | Concurrency | ✅ Implemented |
| C-4: Async/await context | Concurrency | ✅ Implemented |
| C-5: Channel send moves | Concurrency | 🔲 Phase 2 |
| T-1: No implicit coercion | Type Safety | ✅ Enforced at codegen |
| T-2: Narrowing cast check | Type Safety | 🔲 Phase 2 |
| T-3: Exhaustive match | Type Safety | ✅ Partial |
| T-4: Result must be handled | Type Safety | ✅ Implemented |
| T-5: Integer-as-bool error | Type Safety | ✅ Implemented |
| R-1: File handle auto-close | Resources | 🔲 Phase 2 |
| R-2: Mutex lock-unlock | Resources | 🔲 Phase 2 |
| F-1: No match fallthrough | Control Flow | ✅ Structural |
| F-2: Unreachable code | Control Flow | 🔲 Phase 2 |
| F-3: Definite return | Control Flow | ✅ Implemented |
| N-1: Reserved name protection | Names | ✅ Implemented |
| U-1: unsafe: quarantine | Unsafe | ✅ Enforced at codegen |
| U-2: Ownership in unsafe: | Unsafe | ✅ Enforced |
| U-3: Minimal unsafe: scope | Unsafe | 🔲 Phase 2 |

---

## Error Message Design Principles

All Tauraro compiler error messages follow these rules:

1. **Name the variable exactly**: "variable 'counter'" not "a variable"
2. **Name where it went wrong**: "moved into 'process_data()'" not "moved"
3. **Say what to do**: Always include a "FIX:" suggestion
4. **No jargon**: Write in plain English, not compiler theory
5. **One problem per message**: Don't bundle multiple issues into one message

### Example error format:
```
ERROR [M-2] line 12: 'data' was moved into 'send_over_network()' on line 10
  and cannot be used after that point.
  FIX: If you need 'data' in both places, use 'shared data = load_dataset()'
       for shared ownership, or clone it before the call.
```

### Example warning format:
```
WARNING [N-1] line 5: 'print' shadows the built-in print() function.
  After this assignment, the original print() function is unreachable in this scope.
  FIX: Choose a different name — e.g. 'log', 'output', or 'my_print'.
```
