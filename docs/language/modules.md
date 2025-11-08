# Modules and Imports

Tauraro supports Python's module system, allowing you to organize code into reusable packages and import functionality from the rich standard library.

## Importing Modules

### Basic Import

```python
# Import entire module
import math

print(math.pi)      # 3.14159...
print(math.sqrt(16)) # 4.0
```

### Import Specific Items

```python
# Import specific functions/classes
from math import sqrt, pi

print(sqrt(16))  # 4.0
print(pi)        # 3.14159...
```

### Import with Alias

```python
# Rename module
import datetime as dt

now = dt.datetime.now()

# Rename imported items
from collections import defaultdict as dd

counts = dd(int)
```

### Import All (Not Recommended)

```python
# Import everything from module
from math import *

# Can now use all math functions directly
print(sqrt(16))
print(sin(pi/2))

# Warning: Can cause namespace pollution
```

## Built-in Modules

Tauraro includes 38+ built-in modules, all available without installation:

### Core Modules

```python
# Math operations
import math
print(math.sqrt(144))

# System operations
import sys
print(sys.version)
print(sys.argv)

# Operating system interface
import os
print(os.getcwd())
print(os.listdir('.'))

# Random numbers
import random
print(random.randint(1, 100))
print(random.choice(['a', 'b', 'c']))

# Date and time
import datetime
now = datetime.datetime.now()
print(now.strftime("%Y-%m-%d %H:%M:%S"))

# JSON encoding/decoding
import json
data = {"name": "Alice", "age": 30}
json_str = json.dumps(data)
parsed = json.loads(json_str)
```

### HTTP and Networking

```python
# HTTP client (built on Rust's hyper/reqwest)
import httpx

response = httpx.get("https://api.github.com")
print(response.status_code)
print(response.json())

# HTTP utilities
import httptools

url = httptools.parse_url("https://example.com:8080/path?query=value")
print(url)

# WebSocket support
import asyncio
import websockets

async def hello():
    async with websockets.connect("ws://localhost:8080") as ws:
        await ws.send("Hello!")
        response = await ws.recv()
        print(response)

asyncio.run(hello())
```

### Async Programming

```python
# Full async/await support with Tokio runtime
import asyncio

async def fetch_data():
    await asyncio.sleep(1)
    return "Data loaded"

async def main():
    result = await fetch_data()
    print(result)

asyncio.run(main())
```

### Process Management

```python
# Subprocess execution
import subprocess

# Run command
result = subprocess.run("ls -la")
print(f"Exit code: {result['returncode']}")

# Get output
output = subprocess.check_output("pwd")
print(output)

# Multiprocessing (thread-based currently)
import multiprocessing

cpu_count = multiprocessing.cpu_count()
print(f"CPUs: {cpu_count}")
```

### Data Processing

```python
# Collections - specialized data structures
import collections

counter = collections.Counter([1, 2, 2, 3, 3, 3])
print(counter.most_common(2))

deque = collections.deque([1, 2, 3])
deque.appendleft(0)

# CSV processing
import csv

# Read CSV
with open('data.csv', 'r') as f:
    reader = csv.reader(f)
    for row in reader:
        print(row)

# Base64 encoding
import base64

encoded = base64.b64encode(b"Hello World")
decoded = base64.b64decode(encoded)

# Hashing
import hashlib

hash_obj = hashlib.sha256(b"Hello World")
print(hash_obj.hexdigest())
```

### Functional Programming

```python
# Itertools - iterator functions
import itertools

# Infinite iterators
counter = itertools.count(start=1, step=2)
# 1, 3, 5, 7, 9, ...

# Combinations
combos = list(itertools.combinations([1, 2, 3], 2))
# [(1, 2), (1, 3), (2, 3)]

# Functools - higher-order functions
import functools

# Partial application
from functools import partial

def multiply(x, y):
    return x * y

double = partial(multiply, 2)
print(double(5))  # 10

# Reduce
from functools import reduce

numbers = [1, 2, 3, 4, 5]
product = reduce(lambda x, y: x * y, numbers)
print(product)  # 120
```

### Other Useful Modules

```python
# Regular expressions
import re

pattern = r'\d+'
matches = re.findall(pattern, "There are 123 apples and 456 oranges")
print(matches)  # ['123', '456']

# Logging
import logging

logging.info("This is an info message")
logging.error("This is an error message")

# Copying objects
import copy

original = [1, [2, 3], 4]
shallow = copy.copy(original)
deep = copy.deepcopy(original)

# Pickling (serialization)
import pickle

data = {"name": "Alice", "age": 30}
serialized = pickle.dumps(data)
deserialized = pickle.loads(serialized)

# I/O operations
import io

string_buffer = io.StringIO()
string_buffer.write("Hello ")
string_buffer.write("World")
content = string_buffer.getvalue()  # "Hello World"

# Garbage collection
import gc

# Force garbage collection
gc.collect()

# Get stats
stats = gc.get_stats()

# Abstract base classes
import abc

class AbstractBase(abc.ABC):
    @abc.abstractmethod
    def do_something(self):
        pass

# Exception handling utilities
import exceptions

try:
    raise ValueError("Error message")
except ValueError as e:
    print(f"Caught: {e}")
```

## Creating Your Own Modules

### Simple Module

Create a file `mymodule.py`:

```python
# mymodule.py

def greet(name: str) -> str:
    return f"Hello, {name}!"

def add(a: int, b: int) -> int:
    return a + b

PI = 3.14159
```

Use it:

```python
# main.py
import mymodule

print(mymodule.greet("Alice"))
print(mymodule.add(5, 10))
print(mymodule.PI)
```

### Module with Classes

```python
# geometry.py

class Circle:
    def __init__(self, radius: float):
        self.radius = radius

    def area(self) -> float:
        return 3.14159 * self.radius ** 2

class Rectangle:
    def __init__(self, width: float, height: float):
        self.width = width
        self.height = height

    def area(self) -> float:
        return self.width * self.height
```

Use it:

```python
from geometry import Circle, Rectangle

circle = Circle(5)
print(circle.area())

rect = Rectangle(10, 20)
print(rect.area())
```

## Package Structure

### Creating a Package

Directory structure:

```
mypackage/
├── __init__.py
├── module1.py
├── module2.py
└── subpackage/
    ├── __init__.py
    └── module3.py
```

**`mypackage/__init__.py`:**

```python
# Package initialization
from .module1 import function1
from .module2 import function2

__all__ = ['function1', 'function2']
```

**`mypackage/module1.py`:**

```python
def function1():
    return "Function 1"
```

**Using the package:**

```python
import mypackage

print(mypackage.function1())

# Or import specific modules
from mypackage import module1, module2
from mypackage.subpackage import module3
```

## Module Search Path

Tauraro searches for modules in the following order:

1. **Built-in modules** - Standard library modules
2. **Current directory** - Where the script is running
3. **TAURARO_PATH** - Environment variable (if set)
4. **Standard locations** - Installation directories

### Checking Module Path

```python
import sys

print(sys.path)
# Shows all directories searched for modules
```

## Module Attributes

### Special Attributes

```python
# mymodule.py

def my_function():
    pass

class MyClass:
    pass

# Special module attributes
print(__name__)      # Module name
print(__file__)      # Module file path
print(__doc__)       # Module docstring
print(__package__)   # Package name

# Check if running as main script
if __name__ == "__main__":
    print("Running as main script")
    my_function()
```

### Module Introspection

```python
import math

# List all attributes
print(dir(math))

# Check if attribute exists
print(hasattr(math, 'sqrt'))  # True

# Get attribute
sqrt_func = getattr(math, 'sqrt')
print(sqrt_func(16))  # 4.0

# Module documentation
print(math.__doc__)
```

## Lazy Imports

### Import When Needed

```python
def process_data():
    # Import only when function is called
    import pandas as pd
    return pd.DataFrame(data)
```

### Conditional Imports

```python
import sys

if sys.platform == "linux":
    import linux_specific_module
elif sys.platform == "win32":
    import windows_specific_module
```

## Module Reloading

### Reload a Module

```python
import importlib

import mymodule

# Make changes to mymodule.py

# Reload the module
importlib.reload(mymodule)
```

## Best Practices

### 1. Use Descriptive Module Names

```python
# Good
import user_authentication
import data_processing

# Bad
import stuff
import utils
```

### 2. Import at the Top

```python
# Good
import math
import json

def calculate():
    return math.sqrt(16)

# Less ideal - imports inside functions (except when needed)
def calculate():
    import math
    return math.sqrt(16)
```

### 3. Group Imports

```python
# Standard library imports
import sys
import os
import json

# Third-party imports (if any)
# import numpy
# import pandas

# Local application imports
import mymodule
from mypackage import function1
```

### 4. Use Absolute Imports

```python
# Good
from mypackage.subpackage import module

# Less ideal - relative imports
from ..subpackage import module
```

### 5. Avoid Circular Imports

```python
# Bad - module_a.py imports module_b, module_b imports module_a

# Good - use import inside function if needed
def function():
    from module_b import something
    return something()
```

## Module Performance

### Import Cost

```python
# Imports are cached - subsequent imports are fast
import math  # First import - loads module
import math  # Second import - uses cache
```

### Selective Imports

```python
# Import only what you need
from math import sqrt, pi  # Faster than importing everything

# vs
import math  # Imports entire module
```

## Common Patterns

### Fallback Imports

```python
try:
    import fast_module
    processor = fast_module.process
except ImportError:
    import slow_module
    processor = slow_module.process
```

### Version Checking

```python
import sys

if sys.version_info < (3, 8):
    print("Python 3.8+ required")
    sys.exit(1)
```

### Main Guard

```python
# mymodule.py

def main():
    print("Running main function")

if __name__ == "__main__":
    # Only runs when executed directly
    # Not when imported
    main()
```

## Complete Module List

Tauraro includes these built-in modules:

### Core
- `sys` - System-specific parameters
- `os` - Operating system interface
- `io` - I/O operations
- `gc` - Garbage collection

### Math & Numbers
- `math` - Mathematical functions
- `random` - Random number generation

### Data Structures
- `collections` - Specialized container datatypes
- `itertools` - Iterator functions
- `functools` - Higher-order functions

### Text Processing
- `re` - Regular expressions
- `json` - JSON encoding/decoding
- `csv` - CSV file reading/writing
- `base64` - Base64 encoding

### Date & Time
- `datetime` - Date and time operations

### Files & Data
- `pickle` - Object serialization
- `copy` - Shallow and deep copy operations

### Networking & HTTP
- `httpx` - HTTP client (Rust-based)
- `httptools` - HTTP utilities
- `websockets` - WebSocket support

### Async
- `asyncio` - Asynchronous I/O

### Process Management
- `subprocess` - Subprocess management
- `multiprocessing` - Process-based parallelism

### Cryptography
- `hashlib` - Secure hashes and message digests

### Debugging & Development
- `logging` - Logging facility
- `exceptions` - Exception utilities
- `abc` - Abstract base classes

### Memory
- `memory` - Memory management utilities

### ORM (if available)
- `orm` - Object-relational mapping

## Next Steps

- [Functions](functions.md) - Creating reusable functions
- [Classes](classes.md) - Object-oriented programming
- [Standard Library](../stdlib/modules.md) - Complete module reference
- [Package Management](../advanced/packages.md) - Managing dependencies
- [Performance](../advanced/performance.md) - Optimizing imports
