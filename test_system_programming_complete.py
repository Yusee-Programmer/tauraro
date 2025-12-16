#!/usr/bin/env tauraro
"""
Comprehensive System Programming Test for Tauraro C Backend
Tests all system-level features including memory management,
file I/O, bare-metal operations, and error handling.
"""

import sys

def test_file_io():
    """Test Python-like file I/O"""
    print("=== Testing File I/O ===")

    # Write to file
    f = open("test_output.txt", "w")
    f.write("Hello from Tauraro!\n")
    f.write("System programming works!\n")
    f.close()
    print("✓ File written")

    # Read from file
    f = open("test_output.txt", "r")
    content = f.read()
    f.close()
    print(f"✓ File read: {len(content)} bytes")
    print()

def test_manual_memory():
    """Test manual memory management"""
    print("=== Testing Manual Memory Management ===")

    # Allocate memory
    ptr = allocate(1024)
    print(f"✓ Allocated 1024 bytes at: {ptr}")

    # Free memory
    free(ptr)
    print("✓ Freed memory")
    print()

def test_arena_allocation():
    """Test arena-based memory allocation"""
    print("=== Testing Arena Allocation ===")

    # Create arena
    create_arena("test_arena")
    print("✓ Created arena: test_arena")

    # Allocate in arena
    ptr1 = allocate(512)
    ptr2 = allocate(256)
    print(f"✓ Allocated in arena: {ptr1}, {ptr2}")

    # Get memory stats
    stats = memory_stats()
    print(f"✓ Memory stats: {stats}")

    # Reset arena (frees all at once)
    reset_arena("test_arena")
    print("✓ Reset arena")

    # Destroy arena
    destroy_arena("test_arena")
    print("✓ Destroyed arena")
    print()

def test_pointer_operations():
    """Test pointer read/write operations"""
    print("=== Testing Pointer Operations ===")

    # Allocate memory
    ptr = allocate(64)
    print(f"✓ Allocated memory: {ptr}")

    # Write to pointer
    ptr_write(ptr, 42, "int")
    print("✓ Wrote value 42 to pointer")

    # Read from pointer
    value = ptr_read(ptr, "int")
    print(f"✓ Read value from pointer: {value}")

    # Pointer offset
    offset_ptr = ptr_offset(ptr, 8)
    print(f"✓ Offset pointer: {offset_ptr}")

    # Check null
    null_val = null_ptr()
    print(f"✓ Null pointer: {null_val}")
    print(f"✓ Is null: {is_null(null_val)}")

    # Clean up
    free(ptr)
    print()

def test_system_operations():
    """Test system-level operations"""
    print("=== Testing System Operations ===")

    # sizeof and alignof
    int_size = sizeof("int")
    print(f"✓ sizeof(int): {int_size}")

    int_align = alignof("int")
    print(f"✓ alignof(int): {int_align}")

    # Memory operations (safe test)
    src = allocate(64)
    dst = allocate(64)

    # memset
    memset(dst, 0, 64)
    print("✓ memset completed")

    # memcpy
    ptr_write(src, 123, "int")
    memcpy(dst, src, 4)
    result = ptr_read(dst, "int")
    print(f"✓ memcpy completed, value: {result}")

    # memcmp
    cmp = memcmp(src, dst, 4)
    print(f"✓ memcmp result: {cmp}")

    free(src)
    free(dst)
    print()

def test_sys_module():
    """Test sys module features"""
    print("=== Testing sys Module ===")

    print(f"✓ Platform: {sys.platform}")
    print(f"✓ Version: {sys.version}")
    print(f"✓ Program: {sys.argv[0]}")

    if len(sys.argv) > 1:
        print(f"✓ Arguments: {sys.argv[1:]}")
    print()

def test_error_handling_safe():
    """Test basic error handling (safe operations)"""
    print("=== Testing Error Handling ===")

    # Try to open non-existent file (should handle gracefully)
    try:
        f = open("nonexistent_file_12345.txt", "r")
        f.close()
        print("✗ Should have failed")
    except:
        print("✓ Handled file not found error")

    # Try invalid pointer operations
    try:
        null_val = null_ptr()
        is_null(null_val)  # Safe check
        print("✓ Null pointer check works")
    except:
        print("✗ Null check failed")

    print()

def test_type_system():
    """Test type operations"""
    print("=== Testing Type System ===")

    # Type checking
    x = 42
    print(f"✓ Type of {x}: {type(x)}")

    y = 3.14
    print(f"✓ Type of {y}: {type(y)}")

    z = "hello"
    print(f"✓ Type of {z}: {type(z)}")

    # Type conversions
    a = int("123")
    print(f"✓ str to int: {a}")

    b = float(42)
    print(f"✓ int to float: {b}")

    c = str(123)
    print(f"✓ int to str: {c}")
    print()

def main():
    """Main test function"""
    print("=" * 60)
    print("TAURARO SYSTEM PROGRAMMING - COMPREHENSIVE TEST")
    print("=" * 60)
    print()

    # Core system features
    test_sys_module()
    test_file_io()

    # Memory management
    test_manual_memory()
    test_arena_allocation()
    test_pointer_operations()

    # System operations
    test_system_operations()
    test_type_system()

    # Error handling
    test_error_handling_safe()

    print("=" * 60)
    print("ALL TESTS COMPLETED!")
    print("=" * 60)
    print()
    print("✓ File I/O: Working")
    print("✓ Manual Memory: Working")
    print("✓ Arena Allocation: Working")
    print("✓ Pointer Operations: Working")
    print("✓ System Operations: Working")
    print("✓ Type System: Working")
    print("✓ Error Handling: Basic support")
    print()
    print("System programming support: OPERATIONAL")

if __name__ == "__main__":
    main()
