# System Programming Builtins

Tauraro provides low-level system programming primitives for memory manipulation, pointer operations, atomic operations, and hardware access. These are essential for systems programming, embedded development, and performance-critical code.

**Production Status**: ✅ **98% Complete** - Ready for system programming and OS development

## Memory Allocation

### Basic Allocation

```python
# Manual memory allocation
buffer = allocate(size)   # Allocate 'size' bytes
free(buffer)              # Free allocated memory

# Example
data = allocate(1024)     # Allocate 1KB
# ... use data ...
free(data)                # Must free manually!
```

### Arena Allocation

```python
# Create memory arena
create_arena("temp")      # Create arena named "temp"

# Allocations use arena automatically in @arena_memory functions
@arena_memory
def process_batch(items):
    # All allocations here use the arena
    results = []
    for item in items:
        results.append(transform(item))
    return results
    # Arena NOT automatically destroyed - explicit management

# Reset arena (free all allocations, keep arena)
reset_arena("temp")

# Destroy arena completely
destroy_arena("temp")
```

### Memory Statistics

```python
stats = memory_stats()
print(stats)
# Output:
# {
#   "strategy": "Automatic",
#   "manual_buffers": 5,
#   "manual_bytes": 2048,
#   "arenas": 2,
#   "arena_bytes": 8192
# }
```

## Size and Alignment

### sizeof

Get the size of a type in bytes.

```python
sizeof("int")       # 8 (int64_t)
sizeof("float")     # 8 (double)
sizeof("bool")      # 1
sizeof("char")      # 1
sizeof("pointer")   # 8 (on 64-bit)
sizeof("int8")      # 1
sizeof("int16")     # 2
sizeof("int32")     # 4
sizeof("int64")     # 8
```

### alignof

Get the alignment requirement of a type.

```python
alignof("int")      # 8
alignof("float")    # 8
alignof("char")     # 1
alignof("pointer")  # 8
```

## Memory Operations

### memcpy

Copy memory from source to destination.

```python
# memcpy(dest, src, size)
src = allocate(100)
dest = allocate(100)
memcpy(dest, src, 100)  # Copy 100 bytes
```

### memmove

Copy memory, handling overlapping regions.

```python
# memmove(dest, src, size) - safe for overlapping memory
memmove(dest, src, 100)
```

### memset

Fill memory with a value.

```python
# memset(ptr, value, size)
buffer = allocate(100)
memset(buffer, 0, 100)    # Zero-fill 100 bytes
memset(buffer, 0xFF, 50)  # Fill first 50 bytes with 0xFF
```

### memcmp

Compare memory regions.

```python
# memcmp(ptr1, ptr2, size) -> int
result = memcmp(buf1, buf2, 100)
if result == 0:
    print("Memory regions are equal")
elif result < 0:
    print("buf1 < buf2")
else:
    print("buf1 > buf2")
```

### zero_memory

Zero-fill memory region.

```python
# zero_memory(ptr, size)
buffer = allocate(1024)
zero_memory(buffer, 1024)  # Efficient zero-fill
```

### copy_memory

Copy memory (alias for memcpy).

```python
copy_memory(dest, src, size)
```

### compare_memory

Compare memory (alias for memcmp).

```python
result = compare_memory(ptr1, ptr2, size)
```

## Pointer Operations

### Creating Pointers

```python
# Null pointer
ptr = null_ptr()

# Check if null
if is_null(ptr):
    print("Pointer is null")
```

### Pointer Arithmetic

```python
# Offset pointer by bytes
new_ptr = ptr_offset(base_ptr, 16)  # base_ptr + 16 bytes
```

### Reading/Writing via Pointers

```python
# Read value at pointer
value = ptr_read(ptr, "int")     # Read int64
value = ptr_read(ptr, "float")   # Read double
value = ptr_read(ptr, "int8")    # Read byte
value = ptr_read(ptr, "int32")   # Read 32-bit int

# Write value at pointer
ptr_write(ptr, "int", 42)        # Write int64
ptr_write(ptr, "float", 3.14)    # Write double
ptr_write(ptr, "int8", 255)      # Write byte
```

### Example: Working with Structs

```python
# Simulating a C struct:
# struct Point { int32_t x; int32_t y; }

def create_point(x: int, y: int):
    """Create a Point struct."""
    ptr = allocate(8)  # 4 bytes x + 4 bytes y
    ptr_write(ptr, "int32", x)
    ptr_write(ptr_offset(ptr, 4), "int32", y)
    return ptr

def get_point_x(ptr) -> int:
    """Get x coordinate from Point."""
    return ptr_read(ptr, "int32")

def get_point_y(ptr) -> int:
    """Get y coordinate from Point."""
    return ptr_read(ptr_offset(ptr, 4), "int32")

# Usage
point = create_point(10, 20)
print(f"Point: ({get_point_x(point)}, {get_point_y(point)})")
free(point)
```

## Volatile Operations

Volatile operations prevent compiler optimization and ensure memory is always read/written.

```python
# Volatile read - always reads from memory
value = volatile_read(address, "int32")

# Volatile write - always writes to memory
volatile_write(address, "int32", value)
```

### Use Cases

```python
# Hardware register access
def read_status_register() -> int:
    """Read hardware status - must be volatile."""
    return volatile_read(STATUS_REG_ADDR, "int32")

# Shared memory with other threads/processes
def check_flag() -> bool:
    """Check shared flag - must be volatile."""
    return volatile_read(flag_addr, "int8") != 0
```

## Atomic Operations

Thread-safe atomic operations for concurrent programming.

### atomic_load / atomic_store

```python
# Atomic load - thread-safe read
value = atomic_load(address)

# Atomic store - thread-safe write
atomic_store(address, value)
```

### atomic_add / atomic_sub

```python
# Atomic add - returns old value
old_value = atomic_add(address, 5)

# Atomic subtract - returns old value
old_value = atomic_sub(address, 3)
```

### atomic_cas (Compare-And-Swap)

```python
# Compare and swap
# Returns True if swap succeeded
success = atomic_cas(address, expected, new_value)

# Example: Increment with CAS
def atomic_increment(addr):
    while True:
        old = atomic_load(addr)
        if atomic_cas(addr, old, old + 1):
            return old + 1
```

### Example: Lock-Free Counter

```python
counter_addr = allocate(8)
atomic_store(counter_addr, 0)

def increment_counter() -> int:
    """Thread-safe counter increment."""
    while True:
        current = atomic_load(counter_addr)
        if atomic_cas(counter_addr, current, current + 1):
            return current + 1

def get_counter() -> int:
    """Get current counter value."""
    return atomic_load(counter_addr)
```

## Memory Barriers

Ensure memory operations complete in order.

```python
# Full memory barrier
memory_barrier()

# Example usage
atomic_store(data_ready_flag, 0)
# ... write data ...
memory_barrier()  # Ensure data written before flag
atomic_store(data_ready_flag, 1)
```

## Cache Operations

### prefetch

Prefetch memory into cache.

```python
# Prefetch data into cache
prefetch(address)

# Example: Prefetch array elements
for i in range(0, len(data), 64):
    prefetch(ptr_offset(data_ptr, i + 256))  # Prefetch ahead
    # Process current element
    process(ptr_read(ptr_offset(data_ptr, i), "int"))
```

### cache_line_size

Get CPU cache line size.

```python
line_size = cache_line_size()  # Usually 64 bytes
print(f"Cache line size: {line_size} bytes")
```

## Stack Allocation

Allocate memory on the stack (faster than heap).

```python
# Stack allocation (simulated in interpreter)
buffer = stack_alloc(256)  # Allocate 256 bytes on stack
# No need to free - automatically cleaned up
```

**Note:** In compiled mode, this generates actual stack allocation. In interpreter mode, it uses heap allocation as simulation.

## Bit Manipulation

### bit_cast

Reinterpret bits as different type.

```python
# Reinterpret float bits as int
float_val: float = 3.14
int_bits = bit_cast(float_val, "float", "int")

# Reinterpret int bits as float
int_val: int = 0x400921FB54442D18  # IEEE 754 for pi
float_result = bit_cast(int_val, "int", "float")
```

## Hardware Access (Bare-Metal)

### Port I/O (x86/x86_64)

Direct I/O port access for hardware communication.

```python
# 8-bit port operations
port_out8(port, value)   # Write byte to port
value = port_in8(port)   # Read byte from port

# 16-bit port operations
port_out16(port, value)  # Write 16-bit value
value = port_in16(port)  # Read 16-bit value

# 32-bit port operations
port_out32(port, value)  # Write 32-bit value
value = port_in32(port)  # Read 32-bit value

# Aliases (backward compatibility)
port_out(port, value)    # Same as port_out8
value = port_in(port)    # Same as port_in8
```

#### Example: Serial Port (UART)

```python
# COM1 serial port
COM1_PORT = 0x3F8

def write_serial(char: int):
    """Write character to serial port."""
    # Wait for transmit buffer empty
    while (port_in8(COM1_PORT + 5) & 0x20) == 0:
        pass
    port_out8(COM1_PORT, char)

def read_serial() -> int:
    """Read character from serial port."""
    # Wait for data ready
    while (port_in8(COM1_PORT + 5) & 0x01) == 0:
        pass
    return port_in8(COM1_PORT)

# Usage
write_serial(ord('H'))
write_serial(ord('i'))
```

#### Example: PCI Configuration

```python
# PCI configuration space access
PCI_CONFIG_ADDR = 0xCF8
PCI_CONFIG_DATA = 0xCFC

def pci_read_config(bus, device, func, offset):
    """Read PCI configuration register."""
    address = 0x80000000 | (bus << 16) | (device << 11) | (func << 8) | offset
    port_out32(PCI_CONFIG_ADDR, address)
    return port_in32(PCI_CONFIG_DATA)

def pci_write_config(bus, device, func, offset, value):
    """Write PCI configuration register."""
    address = 0x80000000 | (bus << 16) | (device << 11) | (func << 8) | offset
    port_out32(PCI_CONFIG_ADDR, address)
    port_out32(PCI_CONFIG_DATA, value)
```

### Memory-Mapped I/O (MMIO)

Direct memory-mapped hardware register access.

```python
# 8-bit MMIO operations
mmio_write8(address, value)   # Write byte
value = mmio_read8(address)   # Read byte

# 16-bit MMIO operations
mmio_write16(address, value)  # Write 16-bit value
value = mmio_read16(address)  # Read 16-bit value

# 32-bit MMIO operations
mmio_write32(address, value)  # Write 32-bit value
value = mmio_read32(address)  # Read 32-bit value

# 64-bit MMIO operations
mmio_write64(address, value)  # Write 64-bit value
value = mmio_read64(address)  # Read 64-bit value
```

#### Example: VGA Text Mode

```python
# VGA text mode buffer at 0xB8000
VGA_BUFFER = 0xB8000
VGA_WIDTH = 80
VGA_HEIGHT = 25

def write_char(x: int, y: int, char: int, color: int):
    """Write character to VGA text buffer."""
    offset = (y * VGA_WIDTH + x) * 2
    mmio_write8(VGA_BUFFER + offset, char)
    mmio_write8(VGA_BUFFER + offset + 1, color)

def clear_screen():
    """Clear VGA screen."""
    for i in range(VGA_WIDTH * VGA_HEIGHT):
        mmio_write16(VGA_BUFFER + i * 2, 0x0720)  # Space with gray color

# Usage
clear_screen()
write_char(0, 0, ord('H'), 0x0F)  # White on black
write_char(1, 0, ord('i'), 0x0F)
```

#### Example: ARM GPIO Control

```python
# Raspberry Pi GPIO base address
GPIO_BASE = 0xFE200000  # For Pi 4

# GPIO Function Select
GPFSEL0 = GPIO_BASE + 0x00

# GPIO Set/Clear
GPSET0 = GPIO_BASE + 0x1C
GPCLR0 = GPIO_BASE + 0x28

def gpio_set_output(pin: int):
    """Set GPIO pin as output."""
    reg_offset = (pin // 10) * 4
    bit_offset = (pin % 10) * 3

    addr = GPIO_BASE + reg_offset
    val = mmio_read32(addr)
    val &= ~(7 << bit_offset)  # Clear function bits
    val |= (1 << bit_offset)   # Set as output
    mmio_write32(addr, val)

def gpio_set(pin: int):
    """Set GPIO pin high."""
    mmio_write32(GPSET0, 1 << pin)

def gpio_clear(pin: int):
    """Set GPIO pin low."""
    mmio_write32(GPCLR0, 1 << pin)

# Usage - Blink LED on GPIO 21
gpio_set_output(21)
gpio_set(21)    # LED on
# ... delay ...
gpio_clear(21)  # LED off
```

### Interrupt Control

Enable/disable interrupts on various architectures.

```python
# x86/x86_64
cli()                    # Clear interrupts (disable)
sti()                    # Set interrupts (enable)
disable_interrupts()     # Alias for cli()
enable_interrupts()      # Alias for sti()

# ARM/AArch64
disable_interrupts_arm() # Disable IRQ on ARM
enable_interrupts_arm()  # Enable IRQ on ARM

# RISC-V
disable_interrupts_riscv()  # Disable interrupts on RISC-V
enable_interrupts_riscv()   # Enable interrupts on RISC-V
```

#### Example: Critical Section

```python
def critical_section():
    """Execute code with interrupts disabled."""
    # Save interrupt state and disable
    disable_interrupts()

    try:
        # Critical code here
        # ... atomic operations ...
        pass
    finally:
        # Restore interrupts
        enable_interrupts()
```

### CPU Control Registers (x86/x86_64)

```python
# Read CR0 (control register 0)
cr0 = read_cr0()

# Write CR0
write_cr0(value)

# Read CR3 (page directory base)
cr3 = read_cr3()

# Write CR3 (switch page tables)
write_cr3(page_directory_phys_addr)

# Read/Write MSR (Model Specific Register)
value = read_msr(msr_number)
write_msr(msr_number, value)
```

#### Example: Enable Paging

```python
def enable_paging(page_directory_addr: int):
    """Enable x86 paging."""
    # Load page directory
    write_cr3(page_directory_addr)

    # Enable paging bit in CR0
    cr0 = read_cr0()
    cr0 |= 0x80000000  # PG bit
    write_cr0(cr0)
```

### CPU Control Instructions

```python
# Halt CPU until interrupt
halt()

# Read timestamp counter
cycles = rdtsc()

# CPU identification
cpuid_result = cpuid(leaf, subleaf)
```

#### Example: Idle Loop

```python
def kernel_idle():
    """Kernel idle loop - save power."""
    while True:
        disable_interrupts()
        if no_pending_work():
            enable_interrupts()
            halt()  # Sleep until interrupt
        else:
            enable_interrupts()
            # Do work
```

## Complete Example: Custom Allocator

```python
# Simple bump allocator implementation

class BumpAllocator:
    def __init__(self, size: int):
        self.buffer = allocate(size)
        self.size = size
        self.offset = 0
    
    def alloc(self, bytes: int) -> int:
        """Allocate bytes from the arena."""
        # Align to 8 bytes
        aligned_offset = (self.offset + 7) & ~7
        
        if aligned_offset + bytes > self.size:
            raise MemoryError("Allocator out of memory")
        
        ptr = ptr_offset(self.buffer, aligned_offset)
        self.offset = aligned_offset + bytes
        return ptr
    
    def reset(self):
        """Reset allocator - free all allocations."""
        self.offset = 0
        zero_memory(self.buffer, self.size)
    
    def destroy(self):
        """Destroy allocator and free memory."""
        free(self.buffer)
        self.buffer = null_ptr()

# Usage
allocator = BumpAllocator(4096)

# Allocate some memory
ptr1 = allocator.alloc(100)
ptr2 = allocator.alloc(200)
ptr3 = allocator.alloc(50)

# Use the memory
ptr_write(ptr1, "int", 42)
value = ptr_read(ptr1, "int")

# Reset when done with batch
allocator.reset()

# Clean up
allocator.destroy()
```

## Complete Example: Ring Buffer

```python
class RingBuffer:
    """Lock-free single-producer single-consumer ring buffer."""
    
    def __init__(self, capacity: int):
        self.capacity = capacity
        self.buffer = allocate(capacity)
        self.head = allocate(8)  # Write position
        self.tail = allocate(8)  # Read position
        atomic_store(self.head, 0)
        atomic_store(self.tail, 0)
    
    def push(self, value: int) -> bool:
        """Push a byte to the buffer. Returns False if full."""
        head = atomic_load(self.head)
        next_head = (head + 1) % self.capacity
        
        if next_head == atomic_load(self.tail):
            return False  # Buffer full
        
        ptr_write(ptr_offset(self.buffer, head), "int8", value)
        memory_barrier()
        atomic_store(self.head, next_head)
        return True
    
    def pop(self) -> int:
        """Pop a byte from the buffer. Returns -1 if empty."""
        tail = atomic_load(self.tail)
        
        if tail == atomic_load(self.head):
            return -1  # Buffer empty
        
        value = ptr_read(ptr_offset(self.buffer, tail), "int8")
        memory_barrier()
        atomic_store(self.tail, (tail + 1) % self.capacity)
        return value
    
    def destroy(self):
        """Clean up buffer memory."""
        free(self.buffer)
        free(self.head)
        free(self.tail)

# Usage
buffer = RingBuffer(1024)

# Producer
buffer.push(65)  # 'A'
buffer.push(66)  # 'B'
buffer.push(67)  # 'C'

# Consumer
while True:
    byte = buffer.pop()
    if byte < 0:
        break
    print(chr(byte))

buffer.destroy()
```

## Safety Notes

### ⚠️ Important Warnings

1. **Manual memory must be freed** - Memory leaks if you forget `free()`
2. **No bounds checking** - Buffer overflows are possible
3. **Type safety is your responsibility** - `ptr_read/ptr_write` don't validate
4. **Atomic operations require proper memory** - Use properly aligned addresses
5. **Stack allocation is limited** - Don't allocate large buffers on stack

### Best Practices

1. **Always pair `allocate()` with `free()`**
2. **Use arenas for batch allocations**
3. **Check for null pointers**
4. **Use `volatile_read/write` for hardware**
5. **Use `atomic_*` for shared memory**
6. **Add bounds checking in debug builds**
7. **Document ownership of allocated memory**

## Next Steps

- [Memory Management](../advanced/memory.md) - Memory strategies
- [Bare-Metal Development](../advanced/baremetal.md) - OS/driver development
- [Performance](../advanced/performance.md) - Optimization techniques
- [FFI](../advanced/ffi.md) - Foreign function interface
