# Introduction to Tauraro

Tauraro is a flexible programming language that draws inspiration from Python while adding unique features and capabilities. It supports multiple compilation backends, making it suitable for a wide range of applications from scripting to high-performance computing.

## Key Features

### Python-like Syntax
Tauraro uses syntax very similar to Python, making it easy for Python developers to learn and use:

```tauraro
# Hello World in Tauraro
print("Hello, World!")

# Function definition
def greet(name):
    return f"Hello, {name}!"

# Class definition
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def introduce(self):
        return f"I'm {self.name}, {self.age} years old"
```

### Multiple Compilation Backends
Tauraro supports multiple compilation backends:
- **VM Backend** - Interpreted execution (default)
- **LLVM Backend** - Compilation to optimized native code
- **C Backend** - Compilation to C code
- **WebAssembly Backend** - Compilation to WebAssembly

### Enhanced Type System
While Tauraro maintains Python's dynamic nature, it optionally supports static type checking:

```tauraro
# Optional type annotations
def calculate_area(length: float, width: float) -> float:
    return length * width

# Type checking can be enabled with strict_types flag
```

### Built-in Concurrency Support
Tauraro has built-in support for both threading and async/await patterns:

```tauraro
import asyncio
import threading

# Async example
async def fetch_data(url):
    # Simulate async operation
    await asyncio.sleep(1)
    return f"Data from {url}"

# Threading example
def worker_function(data):
    # Process data in a separate thread
    return data * 2
```

### Foreign Function Interface (FFI)
Tauraro can interface with C libraries directly:

```tauraro
# Import C functions
extern "libm.so" {
    fn sqrt(x: double) -> double
}

result = sqrt(16.0)  # Calls C's sqrt function
```

## Getting Started

### Installation
Tauraro can be installed using Cargo (Rust package manager):

```bash
cargo install tauraro
```

### Running Tauraro Programs
You can run Tauraro programs in several ways:

```bash
# Run a script
tauraro run my_script.tr

# Start the REPL
tauraro repl

# Compile to native code
tauraro compile --backend llvm my_script.tr -o my_program
```

### Hello World Example
Create a file called `hello.tr`:

```tauraro
#!/usr/bin/env tauraro

def main():
    print("Welcome to Tauraro!")
    print("This is a simple example program.")
    
    # Variables and basic operations
    name = "Tauraro"
    version = 1.0
    print(f"Hello from {name} v{version}")

if __name__ == "__main__":
    main()
```

Run it with:
```bash
tauraro run hello.tr
```

## Language Comparisons

### Similarities with Python
- Indentation-based block structure
- Dynamic typing
- Similar built-in data types (list, dict, tuple, etc.)
- Similar standard library modules
- Exception handling with try/except
- List comprehensions and generator expressions

### Differences from Python
- Optional static type checking
- Multiple compilation backends
- Enhanced concurrency model
- Direct FFI to C libraries
- Better performance with LLVM backend
- Cross-compilation support

## Use Cases

Tauraro is suitable for a wide range of applications:

1. **Scripting and Automation** - Similar to Python for system administration tasks
2. **Web Development** - Backend services with high performance
3. **Data Science** - Numerical computing with optional static typing
4. **System Programming** - Low-level programming with FFI capabilities
5. **Embedded Systems** - Cross-compilation to various targets
6. **High-Performance Computing** - LLVM backend for optimized execution

## Next Steps

To learn more about Tauraro:
1. Follow the [Tutorial](tutorial.md) for a hands-on introduction
2. Explore the [Standard Library](library/) documentation
3. Check out the [Examples](../examples/) directory for practical code samples
4. Read about [Python Compatibility](python_compatibility.md) if you're migrating from Python