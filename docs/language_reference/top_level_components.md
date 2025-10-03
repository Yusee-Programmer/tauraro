# Top-level Components

This chapter describes the top-level components of Tauraro programs.

## Complete Tauraro Programs

A complete Tauraro program is executed in two steps:

1. **Module compilation**: The module is compiled to an internal representation.
2. **Module execution**: The compiled module is executed.

The compilation step creates a code object, which is then executed. The execution step creates a module object and initializes it.

## Program Startup

When a Tauraro program is executed, the following sequence occurs:

1. The interpreter is initialized
2. The `sys` module is initialized
3. The built-in modules are loaded
4. The main module is compiled and executed

### Command Line Arguments

Command line arguments are available through `sys.argv`:

```tauraro
import sys

# sys.argv[0] is the script name
# sys.argv[1:] are the arguments
if len(sys.argv) > 1:
    print(f"Arguments: {sys.argv[1:]}")
else:
    print("No arguments provided")
```

### Environment Variables

Environment variables can be accessed through `os.environ`:

```tauraro
import os

# Get environment variable
path = os.environ.get("PATH", "")

# Set environment variable
os.environ["MY_VAR"] = "value"

# Iterate over environment variables
for key, value in os.environ.items():
    print(f"{key}={value}")
```

## The `__main__` Module

When a Python script is run directly, the code is executed in the `__main__` module:

```tauraro
#!/usr/bin/env tauraro

def main():
    print("Hello from main function!")

if __name__ == "__main__":
    main()
```

This pattern allows a file to be used both as a script and as an importable module.

## Import System

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

### Module Caching

Modules are cached in `sys.modules` after first import:

```tauraro
import sys

# Check if module is already loaded
if "mymodule" in sys.modules:
    print("Module already loaded")

# Remove module from cache (forces reimport)
if "mymodule" in sys.modules:
    del sys.modules["mymodule"]
```

### Relative Imports

Relative imports use leading dots to indicate the current and parent packages:

```tauraro
# In package/subpackage/module.tr

# Import from current package
from . import sibling_module

# Import from parent package
from .. import parent_module

# Import from grandparent package
from ... import grandparent_module
```

## Packages

A package is a directory containing an `__init__.tr` file.

### Regular Packages

```tauraro
# Directory structure:
# mypackage/
#   __init__.tr
#   module1.tr
#   module2.tr
#   subpackage/
#     __init__.tr
#     submodule.tr

# In mypackage/__init__.tr
"""MyPackage documentation."""

# Export specific names from package
__all__ = ["module1", "subpackage"]

# Package-level initialization
print("Initializing MyPackage")
```

### Namespace Packages

Namespace packages allow splitting a single package across multiple directories:

```tauraro
# Directory structure:
# dir1/mypackage/module1.tr
# dir2/mypackage/module2.tr

# No __init__.tr files needed
# Both modules are part of mypackage namespace
```

## Standard Library Structure

The Tauraro standard library is organized into several categories:

### Text Processing

- `string` - Common string operations
- `re` - Regular expression operations
- `difflib` - Helpers for computing deltas
- `textwrap` - Text wrapping and filling
- `unicodedata` - Unicode database
- `stringprep` - Internet String Preparation

### Binary Data

- `struct` - Interpret bytes as packed binary data
- `codecs` - Codec registry and base classes

### Data Types

- `datetime` - Basic date and time types
- `calendar` - General calendar-related functions
- `collections` - Container datatypes
- `heapq` - Heap queue algorithm
- `bisect` - Array bisection algorithms
- `array` - Efficient arrays of numeric values
- `weakref` - Weak references
- `types` - Dynamic type creation and names for built-in types
- `copy` - Shallow and deep copy operations
- `pprint` - Data pretty printer
- `reprlib` - Alternate repr() implementation
- `enum` - Support for enumerations

### Numeric and Mathematical

- `numbers` - Numeric abstract base classes
- `math` - Mathematical functions
- `cmath` - Mathematical functions for complex numbers
- `decimal` - Decimal fixed point and floating point arithmetic
- `fractions` - Rational numbers
- `random` - Generate pseudo-random numbers
- `statistics` - Mathematical statistics functions

### Functional Programming

- `itertools` - Functions creating iterators for efficient looping
- `functools` - Higher-order functions and operations on callable objects
- `operator` - Standard operators as functions

### File and Directory Access

- `pathlib` - Object-oriented filesystem paths
- `os.path` - Common pathname manipulations
- `fileinput` - Iterate over lines from multiple input streams
- `stat` - Interpreting stat() results
- `filecmp` - File and directory comparisons
- `tempfile` - Generate temporary files and directories
- `glob` - Unix style pathname pattern expansion
- `fnmatch` - Unix filename pattern matching
- `linecache` - Random access to text lines
- `shutil` - High-level file operations
- `pickle` - Python object serialization
- `marshal` - Internal Python object serialization

### Data Persistence

- `dbm` - Interfaces to Unix "databases"
- `sqlite3` - DB-API 2.0 interface for SQLite databases

### Data Compression and Archiving

- `zlib` - Compression compatible with gzip
- `gzip` - Support for gzip files
- `bz2` - Support for bzip2 compression
- `lzma` - Compression using the LZMA algorithm
- `zipfile` - Work with ZIP archives
- `tarfile` - Read and write tar archive files

### File Formats

- `csv` - CSV file reading and writing
- `configparser` - Configuration file parser
- `netrc` - netrc file processing
- `xdrlib` - Encode and decode XDR data
- `plistlib` - Generate and parse Mac OS X .plist files

### Cryptographic Services

- `hashlib` - Secure hashes and message digests
- `hmac` - Keyed-Hashing for Message Authentication
- `secrets` - Generate secure random numbers for managing secrets

### Generic Operating System Services

- `os` - Miscellaneous operating system interfaces
- `io` - Core tools for working with streams
- `time` - Time access and conversions
- `argparse` - Parser for command-line options, arguments and sub-commands
- `getopt` - C-style parser for command line options
- `logging` - Python logging facility
- `logging.config` - Logging configuration
- `logging.handlers` - Logging handlers
- `getpass` - Portable password input
- `curses` - Terminal handling for character-cell displays
- `platform` - Access to underlying platform's identifying data
- `errno` - Standard errno system symbols
- `ctypes` - Python bindings for C libraries

### Concurrent Execution

- `threading` - Thread-based parallelism
- `multiprocessing` - Process-based parallelism
- `concurrent.futures` - Launching parallel tasks
- `subprocess` - Subprocess management
- `sched` - Event scheduler
- `queue` - A synchronized queue class
- `contextvars` - Context Variables
- `_thread` - Low-level threading API

### Networking and Interprocess Communication

- `asyncio` - Asynchronous I/O
- `socket` - Low-level networking interface
- `ssl` - TLS/SSL wrapper for socket objects
- `select` - Waiting for I/O completion
- `selectors` - High-level I/O multiplexing
- `asyncore` - Asynchronous socket handler
- `asynchat` - Async socket command/response handler
- `signal` - Set handlers for asynchronous events

### Internet Data Handling

- `email` - Package for handling email messages
- `json` - JSON encoder and decoder
- `mailcap` - Mailcap file handling
- `mailbox` - Manipulate mailboxes in various formats
- `mimetypes` - Map filenames to MIME types
- `base64` - RFC 3548: Base16, Base32, Base64 Data Encodings
- `binascii` - Tools for converting between binary and ASCII
- `quopri` - Encode and decode MIME quoted-printable data

### Structured Markup Processing Tools

- `html` - HyperText Markup Language support
- `xml` - Package containing XML processing modules
- `xml.etree.ElementTree` - The ElementTree XML API
- `xml.dom` - The Document Object Model API
- `xml.dom.minidom` - Minimal DOM implementation
- `xml.dom.pulldom` - Support for building partial DOM trees
- `xml.sax` - Package containing SAX2 base classes
- `xml.parsers.expat` - Fast XML parsing using Expat

### Internet Protocols and Support

- `webbrowser` - Convenient web-browser controller
- `cgi` - Common Gateway Interface support
- `urllib` - URL handling modules
- `urllib.request` - Extensible library for opening URLs
- `urllib.response` - Response classes used by urllib
- `urllib.parse` - Parse URLs and their components
- `urllib.error` - Exception classes raised by urllib.request
- `urllib.robotparser` - Parser for robots.txt
- `http` - HTTP modules
- `http.client` - HTTP protocol client
- `ftplib` - FTP protocol client
- `poplib` - POP3 protocol client
- `imaplib` - IMAP4 protocol client
- `nntplib` - NNTP protocol client
- `smtplib` - SMTP protocol client
- `smtpd` - SMTP server
- `telnetlib` - Telnet client
- `uuid` - UUID objects according to RFC 4122
- `socketserver` - A framework for network servers
- `http.server` - HTTP servers
- `http.cookies` - HTTP state management
- `http.cookiejar` - Cookie handling for HTTP clients
- `xmlrpc` - XMLRPC server and client modules
- `xmlrpc.client` - XML-RPC client access
- `xmlrpc.server` - Basic XML-RPC servers
- `ipaddress` - IPv4/IPv6 manipulation library

### Multimedia Services

- `audioop` - Manipulate raw audio data
- `aifc` - Read and write AIFF and AIFC files
- `sunau` - Read and write Sun AU files
- `wave` - Read and write WAV files
- `chunk` - Read IFF chunked data
- `colorsys` - Conversions between color systems
- `imghdr` - Determine the type of an image
- `sndhdr` - Determine type of sound file
- `ossaudiodev` - Access to OSS-compatible audio devices

### Internationalization

- `gettext` - Multilingual internationalization services
- `locale` - Internationalization services

### Program Frameworks

- `turtle` - Turtle graphics
- `cmd` - Support for line-oriented command interpreters
- `shlex` - Simple lexical analysis

### Graphical User Interfaces with Tk

- `tkinter` - Python interface to Tcl/Tk
- `tkinter.colorchooser` - Dialog to let the user choose a color
- `tkinter.commondialog` - Dialog window base class
- `tkinter.filedialog` - File selection dialogs
- `tkinter.font` - Tkinter font wrapper
- `tkinter.messagebox` - Messagebox operations
- `tkinter.scrolledtext` - Text widget with a scrollbar
- `tkinter.simpledialog` - Basic dialogs
- `tkinter.ttk` - Tk themed widgets
- `tkinter.tix` - Extension widgets for Tk

### Development Tools

- `typing` - Support for type hints
- `pydoc` - Documentation generator and online help system
- `doctest` - Test interactive Python examples
- `unittest` - Unit testing framework
- `unittest.mock` - Mock object library
- `unittest.mock` - Mock object library
- `test` - Regression tests package containing the testing suite for Python
- `bdb` - Debugger framework
- `faulthandler` - Dump the Python traceback
- `pdb` - The Python Debugger
- `profile` - Python profiler
- `cProfile` - C interface to the profiler
- `timeit` - Measure execution time of small code snippets
- `trace` - Trace or track Python statement execution
- `tracemalloc` - Trace memory allocations

### Debugging and Profiling

- `bdb` - Debugger framework
- `faulthandler` - Dump the Python traceback
- `pdb` - The Python Debugger
- `profile` - Python profiler
- `cProfile` - C interface to the profiler
- `timeit` - Measure execution time of small code snippets
- `trace` - Trace or track Python statement execution
- `tracemalloc` - Trace memory allocations

### Software Packaging and Distribution

- `distutils` - Building and installing Python modules
- `ensurepip` - Bootstrapping the pip installer
- `venv` - Creation of virtual environments
- `zipapp` - Manage executable Python zip archives

### Python Runtime Services

- `sys` - System-specific parameters and functions
- `sysconfig` - Provide access to Python's configuration information
- `builtins` - Built-in objects
- `__main__` - Top-level script environment
- `warnings` - Issue warning messages
- `dataclasses` - Generate special methods on user-defined classes
- `contextlib` - Utilities for with-statement contexts
- `abc` - Abstract Base Classes
- `atexit` - Exit handlers
- `traceback` - Print or retrieve a stack traceback
- `__future__` - Future statement definitions
- `gc` - Garbage Collector interface
- `inspect` - Inspect live objects
- `site` - Site-specific configuration hook
- `fpectl` - Floating point exception control

### Custom Python Interpreters

- `code` - Interpreter base classes
- `codeop` - Compile Python code

### Importing Modules

- `zipimport` - Import modules from Zip archives
- `pkgutil` - Package extension utility
- `modulefinder` - Find modules used by a script
- `runpy` - Locating and executing Python modules
- `importlib` - The implementation of the import machinery
- `importlib.resources` - Resources contained within a package
- `importlib.metadata` - Accessing package metadata

## Built-in Constants

Tauraro defines several built-in constants:

```tauraro
# Boolean values
True
False

# Null value
None

# Special constants
NotImplemented
Ellipsis  # or ...
```

## Built-in Functions

Tauraro provides many built-in functions:

```tauraro
# Numeric functions
abs(x)          # Absolute value
round(x[, n])   # Round to n digits
pow(x, y[, z])  # Power function
divmod(x, y)    # Quotient and remainder

# Sequence functions
len(s)          # Length of sequence
max(iterable)   # Maximum value
min(iterable)   # Minimum value
sum(iterable)   # Sum of elements
sorted(iterable) # Sorted list

# Iterator functions
iter(object)    # Get iterator
next(iterator)  # Get next item
enumerate(iterable) # Enumerate items
zip(*iterables) # Zip iterables together

# Functional programming
map(function, iterable)     # Apply function to items
filter(function, iterable)  # Filter items
reduce(function, iterable)  # Reduce to single value
any(iterable)               # True if any item is true
all(iterable)               # True if all items are true

# Object inspection
type(object)        # Get type of object
isinstance(obj, class) # Check instance type
issubclass(class1, class2) # Check subclass relationship
callable(object)    # Check if object is callable
hasattr(object, name) # Check if object has attribute
getattr(object, name[, default]) # Get attribute
setattr(object, name, value) # Set attribute
delattr(object, name) # Delete attribute
dir([object])       # List attributes
vars([object])      # Object's attribute dictionary
id(object)          # Identity of object
repr(object)        # String representation
str(object)         # String value
ascii(object)       # ASCII representation
format(value[, format_spec]) # Formatted string

# Input/Output
print(*objects, sep=' ', end='\n', file=sys.stdout) # Print to output
input([prompt])     # Read input from user

# File operations
open(file, mode='r') # Open file
chr(i)              # Integer to character
ord(c)              # Character to integer
bin(x)              # Binary representation
oct(x)              # Octal representation
hex(x)              # Hexadecimal representation

# Class and instance creation
object()            # Create new object
super([type[, object-or-type]]) # Access parent methods
property(fget=None, fset=None, fdel=None, doc=None) # Property descriptor
classmethod(function) # Class method descriptor
staticmethod(function) # Static method descriptor

# Compilation and execution
compile(source, filename, mode) # Compile source code
eval(expression[, globals[, locals]]) # Evaluate expression
exec(object[, globals[, locals]]) # Execute code
globals()           # Global symbol table
locals()            # Local symbol table

# Mathematical functions
abs(x)              # Absolute value
pow(x, y[, z])      # Power function
divmod(x, y)        # Quotient and remainder

# Attribute access
getattr(object, name[, default]) # Get attribute
setattr(object, name, value) # Set attribute
delattr(object, name) # Delete attribute
hasattr(object, name) # Check if object has attribute

# Iterator creation
iter(object)        # Get iterator
next(iterator[, default]) # Get next item

# Functional programming
map(function, iterable, ...) # Apply function to items
filter(function, iterable)  # Filter items
zip(*iterables)     # Zip iterables together
enumerate(iterable, start=0) # Enumerate items
reversed(seq)       # Reverse sequence
sorted(iterable, *, key=None, reverse=False) # Sorted list

# Logical operations
all(iterable)       # True if all items are true
any(iterable)       # True if any item is true

# Conversion functions
int(x[, base])      # Convert to integer
float(x)            # Convert to float
complex([real[, imag]]) # Convert to complex
str(object='')      # Convert to string
repr(object)        # String representation
ascii(object)       # ASCII representation
chr(i)              # Integer to character
ord(c)              # Character to integer
bin(x)              # Binary representation
oct(x)              # Octal representation
hex(x)              # Hexadecimal representation
bool([x])           # Convert to boolean
list([iterable])    # Convert to list
tuple([iterable])   # Convert to tuple
set([iterable])     # Convert to set
frozenset([iterable]) # Convert to frozenset
dict(**kwarg)       # Convert to dictionary
range(stop)         # Create range object
slice(stop)         # Create slice object
bytes([source[, encoding[, errors]]]) # Convert to bytes
bytearray([source[, encoding[, errors]]]) # Convert to bytearray
memoryview(obj)     # Create memory view

# Mathematical functions
abs(x)              # Absolute value
round(number[, ndigits]) # Round to n digits
pow(x, y[, z])      # Power function
divmod(x, y)        # Quotient and remainder
max(iterable, *[, key, default]) # Maximum value
min(iterable, *[, key, default]) # Minimum value
sum(iterable[, start]) # Sum of elements

# Object inspection
type(object)        # Get type of object
isinstance(object, classinfo) # Check instance type
issubclass(class, classinfo) # Check subclass relationship
callable(object)    # Check if object is callable
len(s)              # Length of sequence
repr(object)        # String representation
str(object='')      # String value
ascii(object)       # ASCII representation
format(value[, format_spec]) # Formatted string
vars([object])      # Object's attribute dictionary
dir([object])       # List attributes
id(object)          # Identity of object
hash(object)        # Hash value
help([object])      # Invoke help system

# Input/Output
print(*objects, sep=' ', end='\n', file=sys.stdout, flush=False) # Print to output
input([prompt])     # Read input from user

# File operations
open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None) # Open file

# Compilation and execution
compile(source, filename, mode, flags=0, dont_inherit=False, optimize=-1) # Compile source code
eval(expression[, globals[, locals]]) # Evaluate expression
exec(object[, globals[, locals]]) # Execute code
globals()           # Global symbol table
locals()            # Local symbol table

# Class and instance creation
object()            # Create new object
super([type[, object-or-type]]) # Access parent methods
property(fget=None, fset=None, fdel=None, doc=None) # Property descriptor
classmethod(function) # Class method descriptor
staticmethod(function) # Static method descriptor

# Miscellaneous
__import__(name, globals=None, locals=None, fromlist=(), level=0) # Import module
```

## Built-in Types

Tauraro has several built-in types:

### Numeric Types

```tauraro
# Integer
x = 42
y = 0b101010  # Binary
z = 0o52      # Octal
w = 0x2A      # Hexadecimal

# Float
pi = 3.14159
scientific = 1.5e-3

# Complex
c = 3 + 4j
real_part = c.real
imag_part = c.imag
```

### Sequence Types

```tauraro
# String
text = "Hello, World!"
multiline = """
This is a
multiline string
"""

# List
numbers = [1, 2, 3, 4, 5]
mixed = [1, "hello", 3.14, True]

# Tuple
coordinates = (10, 20)
single_element = (42,)  # Note the comma
empty = ()

# Range
r = range(10)        # 0 to 9
r = range(1, 10)     # 1 to 9
r = range(0, 10, 2)  # 0, 2, 4, 6, 8
```

### Mapping Types

```tauraro
# Dictionary
person = {"name": "Alice", "age": 30}
empty = {}
```

### Set Types

```tauraro
# Set
colors = {"red", "green", "blue"}
empty = set()  # Note: {} creates empty dict

# Frozenset
immutable_colors = frozenset(["red", "green", "blue"])
```

### Other Types

```tauraro
# Boolean
is_true = True
is_false = False

# None
nothing = None

# Bytes
data = b"Hello"
empty_bytes = bytes()

# Bytearray
mutable_data = bytearray(b"Hello")

# Memory view
view = memoryview(b"Hello")
```

## Built-in Exceptions

Tauraro has a hierarchy of built-in exceptions:

```tauraro
# BaseException
#  +-- SystemExit
#  +-- KeyboardInterrupt
#  +-- GeneratorExit
#  +-- Exception
#       +-- StopIteration
#       +-- StopAsyncIteration
#       +-- ArithmeticError
#       |    +-- FloatingPointError
#       |    +-- OverflowError
#       |    +-- ZeroDivisionError
#       +-- AssertionError
#       +-- AttributeError
#       +-- BufferError
#       +-- EOFError
#       +-- ImportError
#       |    +-- ModuleNotFoundError
#       +-- LookupError
#       |    +-- IndexError
#       |    +-- KeyError
#       +-- MemoryError
#       +-- NameError
#       |    +-- UnboundLocalError
#       +-- OSError
#       |    +-- BlockingIOError
#       |    +-- ChildProcessError
#       |    +-- ConnectionError
#       |    |    +-- BrokenPipeError
#       |    |    +-- ConnectionAbortedError
#       |    |    +-- ConnectionRefusedError
#       |    |    +-- ConnectionResetError
#       |    +-- FileExistsError
#       |    +-- FileNotFoundError
#       |    +-- InterruptedError
#       |    +-- IsADirectoryError
#       |    +-- NotADirectoryError
#       |    +-- PermissionError
#       |    +-- ProcessLookupError
#       |    +-- TimeoutError
#       +-- ReferenceError
#       +-- RuntimeError
#       |    +-- NotImplementedError
#       |    +-- RecursionError
#       +-- SyntaxError
#       |    +-- IndentationError
#       |         +-- TabError
#       +-- SystemError
#       +-- TypeError
#       +-- ValueError
#       |    +-- UnicodeError
#       |         +-- UnicodeDecodeError
#       |         +-- UnicodeEncodeError
#       |         +-- UnicodeTranslateError
#       +-- Warning
#            +-- DeprecationWarning
#            +-- PendingDeprecationWarning
#            +-- RuntimeWarning
#            +-- SyntaxWarning
#            +-- UserWarning
#            +-- FutureWarning
#            +-- ImportWarning
#            +-- UnicodeWarning
#            +-- BytesWarning
#            +-- ResourceWarning
```

## Built-in Modules

Tauraro provides many built-in modules:

```tauraro
# System modules
import sys          # System-specific parameters and functions
import os           # Miscellaneous operating system interfaces
import io           # Core tools for working with streams
import time         # Time access and conversions

# Data types and structures
import collections  # Container datatypes
import itertools    # Functions creating iterators for efficient looping
import functools    # Higher-order functions and operations on callable objects

# Text processing
import string       # Common string operations
import re           # Regular expression operations

# File and directory access
import pathlib      # Object-oriented filesystem paths
import glob         # Unix style pathname pattern expansion
import shutil       # High-level file operations

# Internet data handling
import json         # JSON encoder and decoder
import base64       # RFC 3548: Base16, Base32, Base64 Data Encodings
import urllib       # URL handling modules

# Mathematical operations
import math         # Mathematical functions
import random       # Generate pseudo-random numbers

# Data persistence
import pickle       # Python object serialization
import csv          # CSV file reading and writing

# Cryptographic services
import hashlib      # Secure hashes and message digests

# Concurrent execution
import threading    # Thread-based parallelism
import asyncio      # Asynchronous I/O

# Networking
import socket       # Low-level networking interface

# Development tools
import unittest     # Unit testing framework
import logging      # Python logging facility

# Debugging and profiling
import traceback    # Print or retrieve a stack traceback
```

## Python Compatibility

Tauraro aims to be highly compatible with Python while adding new features:

```tauraro
# Most Python code will run unchanged in Tauraro
import math
import json
import os

# Tauraro adds new features like optional static typing
def calculate_area(length: float, width: float) -> float:
    return length * width

# And FFI for calling C functions directly
extern "libm.so" {
    fn sqrt(x: double) -> double
}

result = sqrt(16.0)  # Calls C's sqrt function
```

## Performance Considerations

Tauraro offers multiple backends for different performance needs:

```bash
# VM backend (default, interpreted)
tauraro run program.tr

# LLVM backend (compiled to optimized native code)
tauraro compile --backend llvm program.tr -o program

# C backend (compiled to C code)
tauraro compile --backend c program.tr -o program.c

# WebAssembly backend (compiled to WebAssembly)
tauraro compile --backend wasm program.tr -o program.wasm
```

## Best Practices

### Code Organization

```tauraro
#!/usr/bin/env tauraro
"""Module docstring describing the purpose of this module."""

import sys
import os
from typing import List, Dict

# Module-level constants
VERSION = "1.0.0"
DEBUG = False

# Module-level variables
counter = 0

def main():
    """Main function."""
    print("Starting application")
    # Application logic here

if __name__ == "__main__":
    main()
```

### Error Handling

```tauraro
def safe_divide(a, b):
    """Divide a by b safely."""
    try:
        return a / b
    except ZeroDivisionError:
        print("Cannot divide by zero")
        return None
    except TypeError:
        print("Invalid types for division")
        return None
```

### Resource Management

```tauraro
# Always use context managers for resources
with open("data.txt", "r") as file:
    content = file.read()

# Custom context managers for your resources
class DatabaseConnection:
    def __enter__(self):
        self.conn = connect_to_database()
        return self.conn
    
    def __exit__(self, exc_type, exc_value, traceback):
        if hasattr(self, 'conn'):
            self.conn.close()
```

### Type Hints

```tauraro
from typing import List, Dict, Optional

def process_items(items: List[str]) -> Dict[str, int]:
    """Process a list of items and return their lengths."""
    return {item: len(item) for item in items}

def find_user(user_id: int) -> Optional[str]:
    """Find a user by ID, returning None if not found."""
    # Implementation here
    pass
```