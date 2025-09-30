# Tauraro Module System Documentation

## Overview

Tauraro implements a comprehensive Python-like module system that supports:

1. **Built-in Tauraro modules** (written in Rust)
2. **External Tauraro packages** (stored in `tauraro_packages/externals/`)
3. **Python packages from PyPI** (stored in `tauraro_packages/pysites/`)
4. **User modules** (in current directory or sys.path)

## Directory Structure

```
tauraro_packages/
├── externals/          # External Tauraro packages
│   ├── my_package/
│   │   ├── __init__.tr
│   │   ├── module1.tr
│   │   └── package.toml
│   └── another_package/
├── pysites/           # Python packages from PyPI
│   ├── requests/
│   ├── numpy/
│   └── pandas/
└── built-in modules (compiled into Tauraro)
```

## Module Search Order

When importing a module `import foo`, Tauraro searches in this order:

1. **Built-in modules** (sys, os, math, etc.) - compiled into Tauraro
2. **Python packages** in `tauraro_packages/pysites/`
3. **External Tauraro packages** in `tauraro_packages/externals/`
4. **Local modules** in current directory
5. **Modules in sys.path**

## Default sys.path

```python
import sys
print(sys.path)
# Output:
# [
#     '.',                              # Current directory
#     'tauraro_packages',               # Built-in packages
#     'tauraro_packages/externals',     # External Tauraro packages
#     'tauraro_packages/pysites',       # Python packages
#     'C:\\tauraro\\lib',               # Platform-specific (Windows)
#     'C:\\tauraro\\lib\\site-packages'
# ]
```

## Import Syntax

### Basic Import
```python
import sys              # Built-in module
import os               # Built-in module  
import my_package       # External package
import requests         # Python package (if installed)
```

### From Import
```python
from math import pi, sin, cos
from os import path
from my_package import my_function
```

### Import with Alias
```python
import numpy as np
import threading as thread_mod
```

### Import All (not recommended)
```python
from math import *
```

## Built-in Modules

Built-in modules are **only available when explicitly imported**. They are not auto-loaded in the global namespace.

Available built-in modules:
- `sys` - System-specific parameters and functions
- `os` - Operating system interface
- `math` - Mathematical functions
- `random` - Random number generation
- `time` - Time-related functions
- `datetime` - Date and time handling
- `json` - JSON encoder and decoder
- `re` - Regular expressions
- `threading` - Thread-based parallelism
- `socket` - Low-level networking interface
- `asyncio` - Asynchronous I/O
- `io` - Core I/O functionality
- `csv` - CSV file reading and writing
- `logging` - Logging facility
- `unittest` - Unit testing framework
- `copy` - Shallow and deep copy operations
- `pickle` - Python object serialization
- `base64` - Base64 encoding/decoding
- `hashlib` - Secure hash and message digest algorithms
- `urllib` - URL handling modules
- `collections` - Specialized container datatypes
- `functools` - Higher-order functions and operations
- `itertools` - Iterator functions
- `memory` - Memory management utilities
- `gc` - Garbage collection interface

## Package Management

### Installing Python Packages
```python
# Install from PyPI to tauraro_packages/pysites/
install_package('requests')
install_package('numpy', 'python')
```

### Installing Tauraro Packages
```python
# Install from URL or local path to tauraro_packages/externals/
install_package('path/to/package', 'tauraro')
install_package('https://github.com/user/package.zip', 'tauraro')
```

### Listing Installed Packages
```python
packages = list_packages()
for pkg in packages:
    print(f"{pkg['name']} ({pkg['type']}) - {pkg['version']}")
```

### Uninstalling Packages
```python
uninstall_package('requests')
```

## Python Interoperability

When Python interop is enabled (`python-interop` feature), Tauraro can:

1. **Import Python packages** installed in `tauraro_packages/pysites/`
2. **Use existing Python packages** from the system Python installation
3. **Install packages directly to pysites** using pip

```python
# Install Python package to pysites directory
install_package('requests')

# Import and use Python package
import requests
response = requests.get('https://api.github.com')
print(response.json())
```

## Creating Tauraro Packages

### Package Structure
```
my_package/
├── __init__.tr         # Package initialization
├── module1.tr          # Module files
├── module2.tr
├── package.toml        # Package metadata (optional)
└── README.md
```

### package.toml (optional)
```toml
[package]
name = "my_package"
version = "1.0.0"
description = "My awesome Tauraro package"
author = "Your Name"
dependencies = ["other_package"]
```

### __init__.tr
```python
# Package initialization code
from .module1 import function1
from .module2 import Class2

__version__ = "1.0.0"
__all__ = ["function1", "Class2"]
```

## Environment Variables

- `TAURARO_PATH` - Additional search paths (colon-separated on Unix, semicolon-separated on Windows)

## Module Caching

- Modules are cached after first import
- Use `reload()` function to reload a module (if implemented)
- Circular imports are detected and prevented

## Error Handling

```python
try:
    import non_existent_module
except ImportError as e:
    print(f"Module not found: {e}")

try:
    from my_module import non_existent_function
except ImportError as e:
    print(f"Cannot import function: {e}")
```

## Best Practices

1. **Import at the top** of files
2. **Use specific imports** rather than `import *`
3. **Group imports** by type (built-in, external, local)
4. **Use aliases** for long module names
5. **Check availability** of optional modules with try/except

## Examples

See `examples/module_system_test.py` for a comprehensive test of the module system functionality.