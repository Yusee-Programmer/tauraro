# Tauraro Programming Language

**A Python-compatible programming language with Rust-like performance**

Tauraro combines the ease of Python with the speed of compiled languages, offering 100% Python syntax compatibility while executing at native speeds.

## Features

### Performance
- **Native speed execution** - Up to 100x faster than Python
- **Multiple backends** - VM interpreter or compile to native C
- **Optimized compilation** - Type-based optimizations for maximum performance
- **Built on Rust** - Leveraging Rust's safety and speed for the runtime

### Python Compatibility
- **100% Python syntax** - Write standard Python code
- **Familiar modules** - Compatible standard library
- **Easy migration** - Drop-in replacement for many Python scripts
- **No new syntax to learn** - If you know Python, you know Tauraro

### Rich Standard Library
All modules available by default - no feature flags needed!

- **HTTP Client** - `httpx` module built on Rust's hyper and reqwest
- **HTTP Utilities** - `httptools` for URL parsing and HTTP utilities
- **WebSockets** - `websockets` for real-time communication
- **Async/Await** - `asyncio` with full coroutine support
- **Process Management** - `subprocess` and `multiprocessing` modules
- **Core Modules** - `json`, `datetime`, `os`, `sys`, `math`, `random`, and more

### Advanced Features
- **Hybrid typing** - Optional static types with dynamic fallback
- **FFI support** - Call C libraries directly
- **Type inference** - Smart type detection for optimizations
- **REPL** - Interactive shell for exploration
- **Cross-platform** - Linux, macOS, and Windows support

## Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/Yusee-Programmer/tauraro.git
cd tauraro

# Build Tauraro
cargo build --release

# The binary will be at ./target/release/tauraro
```

### Run Your First Program

```python
# hello.py
print("Hello from Tauraro!")

def greet(name: str) -> str:
    return f"Hello, {name}!"

print(greet("World"))
```

Run with the VM:
```bash
./target/release/tauraro run hello.py
```

Compile to native executable:
```bash
./target/release/tauraro compile hello.py -o hello
./hello  # Runs at native C speed!
```

### Try the REPL

```bash
./target/release/tauraro repl
```

```python
>>> import math
>>> math.sqrt(16)
4.0
>>> [x**2 for x in range(10)]
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```

## Examples

### HTTP Client

```python
import httpx

# Simple GET request
response = httpx.get("https://api.github.com")
print(response.status_code)
print(response.json())

# POST with JSON data
data = {"name": "Tauraro", "type": "language"}
response = httpx.post("https://api.example.com/data", json=data)
```

### Async Programming

```python
import asyncio

async def fetch_data():
    await asyncio.sleep(1)
    return "Data loaded!"

async def main():
    result = await fetch_data()
    print(result)

asyncio.run(main())
```

### WebSockets

```python
import asyncio
import websockets

async def hello():
    async with websockets.connect("ws://localhost:8080") as websocket:
        await websocket.send("Hello Server!")
        response = await websocket.recv()
        print(f"Received: {response}")

asyncio.run(hello())
```

### Process Management

```python
import subprocess

# Run a command
result = subprocess.run("ls -la")
print(f"Exit code: {result['returncode']}")

# Get command output
output = subprocess.check_output("pwd")
print(output)

# Check CPU count
import multiprocessing
print(f"CPUs: {multiprocessing.cpu_count()}")
```

### Type-Optimized Performance

```python
# With type annotations - compiles to native operations
def fibonacci(n: int) -> int:
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

# Compile and run at native speed
# Up to 100x faster than interpreted Python!
```

## Performance

### Benchmark Results

| Mode | Speed | Use Case |
|------|-------|----------|
| Interpreted (VM) | 1x | Development, REPL |
| Compiled (no types) | 10-50x | General scripts |
| Compiled (with types) | 50-100x | Performance-critical code |

### Example: Fibonacci(35)

```bash
# Python 3
time python3 fib.py
# ~5.2 seconds

# Tauraro VM
time tauraro run fib.py
# ~4.8 seconds

# Tauraro Compiled (no types)
tauraro compile fib.py -o fib
time ./fib
# ~0.4 seconds (13x faster)

# Tauraro Compiled (with types)
tauraro compile fib_typed.py -o fib_typed
time ./fib_typed
# ~0.05 seconds (104x faster!)
```

## Documentation

Full documentation is available in the [docs](docs/README.md) directory:

- [Getting Started Guide](docs/getting-started/quick-start.md)
- [Language Reference](docs/language/syntax.md)
- [Standard Library](docs/stdlib/modules.md)
- [Compilation Guide](docs/compilation/c-backend.md)
- [Advanced Topics](docs/advanced/performance.md)

## Recent Updates

### New Built-in Modules
All HTTP and async modules are now included by default:
- `subprocess` - Process execution and management
- `multiprocessing` - Process-based parallelism
- `httpx` - Modern HTTP client (built on Rust)
- `httptools` - Fast HTTP parsing and URL utilities
- `websockets` - WebSocket support
- `asyncio` - Full async/await with tokio runtime

### C Code Generation Improvements
- Enhanced type inference for mixed-type variables
- Better variable declaration with correct C types
- Improved handling of polymorphic values
- More reliable native compilation

## Architecture

```
┌─────────────────────────────────────────────┐
│            Tauraro Source Code              │
│              (Python Syntax)                │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
         ┌─────────────────────┐
         │   Lexer & Parser    │
         │   (Rust-based)      │
         └──────────┬──────────┘
                    │
                    ▼
              ┌──────────┐
              │   AST    │
              └────┬─────┘
                   │
        ┌──────────┴──────────┐
        │                     │
        ▼                     ▼
   ┌─────────┐         ┌──────────┐
   │   VM    │         │    IR    │
   │ Bytecode│         │Generator │
   └────┬────┘         └─────┬────┘
        │                    │
        ▼                    ▼
   ┌─────────┐         ┌──────────┐
   │Interpret│         │C Transpiler
   │  & Run  │         └─────┬────┘
   └─────────┘               │
                             ▼
                      ┌─────────────┐
                      │  C Compiler │
                      │ (GCC/Clang) │
                      └──────┬──────┘
                             │
                             ▼
                      ┌─────────────┐
                      │   Native    │
                      │ Executable  │
                      └─────────────┘
```

## Project Structure

```
tauraro/
├── src/
│   ├── lexer.rs           # Tokenization
│   ├── parser.rs          # AST generation
│   ├── ast.rs             # AST definitions
│   ├── ir.rs              # Intermediate representation
│   ├── value.rs           # Runtime value types
│   ├── vm/                # VM interpreter
│   ├── bytecode/          # Bytecode compiler
│   ├── codegen/           # Code generation
│   │   ├── c_transpiler/  # C backend
│   │   └── interpreter.rs # Direct interpreter
│   ├── modules/           # Standard library
│   │   ├── subprocess.rs
│   │   ├── multiprocessing.rs
│   │   ├── httpx.rs
│   │   ├── httptools.rs
│   │   ├── websockets.rs
│   │   └── ...
│   └── main.rs            # CLI entry point
├── docs/                  # Documentation
├── examples/              # Example programs
└── tests/                 # Test suite
```

## Roadmap

### Current Status
- ✅ Python syntax compatibility
- ✅ VM interpreter
- ✅ C code generation
- ✅ Type inference and optimization
- ✅ HTTP/async modules
- ✅ Standard library modules
- ✅ FFI support

### Planned Features
- 🔄 LLVM backend for additional optimizations
- 🔄 JIT compilation with Cranelift
- 🔄 WebAssembly target
- 🔄 Parallel compilation
- 🔄 Package manager
- 🔄 IDE integration (LSP)
- 🔄 True process-based multiprocessing
- 🔄 Advanced optimizer passes

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Building from Source

```bash
# Debug build
cargo build

# Release build (optimized)
cargo build --release

# Run tests
cargo test

# Run specific example
./target/release/tauraro run examples/fibonacci.py
```

## License

Tauraro is open source software. See [LICENSE](LICENSE) for details.

## Credits

Tauraro is built with:
- **Rust** - Core language and runtime
- **Tokio** - Async runtime
- **Hyper/Reqwest** - HTTP implementation
- **Tungstenite** - WebSocket support
- **httparse** - HTTP parsing
- **Various Rust crates** - For standard library functionality

## Community

- **GitHub**: [Yusee-Programmer/tauraro](https://github.com/Yusee-Programmer/tauraro)
- **Documentation**: [docs/](docs/README.md)
- **Issues**: [GitHub Issues](https://github.com/Yusee-Programmer/tauraro/issues)

---

**Start using Tauraro today and experience Python with the speed of C!**
