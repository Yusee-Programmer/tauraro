# Simple File I/O and sys Test
import sys

print("=== sys module test ===")
print("Program:", sys.argv[0])
print("Platform:", sys.platform)

# Simple file operations
print("\n=== File I/O test ===")
f = open("test_output.txt", "w")
f.write("Hello from Tauraro C compilation!\n")
f.close()
print("File written")

f = open("test_output.txt", "r")
content = f.read()
f.close()
print("File read:", content.strip())

print("\nAll tests passed!")
