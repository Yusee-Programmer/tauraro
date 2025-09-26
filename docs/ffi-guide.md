# FFI and Interoperability Guide

TauraroLang provides powerful Foreign Function Interface (FFI) capabilities, allowing seamless integration with C libraries, Python modules, and other programming languages. This guide covers everything you need to know about interoperability.

## Table of Contents

1. [Overview](#overview)
2. [C FFI](#c-ffi)
3. [Python Interoperability](#python-interoperability)
4. [JavaScript Integration](#javascript-integration)
5. [Rust Integration](#rust-integration)
6. [Memory Management](#memory-management)
7. [Error Handling](#error-handling)
8. [Performance Considerations](#performance-considerations)
9. [Best Practices](#best-practices)
10. [Troubleshooting](#troubleshooting)

## Overview

TauraroLang's FFI system enables:

- **C Library Integration**: Call C functions and use C data structures
- **Python Module Access**: Import and use Python libraries
- **JavaScript Interop**: Seamless web integration via WebAssembly
- **Rust Integration**: Direct integration with Rust crates
- **System API Access**: Platform-specific system calls
- **Database Connectivity**: Direct database driver integration

### FFI Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   TauraroLang   │    │   FFI Bridge    │    │  External Code  │
│                 │◄──►│                 │◄──►│                 │
│  - Functions    │    │  - Type Conv.   │    │  - C Libraries  │
│  - Data Types   │    │  - Memory Mgmt  │    │  - Python Mods  │
│  - Objects      │    │  - Error Handle │    │  - JS Functions │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## C FFI

### Basic C Integration

#### Declaring External Functions

```tauraro
// Declare external C functions
extern "C" {
    fn printf(format: string, ...) -> int
    fn malloc(size: int) -> ptr
    fn free(ptr: ptr)
    fn strlen(str: string) -> int
    fn strcmp(str1: string, str2: string) -> int
}

// Use C functions
fn main() {
    printf("Hello from C!\n")
    
    let text = "TauraroLang"
    let length = strlen(text)
    printf("Length of '%s': %d\n", text, length)
}
```

#### Type Mappings

| TauraroLang Type | C Type | Description |
|------------------|--------|-------------|
| `int` | `int32_t` | 32-bit signed integer |
| `long` | `int64_t` | 64-bit signed integer |
| `float` | `float` | 32-bit floating point |
| `double` | `double` | 64-bit floating point |
| `bool` | `bool` | Boolean value |
| `string` | `char*` | Null-terminated string |
| `ptr` | `void*` | Generic pointer |
| `array[T]` | `T*` | Array pointer |

### Advanced C Integration

#### Working with C Structures

**math_lib.h:**
```c
// C header file
typedef struct {
    double x, y;
} Point;

typedef struct {
    Point center;
    double radius;
} Circle;

Point create_point(double x, double y);
Circle create_circle(Point center, double radius);
double circle_area(Circle circle);
double distance(Point p1, Point p2);
```

**TauraroLang Integration:**
```tauraro
// Define corresponding structures
struct Point {
    x: double,
    y: double
}

struct Circle {
    center: Point,
    radius: double
}

// Declare external functions
extern "C" {
    fn create_point(x: double, y: double) -> Point
    fn create_circle(center: Point, radius: double) -> Circle
    fn circle_area(circle: Circle) -> double
    fn distance(p1: Point, p2: Point) -> double
}

// Use C structures and functions
fn main() {
    let p1 = create_point(0.0, 0.0)
    let p2 = create_point(3.0, 4.0)
    
    let circle = create_circle(p1, 5.0)
    let area = circle_area(circle)
    let dist = distance(p1, p2)
    
    printf("Circle area: %.2f\n", area)
    printf("Distance: %.2f\n", dist)
}
```

#### Memory Management with C

```tauraro
extern "C" {
    fn malloc(size: int) -> ptr
    fn free(ptr: ptr)
    fn memcpy(dest: ptr, src: ptr, size: int) -> ptr
}

fn allocate_buffer(size: int) -> ptr {
    let buffer = malloc(size)
    if buffer == null {
        print("Memory allocation failed!")
        return null
    }
    return buffer
}

fn safe_free(ptr: ptr) {
    if ptr != null {
        free(ptr)
    }
}

fn main() {
    let buffer = allocate_buffer(1024)
    if buffer != null {
        // Use buffer...
        safe_free(buffer)
    }
}
```

### C Library Integration Example

**Using SQLite Database:**

```tauraro
// SQLite FFI declarations
extern "C" {
    fn sqlite3_open(filename: string, db: ptr) -> int
    fn sqlite3_close(db: ptr) -> int
    fn sqlite3_exec(db: ptr, sql: string, callback: ptr, data: ptr, errmsg: ptr) -> int
    fn sqlite3_errmsg(db: ptr) -> string
}

class Database {
    fn init(filename: string) {
        self.db = null
        self.filename = filename
    }
    
    fn open() {
        let result = sqlite3_open(self.filename, &self.db)
        if result != 0 {
            print("Cannot open database: " + self.filename)
            return false
        }
        return true
    }
    
    fn close() {
        if self.db != null {
            sqlite3_close(self.db)
            self.db = null
        }
    }
    
    fn execute(sql: string) {
        let result = sqlite3_exec(self.db, sql, null, null, null)
        if result != 0 {
            let error = sqlite3_errmsg(self.db)
            print("SQL error: " + error)
            return false
        }
        return true
    }
}

fn main() {
    let db = Database("test.db")
    
    if db.open() {
        db.execute("CREATE TABLE users (id INTEGER, name TEXT)")
        db.execute("INSERT INTO users VALUES (1, 'Alice')")
        db.execute("INSERT INTO users VALUES (2, 'Bob')")
        db.close()
        print("Database operations completed")
    }
}
```

## Python Interoperability

### Basic Python Integration

#### Importing Python Modules

```tauraro
// Import Python modules
import python "math" as pymath
import python "json" as pyjson
import python "requests" as requests

fn main() {
    // Use Python math functions
    let result = pymath.sqrt(16.0)
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