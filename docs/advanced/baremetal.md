```markdown
# Bare-Metal and OS Development

Tauraro supports bare-metal programming for OS kernels, device drivers, embedded firmware, and real-time systems. This guide covers the low-level hardware access features and compilation modes.

## Overview

Tauraro provides:
- **Freestanding compilation** - No C standard library dependency
- **Hardware I/O primitives** - Port I/O, MMIO, interrupts
- **CPU control** - Control registers, MSRs, halt
- **Inline assembly** - Direct assembly insertion
- **Custom entry points** - For OS kernels and bootloaders

## Compilation Modes

### Standard Mode (Default)

```bash
tauraro compile kernel.tr -o kernel.c --backend c --native
```

- Uses C standard library
- Hardware functions are **stubs** (return 0 / no-op)
- Safe for testing on regular operating systems
- Generated code can run on Windows, Linux, macOS

### Freestanding Mode

```bash
tauraro compile kernel.tr -o kernel.c --backend c --freestanding
```

- No C standard library
- Hardware functions generate **real assembly**
- For actual bare-metal targets
- Requires cross-compiler

### Full Bare-Metal Options

```bash
tauraro compile kernel.tr -o kernel.c --backend c \
    --freestanding \
    --no-stdlib \
    --entry-point kernel_main \
    --target-arch x86_64 \
    --inline-asm
```

| Flag | Description |
|------|-------------|
| `--freestanding` | No C standard library, real hardware access |
| `--no-stdlib` | Don't link standard library |
| `--entry-point <name>` | Custom entry point (default: `main`) |
| `--target-arch <arch>` | Target architecture |
| `--inline-asm` | Enable inline assembly support |

### Supported Architectures

| Architecture | Port I/O | MMIO | Interrupts | Control Regs |
|--------------|----------|------|------------|--------------|
| `x86` | ✅ Full | ✅ | ✅ | ✅ CR0/CR3/MSR |
| `x86_64` | ✅ Full | ✅ | ✅ | ✅ CR0/CR3/MSR |
| `arm` | ❌ | ✅ | ✅ | ✅ CPSR |
| `aarch64` | ❌ | ✅ | ✅ | ✅ System regs |
| `riscv32` | ❌ | ✅ | ✅ | ✅ CSR |
| `riscv64` | ❌ | ✅ | ✅ | ✅ CSR |

## Port I/O (x86 Only)

Port I/O is used to communicate with hardware devices on x86 systems.

### Functions

```python
# 8-bit port I/O
value: int = port_in(port)        # Read byte from port
port_out(port, value)              # Write byte to port

# 16-bit port I/O
value: int = port_in16(port)      # Read word from port
port_out16(port, value)            # Write word to port

# 32-bit port I/O
value: int = port_in32(port)      # Read dword from port
port_out32(port, value)            # Write dword to port
```

### Example: Reading from Keyboard Controller

```python
# Keyboard controller port
KEYBOARD_DATA_PORT: int = 0x60
KEYBOARD_STATUS_PORT: int = 0x64

def read_keyboard_scancode() -> int:
    """Read scancode from keyboard controller."""
    # Wait for data to be available
    while (port_in(KEYBOARD_STATUS_PORT) & 1) == 0:
        pass
    # Read the scancode
    return port_in(KEYBOARD_DATA_PORT)
```

### Example: Serial Port Output

```python
COM1_PORT: int = 0x3F8

def serial_init():
    """Initialize COM1 serial port."""
    port_out(COM1_PORT + 1, 0x00)  # Disable interrupts
    port_out(COM1_PORT + 3, 0x80)  # Enable DLAB
    port_out(COM1_PORT + 0, 0x03)  # Divisor low byte (38400 baud)
    port_out(COM1_PORT + 1, 0x00)  # Divisor high byte
    port_out(COM1_PORT + 3, 0x03)  # 8 bits, no parity, one stop bit
    port_out(COM1_PORT + 2, 0xC7)  # Enable FIFO
    port_out(COM1_PORT + 4, 0x0B)  # IRQs enabled, RTS/DSR set

def serial_write(char: int):
    """Write a character to serial port."""
    # Wait for transmit buffer empty
    while (port_in(COM1_PORT + 5) & 0x20) == 0:
        pass
    port_out(COM1_PORT, char)

def serial_print(msg: str):
    """Print string to serial port."""
    for c in msg:
        serial_write(ord(c))
```

## Memory-Mapped I/O (MMIO)

MMIO is used for hardware that's mapped to memory addresses.

### Functions

```python
# Read MMIO
value: int = mmio_read8(address)   # Read byte
value: int = mmio_read16(address)  # Read word
value: int = mmio_read32(address)  # Read dword
value: int = mmio_read64(address)  # Read qword

# Write MMIO
mmio_write8(address, value)        # Write byte
mmio_write16(address, value)       # Write word
mmio_write32(address, value)       # Write dword
mmio_write64(address, value)       # Write qword
```

### Example: VGA Text Mode

```python
VGA_BUFFER: int = 0xB8000
VGA_WIDTH: int = 80
VGA_HEIGHT: int = 25

def vga_write_char(x: int, y: int, char: int, color: int):
    """Write character to VGA text buffer."""
    offset: int = (y * VGA_WIDTH + x) * 2
    address: int = VGA_BUFFER + offset
    
    # Character byte
    mmio_write8(address, char)
    # Attribute byte (color)
    mmio_write8(address + 1, color)

def vga_clear_screen(color: int):
    """Clear VGA screen with specified background color."""
    for y in range(VGA_HEIGHT):
        for x in range(VGA_WIDTH):
            vga_write_char(x, y, ord(' '), color)

def vga_print(msg: str, x: int, y: int, color: int):
    """Print string to VGA at position."""
    for i in range(len(msg)):
        vga_write_char(x + i, y, ord(msg[i]), color)
```

### Example: APIC Timer

```python
LAPIC_BASE: int = 0xFEE00000

# LAPIC register offsets
LAPIC_ID: int = 0x20
LAPIC_VERSION: int = 0x30
LAPIC_TPR: int = 0x80
LAPIC_EOI: int = 0xB0
LAPIC_TIMER_LVT: int = 0x320
LAPIC_TIMER_INITIAL: int = 0x380
LAPIC_TIMER_CURRENT: int = 0x390
LAPIC_TIMER_DIVIDE: int = 0x3E0

def lapic_read(offset: int) -> int:
    """Read LAPIC register."""
    return mmio_read32(LAPIC_BASE + offset)

def lapic_write(offset: int, value: int):
    """Write LAPIC register."""
    mmio_write32(LAPIC_BASE + offset, value)

def lapic_init_timer(count: int, vector: int):
    """Initialize LAPIC timer."""
    # Set divide value
    lapic_write(LAPIC_TIMER_DIVIDE, 0x3)  # Divide by 16
    # Set timer LVT (periodic mode)
    lapic_write(LAPIC_TIMER_LVT, vector | 0x20000)
    # Set initial count
    lapic_write(LAPIC_TIMER_INITIAL, count)

def lapic_eoi():
    """Send End-Of-Interrupt signal."""
    lapic_write(LAPIC_EOI, 0)
```

## Interrupt Control

### Functions

```python
# Disable/enable interrupts
disable_interrupts()  # CLI instruction (x86)
enable_interrupts()   # STI instruction (x86)

# CPU halt
halt()  # HLT instruction - halt until interrupt
```

### Example: Critical Section

```python
def critical_section():
    """Execute code with interrupts disabled."""
    disable_interrupts()
    try:
        # Critical code here
        modify_kernel_data()
    finally:
        enable_interrupts()
```

### Example: Idle Loop

```python
def idle_loop():
    """CPU idle loop - wait for interrupts."""
    while True:
        enable_interrupts()
        halt()  # Wait for interrupt
        # Handle interrupt here
```

## CPU Control Registers (x86)

### CR0/CR3 Registers

```python
# Read control registers
cr0_value: int = read_cr0()  # Contains PE, PG, etc.
cr3_value: int = read_cr3()  # Page table base address

# Write control registers
write_cr0(value)  # Set CR0 flags
write_cr3(value)  # Set page table address
```

### Example: Enable Paging

```python
CR0_PE: int = 0x00000001  # Protected mode enable
CR0_PG: int = 0x80000000  # Paging enable

def enable_paging(page_table_address: int):
    """Enable paging with given page table."""
    # Set page table address
    write_cr3(page_table_address)
    
    # Enable paging bit in CR0
    cr0: int = read_cr0()
    cr0 = cr0 | CR0_PG
    write_cr0(cr0)
```

### Model-Specific Registers (MSR)

```python
# Read/write MSRs
value: int = read_msr(msr_number)
write_msr(msr_number, value)
```

### Common MSRs

```python
# Common x86 MSR numbers
MSR_APIC_BASE: int = 0x1B
MSR_EFER: int = 0xC0000080
MSR_STAR: int = 0xC0000081
MSR_LSTAR: int = 0xC0000082
MSR_FMASK: int = 0xC0000084
MSR_FS_BASE: int = 0xC0000100
MSR_GS_BASE: int = 0xC0000101
MSR_KERNEL_GS_BASE: int = 0xC0000102

def get_apic_base() -> int:
    """Get APIC base address from MSR."""
    return read_msr(MSR_APIC_BASE) & 0xFFFFF000

def enable_long_mode():
    """Enable long mode via EFER MSR."""
    efer: int = read_msr(MSR_EFER)
    efer = efer | 0x100  # Set LME bit
    write_msr(MSR_EFER, efer)
```

## Inline Assembly

```python
# Execute inline assembly
asm("nop")                    # No operation
asm("cli")                    # Disable interrupts
asm("sti")                    # Enable interrupts
asm("hlt")                    # Halt CPU
asm("xchg %bx, %bx")          # Bochs magic breakpoint
```

### Example: CPUID

```python
def has_apic() -> bool:
    """Check if CPU has APIC using CPUID."""
    # In freestanding mode, this generates real CPUID instruction
    # In standard mode, returns simulated value
    asm("cpuid")  # Placeholder - actual implementation varies
    return True  # Simplified
```

## Complete OS Kernel Example

```python
# kernel.tr - Simple OS kernel

# VGA constants
VGA_BUFFER: int = 0xB8000
VGA_WIDTH: int = 80
WHITE_ON_BLACK: int = 0x0F

def vga_write(x: int, y: int, char: int):
    """Write character to VGA."""
    offset: int = (y * VGA_WIDTH + x) * 2
    mmio_write8(VGA_BUFFER + offset, char)
    mmio_write8(VGA_BUFFER + offset + 1, WHITE_ON_BLACK)

def vga_print(msg: str, row: int):
    """Print string to VGA row."""
    for i in range(len(msg)):
        vga_write(i, row, ord(msg[i]))

def vga_clear():
    """Clear VGA screen."""
    for y in range(25):
        for x in range(80):
            vga_write(x, y, ord(' '))

def kernel_main():
    """Kernel entry point."""
    # Clear screen
    vga_clear()
    
    # Print welcome message
    vga_print("=== Tauraro OS ===", 0)
    vga_print("Kernel loaded successfully!", 2)
    vga_print("Memory-mapped I/O: OK", 3)
    vga_print("Interrupt control: OK", 4)
    
    # Initialize interrupts
    disable_interrupts()
    # ... set up IDT ...
    enable_interrupts()
    
    vga_print("Interrupts enabled", 5)
    vga_print("Entering idle loop...", 7)
    
    # Idle loop
    while True:
        halt()
```

### Compiling the Kernel

```bash
# Generate freestanding C code
tauraro compile kernel.tr -o kernel.c --backend c \
    --freestanding \
    --entry-point kernel_main \
    --target-arch x86_64

# Cross-compile to object file
x86_64-elf-gcc -ffreestanding -nostdlib -c kernel.c -o kernel.o

# Link with bootloader
x86_64-elf-ld -T linker.ld -o kernel.elf boot.o kernel.o

# Create bootable image
# ... (platform specific)
```

## Testing Without Hardware

### Standard Mode Testing

```bash
# Compile with stubs for testing
tauraro compile kernel.tr -o kernel.c --backend c --native

# Compile and run locally
gcc kernel.c -o kernel.exe -lm
./kernel.exe
```

In standard mode:
- MMIO functions return 0
- Port I/O functions return 0 / no-op
- Interrupt functions are no-ops
- Control register functions return 0

This lets you test your kernel logic without actual hardware.

### QEMU Testing

```bash
# Generate freestanding code
tauraro compile kernel.tr -o kernel.c --backend c --freestanding

# Compile for target
x86_64-elf-gcc -ffreestanding -nostdlib kernel.c -o kernel.bin

# Test in QEMU
qemu-system-x86_64 -kernel kernel.bin
```

## Safety Considerations

### ⚠️ Important Warnings

1. **Freestanding code can damage hardware** - Incorrect MMIO/port writes can corrupt devices
2. **No memory protection** - Bare-metal has no OS safeguards
3. **Test in emulators first** - Use QEMU/Bochs before real hardware
4. **Standard mode is safe** - Use for development and testing

### Best Practices

1. **Develop in standard mode** first
2. **Test logic thoroughly** before freestanding
3. **Use QEMU** for initial hardware tests
4. **Verify register values** with datasheets
5. **Handle errors gracefully** in real code
6. **Document hardware assumptions**

## Generated Code Examples

### Standard Mode (Stub)

```c
// Generated for --native (standard mode)
static inline uint8_t mmio_read8(uintptr_t addr) {
    (void)addr;
    return 0;  // Stub - safe for user mode
}

static inline void cli(void) {
    // Stub - no-op in user mode
}
```

### Freestanding Mode (Real)

```c
// Generated for --freestanding
static inline uint8_t mmio_read8(uintptr_t addr) {
    return *(volatile uint8_t*)addr;  // Real MMIO access
}

static inline void cli(void) {
    __asm__ __volatile__ ("cli" ::: "memory");  // Real CLI
}
```

## Next Steps

- [Memory Management](memory.md) - Manual and arena allocation
- [System Programming](system-programming.md) - Low-level primitives
- [C Backend](../compilation/c-backend.md) - Compilation details
- [Performance](performance.md) - Optimization techniques
```
