# Standard Library Modules

Tauraro includes a comprehensive standard library with modules for common programming tasks.

## Built-in Modules Overview

### Core Modules

| Module | Description |
|--------|-------------|
| `sys` | System-specific parameters and functions |
| `os` | Operating system interface |
| `math` | Mathematical functions |
| `random` | Random number generation |
| `time` | Time access and conversions |
| `datetime` | Date and time manipulation |
| `json` | JSON encoding and decoding |
| `re` | Regular expressions |
| `collections` | Specialized container datatypes |
| `itertools` | Iterator building blocks |
| `functools` | Higher-order functions and operations |

### I/O Modules

| Module | Description |
|--------|-------------|
| `io` | Core I/O operations |
| `csv` | CSV file reading and writing |
| `pickle` | Python object serialization |

### System Modules

| Module | Description |
|--------|-------------|
| `threading` | Thread-based parallelism |
| `multiprocessing` | Process-based parallelism |
| `asyncio` | Asynchronous I/O and coroutines |
| `subprocess` | Subprocess management and execution |
| `socket` | Low-level networking |

### HTTP and Web Modules

| Module | Description |
|--------|-------------|
| `httpx` | Modern HTTP client with sync/async support |
| `httptools` | HTTP parsing and URL utilities |
| `websockets` | WebSocket client and server |
| `urllib` | URL handling and HTTP request utilities |

### Utility Modules

| Module | Description |
|--------|-------------|
| `copy` | Shallow and deep copy operations |
| `base64` | Base64 encoding/decoding |
| `hashlib` | Secure hash algorithms |
| `gc` | Garbage collector interface |
| `logging` | Flexible logging system |

## Commonly Used Modules

### sys - System Functions

```python
import sys

# Command line arguments
print(sys.argv)

# Python version
print(sys.version)

# Platform information
print(sys.platform)

# Exit program
sys.exit(0)

# Standard streams
sys.stdout.write("Output\n")
sys.stderr.write("Error\n")

# Path manipulation
sys.path.append("/custom/path")

# Get size of objects
import sys
x = [1, 2, 3, 4, 5]
print(sys.getsizeof(x))
```

### os - Operating System Interface

```python
import os

# Current directory
print(os.getcwd())

# Change directory
os.chdir("/path/to/directory")

# List directory
files = os.listdir(".")

# File operations
os.rename("old.txt", "new.txt")
os.remove("file.txt")

# Directory operations
os.mkdir("new_directory")
os.makedirs("path/to/directory")
os.rmdir("directory")

# Path operations
path = os.path.join("dir", "file.txt")
print(os.path.exists("file.txt"))
print(os.path.isfile("file.txt"))
print(os.path.isdir("directory"))
print(os.path.basename("/path/to/file.txt"))
print(os.path.dirname("/path/to/file.txt"))

# Environment variables
home = os.environ.get("HOME")
os.environ["MY_VAR"] = "value"

# Execute system command
os.system("ls -la")
```

### math - Mathematical Functions

```python
import math

# Constants
print(math.pi)      # 3.141592653589793
print(math.e)       # 2.718281828459045

# Basic functions
math.sqrt(16)       # 4.0
math.pow(2, 3)      # 8.0
math.abs(-5)        # 5

# Rounding
math.ceil(3.2)      # 4
math.floor(3.8)     # 3
math.trunc(3.9)     # 3

# Trigonometry
math.sin(math.pi/2)     # 1.0
math.cos(0)             # 1.0
math.tan(math.pi/4)     # 1.0

# Logarithms
math.log(10)            # Natural log
math.log10(100)         # 2.0
math.log2(8)            # 3.0

# Exponentials
math.exp(1)             # e^1
math.pow(2, 10)         # 1024

# Special functions
math.factorial(5)       # 120
math.gcd(12, 18)        # 6
```

### random - Random Number Generation

```python
import random

# Random float between 0 and 1
print(random.random())

# Random integer in range
print(random.randint(1, 10))

# Random choice from sequence
colors = ["red", "green", "blue"]
print(random.choice(colors))

# Random sample (without replacement)
print(random.sample([1, 2, 3, 4, 5], 3))

# Shuffle list in place
numbers = [1, 2, 3, 4, 5]
random.shuffle(numbers)

# Random float in range
print(random.uniform(1.0, 10.0))

# Seed for reproducibility
random.seed(42)
```

### time - Time Functions

```python
import time

# Current time (seconds since epoch)
now = time.time()

# Sleep
time.sleep(1.0)  # Sleep for 1 second

# Formatted time
print(time.strftime("%Y-%m-%d %H:%M:%S"))

# Performance timing
start = time.time()
# ... code to time ...
end = time.time()
print(f"Elapsed: {end - start:.4f}s")

# High-resolution timer
start = time.perf_counter()
# ... code to time ...
end = time.perf_counter()
```

### datetime - Date and Time

```python
from datetime import datetime, date, time, timedelta

# Current date and time
now = datetime.now()
today = date.today()

# Create specific datetime
dt = datetime(2024, 1, 15, 10, 30, 0)

# Format datetime
print(now.strftime("%Y-%m-%d %H:%M:%S"))

# Parse string to datetime
dt = datetime.strptime("2024-01-15", "%Y-%m-%d")

# Date arithmetic
tomorrow = today + timedelta(days=1)
next_week = today + timedelta(weeks=1)
one_hour_ago = now - timedelta(hours=1)

# Components
print(now.year)
print(now.month)
print(now.day)
print(now.hour)
print(now.minute)
```

### json - JSON Encoding/Decoding

```python
import json

# Python to JSON
data = {
    "name": "Alice",
    "age": 30,
    "hobbies": ["reading", "coding"]
}

# Convert to JSON string
json_string = json.dumps(data, indent=2)

# Write to file
with open("data.json", "w") as f:
    json.dump(data, f, indent=2)

# JSON to Python
json_string = '{"name": "Bob", "age": 25}'
data = json.loads(json_string)

# Read from file
with open("data.json", "r") as f:
    data = json.load(f)
```

### re - Regular Expressions

```python
import re

# Search for pattern
text = "The quick brown fox"
match = re.search(r"quick", text)
if match:
    print("Found:", match.group())

# Find all matches
text = "cat bat rat"
matches = re.findall(r"\w+at", text)
print(matches)  # ['cat', 'bat', 'rat']

# Replace pattern
result = re.sub(r"\d+", "X", "I have 3 cats and 2 dogs")
print(result)  # "I have X cats and X dogs"

# Split by pattern
parts = re.split(r"\s+", "split   by   spaces")

# Compile pattern for reuse
pattern = re.compile(r"\d+")
matches = pattern.findall("1 2 3 4 5")

# Groups
text = "John: 30, Jane: 25"
match = re.search(r"(\w+): (\d+)", text)
if match:
    print(match.group(1))  # Name
    print(match.group(2))  # Age
```

### collections - Specialized Containers

```python
from collections import defaultdict, Counter, deque

# defaultdict - default values for missing keys
dd = defaultdict(int)
dd["count"] += 1  # No KeyError

dd = defaultdict(list)
dd["items"].append(1)  # Creates list automatically

# Counter - count elements
words = ["apple", "banana", "apple", "cherry", "banana", "apple"]
counter = Counter(words)
print(counter["apple"])  # 3
print(counter.most_common(2))  # [('apple', 3), ('banana', 2)]

# deque - double-ended queue
dq = deque([1, 2, 3])
dq.append(4)        # Add to right
dq.appendleft(0)    # Add to left
dq.pop()            # Remove from right
dq.popleft()        # Remove from left
```

### itertools - Iterator Tools

```python
import itertools

# Infinite iterators
counter = itertools.count(start=1, step=2)
# 1, 3, 5, 7, ...

repeater = itertools.repeat("A", times=3)
# A, A, A

cycle = itertools.cycle([1, 2, 3])
# 1, 2, 3, 1, 2, 3, ...

# Combinatorics
list(itertools.permutations([1, 2, 3], 2))
# [(1, 2), (1, 3), (2, 1), (2, 3), (3, 1), (3, 2)]

list(itertools.combinations([1, 2, 3], 2))
# [(1, 2), (1, 3), (2, 3)]

# Chain iterables
list(itertools.chain([1, 2], [3, 4], [5, 6]))
# [1, 2, 3, 4, 5, 6]

# Grouping
data = [('A', 1), ('A', 2), ('B', 3), ('B', 4)]
for key, group in itertools.groupby(data, lambda x: x[0]):
    print(key, list(group))
```

### functools - Function Tools

```python
from functools import reduce, partial, lru_cache

# reduce - apply function cumulatively
numbers = [1, 2, 3, 4, 5]
total = reduce(lambda x, y: x + y, numbers)
print(total)  # 15

# partial - create function with preset arguments
def power(base, exponent):
    return base ** exponent

square = partial(power, exponent=2)
cube = partial(power, exponent=3)

print(square(5))  # 25
print(cube(5))    # 125

# lru_cache - memoization
@lru_cache(maxsize=None)
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

print(fibonacci(100))  # Fast due to caching
```

### subprocess - Process Management

Execute and manage external processes.

```python
import subprocess

# Run a command and wait for completion
result = subprocess.run("echo Hello World")
print(result['returncode'])  # 0 for success
print(result['stdout'])      # Command output
print(result['stderr'])      # Error output

# Run command with list of arguments
result = subprocess.run(["ls", "-la"])

# Call a command and get return code
returncode = subprocess.call("echo Testing")
print(returncode)  # 0 for success

# Check call - raises error if command fails
subprocess.check_call("ls /existing/path")

# Get command output
output = subprocess.check_output("pwd")
print(output)  # Current directory

# Get output and status
status, output = subprocess.getstatusoutput("ls -la")
print(status, output)

# Get just output (ignore status)
output = subprocess.getoutput("date")
print(output)

# Constants for stdio handling
subprocess.PIPE      # Pipe for capturing output
subprocess.STDOUT    # Redirect stderr to stdout
subprocess.DEVNULL   # Null device for discarding output
```

**Common Use Cases:**
- Execute shell commands
- Run external programs
- Capture command output
- Check command exit status
- Integrate with system utilities

### multiprocessing - Process-Based Parallelism

Thread-based parallel execution for CPU-bound tasks.

```python
import multiprocessing

# Get CPU count
cpu_count = multiprocessing.cpu_count()
print(f"Available CPUs: {cpu_count}")

# Create a worker pool
pool = multiprocessing.Pool()
# Note: Full Pool implementation is provided for compatibility
# Currently uses thread-based execution

# Create a process (thread-based)
process = multiprocessing.Process()
# Note: Full Process implementation is provided

# Queue for inter-process communication
queue = multiprocessing.Queue()
# Note: Thread-safe queue implementation

# Lock for synchronization
lock = multiprocessing.Lock()
# Note: Thread-safe lock implementation

# Semaphore for resource management
semaphore = multiprocessing.Semaphore(value=5)
# Note: Limits concurrent access

# Event for signaling
event = multiprocessing.Event()
# Note: Thread-safe event signaling
```

**Features:**
- CPU core detection
- Worker pool management
- Process creation and control
- Inter-process communication via queues
- Synchronization primitives (Lock, Semaphore, Event)
- Compatible with Python's multiprocessing API

**Note:** Current implementation uses thread-based parallelism for compatibility and portability. Full process-based parallelism is planned for future releases.

### asyncio - Asynchronous I/O

Async/await support for concurrent programming.

```python
import asyncio

# Define async function
async def fetch_data():
    await asyncio.sleep(1)
    return "Data fetched"

# Run async function
result = asyncio.run(fetch_data())

# Sleep asynchronously
async def delayed_hello():
    await asyncio.sleep(2)
    print("Hello after 2 seconds")

# Wait for multiple coroutines
async def main():
    result1 = await fetch_data()
    result2 = await fetch_data()
    return result1, result2

# Create tasks
async def concurrent_tasks():
    task1 = asyncio.create_task(fetch_data())
    task2 = asyncio.create_task(fetch_data())
    await task1
    await task2

# Gather multiple coroutines
async def gather_example():
    results = await asyncio.gather(
        fetch_data(),
        fetch_data(),
        fetch_data()
    )
    return results

# Timeout support
async def with_timeout():
    try:
        await asyncio.wait_for(fetch_data(), timeout=5.0)
    except asyncio.TimeoutError:
        print("Operation timed out")
```

**Features:**
- Event loop management
- Async/await syntax support
- Task creation and scheduling
- Sleep and delays
- Timeout handling
- Concurrent task execution
- Coroutine management

### httpx - Modern HTTP Client

High-performance HTTP client with both sync and async support.

```python
import httpx

# GET request
response = httpx.get("https://api.example.com/data")
print(response.status_code)
print(response.text)
print(response.json())

# POST request with JSON data
data = {"name": "Alice", "age": 30}
response = httpx.post("https://api.example.com/users", json=data)

# POST with form data
form_data = {"username": "alice", "password": "secret"}
response = httpx.post("https://example.com/login", data=form_data)

# PUT request
response = httpx.put("https://api.example.com/users/123", json=data)

# DELETE request
response = httpx.delete("https://api.example.com/users/123")

# Custom headers
headers = {"Authorization": "Bearer token123"}
response = httpx.get("https://api.example.com/protected", headers=headers)

# Query parameters
params = {"page": 1, "limit": 10}
response = httpx.get("https://api.example.com/items", params=params)

# Create HTTP client with configuration
client = httpx.Client()
# Note: Client object for connection pooling and session management

# Request with timeout
response = httpx.get("https://example.com", timeout=10.0)
```

**Features:**
- GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS methods
- JSON request/response handling
- Form data submission
- Custom headers
- Query parameters
- Timeout support
- Client configuration
- Connection pooling
- Built on high-performance Rust HTTP libraries (hyper, reqwest)

### httptools - HTTP Utilities

HTTP parsing and URL manipulation utilities.

```python
import httptools

# Parse URL
url = "https://example.com:8080/path/to/resource?key=value&foo=bar"
parts = httptools.parse_url(url)
print(parts['scheme'])    # "https"
print(parts['host'])      # "example.com"
print(parts['port'])      # 8080
print(parts['path'])      # "/path/to/resource"
print(parts['query'])     # "key=value&foo=bar"

# URL encoding
encoded = httptools.quote("hello world")
print(encoded)  # "hello%20world"

encoded = httptools.quote_plus("hello world")
print(encoded)  # "hello+world"

# URL decoding
decoded = httptools.unquote("hello%20world")
print(decoded)  # "hello world"

decoded = httptools.unquote_plus("hello+world")
print(decoded)  # "hello world"

# Parse query string
query = "name=Alice&age=30&city=NYC"
params = httptools.parse_qs(query)
print(params)  # {"name": "Alice", "age": "30", "city": "NYC"}

# Build query string
params = {"name": "Bob", "age": "25"}
query = httptools.urlencode(params)
print(query)  # "name=Bob&age=25"

# Parse HTTP headers
headers = """Content-Type: application/json
Authorization: Bearer token123
Content-Length: 42"""

parsed_headers = httptools.parse_headers(headers)
print(parsed_headers)

# Parse HTTP request
request = """GET /api/users HTTP/1.1
Host: example.com
User-Agent: MyClient/1.0

"""
parsed_request = httptools.parse_request(request)
print(parsed_request['method'])    # "GET"
print(parsed_request['path'])      # "/api/users"
print(parsed_request['version'])   # "HTTP/1.1"

# Parse HTTP response
response = """HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 13

{"ok": true}"""

parsed_response = httptools.parse_response(response)
print(parsed_response['status'])   # 200
print(parsed_response['reason'])   # "OK"
```

**Features:**
- URL parsing and decomposition
- URL encoding/decoding
- Query string parsing and building
- HTTP header parsing
- HTTP request parsing
- HTTP response parsing
- Built on httparse for performance

### websockets - WebSocket Support

WebSocket client and server implementation.

```python
import websockets
import asyncio

# WebSocket client connection
async def websocket_client():
    uri = "ws://localhost:8080"
    async with websockets.connect(uri) as websocket:
        # Send message
        await websocket.send("Hello Server")

        # Receive message
        response = await websocket.recv()
        print(f"Received: {response}")

# Run WebSocket client
asyncio.run(websocket_client())

# WebSocket server
async def echo_handler(websocket):
    async for message in websocket:
        # Echo received message back
        await websocket.send(f"Echo: {message}")

async def websocket_server():
    async with websockets.serve(echo_handler, "localhost", 8080):
        await asyncio.Future()  # Run forever

# Run WebSocket server
asyncio.run(websocket_server())

# Send JSON data
import json

async def send_json():
    async with websockets.connect("ws://localhost:8080") as ws:
        data = {"type": "message", "content": "Hello"}
        await ws.send(json.dumps(data))
        response = await ws.recv()
        print(json.loads(response))

# Handle connection errors
async def robust_client():
    try:
        async with websockets.connect("ws://localhost:8080") as ws:
            await ws.send("Hello")
            response = await ws.recv()
            print(response)
    except websockets.ConnectionClosed:
        print("Connection closed")
    except Exception as e:
        print(f"Error: {e}")

# Custom headers
async def client_with_headers():
    headers = {"Authorization": "Bearer token123"}
    async with websockets.connect("ws://localhost:8080",
                                  extra_headers=headers) as ws:
        await ws.send("Authenticated message")
```

**Features:**
- WebSocket client connections
- WebSocket server implementation
- Send/receive text messages
- Send/receive binary data
- Connection management
- Error handling
- Custom headers
- Built on tungstenite WebSocket library

**Common Use Cases:**
- Real-time communication
- Live data feeds
- Chat applications
- Game servers
- IoT device communication
- Live dashboards

## Module Import Patterns

### Basic Import

```python
import math
print(math.pi)

# Import specific items
from math import pi, sqrt
print(pi)
print(sqrt(16))

# Import with alias
import math as m
print(m.pi)

from datetime import datetime as dt
now = dt.now()

# Import all (not recommended)
from math import *
```

### Conditional Imports

```python
try:
    import optional_module
except ImportError:
    optional_module = None

if optional_module:
    optional_module.function()
```

### Package Structure

```python
# Import from package
from mypackage import module
from mypackage.subpackage import another_module

# Relative imports (within package)
from . import sibling_module
from .. import parent_module
from ..sibling_package import module
```

## Creating Custom Modules

### Simple Module (mymodule.py)

```python
"""My custom module."""

def greet(name):
    """Greet someone by name."""
    return f"Hello, {name}!"

def farewell(name):
    """Say goodbye to someone."""
    return f"Goodbye, {name}!"

# Module constant
VERSION = "1.0.0"

# Module initialization
print("mymodule loaded")
```

### Using Custom Module

```python
import mymodule

print(mymodule.greet("Alice"))
print(mymodule.VERSION)
```

### Package Structure

```
mypackage/
    __init__.py
    module1.py
    module2.py
    subpackage/
        __init__.py
        module3.py
```

**__init__.py:**
```python
"""MyPackage - A custom package."""

from .module1 import function1
from .module2 import function2

__version__ = "1.0.0"
__all__ = ["function1", "function2"]
```

## Best Practices

1. **Import at Top**: Place imports at the beginning of files
2. **Use Specific Imports**: Import only what you need
3. **Avoid Wildcard Imports**: Don't use `from module import *`
4. **Use Standard Library**: Leverage built-in modules before external packages
5. **Check Module Availability**: Handle ImportError gracefully
6. **Document Modules**: Include docstrings in custom modules
7. **Use __all__**: Define public API in __init__.py
8. **Use Async for I/O**: Use asyncio for network and file I/O operations
9. **Use Multiprocessing for CPU**: Use multiprocessing for CPU-intensive tasks
10. **HTTP Best Practices**: Use httpx for modern HTTP requests, httptools for parsing

## Performance Notes

All HTTP and async modules are **always available by default** in Tauraro - no feature flags needed!

- **httpx**: Built on high-performance Rust libraries (hyper, reqwest)
- **websockets**: Built on tungstenite for fast WebSocket handling
- **asyncio**: Native async/await with tokio runtime
- **httptools**: Fast HTTP parsing with httparse
- **subprocess**: Efficient process execution and output capture
- **multiprocessing**: Thread-based parallelism with plans for true process-based execution

## Next Steps

- [Math Module Reference](math.md)
- [File I/O](io.md)
- [Regular Expressions](re.md)
- [Date and Time](datetime.md)
- [Async Programming](../advanced/async.md)
- [HTTP Clients](../advanced/http.md)
- [Process Management](../advanced/subprocess.md)
- [Creating Packages](../advanced/packages.md)
