# I/O Module

File and stream I/O operations in Tauraro.

## File Operations

### Reading Files

```python
# Read entire file
with open("file.txt", "r") as f:
    content = f.read()

# Read lines
with open("file.txt", "r") as f:
    lines = f.readlines()

# Read line by line
with open("file.txt", "r") as f:
    for line in f:
        print(line.strip())
```

### Writing Files

```python
# Write to file
with open("output.txt", "w") as f:
    f.write("Hello, World!\n")

# Write multiple lines
lines = ["Line 1\n", "Line 2\n", "Line 3\n"]
with open("output.txt", "w") as f:
    f.writelines(lines)
```

### Appending to Files

```python
with open("log.txt", "a") as f:
    f.write("New log entry\n")
```

## Binary Files

```python
# Read binary
with open("image.png", "rb") as f:
    data = f.read()

# Write binary
with open("output.bin", "wb") as f:
    f.write(b"\x00\x01\x02\x03")
```

## String I/O

```python
import io

# String buffer
buffer = io.StringIO()
buffer.write("Hello ")
buffer.write("World")
content = buffer.getvalue()  # "Hello World"

# Bytes buffer
buffer = io.BytesIO()
buffer.write(b"Hello")
data = buffer.getvalue()  # b"Hello"
```

## Next Steps

- [OS Module](#) - File system operations
- [Built-in I/O](../builtins/io.md) - I/O functions
