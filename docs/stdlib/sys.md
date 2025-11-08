# System Module (sys)

The `sys` module provides access to system-specific parameters and functions.

## System Information

### Platform Information

```python
import sys

# Platform identifier
print(sys.platform)
# 'linux' on Linux
# 'darwin' on macOS
# 'win32' on Windows

# Python version
print(sys.version)
print(sys.version_info)
```

### Implementation Details

```python
import sys

# Python implementation
print(sys.implementation.name)  # 'tauraro'

# Integer size
print(sys.maxsize)  # Maximum integer size

# Float info
print(sys.float_info)
```

## Command-Line Arguments

### Accessing Arguments

```python
import sys

# script.py
print(f"Script name: {sys.argv[0]}")
print(f"Arguments: {sys.argv[1:]}")

# Run: tauraro run script.py arg1 arg2 arg3
# Output:
# Script name: script.py
# Arguments: ['arg1', 'arg2', 'arg3']
```

### Processing Arguments

```python
import sys

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    print(f"Processing {filename}")

main()
```

## Standard Streams

### stdin, stdout, stderr

```python
import sys

# Write to stdout
sys.stdout.write("Hello, World!\n")

# Write to stderr
sys.stderr.write("Error message\n")

# Read from stdin
line = sys.stdin.readline()
print(f"You entered: {line}")
```

### Redirecting Output

```python
import sys

# Save original stdout
original_stdout = sys.stdout

# Redirect to file
with open('output.log', 'w') as f:
    sys.stdout = f
    print("This goes to the file")

# Restore stdout
sys.stdout = original_stdout
print("This goes to console")
```

## Module and Path Management

### Module Search Path

```python
import sys

# View module search paths
for path in sys.path:
    print(path)

# Add custom path
sys.path.append('/custom/module/path')

# Insert at beginning (higher priority)
sys.path.insert(0, '/high/priority/path')
```

### Loaded Modules

```python
import sys

# Check if module is loaded
if 'math' in sys.modules:
    print("Math module is loaded")

# Get loaded module
math_module = sys.modules.get('math')

# List all loaded modules
for name in sys.modules:
    print(name)
```

## Program Exit

### Exit with Code

```python
import sys

def process_file(filename):
    try:
        # Process file
        pass
    except FileNotFoundError:
        print(f"Error: {filename} not found")
        sys.exit(1)  # Exit with error code
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(2)

    sys.exit(0)  # Success
```

### Exit Hooks

```python
import sys
import atexit

def cleanup():
    print("Cleaning up before exit...")

# Register cleanup function
atexit.register(cleanup)

# This will call cleanup() before exiting
sys.exit(0)
```

## Memory and Performance

### Reference Counting

```python
import sys

x = [1, 2, 3]
print(sys.getrefcount(x))  # Number of references to x

y = x  # Create another reference
print(sys.getrefcount(x))  # Increased by 1
```

### Size of Objects

```python
import sys

# Get size of object in bytes
print(sys.getsizeof(42))         # Size of int
print(sys.getsizeof("hello"))    # Size of string
print(sys.getsizeof([1, 2, 3]))  # Size of list
```

## Exception Information

### Current Exception

```python
import sys

try:
    1 / 0
except:
    exc_type, exc_value, exc_traceback = sys.exc_info()
    print(f"Exception type: {exc_type}")
    print(f"Exception value: {exc_value}")
```

## Recursion Limit

### Get and Set Recursion Limit

```python
import sys

# Get current recursion limit
limit = sys.getrecursionlimit()
print(f"Recursion limit: {limit}")

# Set new recursion limit
sys.setrecursionlimit(2000)
```

## Environment and Configuration

### Byte Order

```python
import sys

# Check byte order
print(sys.byteorder)  # 'little' or 'big'
```

### Encoding

```python
import sys

# Default encoding
print(sys.getdefaultencoding())  # Usually 'utf-8'

# File system encoding
print(sys.getfilesystemencoding())
```

## Complete Examples

### Command-Line Tool

```python
import sys

def process_command(args):
    """Simple CLI tool."""
    if len(args) < 2:
        print("Usage: tool.py <command> [options]")
        sys.exit(1)

    command = args[1]

    if command == "help":
        print("Available commands: help, process, convert")
    elif command == "process":
        if len(args) < 3:
            print("Error: filename required")
            sys.exit(1)
        filename = args[2]
        print(f"Processing {filename}")
    else:
        print(f"Unknown command: {command}")
        sys.exit(1)

    sys.exit(0)

if __name__ == "__main__":
    process_command(sys.argv)
```

### Cross-Platform Script

```python
import sys
import os

def get_config_dir():
    """Get platform-specific config directory."""
    if sys.platform == "win32":
        return os.path.join(os.environ['APPDATA'], 'MyApp')
    elif sys.platform == "darwin":
        return os.path.expanduser('~/Library/Application Support/MyApp')
    else:  # Linux and others
        return os.path.expanduser('~/.config/myapp')

config_dir = get_config_dir()
print(f"Config directory: {config_dir}")
```

## Next Steps

- [OS Module](os.md) - Operating system interface
- [Command Line](../advanced/cli.md) - Building CLI tools
- [Error Handling](../language/exceptions.md) - Exception handling
