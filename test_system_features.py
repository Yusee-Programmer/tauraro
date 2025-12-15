#!/usr/bin/env tauraro
"""
Complete system programming test (without sys module)
"""

def test_memory():
    print("=== Memory Management ===")
    ptr = allocate(1024)
    ptr_write(ptr, 42, "int")
    value = ptr_read(ptr, "int")
    print(f"✓ Memory operations: {value}")
    free(ptr)
    print()

def test_file_io():
    print("=== File I/O ===")
    f = open("test.txt", "w")
    f.write("Hello Tauraro!\n")
    f.close()
    print("✓ File written")

    f = open("test.txt", "r")
    content = f.read()
    f.close()
    print(f"✓ File read: {len(content)} bytes")
    print()

def test_types():
    print("=== Type Conversions ===")
    a = int("123")
    print(f"✓ str to int: {a}")

    b = float(42)
    print(f"✓ int to float: {b}")

    c = str(a)
    print(f"✓ int to str: {c}")
    print()

def main():
    print("=== Tauraro System Programming Test ===\n")
    test_memory()
    test_file_io()
    test_types()
    print("=== All Tests Passed! ===")

if __name__ == "__main__":
    main()
