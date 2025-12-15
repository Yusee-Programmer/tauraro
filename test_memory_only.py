#!/usr/bin/env tauraro
"""
Minimal test for manual memory management
"""

def main():
    print("=== Testing Manual Memory Management ===")

    # Allocate memory
    ptr = allocate(1024)
    print("Allocated 1024 bytes")

    # Write to pointer
    ptr_write(ptr, 42, "int")
    print("Wrote value 42 to pointer")

    # Read from pointer
    value = ptr_read(ptr, "int")
    print(f"Read value from pointer: {value}")

    # Free memory
    free(ptr)
    print("Freed memory")

    print("Manual memory test completed!")

if __name__ == "__main__":
    main()
