# Tauraro Builtin Modules Test Suite

This directory contains comprehensive test files for all 30 builtin modules in Tauraro.

## Modules Tested

1. `abc` - Abstract Base Classes
2. `asyncio` - Asynchronous I/O
3. `base64` - Base64 encoding/decoding
4. `collections` - Container datatypes
5. `copy` - Copy operations
6. `csv` - CSV file operations
7. `datetime` - Date and time operations
8. `exceptions` - Exception handling
9. `functools` - Functional programming tools
10. `gc` - Garbage collection
11. `hashlib` - Secure hash algorithms
12. `httptools` - HTTP protocol tools
13. `httpx` - HTTP client
14. `io` - Input/output operations
15. `itertools` - Iterator functions
16. `json` - JSON encoder/decoder
17. `logging` - Logging facility
18. `math` - Mathematical functions
19. `memory` - Memory management
20. `os` - Operating system interface
21. `pickle` - Object serialization
22. `random` - Random number generation
23. `re` - Regular expressions
24. `socket` - Low-level networking
25. `sys` - System-specific parameters
26. `threading` - Thread-based parallelism
27. `time` - Time access/conversions
28. `unittest` - Unit testing framework
29. `urllib` - URL handling
30. `websockets` - WebSocket protocol

## How to Run Tests

### Run Individual Module Tests

```bash
# Navigate to the Tauraro project directory
cd /path/to/tauraro

# Run a specific module test
cargo run --bin tauraro -- run examples/abc_test.tauraro
cargo run --bin tauraro -- run examples/asyncio_test.tauraro
# ... repeat for each module
```

### Run All Modules Test

```bash
# Test all modules can be imported
cargo run --bin tauraro -- run examples/all_modules_test.tauraro
```

## Test Files

Each `.tauraro` file contains examples demonstrating the core functionality of its respective module:

- **advanced_collections_test.tauraro** - Enhanced collections functionality (Counter, defaultdict, deque)
- **advanced_functools_test.tauraro** - Enhanced functools functionality (lru_cache, partial, wraps)
- **advanced_itertools_test.tauraro** - Enhanced itertools functionality (all iterator functions)
- **advanced_math_test.tauraro** - Enhanced math functionality (new functions and special functions)

- **abc_test.tauraro** - Abstract base classes and metaclasses
- **asyncio_test.tauraro** - Async/await syntax and concurrent execution
- **base64_test.tauraro** - Encoding/decoding binary data
- **collections_test.tauraro** - Basic container types (Counter, defaultdict, deque, namedtuple)
- **copy_test.tauraro** - Shallow and deep copying
- **csv_test.tauraro** - CSV reading and writing
- **datetime_test.tauraro** - Date/time manipulation
- **exceptions_test.tauraro** - Exception handling and custom exceptions
- **functools_test.tauraro** - Basic function decorators and utilities
- **gc_test.tauraro** - Garbage collection control
- **hashlib_test.tauraro** - Cryptographic hashing
- **httptools_test.tauraro** - HTTP protocol utilities
- **httpx_test.tauraro** - HTTP client functionality
- **io_test.tauraro** - Input/output streams
- **itertools_test.tauraro** - Basic iterator utilities
- **json_test.tauraro** - JSON serialization
- **logging_test.tauraro** - Logging framework
- **math_test.tauraro** - Basic mathematical functions
- **memory_test.tauraro** - Memory management
- **os_test.tauraro** - Operating system interface
- **pickle_test.tauraro** - Object serialization
- **random_test.tauraro** - Random number generation
- **re_test.tauraro** - Regular expressions
- **socket_test.tauraro** - Network socket operations
- **sys_test.tauraro** - System-specific parameters
- **threading_test.tauraro** - Thread-based concurrency
- **time_test.tauraro** - Time-related functions
- **unittest_test.tauraro** - Unit testing framework
- **urllib_test.tauraro** - URL handling and HTTP utilities
- **websockets_test.tauraro** - WebSocket protocol

## Notes

- Some modules may have limited functionality compared to their Python counterparts
- Tests focus on verifying core functionality rather than edge cases
- Actual network or file I/O operations are minimized to keep tests self-contained
- Modules with external dependencies may require additional setup