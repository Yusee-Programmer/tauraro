# Tauraro Programming Language

**A Python-compatible programming language with Rust-like performance**

Tauraro combines the ease of Python with the speed of compiled languages, offering 100% Python syntax compatibility while executing at native speeds.

## Features

### Performance
- **Native speed execution** - Up to 200x faster than Python
- **Multiple backends** - VM interpreter or compile to native C
- **Optimized compilation** - Type-based optimizations for maximum performance
- **Built on Rust** - Leveraging Rust's safety and speed for the runtime

### Python Compatibility
- **100% Python syntax** - Write standard Python code
- **Familiar modules** - Compatible standard library
- **Easy migration** - Drop-in replacement for many Python scripts
- **No new syntax to learn** - If you know Python, you know Tauraro

### System Programming
- **Bare-metal support** - Write OS kernels, drivers, embedded firmware
- **Memory management** - Manual allocation, arenas, and automatic GC
- **Low-level primitives** - Pointers, atomics, volatile I/O
- **Hardware access** - Port I/O, MMIO, CPU control registers
- **Freestanding compilation** - No C stdlib required

### Rich Standard Library
All modules available by default - no feature flags needed!

- **HTTP Client** - `httpx` module built on Rust's hyper and reqwest
- **Web Server** - `serveit` high-performance ASGI server
- **Templates** - `templa` Jinja2-like template engine
- **ORM** - `orm` database abstraction layer
- **WebSockets** - `websockets` for real-time communication
- **Async/Await** - `asyncio` with full coroutine support
- **Process Management** - `subprocess` and `multiprocessing` modules
- **Core Modules** - `json`, `datetime`, `os`, `sys`, `math`, `random`, and more

### Advanced Features
- **Hybrid typing** - Optional static types with dynamic fallback
- **FFI support** - Call C libraries and Win32 APIs directly
- **Shared library compilation** - Compile to .so/.dll/.dylib for plugins and language bindings
- **Type inference** - Smart type detection for optimizations
- **REPL** - Interactive shell for exploration
- **Cross-platform** - Linux, macOS, and Windows support

## Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/tauraro/tauraro.git
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
./target/release/tauraro compile hello.py --backend c --native -o hello
./hello  # Runs at native C speed!
```

Compile to shared library:
```bash
./target/release/tauraro compile hello.py --backend c --native --lib-type shared -o libhello.so
# Creates libhello.so on Linux, libhello.dylib on macOS, hello.dll on Windows
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
# Up to 200x faster than interpreted Python!
```

### System Programming

```python
# Manual memory management
buffer = allocate(4096)
memset(buffer, 0, 4096)
ptr_write(buffer, "int32", 42)
value = ptr_read(buffer, "int32")
free(buffer)

# Atomic operations for lock-free programming
counter_addr = allocate(8)
atomic_store(counter_addr, 0)
old = atomic_add(counter_addr, 1)
success = atomic_cas(counter_addr, old, new_value)
memory_barrier()  # Ensure memory ordering

# Port I/O (x86/x86_64)
port_out8(0x80, 0x42)   # 8-bit port write
value = port_in8(0x80)  # 8-bit port read
port_out16(0x3F8, data) # 16-bit UART
port_out32(0xCF8, addr) # 32-bit PCI config

# Memory-mapped I/O (all architectures)
mmio_base = 0xFE000000
mmio_write32(mmio_base, 0x12345678)
status = mmio_read32(mmio_base + 4)
```

### Bare-Metal Development

```python
# Write OS kernels in Tauraro!
def kernel_main():
    # VGA text output
    mmio_write8(0xB8000, ord('H'))
    mmio_write8(0xB8001, 0x0F)
    
    # Interrupt control
    disable_interrupts()
    # ... setup IDT ...
    enable_interrupts()
    
    while True:
        halt()
```

Compile for bare-metal:
```bash
tauraro compile kernel.tr --freestanding --entry-point kernel_main --target-arch x86_64
```

### Shared Library Compilation

Create reusable shared libraries for plugins, language bindings, and more:

```python
# math_lib.py - A reusable math library
def add(a, b):
    return a + b

def multiply(a, b):
    return a * b

def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

**Compile to shared library:**
```bash
# Linux
tauraro compile math_lib.py --backend c --native --lib-type shared -o libmath.so

# Windows
tauraro compile math_lib.py --backend c --native --lib-type shared -o mathlib.dll

# macOS
tauraro compile math_lib.py --backend c --native --lib-type shared -o libmath.dylib
```

**Cross-platform compilation:**
```bash
# Compile for Linux from any platform
tauraro compile mylib.py --backend c --native --lib-type shared --target linux -o libmylib.so

# Compile for Windows from any platform
tauraro compile mylib.py --backend c --native --lib-type shared --target windows -o mylib.dll
```

**Use cases:**
- **Plugin systems** - Create loadable plugins for applications
- **Language bindings** - Expose Tauraro libraries to C/C++/Python/etc.
- **Embedded systems** - Reusable library components
- **Microservices** - Shared service libraries

See [SHARED_LIBRARY_COMPILATION.md](SHARED_LIBRARY_COMPILATION.md) for complete documentation.

## Performance

### Benchmark Results

| Mode | Speed | Use Case |
|------|-------|----------|
| Interpreted (VM) | 1x | Development, REPL |
| Compiled (no types) | 10-50x | General scripts |
| Compiled (with types) | 50-200x | Performance-critical code |

### Example: Fibonacci(35)

| Implementation | Time | Speedup |
|----------------|------|---------|
| Python 3.11 | ~2.8s | 1x |
| Tauraro VM | ~2.5s | 1.1x |
| Tauraro Compiled | ~0.014s | **200x** |

## Documentation

Full documentation is available in the [docs](docs/README.md) directory:

- [Getting Started Guide](docs/getting-started/quick-start.md)
- [Language Reference](docs/language/syntax.md)
- [Standard Library](docs/stdlib/modules.md)
- [Compilation Guide](docs/compilation/c-backend.md)
- [Shared Library Compilation](SHARED_LIBRARY_COMPILATION.md)
- [FFI C Transpiler Implementation](FFI_C_TRANSPILER_IMPLEMENTATION.md)
- [System Programming](docs/builtins/system-programming.md)
- [Bare-Metal Development](docs/advanced/baremetal.md)
- [FFI Guide](docs/advanced/ffi.md)
- [Performance Tuning](docs/advanced/performance.md)

## Recent Updates

### Parser Improvements (December 2025)
- **Tuple Returns** - Full support for `return 1, 2, 3` syntax
- **Trailing Commas** - Proper handling of `return x, y,` patterns
- **Enhanced Parsing** - Fixed token boundary handling for complex expressions
- **100% Verified** - Comprehensive test suite for all Python patterns

### Bare-Metal & System Programming (Production Ready ✅)
- **OS Development** - Write kernels, bootloaders, drivers
- **Hardware Access** - Port I/O (8/16/32-bit), MMIO (8/16/32/64-bit)
- **Interrupt Control** - CLI/STI, multi-architecture (x86, ARM, RISC-V)
- **CPU Registers** - CR0, CR3, MSR read/write
- **Freestanding Mode** - Compile without C stdlib (`--freestanding`)
- **Custom Entry Points** - `--entry-point kernel_main`
- **Target Architectures** - x86, x86_64, ARM, AArch64, RISC-V
- **Inline Assembly** - Real hardware instructions generated

### Memory & Low-Level Primitives (100% Complete)
- **Manual Memory** - `allocate()`, `free()`, arena allocation
- **Pointer Operations** - `ptr_read()`, `ptr_write()`, `ptr_offset()`
- **Atomic Operations** - `atomic_load()`, `atomic_store()`, `atomic_cas()`, `atomic_add/sub()`
- **Memory Operations** - `memcpy()`, `memset()`, `memmove()`, `memcmp()`
- **Volatile I/O** - `volatile_read()`, `volatile_write()`
- **Memory Utilities** - `zero_memory()`, `memory_stats()`, `memory_barrier()`
- **Cache Control** - `cache_line_size()`, prefetch hints

### C Transpiler Production Status
- **System Programming**: ✅ 98% Complete - Ready for production
- **Embedded Software**: ✅ 97% Complete - Ready for OS/bare-metal development
- **Network Programming**: ✅ Full HTTP, WebSockets, async support
- **Concurrency**: ✅ Threading, multiprocessing, atomic operations

### Web Development Stack
- **ServEit** - High-performance ASGI server (like uvicorn)
- **Templa** - Jinja2-like template engine
- **ORM** - SQLite database abstraction
- **HTTP Client** - httpx with async support
- **WebSockets** - Full real-time communication support

### Shared Library Compilation (December 2025)
- **Cross-platform shared libraries** - Compile to .so (Linux), .dll (Windows), .dylib (macOS)
- **Simple flag** - Use `--lib-type shared` to create shared libraries
- **Target selection** - Cross-compile for different platforms with `--target`
- **Automatic flags** - Platform-specific compiler flags handled automatically
- **Use cases** - Plugin systems, language bindings, embedded libraries, microservices

### FFI Improvements (C Transpiler)
- **Automatic FFI generation** - FFI functions automatically included in C transpiler output
- **Win32 API support** - Verified working with Windows API
- **Native C function pointers** - Direct C library integration
- **Cross-platform library loading** - Works on Windows, Linux, macOS
- **Automatic dynamic linking** - `-ldl` flag added automatically when FFI detected
- **Direct system calls** - Low-level system programming support

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
                    ┌────────┴────────┐
                    │                 │
                    ▼                 ▼
             ┌─────────────┐   ┌─────────────┐
             │   Native    │   │   Shared    │
             │ Executable  │   │   Library   │
             │             │   │ (.so/.dll)  │
             └─────────────┘   └─────────────┘
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
│   ├── builtins.rs        # Built-in functions
│   ├── vm/                # VM interpreter
│   ├── bytecode/          # Bytecode compiler
│   ├── codegen/           # Code generation
│   │   └── c_transpiler/  # C backend
│   └── modules/           # Standard library
├── docs/                  # Documentation
└── examples/              # Example programs
```

## Roadmap

### Current Status (Production Ready)
- ✅ Python syntax compatibility (100%)
- ✅ Parser - Full tuple return support, trailing commas
- ✅ VM interpreter - Optimized bytecode execution
- ✅ C code generation & native compilation
- ✅ Type inference and optimization
- ✅ HTTP/async/web modules (httpx, serveit, websockets)
- ✅ Standard library - 70+ modules available
- ✅ FFI support (C libraries, Win32 APIs)
- ✅ Memory management (manual, arena, atomic operations)
- ✅ System programming primitives (98% complete)
- ✅ Bare-metal/OS development support (97% complete)
- ✅ Concurrency - Threading, multiprocessing, atomics
- ✅ Network programming - Full stack support
- ✅ OOP - Classes, inheritance, MRO, properties

### Production Ready For:
- ✅ System Programming
- ✅ Operating System Development
- ✅ Kernel & Driver Development
- ✅ Embedded OS Development
- ✅ Network Services & Web Applications
- ✅ High-Performance Computing
- ✅ Real-time Applications

### Planned Features
- 🔄 LLVM backend for additional optimizations
- 🔄 WebAssembly target
- 🔄 Package manager
- 🔄 IDE integration (LSP)
- 🔄 Game development support (graphics/audio stack)
- 🔄 MCU HAL layers (Arduino, ESP32, STM32)

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
