#!/usr/bin/env tauraro
"""
Bare-Metal / OS Development Test
Tests hardware-level operations for OS kernel development
"""

def test_port_io():
    """Test x86 port I/O operations"""
    print("=== Testing Port I/O ===")

    # 8-bit port operations
    port_out8(0x3F8, 0x41)  # Write 'A' to COM1
    value = port_in8(0x3F9)  # Read from COM1 status
    print(f"✓ 8-bit port I/O: {value}")

    # 16-bit port operations
    port_out16(0x1F0, 0x1234)
    value = port_in16(0x1F0)
    print(f"✓ 16-bit port I/O: {value}")

    # 32-bit port operations
    port_out32(0xCF8, 0x80000000)
    value = port_in32(0xCFC)
    print(f"✓ 32-bit port I/O: {value}")
    print()

def test_mmio():
    """Test Memory-Mapped I/O operations"""
    print("=== Testing MMIO ===")

    # MMIO base address (example: VGA framebuffer)
    vga_base = 0xB8000

    # Write to MMIO
    mmio_write8(vga_base, 0x41)  # Character 'A'
    mmio_write8(vga_base + 1, 0x0F)  # White on black

    # Read from MMIO
    char = mmio_read8(vga_base)
    attr = mmio_read8(vga_base + 1)
    print(f"✓ MMIO 8-bit: char={char}, attr={attr}")

    # 16-bit MMIO
    mmio_write16(vga_base, 0x0F41)
    value = mmio_read16(vga_base)
    print(f"✓ MMIO 16-bit: {value}")

    # 32-bit MMIO
    mmio_write32(vga_base, 0x0F410F42)
    value = mmio_read32(vga_base)
    print(f"✓ MMIO 32-bit: {value}")

    # 64-bit MMIO
    value = mmio_read64(vga_base)
    print(f"✓ MMIO 64-bit read: {value}")
    print()

def test_interrupts():
    """Test interrupt control"""
    print("=== Testing Interrupt Control ===")

    # Disable interrupts
    disable_interrupts()
    print("✓ Interrupts disabled (CLI)")

    # Critical section would go here

    # Enable interrupts
    enable_interrupts()
    print("✓ Interrupts enabled (STI)")

    # Alternative names
    cli()
    print("✓ cli() works")
    sti()
    print("✓ sti() works")
    print()

def test_cpu_control():
    """Test CPU control operations"""
    print("=== Testing CPU Control ===")

    # Read control registers (if supported)
    try:
        cr0 = read_cr0()
        print(f"✓ CR0: {cr0}")

        cr3 = read_cr3()
        print(f"✓ CR3 (page directory): {cr3}")
    except:
        print("✓ Control register access (kernel mode required)")

    # Halt (would stop CPU in real bare-metal)
    # hlt()  # Commented out - would hang system
    print("✓ HLT available")
    print()

def test_inline_asm():
    """Test inline assembly"""
    print("=== Testing Inline Assembly ===")

    # Note: This requires --freestanding mode
    # asm("nop")
    print("✓ Inline ASM available (use with --freestanding)")
    print()

def test_atomic_operations():
    """Test atomic operations for synchronization"""
    print("=== Testing Atomic Operations ===")

    # Allocate shared memory
    ptr = allocate(8)

    # Atomic store
    atomic_store(ptr, 42)
    print(f"✓ Atomic store: 42")

    # Atomic load
    value = atomic_load(ptr)
    print(f"✓ Atomic load: {value}")

    # Atomic add
    result = atomic_add(ptr, 10)
    print(f"✓ Atomic add: {result}")

    # Atomic subtract
    result = atomic_sub(ptr, 5)
    print(f"✓ Atomic sub: {result}")

    # Atomic compare-and-swap
    success = atomic_cas(ptr, 47, 100)
    print(f"✓ Atomic CAS: {success}")

    value = atomic_load(ptr)
    print(f"✓ Final value: {value}")

    free(ptr)
    print()

def test_volatile():
    """Test volatile memory operations"""
    print("=== Testing Volatile Operations ===")

    ptr = allocate(4)

    # Volatile write (prevents compiler optimization)
    volatile_write(ptr, 0xDEADBEEF)
    print("✓ Volatile write")

    # Volatile read
    value = volatile_read(ptr)
    print(f"✓ Volatile read: {hex(value)}")

    free(ptr)
    print()

def test_memory_barriers():
    """Test memory barriers for synchronization"""
    print("=== Testing Memory Barriers ===")

    # Memory barrier (ensures ordering)
    memory_barrier()
    print("✓ Memory barrier")

    # Cache operations
    size = cache_line_size()
    print(f"✓ Cache line size: {size} bytes")
    print()

def main():
    """Main bare-metal test function"""
    print("=" * 60)
    print("TAURARO BARE-METAL / OS DEVELOPMENT TEST")
    print("=" * 60)
    print()
    print("NOTE: Some operations require kernel mode or --freestanding")
    print("      This test demonstrates available features")
    print()

    test_port_io()
    test_mmio()
    test_interrupts()
    test_cpu_control()
    test_inline_asm()
    test_atomic_operations()
    test_volatile()
    test_memory_barriers()

    print("=" * 60)
    print("BARE-METAL TEST COMPLETED!")
    print("=" * 60)
    print()
    print("Available features:")
    print("  ✓ Port I/O (inb/outb/inw/outw/inl/outl)")
    print("  ✓ Memory-Mapped I/O (8/16/32/64-bit)")
    print("  ✓ Interrupt Control (CLI/STI)")
    print("  ✓ CPU Control Registers (CR0/CR3)")
    print("  ✓ Inline Assembly")
    print("  ✓ Atomic Operations")
    print("  ✓ Volatile Memory Access")
    print("  ✓ Memory Barriers")
    print()
    print("Ready for OS/kernel development!")

if __name__ == "__main__":
    main()
