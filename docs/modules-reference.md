# Built-in Modules Reference

This document provides comprehensive documentation for all built-in modules available in TauraroLang. These modules provide functionality similar to Python's standard library, implemented natively in Rust for optimal performance.

## Module System Overview

TauraroLang includes 26 built-in modules that provide essential functionality for system operations, data manipulation, networking, concurrency, and more. All modules are automatically available and can be imported using the standard `import` statement.

```python
import os
import sys
import asyncio
from threading import Thread
```

## Core System Modules

### `sys` - System-specific Parameters and Functions

The `sys` module provides access to interpreter variables and functions that interact with the interpreter.

#### Key Attributes
- `version`: Tauraro version string
- `version_info`: Version information tuple
- `platform`: Platform identifier
- `path`: Module search path list
- `executable`: Path to the Tauraro executable
- `maxsize`: Maximum value for integers
- `byteorder`: Native byte order ('little' or 'big')
- `argv`: Command line arguments
- `modules`: Dictionary of loaded modules

#### Key Functions
- `exit(code=0)`: Exit the interpreter
- `getrefcount(obj)`: Get reference count of an object
- `getsizeof(obj)`: Get size of an object in bytes
- `intern(string)`: Intern a string
- `path_append(path)`: Add path to sys.path
- `path_insert(index, path)`: Insert path at specific index
- `path_remove(path)`: Remove path from sys.path

### `os` - Operating System Interface

Provides a portable way to interact with the operating system.

#### Environment Variables
- `environ`: Dictionary of environment variables

#### Path Operations
- `getcwd()`: Get current working directory
- `chdir(path)`: Change current directory
- `listdir(path='.')`: List directory contents
- `mkdir(path, mode=0o777)`: Create directory
- `makedirs(path, mode=0o777, exist_ok=False)`: Create directories recursively
- `rmdir(path)`: Remove directory
- `remove(path)`: Remove file
- `rename(src, dst)`: Rename file or directory
- `stat(path)`: Get file statistics

#### Process Operations
- `getpid()`: Get process ID
- `system(command)`: Execute system command
- `getenv(key, default=None)`: Get environment variable
- `putenv(key, value)`: Set environment variable

#### Path Module (`os.path`)
- `join(*paths)`: Join path components
- `split(path)`: Split path into directory and filename
- `dirname(path)`: Get directory name
- `basename(path)`: Get base name
- `exists(path)`: Check if path exists
- `isfile(path)`: Check if path is a file
- `isdir(path)`: Check if path is a directory
- `abspath(path)`: Get absolute path
- `normpath(path)`: Normalize path

## Data Processing Modules

### `math` - Mathematical Functions

Provides mathematical functions and constants.

#### Constants
- `pi`: π (3.14159...)
- `e`: Euler's number (2.71828...)
- `tau`: τ (2π)
- `inf`: Positive infinity
- `nan`: Not a Number

#### Power and Logarithmic Functions
- `pow(x, y)`: x raised to power y
- `sqrt(x)`: Square root
- `exp(x)`: e^x
- `exp2(x)`: 2^x
- `expm1(x)`: e^x - 1
- `log(x, base=e)`: Logarithm
- `log2(x)`: Base-2 logarithm
- `log10(x)`: Base-10 logarithm
- `log1p(x)`: log(1 + x)

#### Trigonometric Functions
- `sin(x)`, `cos(x)`, `tan(x)`: Basic trigonometric functions
- `asin(x)`, `acos(x)`, `atan(x)`: Inverse trigonometric functions
- `atan2(y, x)`: Two-argument arctangent
- `sinh(x)`, `cosh(x)`, `tanh(x)`: Hyperbolic functions
- `asinh(x)`, `acosh(x)`, `atanh(x)`: Inverse hyperbolic functions

#### Utility Functions
- `ceil(x)`: Ceiling function
- `floor(x)`: Floor function
- `trunc(x)`: Truncate to integer
- `fabs(x)`: Absolute value
- `factorial(x)`: Factorial
- `gcd(a, b)`: Greatest common divisor
- `lcm(a, b)`: Least common multiple

### `random` - Generate Random Numbers

Provides functions for generating random numbers and making random choices.

#### Basic Functions
- `random()`: Random float in [0.0, 1.0)
- `uniform(a, b)`: Random float in [a, b]
- `randint(a, b)`: Random integer in [a, b]
- `randrange(start, stop, step=1)`: Random integer from range
- `choice(seq)`: Random element from sequence
- `choices(population, weights=None, k=1)`: Multiple random choices
- `sample(population, k)`: Random sample without replacement
- `shuffle(seq)`: Shuffle sequence in place

#### Distributions
- `gauss(mu, sigma)`: Gaussian distribution
- `normalvariate(mu, sigma)`: Normal distribution
- `lognormvariate(mu, sigma)`: Log normal distribution
- `expovariate(lambd)`: Exponential distribution
- `weibullvariate(alpha, beta)`: Weibull distribution
- `gammavariate(alpha, beta)`: Gamma distribution
- `betavariate(alpha, beta)`: Beta distribution

#### State Management
- `seed(a=None)`: Initialize random number generator
- `getstate()`: Get internal state
- `setstate(state)`: Set internal state

### `json` - JSON Encoder and Decoder

Provides JSON serialization and deserialization.

#### Functions
- `dumps(obj, indent=None, separators=None, sort_keys=False)`: Serialize to JSON string
- `loads(s)`: Deserialize from JSON string
- `dump(obj, fp, indent=None, separators=None, sort_keys=False)`: Serialize to file
- `load(fp)`: Deserialize from file

#### Classes
- `JSONEncoder`: Extensible JSON encoder
- `JSONDecoder`: Extensible JSON decoder

### `collections` - Specialized Container Datatypes

Provides alternatives to built-in containers with additional functionality.

#### Container Types
- `deque`: Double-ended queue
- `Counter`: Dictionary subclass for counting hashable objects
- `defaultdict`: Dictionary with default factory function
- `OrderedDict`: Dictionary that maintains insertion order
- `ChainMap`: Dictionary-like class for creating a single view of multiple mappings
- `UserDict`, `UserList`, `UserString`: Wrapper classes for easier subclassing

#### Named Tuples
- `namedtuple(typename, field_names)`: Factory function for creating tuple subclasses

## Text Processing Modules

### `re` - Regular Expression Operations

Provides regular expression matching operations.

#### Functions
- `compile(pattern, flags=0)`: Compile regular expression
- `match(pattern, string, flags=0)`: Match at beginning of string
- `search(pattern, string, flags=0)`: Search anywhere in string
- `findall(pattern, string, flags=0)`: Find all matches
- `finditer(pattern, string, flags=0)`: Find all matches as iterator
- `sub(pattern, repl, string, count=0, flags=0)`: Replace matches
- `subn(pattern, repl, string, count=0, flags=0)`: Replace matches with count
- `split(pattern, string, maxsplit=0, flags=0)`: Split by pattern

#### Flags
- `IGNORECASE` or `I`: Case-insensitive matching
- `MULTILINE` or `M`: Multi-line mode
- `DOTALL` or `S`: Dot matches newlines
- `VERBOSE` or `X`: Verbose mode

### `csv` - CSV File Reading and Writing

Provides functionality to read and write CSV files.

#### Functions
- `reader(csvfile, dialect='excel', **fmtparams)`: Create CSV reader
- `writer(csvfile, dialect='excel', **fmtparams)`: Create CSV writer
- `DictReader(csvfile, fieldnames=None, **kwds)`: CSV reader with dictionary output
- `DictWriter(csvfile, fieldnames, **kwds)`: CSV writer with dictionary input

#### Dialects
- `excel`: Excel-compatible CSV format
- `excel_tab`: Tab-delimited format
- `unix_dialect`: Unix-style CSV format

## I/O and Serialization Modules

### `io` - Core Tools for Working with Streams

Provides tools for working with I/O streams.

#### Classes
- `StringIO`: In-memory string buffer
- `BytesIO`: In-memory bytes buffer
- `TextIOWrapper`: Text stream wrapper
- `BufferedReader`: Buffered binary reader
- `BufferedWriter`: Buffered binary writer

#### Functions
- `open(file, mode='r', buffering=-1, encoding=None)`: Open file
- `StringIO(initial_value='')`: Create string buffer
- `BytesIO(initial_bytes=b'')`: Create bytes buffer

### `pickle` - Python Object Serialization

Provides object serialization and deserialization.

#### Functions
- `dump(obj, file, protocol=None)`: Serialize object to file
- `dumps(obj, protocol=None)`: Serialize object to bytes
- `load(file)`: Deserialize object from file
- `loads(data)`: Deserialize object from bytes

#### Protocol Versions
- Protocol 0-5 supported
- Default protocol: 4
- Highest protocol: 5

### `base64` - Base64 Data Encoding

Provides Base64 encoding and decoding.

#### Functions
- `b64encode(s)`: Encode bytes using Base64
- `b64decode(s)`: Decode Base64 to bytes
- `urlsafe_b64encode(s)`: URL-safe Base64 encoding
- `urlsafe_b64decode(s)`: URL-safe Base64 decoding
- `b32encode(s)`: Base32 encoding
- `b32decode(s)`: Base32 decoding
- `b16encode(s)`: Base16 encoding
- `b16decode(s)`: Base16 decoding

## Networking Modules

### `socket` - Low-level Networking Interface

Provides low-level networking interface.

#### Socket Types
- `AF_INET`: IPv4 address family
- `AF_INET6`: IPv6 address family
- `SOCK_STREAM`: TCP socket
- `SOCK_DGRAM`: UDP socket

#### Functions
- `socket(family=AF_INET, type=SOCK_STREAM, proto=0)`: Create socket
- `gethostname()`: Get hostname
- `gethostbyname(hostname)`: Get IP address by hostname
- `getaddrinfo(host, port, family=0, type=0, proto=0, flags=0)`: Get address info

#### Socket Methods
- `bind(address)`: Bind to address
- `listen(backlog=5)`: Listen for connections
- `accept()`: Accept connection
- `connect(address)`: Connect to address
- `send(data)`: Send data
- `recv(bufsize)`: Receive data
- `close()`: Close socket

### `httpx` - HTTP Client Library

Modern HTTP client library with async support.

#### Synchronous API
- `get(url, **kwargs)`: GET request
- `post(url, data=None, json=None, **kwargs)`: POST request
- `put(url, data=None, **kwargs)`: PUT request
- `delete(url, **kwargs)`: DELETE request
- `head(url, **kwargs)`: HEAD request
- `options(url, **kwargs)`: OPTIONS request
- `patch(url, data=None, **kwargs)`: PATCH request

#### Asynchronous API
- `AsyncClient`: Async HTTP client class
- All methods available with `async`/`await` support

#### Features
- HTTP/1.1 and HTTP/2 support
- Connection pooling
- Request/response streaming
- Authentication support
- Cookie handling
- Proxy support

### `websockets` - WebSocket Client and Server

Provides WebSocket protocol implementation.

#### Client Functions
- `connect(uri, **kwargs)`: Connect to WebSocket server
- `serve(handler, host, port, **kwargs)`: Create WebSocket server

#### WebSocket Methods
- `send(message)`: Send message
- `recv()`: Receive message
- `ping(data=b'')`: Send ping frame
- `pong(data=b'')`: Send pong frame
- `close(code=1000, reason='')`: Close connection

### `httptools` - HTTP Protocol Implementation

Low-level HTTP protocol parsing and handling.

#### Parser Classes
- `HttpRequestParser`: Parse HTTP requests
- `HttpResponseParser`: Parse HTTP responses

#### Functions
- `parse_url(url)`: Parse URL components
- `build_url(**components)`: Build URL from components

## Concurrency Modules

### `threading` - Thread-based Parallelism

Provides thread-based parallelism without Global Interpreter Lock (GIL).

#### Thread Management
- `Thread(target=None, name=None, args=(), kwargs={}, daemon=None)`: Thread class
- `current_thread()`: Get current thread
- `main_thread()`: Get main thread
- `active_count()`: Number of active threads
- `enumerate()`: List all active threads
- `get_ident()`: Get thread identifier

#### Synchronization Primitives
- `Lock()`: Basic lock (mutex)
- `RLock()`: Reentrant lock
- `Condition(lock=None)`: Condition variable
- `Event()`: Event object
- `Semaphore(value=1)`: Semaphore
- `BoundedSemaphore(value=1)`: Bounded semaphore
- `Barrier(parties, action=None, timeout=None)`: Barrier synchronization

#### Thread-Local Data
- `local()`: Thread-local storage

### `asyncio` - Asynchronous I/O

Provides asynchronous programming support with event loops and coroutines.

#### Event Loop Management
- `get_event_loop()`: Get current event loop
- `new_event_loop()`: Create new event loop
- `set_event_loop(loop)`: Set event loop
- `run(coro)`: Run coroutine
- `run_until_complete(coro)`: Run until coroutine completes

#### Task Management
- `create_task(coro)`: Create task from coroutine
- `gather(*awaitables)`: Run awaitables concurrently
- `wait_for(awaitable, timeout)`: Wait with timeout
- `shield(awaitable)`: Shield from cancellation

#### Synchronization Primitives
- `Lock()`: Async lock
- `Event()`: Async event
- `Semaphore(value=1)`: Async semaphore
- `Queue(maxsize=0)`: Async queue

#### Utilities
- `sleep(delay)`: Async sleep
- `iscoroutine(obj)`: Check if object is coroutine
- `iscoroutinefunction(func)`: Check if function is coroutine function
- `isfuture(obj)`: Check if object is future

## Utility Modules

### `functools` - Higher-order Functions and Operations on Callable Objects

Provides utilities for working with functions and callable objects.

#### Decorators
- `lru_cache(maxsize=128, typed=False)`: LRU cache decorator
- `cache`: Unbounded cache decorator
- `cached_property`: Cached property decorator
- `wraps(wrapped)`: Decorator factory for wrapper functions

#### Higher-order Functions
- `reduce(function, iterable, initializer=None)`: Apply function cumulatively
- `partial(func, *args, **keywords)`: Partial function application
- `partialmethod(func, *args, **keywords)`: Partial method application

#### Comparison Utilities
- `cmp_to_key(func)`: Convert comparison function to key function
- `total_ordering`: Class decorator for rich comparison methods

#### Single Dispatch
- `singledispatch(func)`: Single-dispatch generic function
- `singledispatchmethod(func)`: Single-dispatch generic method

### `itertools` - Functions Creating Iterators for Efficient Looping

Provides functions for creating iterators for efficient looping.

#### Infinite Iterators
- `count(start=0, step=1)`: Count from start by step
- `cycle(iterable)`: Cycle through iterable infinitely
- `repeat(object, times=None)`: Repeat object

#### Finite Iterators
- `accumulate(iterable, func=operator.add)`: Cumulative results
- `chain(*iterables)`: Chain iterables together
- `compress(data, selectors)`: Filter by selectors
- `dropwhile(predicate, iterable)`: Drop while predicate is true
- `takewhile(predicate, iterable)`: Take while predicate is true
- `filterfalse(predicate, iterable)`: Filter false values
- `groupby(iterable, key=None)`: Group consecutive elements
- `islice(iterable, start, stop, step)`: Slice iterator
- `starmap(function, iterable)`: Apply function to argument tuples
- `tee(iterable, n=2)`: Split iterator into n independent iterators
- `zip_longest(*iterables, fillvalue=None)`: Zip with padding

#### Combinatorial Iterators
- `product(*iterables, repeat=1)`: Cartesian product
- `permutations(iterable, r=None)`: Permutations
- `combinations(iterable, r)`: Combinations
- `combinations_with_replacement(iterable, r)`: Combinations with replacement

### `copy` - Shallow and Deep Copy Operations

Provides functions for copying objects.

#### Functions
- `copy(obj)`: Shallow copy
- `deepcopy(obj, memo=None)`: Deep copy

### `time` - Time Access and Conversions

Provides time-related functions.

#### Time Functions
- `time()`: Current time as timestamp
- `sleep(seconds)`: Sleep for specified seconds
- `gmtime(secs=None)`: Convert timestamp to UTC struct_time
- `localtime(secs=None)`: Convert timestamp to local struct_time
- `mktime(t)`: Convert struct_time to timestamp
- `strftime(format, t=None)`: Format time as string
- `strptime(string, format)`: Parse time string

#### Performance Counters
- `perf_counter()`: High-resolution performance counter
- `process_time()`: Process time
- `monotonic()`: Monotonic clock

### `datetime` - Basic Date and Time Types

Provides classes for working with dates and times.

#### Classes
- `date(year, month, day)`: Date object
- `time(hour=0, minute=0, second=0, microsecond=0)`: Time object
- `datetime(year, month, day, hour=0, minute=0, second=0, microsecond=0)`: DateTime object
- `timedelta(days=0, seconds=0, microseconds=0, ...)`: Time difference
- `timezone(offset, name=None)`: Timezone object

#### Class Methods
- `date.today()`: Current date
- `datetime.now(tz=None)`: Current datetime
- `datetime.utcnow()`: Current UTC datetime
- `datetime.fromtimestamp(timestamp, tz=None)`: From timestamp

## Security and Hashing Modules

### `hashlib` - Secure Hash and Message Digest Algorithms

Provides secure hash functions.

#### Hash Algorithms
- `md5(data=b'')`: MD5 hash
- `sha1(data=b'')`: SHA-1 hash
- `sha224(data=b'')`: SHA-224 hash
- `sha256(data=b'')`: SHA-256 hash
- `sha384(data=b'')`: SHA-384 hash
- `sha512(data=b'')`: SHA-512 hash
- `blake2b(data=b'', digest_size=64)`: BLAKE2b hash
- `blake2s(data=b'', digest_size=32)`: BLAKE2s hash

#### Hash Object Methods
- `update(data)`: Update hash with data
- `digest()`: Get digest as bytes
- `hexdigest()`: Get digest as hex string
- `copy()`: Copy hash object

### `urllib` - URL Handling Modules

Provides utilities for working with URLs.

#### URL Parsing (`urllib.parse`)
- `urlparse(urlstring)`: Parse URL into components
- `urlunparse(parts)`: Construct URL from parts
- `urljoin(base, url)`: Join base URL with relative URL
- `quote(string, safe='/')`: Quote URL string
- `unquote(string)`: Unquote URL string
- `urlencode(query)`: Encode query parameters

#### URL Opening (`urllib.request`)
- `urlopen(url, data=None, timeout=None)`: Open URL
- `Request(url, data=None, headers={})`: HTTP request object

## Development and Testing Modules

### `logging` - Logging Facility

Provides flexible logging for applications.

#### Logging Functions
- `debug(msg, *args, **kwargs)`: Log debug message
- `info(msg, *args, **kwargs)`: Log info message
- `warning(msg, *args, **kwargs)`: Log warning message
- `error(msg, *args, **kwargs)`: Log error message
- `critical(msg, *args, **kwargs)`: Log critical message
- `exception(msg, *args, **kwargs)`: Log exception with traceback

#### Configuration
- `basicConfig(**kwargs)`: Basic logging configuration
- `getLogger(name=None)`: Get logger instance
- `disable(level)`: Disable logging below level
- `addLevelName(level, levelName)`: Add custom level name
- `getLevelName(level)`: Get level name

#### Logging Levels
- `DEBUG`: 10
- `INFO`: 20
- `WARNING`: 30
- `ERROR`: 40
- `CRITICAL`: 50

### `unittest` - Unit Testing Framework

Provides unit testing framework.

#### Test Classes
- `TestCase`: Base class for test cases
- `TestSuite`: Collection of test cases
- `TestLoader`: Load tests from modules and classes
- `TextTestRunner`: Run tests and display results

#### Assertion Methods
- `assertEqual(a, b)`: Check equality
- `assertNotEqual(a, b)`: Check inequality
- `assertTrue(x)`: Check truth value
- `assertFalse(x)`: Check false value
- `assertIs(a, b)`: Check identity
- `assertIsNot(a, b)`: Check non-identity
- `assertIsNone(x)`: Check None
- `assertIsNotNone(x)`: Check not None
- `assertIn(a, b)`: Check membership
- `assertNotIn(a, b)`: Check non-membership
- `assertRaises(exc, callable, *args)`: Check exception raised

#### Test Discovery
- `main()`: Run tests from command line
- `discover(start_dir, pattern='test*.py')`: Discover tests

## Module Import and Usage Examples

### Basic Import
```python
import os
import sys
from math import pi, sqrt
from collections import defaultdict, Counter
```

### Module Aliasing
```python
import json as js
import datetime as dt
from urllib.parse import urlparse as parse_url
```

### Conditional Imports
```python
try:
    import asyncio
    ASYNC_AVAILABLE = True
except ImportError:
    ASYNC_AVAILABLE = False
```

### Dynamic Module Loading
```python
import sys
module_name = "json"
if module_name in sys.modules:
    module = sys.modules[module_name]
else:
    module = __import__(module_name)
```

## Performance Considerations

### Thread Safety
- All built-in modules are designed to be thread-safe
- No Global Interpreter Lock (GIL) restrictions
- True parallel execution supported

### Memory Management
- Automatic memory management with reference counting
- Garbage collection for circular references
- Efficient memory allocation strategies

### Optimization Features
- Native Rust implementation for performance
- SIMD optimizations where applicable
- Zero-copy operations for large data
- Lazy evaluation for iterators

## Module Development Guidelines

### Creating Custom Modules
1. Follow Python-compatible API design
2. Implement proper error handling
3. Provide comprehensive documentation
4. Include unit tests
5. Consider thread safety

### Best Practices
- Use type hints where applicable
- Implement `__all__` for public API
- Provide meaningful error messages
- Follow naming conventions
- Document all public functions and classes

This comprehensive reference covers all built-in modules available in TauraroLang. Each module provides Python-compatible functionality while leveraging Rust's performance and safety features.