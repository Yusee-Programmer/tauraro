# TauraroLang Examples

This directory contains comprehensive examples and tutorials demonstrating TauraroLang's features and capabilities. All examples use Python syntax with .tr file extensions.

## Directory Structure

```
examples/
├── README.md                    # This file
├── basic/                       # Basic language features
├── advanced/                    # Advanced programming concepts
├── backends/                    # Backend-specific examples
├── interop/                     # FFI and Python interoperability
├── async/                      # Asynchronous programming
├── web/                        # Web development examples
├── data-science/              # Data analysis and scientific computing
├── games/                     # Game development examples
├── cli-tools/                 # Command-line applications
└── tutorials/                 # Step-by-step tutorials
```

## Getting Started

### Prerequisites

1. **Install TauraroLang**: Follow the installation guide in the main README
2. **Enable Features**: Some examples require specific features:
   ```bash
   # For FFI examples
   cargo build --features ffi
   
   # For Python interop examples
   cargo build --features python-interop
   
   # For async examples
   cargo build --features async
   
   # For web examples
   cargo build --features "http,wasm"
   ```

### Running Examples

#### Using the Interpreter (Default)
```bash
# Run with the interpreter backend
tauraro run examples/basic/hello_world.tr

# Run with specific backend
tauraro run --backend vm examples/basic/variables.tr
```

#### Compiling Examples
```bash
# Compile to C
tauraro compile --backend c examples/basic/functions.tr -o output.c

# Compile to LLVM IR
tauraro compile --backend llvm examples/basic/classes.tr -o output.ll

# Compile to WebAssembly
tauraro compile --backend wasm examples/basic/data_structures.tr -o data_structures.wasm
```

## Example Categories

### Basic Examples (`basic/`)
- **Hello World**: Simple program introduction (`hello_world.tr`)
- **Variables and Types**: Variable declaration and type system (`variables.tr`)
- **Control Flow**: If statements, loops, and conditionals (`control_flow.tr`)
- **Functions**: Function definition and calling (`functions.tr`)
- **Classes and Objects**: Object-oriented programming (`classes.tr`)
- **Method Calls**: Object method calling and chaining (`method_calls.tr`)
- **Data Structures**: Arrays, dictionaries, tuples, and sets (`data_structures.tr`)
- **Error Handling**: Exception handling and error management (`error_handling.tr`)

### Advanced Examples (`advanced/`)
- **Object-Oriented Programming**: Classes, inheritance, and polymorphism
- **Functional Programming**: Higher-order functions, lambdas, and closures
- **Metaprogramming**: Dynamic code generation and reflection
- **Memory Management**: Manual memory control and optimization
- **Pattern Matching**: Advanced pattern matching techniques
- **Generics**: Generic programming and type parameters

### Backend Examples (`backends/`)
- **Interpreter**: VM-specific features and optimizations
- **C Transpiler**: C code generation and integration
- **LLVM**: Native code compilation and optimization
- **WebAssembly**: Browser integration and WASM features

### Interoperability Examples (`interop/`)
- **C FFI**: Calling C libraries and functions
- **Python Integration**: Bidirectional Python interop
- **System APIs**: Operating system integration
- **Database Connectivity**: Database drivers and ORM usage

### Asynchronous Programming (`async/`)
- **Basic Async/Await**: Asynchronous function basics
- **Concurrent Programming**: Multi-threading and parallelism
- **Network Programming**: HTTP clients and servers
- **Real-time Applications**: WebSocket and streaming examples

### Web Development (`web/`)
- **HTTP Servers**: Web server implementation
- **REST APIs**: RESTful service development
- **WebAssembly Integration**: Browser-based applications
- **Frontend Development**: Client-side programming

### Data Science (`data-science/`)
- **Data Analysis**: Statistical analysis and visualization
- **Machine Learning**: ML algorithms and model training
- **Scientific Computing**: Numerical computation examples
- **Data Processing**: ETL pipelines and data transformation

### Game Development (`games/`)
- **2D Games**: Simple 2D game examples
- **Game Engines**: Game engine integration
- **Graphics Programming**: Rendering and graphics examples
- **Physics Simulation**: Physics engine integration

### CLI Tools (`cli-tools/`)
- **Command-Line Parsers**: Argument parsing and CLI design
- **File Processing**: Text processing and file manipulation
- **System Utilities**: System administration tools
- **Development Tools**: Build tools and code generators

### Tutorials (`tutorials/`)
- **Getting Started**: Step-by-step introduction
- **Language Tour**: Comprehensive language overview
- **Best Practices**: Coding standards and patterns
- **Performance Optimization**: Optimization techniques
- **Deployment Guide**: Production deployment strategies

## Contributing Examples

We welcome contributions of new examples! Please follow these guidelines:

1. **Clear Documentation**: Include comprehensive comments and README files
2. **Working Code**: Ensure all examples compile and run correctly
3. **Multiple Backends**: Test with different compilation backends when applicable
4. **Error Handling**: Include proper error handling and edge cases
5. **Performance**: Consider performance implications and optimizations

### Example Template

```python
#!/usr/bin/env tauraro
"""
Example: [Brief Description]

This example demonstrates [detailed description of what it shows].

Features demonstrated:
- Feature 1
- Feature 2
- Feature 3

Usage:
    tauraro run example_name.tr
    
Requirements:
    - TauraroLang with [required features]
    - [Any external dependencies]
"""

# Your example code here
def main():
    print("Hello, TauraroLang!")

if __name__ == "__main__":
    main()
```

## Learning Path

For beginners, we recommend following this learning path:

1. **Start with Basics**: `basic/hello_world.tr` → `basic/variables.tr` → `basic/functions.tr`
2. **Control Flow**: `basic/control_flow.tr`
3. **Data Structures**: `basic/data_structures.tr`
4. **Object-Oriented**: `basic/classes.tr` → `basic/method_calls.tr`
5. **Error Handling**: `basic/error_handling.tr`
6. **Advanced Features**: Choose based on your interests (async, web, data science, etc.)

## Performance Benchmarks

Many examples include performance benchmarks comparing different backends:

```bash
# Run benchmarks
cd benchmarks/
python run_benchmarks.py

# Compare backends
tauraro run --backend vm examples/basic/fibonacci.tr
tauraro compile --backend llvm examples/basic/fibonacci.tr && ./fibonacci
```

## Troubleshooting

### Common Issues

1. **Feature Not Enabled**: Ensure required features are enabled during compilation
2. **Missing Dependencies**: Install required system libraries for FFI examples
3. **Python Not Found**: Ensure Python is installed for interop examples
4. **Permission Errors**: Check file permissions for file I/O examples

### Getting Help

- **Documentation**: Check the `docs/` directory for detailed guides
- **Issues**: Report bugs or ask questions on GitHub
- **Community**: Join community discussions for help and tips

---

Happy coding with TauraroLang! 🚀