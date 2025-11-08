# Memory Management

Tauraro provides flexible memory management with three strategies: automatic, manual, and arena-based. This allows you to choose between ease-of-use and performance.

## Memory Strategies

### 1. Automatic (Default)

Reference counting with automatic cleanup - like Python.

```python
# Default behavior
def process():
    data = [1, 2, 3, 4, 5]
    return sum(data)
    # 'data' automatically cleaned up
```

**When to use:**
- General-purpose programming (default)
- When you want Python-like convenience
- Complex object lifetimes

**Performance:**
- Small overhead for reference counting
- Automatic cleanup when refcount reaches zero

### 2. Manual

Explicit allocation and deallocation - like C/C++.

```python
@manual_memory
def performance_critical():
    buffer = allocate(1024)  # Manual allocation
    try:
        # Use buffer...
        process(buffer)
    finally:
        free(buffer)  # Manual cleanup
```

**When to use:**
- High-performance applications
- Systems programming
- Working with C libraries
- Real-time systems

**Performance:**
- Zero overhead - no reference counting
- Predictable memory layout
- Full control over timing

### 3. Arena (Region-Based)

Bulk allocation from memory pools.

```python
@arena_memory
def batch_processing(items):
    results = []
    for item in items:
        # All allocations use arena
        processed = transform(item)
        results.append(processed)
    return results
    # Arena freed automatically
```

**When to use:**
- Temporary allocations with clear boundaries
- Request handling in web servers
- Game frame allocations
- Parsing/compilation phases

**Performance:**
- Very fast allocation (pointer bumping)
- Zero fragmentation
- Bulk deallocation is O(1)

## Built-in Functions

### allocate(size)

Manually allocate memory buffer.

```python
buffer = allocate(1024)  # 1KB buffer
```

### free(buffer)

Free manually allocated buffer.

```python
free(buffer)
```

### create_arena(name)

Create a new memory arena.

```python
create_arena("temp_data")
```

### reset_arena(name)

Reset arena, freeing all allocations.

```python
reset_arena("temp_data")
```

### destroy_arena(name)

Destroy arena completely.

```python
destroy_arena("temp_data")
```

### memory_stats()

Get memory allocation statistics.

```python
stats = memory_stats()
print(stats)
# Output:
# Memory Strategy: Automatic
# Manual Buffers: 5 (2048 bytes)
# Arenas: 2 (8192 bytes)
```

## Mixing Strategies

You can use different strategies in different parts of your code:

```python
# Default: automatic

@manual_memory
def low_level_operation():
    data = allocate(1000)
    try:
        # Fast manual operations
        process(data)
    finally:
        free(data)

@arena_memory
def handle_request(request):
    # Fast bulk allocation
    parsed = parse(request)
    result = process(parsed)
    return result
    # Arena destroyed here

# Regular automatic management
def main():
    data = load_data()
    result1 = low_level_operation()  # Uses manual
    result2 = handle_request(data)    # Uses arena
    return combine(result1, result2)  # Uses automatic
```

## Compilation Behavior

### VM Execution

```bash
# In VM mode, memory management uses Rust runtime
tauraro run script.py
```

### C Compilation

```bash
# Automatic (default)
tauraro compile script.py -o program --memory-strategy auto

# Manual
tauraro compile script.py -o program --memory-strategy manual

# Arena
tauraro compile script.py -o program --memory-strategy arena
```

### Generated C Code

#### Automatic Mode

```c
tauraro_refcounted_t* x_rc = tauraro_alloc_rc(sizeof(int64_t), NULL);
int64_t* x = (int64_t*)tauraro_rc_ptr(x_rc);
*x = 42;
tauraro_decref(x_rc);  // Automatic cleanup
```

#### Manual Mode

```c
int64_t* x = (int64_t*)tauraro_alloc(sizeof(int64_t));
*x = 42;
tauraro_free(x);  // Explicit cleanup
```

#### Arena Mode

```c
tauraro_arena_t* arena = tauraro_arena_create(4096);
int64_t* x = (int64_t*)tauraro_arena_alloc(arena, sizeof(int64_t));
*x = 42;
tauraro_arena_destroy(arena);  // Bulk cleanup
```

## Performance Characteristics

| Strategy | Allocation | Deallocation | Overhead | Cache Locality |
|----------|-----------|--------------|----------|----------------|
| Automatic | Medium | Fast O(1) | 24 bytes/obj | Medium |
| Manual | Fast | Fast O(1) | 0 bytes | Medium |
| Arena | Very Fast | Very Fast | 8 bytes | Excellent |

## Real-World Examples

### Web Server Request Handler

```python
@arena_memory
def handle_http_request(request_data: bytes):
    # All parsing uses arena allocation
    headers = parse_headers(request_data)
    body = parse_body(request_data)
    result = route_request(headers, body)
    return build_response(result)
    # Arena destroyed - all parsing data freed
```

### Game Engine Frame

```python
class GameEngine:
    def __init__(self):
        create_arena("frame_arena")

    @arena_memory
    def render_frame(self):
        # Temporary rendering data
        visible = cull_scene(self.scene)
        commands = generate_commands(visible)
        execute_commands(commands)

        # Reset arena for next frame
        reset_arena("frame_arena")
```

### Compiler with Mixed Strategies

```python
class Compiler:
    @arena_memory
    def parse(self, source: str):
        # Lots of temporary nodes
        tokens = tokenize(source)
        return parse_tokens(tokens)

    @manual_memory
    def optimize(self, ir):
        # Precise control for optimization passes
        buffer = allocate(ir.estimate_size())
        optimized = run_passes(ir, buffer)
        free(buffer)
        return optimized

    def compile(self, source: str):
        # Automatic for overall flow
        ast = self.parse(source)
        ir = self.lower(ast)
        optimized = self.optimize(ir)
        return self.emit(optimized)
```

## Safety Considerations

### Automatic Mode
- ✅ Memory leaks prevented
- ✅ Use-after-free prevented
- ✅ Double-free prevented

### Manual Mode
- ⚠️ Must free memory manually
- ⚠️ Possible use-after-free
- ⚠️ Possible double-free
- ⚠️ Must track ownership

### Arena Mode
- ✅ No individual leaks
- ⚠️ Data must not escape arena
- ⚠️ Possible dangling pointers

## Debugging

### Enable Memory Debugging

```bash
tauraro compile script.py -o program --memory-debug
```

This adds:
- Allocation tracking
- Leak detection
- Use-after-free detection
- Double-free detection

### Memory Profiling

```python
# Check memory usage
stats = memory_stats()
print(f"Manual buffers: {stats['manual_count']}")
print(f"Arena usage: {stats['arena_bytes']}")
```

## Best Practices

### 1. Default to Automatic

```python
# Use automatic for most code
def normal_function():
    data = load_data()
    return process(data)
```

### 2. Use Manual for C Interop

```python
@manual_memory
def call_c_library():
    buffer = allocate_aligned(4096, 64)
    result = c_function(buffer)
    free(buffer)
    return result
```

### 3. Use Arena for Batches

```python
@arena_memory
def process_batch(items):
    results = []
    for item in items:
        results.append(transform(item))
    return results
```

### 4. Document Ownership

```python
@manual_memory
def create_buffer() -> owned(Buffer):
    """Returns OWNED buffer - caller must free"""
    return allocate(1024)

@manual_memory
def process_buffer(buf: borrowed(Buffer)):
    """Takes BORROWED buffer - does not free"""
    # Use buf
    pass
```

## Next Steps

- [Performance Tuning](performance.md) - Optimization techniques
- [C Backend](../compilation/c-backend.md) - Understanding compilation
- [FFI](ffi.md) - Foreign function interface
- [Full Memory Management Guide](../MEMORY_MANAGEMENT.md) - Complete details
