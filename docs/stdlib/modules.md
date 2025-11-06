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
| `serveit` | High-performance ASGI web server (like uvicorn) |
| `templa` | High-performance template engine (like Jinja2) |
| `orm` | High-performance ORM (like SQLAlchemy) |
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

### serveit - ASGI Web Server

High-performance ASGI web server built on Tokio and Hyper, similar to Python's uvicorn.

```python
import serveit

# Simple application
def app(scope):
    method = scope.get("method", "GET")
    path = scope.get("path", "/")

    if path == "/":
        return serveit.HTMLResponse("<h1>Hello from ServEit!</h1>")
    elif path == "/json":
        return serveit.JSONResponse({"message": "Hello", "status": "ok"})
    elif path == "/redirect":
        return serveit.RedirectResponse("/")
    else:
        return serveit.Response(404, "Not Found")

# Run server
serveit.run(app, host="127.0.0.1", port=8000)

# Run with options
serveit.run(app, "0.0.0.0", 8080, {
    "log_level": "info",    # Log level: debug, info, warn, error
    "reload": False,        # Hot reload on code changes
    "workers": 1            # Number of worker processes
})
```

**Response Types:**

```python
# HTML Response
response = serveit.HTMLResponse(
    "<h1>Hello World</h1>",
    status=200
)

# JSON Response
response = serveit.JSONResponse(
    {"key": "value", "data": [1, 2, 3]},
    status=200
)

# Plain Response
response = serveit.Response(
    status=200,
    body="Plain text response"
)

# Redirect Response
response = serveit.RedirectResponse(
    "/new-location",
    status=307  # 307 Temporary Redirect, 301 Permanent
)

# File Response
response = serveit.FileResponse(
    "static/index.html"
)
```

**ASGI Scope:**

The `scope` dict passed to your app contains:

```python
{
    "type": "http",
    "asgi": {"version": "3.0", "spec_version": "2.3"},
    "http_version": "1.1",  # or "2", "3"
    "method": "GET",  # HTTP method
    "path": "/api/users",  # URL path
    "query_string": "page=1&limit=10",  # Query parameters
    "headers": [  # List of (name, value) tuples
        ("host", "localhost:8000"),
        ("user-agent", "Mozilla/5.0"),
        ("accept", "application/json")
    ],
    "server": ("127.0.0.1", 8000)  # (host, port)
}
```

**Routing Example:**

```python
import serveit

def app(scope):
    path = scope.get("path", "/")
    method = scope.get("method", "GET")

    # Simple routing
    routes = {
        "/": handle_home,
        "/api/users": handle_users,
        "/api/posts": handle_posts,
    }

    handler = routes.get(path)
    if handler:
        return handler(scope)

    return serveit.Response(404, "Not Found")

def handle_home(scope):
    return serveit.HTMLResponse("<h1>Home Page</h1>")

def handle_users(scope):
    users = [
        {"id": 1, "name": "Alice"},
        {"id": 2, "name": "Bob"}
    ]
    return serveit.JSONResponse(users)

def handle_posts(scope):
    return serveit.JSONResponse({"posts": []})

serveit.run(app, port=8000)
```

**Status Codes:**

```python
import serveit

# Access HTTP status codes
serveit.status.OK                      # 200
serveit.status.CREATED                 # 201
serveit.status.NO_CONTENT              # 204
serveit.status.MOVED_PERMANENTLY       # 301
serveit.status.FOUND                   # 302
serveit.status.TEMPORARY_REDIRECT      # 307
serveit.status.BAD_REQUEST             # 400
serveit.status.UNAUTHORIZED            # 401
serveit.status.FORBIDDEN               # 403
serveit.status.NOT_FOUND               # 404
serveit.status.METHOD_NOT_ALLOWED      # 405
serveit.status.INTERNAL_SERVER_ERROR   # 500
serveit.status.SERVICE_UNAVAILABLE     # 503
```

**Features:**
- ASGI 3.0 protocol support
- HTTP/1.1 and HTTP/2 ready
- Built on Tokio async runtime
- Powered by Hyper for HTTP
- Request/response helpers
- JSON/HTML response types
- Static file serving
- File responses with auto content-type
- Access logging
- Hot reload (development mode)
- Multiple workers support
- Low latency and high throughput
- Similar API to uvicorn

**Performance:**
ServEit is built on Rust's Tokio and Hyper, providing:
- Native performance (no Python interpreter overhead)
- Async/await for high concurrency
- Low memory footprint
- Production-ready stability

**Common Use Cases:**
- REST API servers
- Web applications
- Microservices
- API gateways
- WebSocket servers (via ASGI)
- Static file serving
- SSR (Server-Side Rendering)

### templa - Template Engine

High-performance template engine similar to Jinja2, built for speed and security.

```python
import templa

# Simple template rendering
result = templa.render_string("Hello, {{ name }}!", {"name": "World"})
# Output: "Hello, World!"

# With filters
result = templa.render_string(
    "{{ title|upper }} by {{ author|capitalize }}",
    {"title": "tauraro guide", "author": "team"}
)
# Output: "TAURARO GUIDE by Team"

# Conditional rendering
template = """
{% if user %}
    Welcome, {{ user.name }}!
{% endif %}
"""
result = templa.render_string(template, {"user": {"name": "Alice"}})

# Loop rendering
template = """
<ul>
{% for item in items %}
    <li>{{ item }}</li>
{% endfor %}
</ul>
"""
result = templa.render_string(template, {"items": ["Apple", "Banana", "Cherry"]})

# Create reusable templates
template = templa.Template("{{ greeting }}, {{ name }}!")
result1 = template.render({"greeting": "Hello", "name": "Alice"})
result2 = template.render({"greeting": "Hi", "name": "Bob"})
```

**Built-in Filters:**

```python
# String filters
{{ text|upper }}        # Convert to uppercase
{{ text|lower }}        # Convert to lowercase
{{ text|capitalize }}   # Capitalize first letter
{{ text|title }}        # Title case (capitalize each word)
{{ text|trim }}         # Remove whitespace
{{ text|reverse }}      # Reverse string

# Utility filters
{{ items|length }}      # Get length
{{ html|escape }}       # HTML escape (automatic by default)
{{ html|safe }}         # Mark as safe (no escaping)

# Chained filters
{{ text|lower|capitalize }}  # Apply multiple filters
```

**Template Syntax:**

```python
# Variables
{{ variable }}
{{ object.property }}

# Filters
{{ variable|filter }}
{{ variable|filter1|filter2 }}

# Conditionals
{% if condition %}
    ...
{% endif %}

# Loops
{% for item in list %}
    {{ item }}
{% endfor %}

{% for key, value in dict %}
    {{ key }}: {{ value }}
{% endfor %}

# Comments
{# This is a comment #}
```

**Security Features:**

```python
# Auto-escaping enabled by default
result = templa.render_string(
    "{{ html }}",
    {"html": "<script>alert('xss')</script>"}
)
# Output: "&lt;script&gt;alert(&#x27;xss&#x27;)&lt;/script&gt;"

# Disable auto-escaping if needed
template = templa.Template("{{ html }}", autoescape=False)

# Or use the safe filter
result = templa.render_string("{{ html|safe }}", {"html": "<b>Bold</b>"})
```

**Environment and Loader:**

```python
# Create environment
env = templa.Environment()

# File system loader
loader = templa.FileSystemLoader("templates/")
template = loader.load(loader, "index.html")
result = template.render({"title": "Home"})

# Escape HTML manually
safe_html = templa.escape("<script>alert('xss')</script>")
```

**Features:**
- Jinja2-like syntax
- Variable interpolation with `{{ }}`
- Control structures: `{% if %}`, `{% for %}`
- Built-in filters for common operations
- Auto-escaping for XSS protection
- Template caching for performance
- Dot notation for nested objects
- Filter chaining
- Comments with `{# #}`
- Fast rendering (compiled templates)
- Memory-safe (built in Rust)

**Performance:**
Templa is built in Rust and optimized for:
- Fast template parsing
- Efficient rendering
- Low memory usage
- Thread-safe operations
- Production-ready performance

**Common Use Cases:**
- HTML generation for web apps
- Email templates
- Report generation
- Dynamic configuration files
- SSR (Server-Side Rendering)
- Static site generation

**Integration with ServEit:**

```python
import serveit
import templa

def app(scope):
    path = scope.get("path", "/")

    if path == "/":
        html = templa.render_string("""
        <!DOCTYPE html>
        <html>
        <head><title>{{ title }}</title></head>
        <body>
            <h1>{{ heading }}</h1>
            <p>{{ message }}</p>
        </body>
        </html>
        """, {
            "title": "Welcome",
            "heading": "Hello from Tauraro!",
            "message": "Powered by ServEit + Templa"
        })
        return serveit.HTMLResponse(html)

serveit.run(app, port=8000)
```

### orm - Object-Relational Mapping

High-performance ORM similar to SQLAlchemy, providing database abstraction and querying capabilities.

**Features:**
- Database engine with connection management
- Table/Model definitions with schema
- Query builder with fluent API
- Session and transaction management
- Multiple column types (Integer, String, Text, Boolean, Float, Blob, DateTime)
- Constraints (NOT NULL, UNIQUE)
- SQLite support (PostgreSQL and MySQL coming soon)

**Basic Usage:**

```python
import orm

# Create database engine
engine = orm.Engine("sqlite:///myapp.db")

# Define table schema
columns = {}
columns["name"] = orm.Column(orm.String, False)  # NOT NULL
columns["email"] = orm.Column(orm.String, False, True)  # NOT NULL, UNIQUE
columns["age"] = orm.Column(orm.Integer, True)  # NULLABLE

users_table = orm.Table("users", columns)

# Create table in database
create_fn = users_table["create"]
create_fn(users_table, engine)

# Create session
connect_fn = engine["connect"]
session = connect_fn(engine)

# Insert data
user = {}
user["__table__"] = users_table
user["name"] = "Alice"
user["email"] = "alice@example.com"
user["age"] = 30

add_fn = session["add"]
add_fn(session, user)  # user["id"] is auto-populated

# Query all records
query_fn = session["query"]
query = query_fn(session, users_table)
all_fn = query["all"]
users = all_fn(query)

for user in users:
    print(f"{user['name']}: {user['email']}")

# Filter queries
filter_dict = {}
filter_dict["age"] = 30
query2 = query_fn(session, users_table)
filter_by_fn = query2["filter_by"]
query2 = filter_by_fn(query2, filter_dict)
all_fn2 = query2["all"]
filtered = all_fn2(query2)

# Order by
query3 = query_fn(session, users_table)
order_by_fn = query3["order_by"]
query3 = order_by_fn(query3, "age")
all_fn3 = query3["all"]
ordered = all_fn3(query3)

# Limit and offset (pagination)
query4 = query_fn(session, users_table)
limit_fn = query4["limit"]
query4 = limit_fn(query4, 10)
offset_fn = query4["offset"]
query4 = offset_fn(query4, 20)
all_fn4 = query4["all"]
page = all_fn4(query4)

# Get first record
query5 = query_fn(session, users_table)
first_fn = query5["first"]
first_user = first_fn(query5)

# Raw SQL execution (UPDATE/INSERT/DELETE only)
execute_fn = engine["execute"]
rows = execute_fn(engine, "UPDATE users SET age = age + 1 WHERE age < 30")

# Close session
close_fn = session["close"]
close_fn(session)
```

**Column Types:**
- `orm.Integer` - Integer numbers
- `orm.String` - Variable-length strings
- `orm.Text` - Long text fields
- `orm.Float` - Floating-point numbers
- `orm.Boolean` - Boolean values (stored as INTEGER in SQLite)
- `orm.DateTime` - Date and time values
- `orm.Blob` - Binary data

**Available Methods:**

**Engine:**
- `connect(engine)` - Create a new session
- `execute(engine, sql)` - Execute raw SQL (UPDATE/INSERT/DELETE)

**Session:**
- `query(session, table)` - Create a new query
- `add(session, object)` - Insert a new record
- `delete(session, object)` - Delete a record
- `commit(session)` - Commit transaction
- `rollback(session)` - Rollback transaction
- `close(session)` - Close the session

**Query:**
- `all(query)` - Get all matching records
- `first(query)` - Get first matching record
- `count(query)` - Count matching records
- `filter_by(query, dict)` - Filter by exact values
- `order_by(query, column)` - Order results
- `limit(query, n)` - Limit number of results
- `offset(query, n)` - Skip first n results

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
