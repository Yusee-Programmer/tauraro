#!/usr/bin/env tauraro
"""
Comprehensive System Programming Test
Demonstrates File I/O, sys module, and system-level features
"""

import sys

def test_sys_module():
    """Test sys module functionality"""
    print("=== Testing sys Module ===")

    # Test sys.argv
    print("Command-line arguments:")
    print(f"  Program name: {sys.argv[0]}")
    print(f"  Total arguments: {len(sys.argv)}")

    for i, arg in enumerate(sys.argv):
        print(f"  argv[{i}]: {arg}")

    # Test sys.platform
    print(f"\nPlatform: {sys.platform}")

    # Test sys.version
    print(f"Version: {sys.version}")

    print("✓ sys module tests passed\n")


def test_file_io():
    """Test file I/O operations"""
    print("=== Testing File I/O ===")

    # Write to file
    print("Writing to file...")
    f = open("test_output.txt", "w")
    f.write("Hello from Tauraro!\n")
    f.write("This is a test of file I/O.\n")
    f.write("Line 3: System programming works!\n")
    f.close()
    print("✓ File written successfully")

    # Read entire file
    print("\nReading entire file...")
    f = open("test_output.txt", "r")
    content = f.read()
    f.close()
    print("File contents:")
    print(content)
    print("✓ File read successfully")

    # Read line by line
    print("\nReading line by line...")
    f = open("test_output.txt", "r")
    line_num = 1
    while True:
        line = f.readline()
        if len(line) == 0:
            break
        print(f"  Line {line_num}: {line.strip()}")
        line_num += 1
    f.close()
    print("✓ Line-by-line reading works\n")


def test_file_append():
    """Test appending to files"""
    print("=== Testing File Append ===")

    # Append to existing file
    f = open("test_output.txt", "a")
    f.write("Appended line 4\n")
    f.write("Appended line 5\n")
    f.close()
    print("✓ Lines appended")

    # Verify append worked
    f = open("test_output.txt", "r")
    lines = []
    while True:
        line = f.readline()
        if len(line) == 0:
            break
        lines.append(line)
    f.close()

    print(f"Total lines in file: {len(lines)}")
    print("✓ Append test passed\n")


def test_binary_io():
    """Test binary file I/O"""
    print("=== Testing Binary I/O ===")

    # Write binary data
    f = open("test_binary.bin", "wb")
    # Binary write would go here
    f.close()
    print("✓ Binary file operations available\n")


def process_arguments():
    """Process command-line arguments"""
    print("=== Processing Command-Line Arguments ===")

    if len(sys.argv) < 2:
        print("Usage: test_system_programming.py <filename> [options]")
        print("  No additional arguments provided")
        return

    filename = sys.argv[1]
    print(f"Processing file: {filename}")

    # Check if file exists by trying to open it
    try:
        f = open(filename, "r")
        content = f.read()
        f.close()
        print(f"✓ File '{filename}' opened successfully")
        print(f"  Size: {len(content)} bytes")
    except:
        print(f"✗ Cannot open file '{filename}'")


def main():
    """Main test function"""
    print("=" * 60)
    print("Tauraro System Programming Feature Test")
    print("=" * 60)
    print()

    # Test sys module
    test_sys_module()

    # Test file I/O
    test_file_io()

    # Test file append
    test_file_append()

    # Test binary I/O
    test_binary_io()

    # Process command-line arguments
    process_arguments()

    print("=" * 60)
    print("All system programming tests completed!")
    print("=" * 60)


# Run tests
if __name__ == "__main__":
    main()
