# Tauraro Advanced Memory Management System

## Overview

Tauraro provides a flexible memory management system that gives developers control over how memory is allocated and freed, while maintaining the ease-of-use of Python. By default, Tauraro uses automatic memory management, but developers can opt into manual or arena-based strategies for specific use cases.

## Memory Management Strategies

### 1. Automatic (Default)

**How it works**: Reference counting with automatic cleanup, similar to Python's memory management.

**When to use**:
- General-purpose programming (default choice)
- When you want Python-like convenience
- When object lifetimes are complex

**Advantages**:
- No manual cleanup required
- Prevents memory leaks automatically
- Deterministic cleanup when reference count reaches zero

**C Code Generated**:
```c
// Automatic reference counting
tauraro_refcounted_t* x_rc = tauraro_alloc_rc(sizeof(int64_t), NULL);
int64_t* x = (int64_t*)tauraro_rc_ptr(x_rc);
*x = 42;

// Automatically cleaned up when refcount reaches 0
tauraro_decref(x_rc);
```

**Usage**:
```bash
# Default - automatic memory management
tauraro compile myfile.tr --backend c --use-native-transpiler

# Explicit
tauraro compile myfile.tr --backend c --use-native-transpiler --memory-strategy auto
```

### 2. Manual

**How it works**: Explicit allocation and deallocation, similar to C/C++.

**When to use**:
- High-performance applications where you need precise control
- Systems programming
- When working with external C libraries
- Real-time systems with strict timing requirements

**Advantages**:
- Zero overhead - no reference counting
- Predictable memory layout
- Full control over allocation/deallocation timing
- Compatible with existing C memory management

**C Code Generated**:
```c
// Manual allocation
int64_t* x = (int64_t*)tauraro_alloc(sizeof(int64_t));
*x = 42;

// Must explicitly free
tauraro_free(x);
```

**Usage**:
```bash
tauraro compile myfile.tr --backend c --use-native-transpiler --memory-strategy manual
```

**Example Tauraro Code**:
```python
# Using manual memory management
@manual_memory
def process_large_data():
    # Memory is manually managed in this scope
    buffer = allocate(1024 * 1024)  # 1MB buffer

    # Process data...

    # Must explicitly free
    free(buffer)
```

### 3. Arena (Region-Based)

**How it works**: Bulk allocation from memory pools, with entire pools freed at once.

**When to use**:
- Temporary allocations with clear lifetime boundaries
- Parsing/compilation phases
- Request handling in web servers
- Game frame allocations

**Advantages**:
- Very fast allocation (just pointer bumping)
- Zero fragmentation
- Bulk deallocation is O(1)
- Excellent cache locality

**C Code Generated**:
```c
// Create arena
tauraro_arena_t* _arena = tauraro_arena_create(4096);

// Fast allocation from arena
int64_t* x = (int64_t*)tauraro_arena_alloc(_arena, sizeof(int64_t));
*x = 42;

// Free entire arena at once
tauraro_arena_destroy(_arena);
```

**Usage**:
```bash
tauraro compile myfile.tr --backend c --use-native-transpiler --memory-strategy arena
```

**Example Tauraro Code**:
```python
# Using arena memory management
@arena_memory
def handle_request(request):
    # All allocations in this scope use arena
    parsed_data = parse(request)  # Allocated from arena
    result = process(parsed_data)  # Allocated from arena
    return result
    # Arena is automatically destroyed at scope end
```

## Mixing Strategies

You can mix different memory strategies within the same program using decorators:

```python
# Default: automatic memory management

@manual_memory
def performance_critical_section():
    # This function uses manual memory management
    data = allocate(1000)
    try:
        process(data)
    finally:
        free(data)  # Must free explicitly

@arena_memory
def parse_request(req_data):
    # This function uses arena allocation
    tokens = tokenize(req_data)  # Allocated from arena
    ast = parse_tokens(tokens)    # Allocated from arena
    return compile(ast)
    # Arena is destroyed here, freeing all allocations

# Regular functions use automatic memory management
def normal_function():
    x = 42  # Automatic reference counting
    return x * 2
```

## C Transpilation Details

### Automatic Mode Runtime

```c
typedef struct {
    void* ptr;
    size_t refcount;
    size_t size;
    void (*destructor)(void*);
} tauraro_refcounted_t;

static inline void tauraro_incref(tauraro_refcounted_t* rc) {
    if (rc) rc->refcount++;
}

static inline void tauraro_decref(tauraro_refcounted_t* rc) {
    if (rc && --rc->refcount == 0) {
        if (rc->destructor) rc->destructor(rc->ptr);
        free(rc->ptr);
        free(rc);
    }
}
```

### Manual Mode Runtime

```c
static inline void* tauraro_alloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Tauraro: Memory allocation failed\\n");
        exit(1);
    }
    return ptr;
}

static inline void tauraro_free(void* ptr) {
    if (ptr) free(ptr);
}
```

### Arena Mode Runtime

```c
typedef struct tauraro_arena {
    char* memory;
    size_t size;
    size_t offset;
    struct tauraro_arena* next;
} tauraro_arena_t;

static inline void* tauraro_arena_alloc(tauraro_arena_t* arena, size_t size) {
    size_t aligned_size = (size + 7) & ~7;  // 8-byte alignment

    if (arena->offset + aligned_size > arena->size) {
        // Allocate new block
        size_t new_size = arena->size * 2;
        if (new_size < aligned_size) new_size = aligned_size * 2;

        tauraro_arena_t* new_block = malloc(sizeof(tauraro_arena_t));
        new_block->memory = malloc(new_size);
        new_block->size = new_size;
        new_block->offset = 0;
        new_block->next = arena->next;
        arena->next = new_block;
        arena = new_block;
    }

    void* ptr = arena->memory + arena->offset;
    arena->offset += aligned_size;
    return ptr;
}

static inline void tauraro_arena_destroy(tauraro_arena_t* arena) {
    while (arena) {
        tauraro_arena_t* next = arena->next;
        free(arena->memory);
        free(arena);
        arena = next;
    }
}
```

## Performance Characteristics

| Strategy | Allocation Speed | Deallocation Speed | Memory Overhead | Cache Locality |
|----------|-----------------|-------------------|-----------------|----------------|
| **Automatic** | Medium (malloc + refcount) | Fast (O(1)) | 24 bytes/object | Medium |
| **Manual** | Fast (malloc only) | Fast (O(1)) | 0 bytes | Medium |
| **Arena** | Very Fast (pointer bump) | Very Fast (bulk) | 8 bytes alignment | Excellent |

## Best Practices

### 1. Choose Automatic by Default
Unless you have specific performance requirements, use automatic memory management. It's safe and convenient.

### 2. Use Manual for Interfacing with C
When writing bindings to C libraries or low-level systems code, manual management gives you precise control.

```python
@manual_memory
def call_c_library():
    buffer = allocate_aligned(4096, 64)  # Aligned allocation
    result = c_function(buffer)
    free(buffer)
    return result
```

### 3. Use Arena for Batch Processing
Perfect for operations with clear phases:

```python
@arena_memory
def process_batch(items):
    results = []
    for item in items:
        # All intermediate allocations use arena
        processed = transform(item)
        results.append(processed)
    return results  # Only results escape the arena
```

### 4. Document Memory Ownership
When using manual mode, clearly document who owns which memory:

```python
@manual_memory
def create_buffer() -> owned(Buffer):
    """Returns an OWNED buffer - caller must free it"""
    return allocate(1024)

@manual_memory
def process_buffer(buf: borrowed(Buffer)):
    """Takes a BORROWED buffer - does not free it"""
    # Use buf
    pass
```

## Advanced: Custom Destructors

For automatic mode, you can define custom cleanup logic:

```python
class FileHandle:
    def __init__(self, path: str):
        self.fd = open_file(path)

    def __del__(self):
        # Custom cleanup - called when refcount reaches 0
        close_file(self.fd)
```

This transpiles to C with a custom destructor:

```c
void FileHandle_destructor(void* ptr) {
    struct FileHandle_t* handle = (struct FileHandle_t*)ptr;
    close_file(handle->fd);
}

struct FileHandle_t* handle_rc = tauraro_alloc_rc(
    sizeof(struct FileHandle_t),
    FileHandle_destructor  // Custom destructor
);
```

## Safety Considerations

### Automatic Mode
- ✅ Memory leaks prevented (unless circular references)
- ✅ Use-after-free prevented
- ✅ Double-free prevented

### Manual Mode
- ⚠️  Developer responsible for freeing memory
- ⚠️  Possible use-after-free if not careful
- ⚠️  Possible double-free if not careful
- ⚠️  Must track ownership manually

### Arena Mode
- ✅ No individual leaks (entire arena freed)
- ⚠️  Data must not escape arena scope
- ⚠️  Possible dangling pointers if arena destroyed early

## Debugging

Use the built-in memory debugging tools:

```bash
# Enable memory debugging
tauraro compile myfile.tr --backend c --use-native-transpiler --memory-debug

# This adds:
# - Allocation tracking
# - Leak detection
# - Use-after-free detection
# - Double-free detection
```

## Examples

### Example 1: Web Server Request Handling

```python
@arena_memory
def handle_http_request(request_data: bytes) -> Response:
    # Parse using arena allocation (fast, no cleanup needed)
    headers = parse_headers(request_data)
    body = parse_body(request_data)

    # Process request
    result = route_request(headers, body)

    # Only the response escapes the arena
    return build_response(result)
    # Arena destroyed here - all parsing data freed
```

### Example 2: Game Engine Frame

```python
class GameEngine:
    def __init__(self):
        self.frame_arena = create_arena(10 * 1024 * 1024)  # 10MB arena

    @arena_memory(self.frame_arena)
    def render_frame(self):
        # All temporary rendering data uses arena
        visible_objects = cull_scene(self.scene)
        render_commands = generate_commands(visible_objects)
        execute_commands(render_commands)

        # Arena reset at end of frame
        reset_arena(self.frame_arena)
```

### Example 3: Mixed Strategy Compiler

```python
class Compiler:
    # Default automatic management for the compiler object

    @arena_memory
    def parse(self, source: str) -> AST:
        # Parsing uses arena - lots of temporary nodes
        tokens = tokenize(source)
        ast = parse_tokens(tokens)
        return ast  # AST escapes, but temporaries are arena-freed

    @manual_memory
    def optimize(self, ir: IR) -> IR:
        # Optimization uses manual for precise control
        buffer = allocate(ir.estimate_size())
        optimized = run_passes(ir, buffer)
        free(buffer)
        return optimized

    def compile(self, source: str) -> Bytecode:
        # Regular automatic management
        ast = self.parse(source)  # Uses arena internally
        ir = self.lower(ast)      # Automatic
        optimized = self.optimize(ir)  # Uses manual internally
        return self.emit(optimized)  # Automatic
```

## Summary

Tauraro's memory management system provides:

1. **Automatic by default** - Python-like convenience
2. **Manual when needed** - C-like control
3. **Arena for performance** - Fast bulk allocation
4. **Seamless C integration** - Works perfectly with C transpilation
5. **Zero runtime overhead options** - Choose your trade-offs

The system is designed to be:
- **Safe by default** (automatic)
- **Fast when needed** (manual/arena)
- **Flexible** (mix strategies)
- **Easy to use** (Python-like syntax)
- **C-compatible** (clean generated code)
