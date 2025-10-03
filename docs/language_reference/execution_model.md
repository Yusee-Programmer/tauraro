# Execution Model

This chapter describes how Tauraro programs are executed.

## Structure of a Program

A Tauraro program is constructed from code blocks. A block is a piece of Python code which is executed as a unit. The following are blocks:

- A module
- A function body
- A class definition
- An interactive command (typed at the REPL)
- Each invocation of a generator's body

A block typically contains other blocks, forming a hierarchy.

## Naming and Binding

### Binding of Names

The following constructs bind names:

- Formal parameters to functions
- Target in assignment statements
- Target in for loops
- Target in with statements
- Target in except clauses
- Names in import statements

The `del` statement can unbind names.

### Resolution of Names

Name resolution follows the LEGB rule:

1. **Local** - Names assigned in the current function
2. **Enclosing** - Names in the local scope of enclosing functions
3. **Global** - Names assigned at the top-level of a module
4. **Built-in** - Names preassigned in the built-in names module

### The `global` Statement

The `global` statement allows you to assign to global names:

```tauraro
count = 0

def increment():
    global count
    count += 1
```

### The `nonlocal` Statement

The `nonlocal` statement allows you to assign to names in an enclosing (but non-global) scope:

```tauraro
def outer():
    x = 1
    
    def inner():
        nonlocal x
        x = 2
    
    inner()
    print(x)  # Prints 2
```

## Exceptions

Exceptions are raised using the `raise` statement and handled using `try` statements.

### Exception Handling

```tauraro
try:
    # Code that might raise an exception
    result = 10 / 0
except ZeroDivisionError:
    # Handle specific exception
    print("Cannot divide by zero")
except (ValueError, TypeError) as e:
    # Handle multiple exceptions
    print(f"Value or type error: {e}")
except Exception as e:
    # Handle any other exception
    print(f"Unexpected error: {e}")
else:
    # Executes if no exception occurred
    print("No exceptions occurred")
finally:
    # Always executes
    print("Cleanup code")
```

### Built-in Exceptions

The built-in exceptions are organized in a hierarchy:

- `BaseException`
  - `SystemExit`
  - `KeyboardInterrupt`
  - `GeneratorExit`
  - `Exception`
    - `StopIteration`
    - `ArithmeticError`
      - `ZeroDivisionError`
    - `LookupError`
      - `IndexError`
      - `KeyError`
    - `ValueError`
    - `TypeError`
    - `NameError`
    - `AttributeError`
    - `ImportError`
    - `IOError`
    - And many more...

## Execution Frames

Each execution context has an associated frame. A frame contains:

- The code object being executed
- Local variables
- Global variables
- Built-in variables
- The previous frame (for nested calls)

## The Evaluation Stack

Tauraro's virtual machine uses a stack-based evaluation model. Operations are performed by pushing values onto the stack and then applying operations that pop values from the stack and push results back.

## Data Model Integration

The execution model interacts with the data model through:

1. **Name binding** - How names are associated with objects
2. **Attribute access** - How `obj.attr` is resolved
3. **Method calls** - How `obj.method()` works
4. **Function calls** - How functions are invoked with arguments

## Import System

### The Import Process

When an `import` statement is executed, the following steps occur:

1. Search for the module in `sys.modules` (module cache)
2. If not found, search for the module using the module search path
3. If found, create a module object
4. Execute the module's code
5. Add the module to `sys.modules`
6. Bind the module name in the local namespace

### Module Search Path

The module search path is determined by:

1. Built-in modules (handled specially)
2. Directories in `sys.path` in order:
   - `tauraro_packages/`
   - `tauraro_packages/externals/`
   - `tauraro_packages/pysites/`
   - Current directory
   - Platform-specific paths
   - Paths from `TAURARO_PATH` environment variable

### Packages

A package is a directory containing an `__init__.tr` file. When a package is imported, the `__init__.tr` file is executed.

```tauraro
# Directory structure:
# mypackage/
#   __init__.tr
#   module1.tr
#   module2.tr

# Importing the package
import mypackage

# Importing from the package
from mypackage import module1
from mypackage.module1 import some_function
```

## Concurrency Model

Tauraro supports both threading and asynchronous programming.

### Threading

Tauraro threads are implemented using the operating system's threading facilities:

```tauraro
import threading

def worker(data):
    # Thread function
    print(f"Processing {data}")

# Create and start a thread
thread = threading.Thread(target=worker, args=("data",))
thread.start()
thread.join()  # Wait for completion
```

### Async/Await

Tauraro supports asynchronous programming with coroutines:

```tauraro
import asyncio

async def fetch_data(url):
    # Simulate async operation
    await asyncio.sleep(1)
    return f"Data from {url}"

async def main():
    # Run multiple coroutines concurrently
    tasks = [
        fetch_data("url1"),
        fetch_data("url2"),
        fetch_data("url3")
    ]
    results = await asyncio.gather(*tasks)
    print(results)

# Run the async program
asyncio.run(main())
```

## Memory Management

Tauraro uses automatic memory management with reference counting and garbage collection for cyclic references.

### Reference Counting

Each object maintains a count of references to it. When the count reaches zero, the object is deallocated.

### Garbage Collection

The garbage collector handles cyclic references that reference counting cannot clean up:

```tauraro
import gc

# Manually trigger garbage collection
gc.collect()

# Get statistics
stats = gc.get_stats()
print(stats)
```

## Foreign Function Interface

Tauraro can interface with C libraries through its FFI:

```tauraro
# Import C functions
extern "libm.so" {
    fn sqrt(x: double) -> double
    fn sin(x: double) -> double
}

# Use C functions
result = sqrt(16.0)
print(result)  # 4.0
```

## Compilation Backends

Tauraro supports multiple compilation backends:

### VM Backend (Default)

Interpreted execution using the built-in virtual machine.

### LLVM Backend

Compilation to optimized native code using LLVM:

```bash
tauraro compile --backend llvm program.tr -o program
```

### C Backend

Compilation to C code:

```bash
tauraro compile --backend c program.tr -o program.c
```

### WebAssembly Backend

Compilation to WebAssembly:

```bash
tauraro compile --backend wasm program.tr -o program.wasm
```

## Optimization

Tauraro provides several levels of optimization:

- **Level 0** - No optimization (default)
- **Level 1** - Basic optimizations
- **Level 2** - More aggressive optimizations
- **Level 3** - Maximum optimizations

```bash
tauraro compile --optimization 2 program.tr -o program
```

## Debugging Support

Tauraro includes debugging support:

```bash
# Run with debugging information
tauraro run --debug program.tr

# Compile with debug symbols
tauraro compile --debug program.tr -o program
```

The debugging features include:
- Stack traces on exceptions
- Variable inspection
- Breakpoint support
- Profiling capabilities