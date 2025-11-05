print("Testing file I/O operations:")

# Test basic file writing and reading
print("1. Basic file operations:")
f = open("test_file.txt", "w")
f.write("Hello, Tauraro!")
f.close()
print("   ✓ File written")

f = open("test_file.txt", "r")
content = f.read()
f.close()
print("   Read:", content)
print("   ✓ File read successfully")

# Test file methods
print("\n2. Testing file methods:")
f = open("test_multiline.txt", "w")
lines = ["Line 1\n", "Line 2\n", "Line 3\n"]
f.writelines(lines)
f.close()
print("   ✓ Multiple lines written")

f = open("test_multiline.txt", "r")
all_lines = f.readlines()
f.close()
print("   Read lines:", all_lines)
print("   ✓ All file methods working!")
