# FFI and Python Interoperability Guide

This guide covers TauraroLang's Foreign Function Interface (FFI) system and Python interoperability features, enabling seamless integration with C libraries and Python modules.

## Table of Contents

1. [Overview](#overview)
2. [FFI System](#ffi-system)
3. [Python Interoperability](#python-interoperability)
4. [Type Conversion](#type-conversion)
5. [Memory Management](#memory-management)
6. [Error Handling](#error-handling)
7. [Best Practices](#best-practices)
8. [Examples](#examples)
9. [Troubleshooting](#troubleshooting)

## Overview

TauraroLang provides two main interoperability mechanisms:

1. **FFI (Foreign Function Interface)**: Direct integration with C libraries and native code
2. **Python Interoperability**: Bidirectional integration with Python modules and functions

Both systems are designed with safety and performance in mind, providing automatic memory management and type conversion while maintaining native performance.

### Feature Availability

FFI and Python interoperability are optional features that can be enabled during compilation:

```bash
# Enable FFI support
cargo build --features ffi

# Enable Python interoperability
cargo build --features python-interop

# Enable both
cargo build --features "ffi,python-interop"
```

## FFI System

The FFI system allows TauraroLang to call functions from C libraries and native code directly.

### Architecture

```
TauraroLang Code
       ↓
   FFI Manager
       ↓
  Type Conversion
       ↓
   C Function Call
       ↓
  Result Conversion
       ↓
  TauraroLang Value
```

### Core Components

#### FFI Types

TauraroLang supports the following FFI types for C interoperability:

```rust
enum FFIType {
    Void,           // void
    Int8,           // int8_t
    Int16,          // int16_t
    Int32,          // int32_t
    Int64,          // int64_t
    UInt8,          // uint8_t
    UInt16,         // uint16_t
    UInt32,         // uint32_t
    UInt64,         // uint64_t
    Float32,        // float
    Float64,        // double
    Bool,           // bool
    Pointer,        // void*
    String,         // const char*
    Buffer,         // Binary data
}
```

#### Calling Conventions

Supported calling conventions:

- **C**: Standard C ABI (default)
- **StdCall**: Windows stdcall convention
- **FastCall**: Fast call convention
- **System**: System default convention

### Loading External Libraries

#### Basic Library Loading

```python
# Load a shared library
import ffi

# Load library (platform-specific extension automatically detected)
lib = ffi.load_library("mylib")  # Loads mylib.dll/.so/.dylib

# Register functions with their signatures
lib.register_function("add", [ffi.Int32, ffi.Int32], ffi.Int32)
lib.register_function("sqrt", [ffi.Float64], ffi.Float64)
lib.register_function("strlen", [ffi.String], ffi.Int32)

# Call functions
result = lib.call("add", [10, 20])  # Returns 30
length = lib.call("strlen", ["Hello"])  # Returns 5
```

#### Function Registration

Functions must be registered with their signatures before calling:

```python
# Register a function with signature
lib.register_function(
    name="function_name",
    param_types=[ffi.Int32, ffi.String],  # Parameter types
    return_type=ffi.Float64,              # Return type
    calling_convention=ffi.CallingConvention.C  # Optional
)
```

#### Safe FFI Calls

The FFI system provides memory-safe function calls:

```python
# Safe FFI call with automatic cleanup
with ffi.SafeFFI() as safe_ffi:
    lib = safe_ffi.load_library("mylib")
    lib.register_function("process_data", [ffi.Buffer], ffi.Int32)
    
    data = b"Hello, World!"
    result = lib.call("process_data", [data])
    # Library automatically unloaded and memory cleaned up
```

### Built-in C Functions

TauraroLang provides built-in support for common C standard library functions:

```python
# These functions are automatically available
print("Hello, World!")           # Calls printf internally
result = malloc(1024)            # Allocate memory
free(result)                     # Free memory

# String functions
length = strlen("test")          # Get string length
comparison = strcmp("a", "b")    # Compare strings
```

## Python Interoperability

TauraroLang provides bidirectional integration with Python, allowing you to call Python functions from TauraroLang and vice versa.

### Architecture

```
TauraroLang ←→ Python Bridge ←→ Python Runtime
     ↑              ↑                  ↑
   Values      Type Conversion    Python Objects
```

### Core Components

#### PythonInterop Manager

The `PythonInterop` manager handles all Python integration:

```python
# Initialize Python interoperability
python = PythonInterop.new()

# Import Python modules
numpy = python.import_module("numpy")
pandas = python.import_module("pandas")

# Call Python functions
result = python.call_function("numpy.array", [[1, 2, 3, 4]])
```

#### Python Integration in VM

The TauraroLang VM can be extended with Python modules:

```python
# Create VM with Python integration
vm = TauraroVM.new()
integration = PythonIntegration.new()

# Add Python modules to VM
integration.add_module("math", python.import_module("math"))
integration.add_module("json", python.import_module("json"))

vm.set_python_integration(integration)
```

### Using Python from TauraroLang

#### Importing Python Modules

```python
# Import standard library modules
import math
import json
import os

# Import third-party packages
import numpy as np
import pandas as pd
import requests

# Use Python functions
result = math.sqrt(16)           # Returns 4.0
data = json.loads('{"key": "value"}')
files = os.listdir(".")
```

#### Calling Python Functions

```python
# Call Python functions with arguments
def python_function(a, b, c=None):
    return a + b + (c or 0)

# From TauraroLang
result = python_function(1, 2, c=3)  # Returns 6

# Call with keyword arguments
result = python_function(a=10, b=20)  # Returns 30
```

#### Working with Python Objects

```python
# Create and manipulate Python objects
data = {
    "name": "TauraroLang",
    "version": "1.0",
    "features": ["FFI", "Python", "WebAssembly"]
}

# Access object properties
name = data["name"]
features = data["features"]

# Call object methods
features.append("LLVM")
length = len(features)
```

### Using TauraroLang from Python

#### PyO3 Bindings

TauraroLang provides Python bindings through PyO3:

```python
import tauraro

# Evaluate TauraroLang expressions
result = tauraro.eval("2 + 3 * 4")  # Returns 14

# Execute TauraroLang code
tauraro.exec("""
    fn fibonacci(n):
        if n <= 1:
            return n
        return fibonacci(n-1) + fibonacci(n-2)
""")

# Call TauraroLang functions
result = tauraro.call("fibonacci", [10])  # Returns 55
```

#### TauraroVM Class

```python
# Create and manage TauraroLang VM from Python
vm = tauraro.TauraroVM()

# Load and execute TauraroLang code
vm.load_file("script.tr")
vm.execute()

# Get values from VM
result = vm.get_variable("result")

# Set values in VM
vm.set_variable("input_data", [1, 2, 3, 4, 5])
```

## Type Conversion

### Automatic Type Conversion

TauraroLang automatically converts between TauraroLang and external types:

#### TauraroLang ↔ C Conversion

| TauraroLang | C Type | Notes |
|-------------|--------|-------|
| `int` | `int32_t` | 32-bit signed integer |
| `long` | `int64_t` | 64-bit signed integer |
| `float` | `float` | 32-bit floating point |
| `double` | `double` | 64-bit floating point |
| `bool` | `bool` | Boolean value |
| `string` | `const char*` | UTF-8 encoded, null-terminated |
| `array[T]` | `T*` | Contiguous memory layout |
| `ptr` | `void*` | Raw pointer |

#### TauraroLang ↔ Python Conversion

| TauraroLang | Python | Notes |
|-------------|--------|-------|
| `int` | `int` | Arbitrary precision in Python |
| `float` | `float` | 64-bit floating point |
| `bool` | `bool` | Boolean value |
| `string` | `str` | Unicode string |
| `array` | `list` | Dynamic array |
| `map` | `dict` | Key-value mapping |
| `null` | `None` | Null/None value |

### Manual Type Conversion

For complex types, manual conversion may be required:

```python
# Convert TauraroLang array to C buffer
tauraro_array = [1, 2, 3, 4, 5]
c_buffer = ffi.array_to_buffer(tauraro_array, ffi.Int32)

# Convert C string to TauraroLang string
c_string = lib.call("get_string", [])
tauraro_string = ffi.c_string_to_string(c_string)

# Convert Python object to TauraroLang value
python_dict = {"key": "value", "number": 42}
tauraro_map = python.to_tauraro_value(python_dict)
```
## Memory Management

### Automatic Memory Management

TauraroLang provides automatic memory management for FFI and Python interoperability:

#### FFI Memory Management

```python
# Automatic cleanup with SafeFFI
with ffi.SafeFFI() as safe_ffi:
    lib = safe_ffi.load_library("mylib")
    # All allocated memory is automatically freed when exiting the context
    
# Manual memory management
lib = ffi.load_library("mylib")
try:
    # Use library functions
    result = lib.call("process_data", [data])
finally:
    lib.unload()  # Explicitly unload library
```

#### Python Memory Management

```python
# Python objects are automatically managed by Python's garbage collector
python = PythonInterop.new()
numpy_array = python.call_function("numpy.array", [[1, 2, 3]])
# numpy_array is automatically cleaned up when no longer referenced
```

### Manual Memory Management

For performance-critical applications, manual memory management is available:

```python
# Allocate and manage C memory manually
buffer = ffi.allocate(1024, ffi.UInt8)
try:
    # Use buffer
    lib.call("process_buffer", [buffer])
finally:
    ffi.deallocate(buffer)  # Must manually free
```

## Error Handling

### FFI Error Handling

```python
try:
    lib = ffi.load_library("nonexistent_lib")
except ffi.LibraryLoadError as e:
    print(f"Failed to load library: {e}")

try:
    result = lib.call("undefined_function", [])
except ffi.FunctionNotFoundError as e:
    print(f"Function not found: {e}")
except ffi.FFICallError as e:
    print(f"FFI call failed: {e}")
```

### Python Error Handling

```python
try:
    python = PythonInterop.new()
    result = python.call_function("math.sqrt", [-1])
except python.PythonError as e:
    print(f"Python error: {e}")
except python.ImportError as e:
    print(f"Failed to import module: {e}")
```

### Error Recovery

```python
# Graceful error recovery
def safe_ffi_call(lib, function_name, args):
    try:
        return lib.call(function_name, args)
    except ffi.FFICallError:
        # Log error and return default value
        print(f"FFI call to {function_name} failed, using default")
        return None

# Retry mechanism
def retry_python_call(python, function_name, args, max_retries=3):
    for attempt in range(max_retries):
        try:
            return python.call_function(function_name, args)
        except python.PythonError as e:
            if attempt == max_retries - 1:
                raise e
            print(f"Attempt {attempt + 1} failed, retrying...")
            time.sleep(0.1)
```

## Best Practices

### FFI Best Practices

1. **Always Register Function Signatures**
   ```python
   # Good: Register with proper types
   lib.register_function("add", [ffi.Int32, ffi.Int32], ffi.Int32)
   
   # Bad: Calling without registration may cause crashes
   result = lib.call("add", [1, 2])  # Unsafe!
   ```

2. **Use Safe FFI Context**
   ```python
   # Good: Automatic cleanup
   with ffi.SafeFFI() as safe_ffi:
       lib = safe_ffi.load_library("mylib")
       # Use library safely
   
   # Acceptable: Manual cleanup
   lib = ffi.load_library("mylib")
   try:
       # Use library
       pass
   finally:
       lib.unload()
   ```

3. **Validate Input Parameters**
   ```python
   def safe_call(lib, func_name, args):
       # Validate arguments before FFI call
       if not all(isinstance(arg, (int, float, str)) for arg in args):
           raise ValueError("Invalid argument types")
       
       return lib.call(func_name, args)
   ```

4. **Handle Platform Differences**
   ```python
   import platform
   
   if platform.system() == "Windows":
       lib_name = "mylib.dll"
   elif platform.system() == "Darwin":
       lib_name = "libmylib.dylib"
   else:
       lib_name = "libmylib.so"
   
   lib = ffi.load_library(lib_name)
   ```

### Python Interoperability Best Practices

1. **Import Modules Once**
   ```python
   # Good: Import once and reuse
   python = PythonInterop.new()
   numpy = python.import_module("numpy")
   
   # Use numpy multiple times
   array1 = python.call_function("numpy.array", [[1, 2, 3]])
   array2 = python.call_function("numpy.array", [[4, 5, 6]])
   ```

2. **Handle Python Exceptions**
   ```python
   try:
       result = python.call_function("risky_function", [args])
   except python.PythonError as e:
       # Handle Python-specific errors
       print(f"Python error: {e}")
   except Exception as e:
       # Handle other errors
       print(f"Unexpected error: {e}")
   ```

3. **Use Type Hints**
   ```python
   def call_python_function(python: PythonInterop, 
                           func_name: str, 
                           args: list) -> any:
       """Call a Python function with proper error handling."""
       try:
           return python.call_function(func_name, args)
       except python.PythonError as e:
           print(f"Failed to call {func_name}: {e}")
           return None
   ```

## Examples

### Complete FFI Example: Image Processing

```python
# Load image processing library
lib = ffi.load_library("libimageproc")

# Register functions
lib.register_function("load_image", [ffi.String], ffi.Pointer)
lib.register_function("resize_image", [ffi.Pointer, ffi.Int32, ffi.Int32], ffi.Pointer)
lib.register_function("save_image", [ffi.Pointer, ffi.String], ffi.Bool)
lib.register_function("free_image", [ffi.Pointer], ffi.Void)

def process_image(input_path, output_path, width, height):
    """Process an image using C library."""
    image = None
    resized = None
    
    try:
        # Load image
        image = lib.call("load_image", [input_path])
        if not image:
            raise RuntimeError("Failed to load image")
        
        # Resize image
        resized = lib.call("resize_image", [image, width, height])
        if not resized:
            raise RuntimeError("Failed to resize image")
        
        # Save image
        success = lib.call("save_image", [resized, output_path])
        if not success:
            raise RuntimeError("Failed to save image")
        
        print(f"Successfully processed {input_path} -> {output_path}")
        
    finally:
        # Clean up memory
        if resized:
            lib.call("free_image", [resized])
        if image:
            lib.call("free_image", [image])

# Usage
process_image("input.jpg", "output.jpg", 800, 600)
```

### Complete Python Interoperability Example: Data Analysis

```python
# Initialize Python interoperability
python = PythonInterop.new()

# Import required modules
pandas = python.import_module("pandas")
numpy = python.import_module("numpy")
matplotlib = python.import_module("matplotlib.pyplot")

def analyze_data(csv_file):
    """Analyze data using Python libraries."""
    try:
        # Load data with pandas
        df = python.call_function("pandas.read_csv", [csv_file])
        
        # Get basic statistics
        stats = python.call_function("df.describe", [])
        print("Data Statistics:")
        print(stats)
        
        # Calculate correlation matrix
        corr = python.call_function("df.corr", [])
        
        # Create visualization
        python.call_function("matplotlib.pyplot.figure", [{"figsize": (10, 8)}])
        python.call_function("matplotlib.pyplot.imshow", [corr, {"cmap": "coolwarm"}])
        python.call_function("matplotlib.pyplot.colorbar", [])
        python.call_function("matplotlib.pyplot.title", ["Correlation Matrix"])
        python.call_function("matplotlib.pyplot.savefig", ["correlation.png"])
        
        print("Analysis complete. Correlation matrix saved as correlation.png")
        
        return {
            "stats": stats,
            "correlation": corr
        }
        
    except python.PythonError as e:
        print(f"Python error during analysis: {e}")
        return None

# Usage
results = analyze_data("data.csv")
if results:
    print("Analysis successful!")
```

## Troubleshooting

### Common FFI Issues

1. **Library Not Found**
   ```
   Error: LibraryLoadError: Cannot load library 'mylib'
   
   Solutions:
   - Check library path and filename
   - Ensure library is in system PATH or LD_LIBRARY_PATH
   - Use absolute path to library file
   - Verify library architecture matches (32-bit vs 64-bit)
   ```

2. **Function Not Found**
   ```
   Error: FunctionNotFoundError: Function 'my_function' not found
   
   Solutions:
   - Check function name spelling
   - Verify function is exported from library
   - Use nm/objdump (Linux) or dumpbin (Windows) to list exports
   - Check for C++ name mangling
   ```

3. **Type Mismatch**
   ```
   Error: FFICallError: Type mismatch in function call
   
   Solutions:
   - Verify parameter types match C function signature
   - Check return type registration
   - Ensure proper type conversion
   - Review calling convention
   ```

4. **Memory Access Violations**
   ```
   Error: Segmentation fault / Access violation
   
   Solutions:
   - Check pointer validity before dereferencing
   - Ensure proper memory allocation/deallocation
   - Verify buffer sizes
   - Use SafeFFI context for automatic cleanup
   ```

### Common Python Interoperability Issues

1. **Module Import Errors**
   ```
   Error: ImportError: No module named 'numpy'
   
   Solutions:
   - Install required Python packages: pip install numpy
   - Check Python environment and PATH
   - Verify Python version compatibility
   - Use virtual environments for isolation
   ```

2. **Type Conversion Errors**
   ```
   Error: PythonError: Cannot convert TauraroLang value to Python
   
   Solutions:
   - Check supported type conversions
   - Use manual conversion for complex types
   - Verify data structure compatibility
   - Handle None/null values properly
   ```

3. **Python Exception Propagation**
   ```
   Error: PythonError: ZeroDivisionError: division by zero
   
   Solutions:
   - Wrap Python calls in try-catch blocks
   - Validate input parameters before Python calls
   - Handle Python exceptions gracefully
   - Use error recovery mechanisms
   ```

### Performance Issues

1. **Slow FFI Calls**
   - Minimize FFI call frequency
   - Batch operations when possible
   - Use appropriate data types
   - Consider caching results

2. **Memory Leaks**
   - Always pair malloc/free calls
   - Use SafeFFI for automatic cleanup
   - Monitor memory usage during development
   - Implement proper error handling

3. **Python Performance**
   - Minimize Python/TauraroLang context switches
   - Use NumPy for numerical operations
   - Cache imported modules
   - Consider using compiled Python extensions

### Debugging Tips

1. **Enable Debug Logging**
   ```python
   # Enable FFI debug logging
   ffi.set_debug_level(ffi.DEBUG_VERBOSE)
   
   # Enable Python interop debugging
   python.set_debug_mode(True)
   ```

2. **Use Memory Debugging Tools**
   - Valgrind (Linux/macOS)
   - AddressSanitizer
   - Windows Application Verifier
   - Custom memory tracking

3. **Test with Simple Cases**
   - Start with basic function calls
   - Gradually increase complexity
   - Test error conditions
   - Verify cleanup behavior

This comprehensive guide covers all aspects of FFI and Python interoperability in TauraroLang. For additional help, consult the API reference or community forums.
    print("Square root of 16: " + str(result))
    
    // Use Python JSON
    let data = {name: "Alice", age: 25}
    let json_str = pyjson.dumps(data)
    print("JSON: " + json_str)
    
    // Parse JSON back
    let parsed = pyjson.loads(json_str)
    print("Name: " + parsed.name)
}
```

#### Python Function Calls

```tauraro
import python "os" as os
import python "sys" as sys
import python "datetime" as datetime

fn system_info() {
    print("Python version: " + sys.version)
    print("Current directory: " + os.getcwd())
    
    let now = datetime.datetime.now()
    print("Current time: " + str(now))
}

fn file_operations() {
    // Create directory
    os.makedirs("temp_dir", exist_ok=true)
    
    // List files
    let files = os.listdir(".")
    print("Files in current directory:")
    for file in files {
        print("  " + file)
    }
}
```

### Advanced Python Integration

#### Using NumPy for Numerical Computing

```tauraro
import python "numpy" as np
import python "matplotlib.pyplot" as plt

fn data_analysis() {
    // Create NumPy arrays
    let data = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
    let squared = np.power(data, 2)
    
    print("Original data: " + str(data))
    print("Squared data: " + str(squared))
    
    // Statistical operations
    let mean = np.mean(data)
    let std = np.std(data)
    
    print("Mean: " + str(mean))
    print("Standard deviation: " + str(std))
}

fn create_plot() {
    let x = np.linspace(0, 10, 100)
    let y = np.sin(x)
    
    plt.figure(figsize=(10, 6))
    plt.plot(x, y, label="sin(x)")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title("Sine Wave")
    plt.legend()
    plt.grid(true)
    plt.savefig("sine_wave.png")
    plt.show()
}
```

#### Machine Learning with scikit-learn

```tauraro
import python "sklearn.datasets" as datasets
import python "sklearn.model_selection" as model_selection
import python "sklearn.linear_model" as linear_model
import python "sklearn.metrics" as metrics

fn machine_learning_example() {
    // Load dataset
    let iris = datasets.load_iris()
    let X = iris.data
    let y = iris.target
    
    // Split data
    let split_result = model_selection.train_test_split(
        X, y, test_size=0.2, random_state=42
    )
    let X_train = split_result[0]
    let X_test = split_result[1]
    let y_train = split_result[2]
    let y_test = split_result[3]
    
    // Train model
    let model = linear_model.LogisticRegression()
    model.fit(X_train, y_train)
    
    // Make predictions
    let predictions = model.predict(X_test)
    let accuracy = metrics.accuracy_score(y_test, predictions)
    
    print("Model accuracy: " + str(accuracy))
}
```

### Python Class Integration

```tauraro
// Define Python class in TauraroLang
python_class DataProcessor {
    fn __init__(self, data) {
        self.data = data
        self.processed = false
    }
    
    fn process(self) {
        // Python code embedded in TauraroLang
        python {
            import pandas as pd
            self.df = pd.DataFrame(self.data)
            self.df['processed'] = True
            self.processed = True
        }
    }
    
    fn get_summary(self) {
        if not self.processed {
            self.process()
        }
        
        python {
            return self.df.describe().to_dict()
        }
    }
}

fn main() {
    let data = [
        {name: "Alice", age: 25, salary: 50000},
        {name: "Bob", age: 30, salary: 60000},
        {name: "Charlie", age: 35, salary: 70000}
    ]
    
    let processor = DataProcessor(data)
    let summary = processor.get_summary()
    
    print("Data summary:")
    print(str(summary))
}
```

## JavaScript Integration

### WebAssembly JavaScript Bindings

```tauraro
// Export functions to JavaScript
export fn calculate_fibonacci(n: int) -> int {
    if n <= 1 {
        return n
    }
    return calculate_fibonacci(n - 1) + calculate_fibonacci(n - 2)
}

export fn process_array(data: array[int]) -> array[int] {
    let result = []
    for item in data {
        result = result + [item * 2]
    }
    return result
}

// Import JavaScript functions
import js fn console_log(message: string)
import js fn fetch_data(url: string) -> Promise[string]
import js fn set_timeout(callback: function, delay: int)

fn main() {
    console_log("TauraroLang running in browser!")
    
    // Async operation
    let data_promise = fetch_data("https://api.example.com/data")
    // Handle promise in JavaScript context
}
```

### DOM Manipulation

```tauraro
// DOM API bindings
import js fn document_get_element_by_id(id: string) -> Element
import js fn document_create_element(tag: string) -> Element
import js fn element_set_text_content(element: Element, text: string)
import js fn element_append_child(parent: Element, child: Element)

fn create_todo_item(text: string) {
    let item = document_create_element("li")
    element_set_text_content(item, text)
    
    let list = document_get_element_by_id("todo-list")
    element_append_child(list, item)
}

export fn add_todo(text: string) {
    create_todo_item(text)
    console_log("Added todo: " + text)
}
```

## Rust Integration

### Calling Rust Functions

```tauraro
// Rust FFI declarations
extern "rust" {
    fn rust_fibonacci(n: u32) -> u64
    fn rust_sort_array(data: &mut [i32])
    fn rust_hash_string(input: &str) -> u64
}

fn performance_comparison() {
    let n = 40
    
    // TauraroLang implementation
    let start_time = get_time()
    let tauraro_result = fibonacci(n)
    let tauraro_time = get_time() - start_time
    
    // Rust implementation
    let start_time = get_time()
    let rust_result = rust_fibonacci(n)
    let rust_time = get_time() - start_time
    
    print("TauraroLang result: " + str(tauraro_result) + " (time: " + str(tauraro_time) + "ms)")
    print("Rust result: " + str(rust_result) + " (time: " + str(rust_time) + "ms)")
}
```

### Rust Struct Integration

```rust
// Rust side (lib.rs)
#[repr(C)]
pub struct Point {
    pub x: f64,
    pub y: f64,
}

#[no_mangle]
pub extern "C" fn create_point(x: f64, y: f64) -> Point {
    Point { x, y }
}

#[no_mangle]
pub extern "C" fn distance(p1: Point, p2: Point) -> f64 {
    ((p1.x - p2.x).powi(2) + (p1.y - p2.y).powi(2)).sqrt()
}
```

```tauraro
// TauraroLang side
struct Point {
    x: double,
    y: double
}

extern "rust" {
    fn create_point(x: double, y: double) -> Point
    fn distance(p1: Point, p2: Point) -> double
}

fn main() {
    let p1 = create_point(0.0, 0.0)
    let p2 = create_point(3.0, 4.0)
    let dist = distance(p1, p2)
    
    print("Distance: " + str(dist))  // Output: Distance: 5.0
}
```

## Memory Management

### Cross-Language Memory Safety

#### Ownership and Borrowing

```tauraro
// Safe memory management across FFI boundaries
fn safe_string_processing() {
    let text = "Hello, World!"
    
    // Pass string to C function safely
    let length = strlen(text)  // C function receives copy
    
    // Allocate memory in C, manage in TauraroLang
    let buffer = malloc(1024)
    defer free(buffer)  // Automatic cleanup
    
    // Use buffer safely
    if buffer != null {
        memcpy(buffer, text, length)
    }
}
```

#### Reference Counting

```tauraro
class ManagedResource {
    fn init(size: int) {
        self.data = malloc(size)
        self.size = size
        self.ref_count = 1
    }
    
    fn acquire() {
        self.ref_count = self.ref_count + 1
        return self
    }
    
    fn release() {
        self.ref_count = self.ref_count - 1
        if self.ref_count == 0 {
            free(self.data)
            self.data = null
        }
    }
}

fn resource_sharing() {
    let resource = ManagedResource(1024)
    
    // Share with C function
    let shared = resource.acquire()
    c_function_that_uses_resource(shared.data)
    shared.release()
    
    resource.release()  // Final cleanup
}
```

### Memory Pools

```tauraro
class MemoryPool {
    fn init(block_size: int, block_count: int) {
        self.block_size = block_size
        self.total_size = block_size * block_count
        self.pool = malloc(self.total_size)
        self.free_blocks = []
        
        // Initialize free block list
        let i = 0
        while i < block_count {
            let block_ptr = self.pool + (i * block_size)
            self.free_blocks = self.free_blocks + [block_ptr]
            i = i + 1
        }
    }
    
    fn allocate() -> ptr {
        if len(self.free_blocks) == 0 {
            return null
        }
        
        let block = self.free_blocks[0]
        self.free_blocks = self.free_blocks[1:]
        return block
    }
    
    fn deallocate(ptr: ptr) {
        self.free_blocks = self.free_blocks + [ptr]
    }
    
    fn destroy() {
        free(self.pool)
        self.pool = null
    }
}
```

## Error Handling

### FFI Error Patterns

#### C Error Handling

```tauraro
extern "C" {
    fn fopen(filename: string, mode: string) -> ptr
    fn fclose(file: ptr) -> int
    fn fread(buffer: ptr, size: int, count: int, file: ptr) -> int
    fn ferror(file: ptr) -> int
}

fn safe_file_read(filename: string) -> {success: bool, data: string, error: string} {
    let file = fopen(filename, "r")
    if file == null {
        return {
            success: false,
            data: "",
            error: "Could not open file: " + filename
        }
    }
    
    defer fclose(file)  // Ensure file is closed
    
    let buffer = malloc(1024)
    if buffer == null {
        return {
            success: false,
            data: "",
            error: "Memory allocation failed"
        }
    }
    
    defer free(buffer)  // Ensure memory is freed
    
    let bytes_read = fread(buffer, 1, 1024, file)
    if ferror(file) != 0 {
        return {
            success: false,
            data: "",
            error: "File read error"
        }
    }
    
    let data = string_from_buffer(buffer, bytes_read)
    return {
        success: true,
        data: data,
        error: ""
    }
}
```

#### Python Exception Handling

```tauraro
import python "json" as json

fn safe_json_parse(json_string: string) -> {success: bool, data: object, error: string} {
    try {
        let data = json.loads(json_string)
        return {
            success: true,
            data: data,
            error: ""
        }
    } catch python.JSONDecodeError as e {
        return {
            success: false,
            data: {},
            error: "JSON parse error: " + str(e)
        }
    } catch Exception as e {
        return {
            success: false,
            data: {},
            error: "Unexpected error: " + str(e)
        }
    }
}
```

## Performance Considerations

### FFI Call Overhead

```tauraro
// Minimize FFI calls in loops
fn inefficient_approach(data: array[int]) {
    for item in data {
        // FFI call in loop - expensive!
        let result = c_process_single_item(item)
        print(str(result))
    }
}

fn efficient_approach(data: array[int]) {
    // Single FFI call for entire array
    let results = c_process_array(data, len(data))
    for result in results {
        print(str(result))
    }
}
```

### Data Marshaling Optimization

```tauraro
// Efficient data transfer
fn optimized_data_transfer() {
    let large_array = range(0, 1000000)
    
    // Convert to C-compatible format once
    let c_array = to_c_array(large_array)
    defer free_c_array(c_array)
    
    // Multiple operations on same data
    let sum = c_calculate_sum(c_array, len(large_array))
    let avg = c_calculate_average(c_array, len(large_array))
    let max_val = c_find_maximum(c_array, len(large_array))
    
    print("Sum: " + str(sum))
    print("Average: " + str(avg))
    print("Maximum: " + str(max_val))
}
```

### Async FFI Operations

```tauraro
// Asynchronous FFI calls
async fn fetch_data_async(url: string) -> string {
    return await python_async {
        import aiohttp
        import asyncio
        
        async with aiohttp.ClientSession() as session:
            async with session.get(url) as response:
                return await response.text()
    }
}

async fn main() {
    let urls = [
        "https://api.example1.com/data",
        "https://api.example2.com/data",
        "https://api.example3.com/data"
    ]
    
    let tasks = []
    for url in urls {
        tasks = tasks + [fetch_data_async(url)]
    }
    
    let results = await gather(tasks)
    for result in results {
        print("Received: " + result[:100] + "...")
    }
}
```

## Best Practices

### 1. Type Safety

```tauraro
// Use strong typing for FFI interfaces
struct FFIResult[T] {
    success: bool,
    value: T,
    error_code: int,
    error_message: string
}

fn safe_ffi_call[T](operation: function() -> T) -> FFIResult[T] {
    try {
        let result = operation()
        return FFIResult[T] {
            success: true,
            value: result,
            error_code: 0,
            error_message: ""
        }
    } catch FFIError as e {
        return FFIResult[T] {
            success: false,
            value: default[T](),
            error_code: e.code,
            error_message: e.message
        }
    }
}
```

### 2. Resource Management

```tauraro
// RAII pattern for FFI resources
class FFIResource {
    fn init(resource_id: string) {
        self.handle = ffi_acquire_resource(resource_id)
        self.acquired = (self.handle != null)
    }
    
    fn use_resource() -> bool {
        if not self.acquired {
            return false
        }
        
        return ffi_use_resource(self.handle)
    }
    
    fn __del__() {
        if self.acquired {
            ffi_release_resource(self.handle)
            self.acquired = false
        }
    }
}
```

### 3. Error Propagation

```tauraro
// Consistent error handling across FFI boundaries
enum FFIError {
    None,
    InvalidArgument(string),
    ResourceNotFound(string),
    PermissionDenied(string),
    NetworkError(string),
    UnknownError(string)
}

fn handle_ffi_result[T](result: FFIResult[T]) -> T {
    match result.error_code {
        0 => return result.value,
        1 => throw FFIError.InvalidArgument(result.error_message),
        2 => throw FFIError.ResourceNotFound(result.error_message),
        3 => throw FFIError.PermissionDenied(result.error_message),
        4 => throw FFIError.NetworkError(result.error_message),
        _ => throw FFIError.UnknownError(result.error_message)
    }
}
```

### 4. Testing FFI Code

```tauraro
// Mock FFI functions for testing
#[cfg(test)]
extern "C" {
    fn mock_c_function(input: int) -> int
}

#[cfg(not(test))]
extern "C" {
    fn real_c_function(input: int) -> int
}

fn c_function(input: int) -> int {
    #[cfg(test)]
    return mock_c_function(input)
    
    #[cfg(not(test))]
    return real_c_function(input)
}

#[test]
fn test_ffi_integration() {
    let result = c_function(42)
    assert_eq(result, 84)  // Mock returns input * 2
}
```

## Troubleshooting

### Common Issues

#### 1. Symbol Not Found
```
Error: undefined symbol: my_function
```

**Solutions:**
- Check library linking: `--link-lib mylib`
- Verify function name mangling
- Ensure library is in search path

#### 2. Type Mismatch
```
Error: type mismatch in FFI call
```

**Solutions:**
- Verify type mappings
- Check struct alignment
- Use explicit type conversions

#### 3. Memory Corruption
```
Error: segmentation fault in FFI call
```

**Solutions:**
- Check pointer validity
- Verify memory ownership
- Use memory debugging tools

### Debugging FFI Code

```tauraro
// Enable FFI debugging
#[debug_ffi]
fn debug_ffi_call() {
    let result = some_c_function(42)
    print("FFI call result: " + str(result))
}

// Trace FFI calls
fn traced_ffi_operations() {
    ffi_trace_enable()
    
    let file = fopen("test.txt", "r")
    let data = fread_string(file, 1024)
    fclose(file)
    
    ffi_trace_disable()
    ffi_trace_print()
}
```

### Performance Profiling

```bash
# Profile FFI performance
tauraro run program.tr --profile-ffi

# Generate FFI call statistics
tauraro run program.tr --ffi-stats

# Memory usage analysis
tauraro run program.tr --memory-profile
```

---

This comprehensive FFI guide covers all aspects of interoperability in TauraroLang. Use these patterns and best practices to build robust, efficient applications that leverage the power of multiple programming languages and libraries.