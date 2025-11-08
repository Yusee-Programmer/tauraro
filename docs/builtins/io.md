# I/O Functions

Built-in functions for input/output operations.

## print()

Output to console:

```python
print("Hello, World!")
print("Value:", 42)
print("x =", x, "y =", y)

# With separator
print("a", "b", "c", sep=", ")  # a, b, c

# With end character
print("Hello", end=" ")
print("World")  # Hello World
```

## input()

Read user input:

```python
name = input("Enter your name: ")
age = int(input("Enter your age: "))
```

## open()

Open files:

```python
# Read file
with open("file.txt", "r") as f:
    content = f.read()

# Write file
with open("output.txt", "w") as f:
    f.write("Hello, World!\n")

# Append to file
with open("log.txt", "a") as f:
    f.write("New log entry\n")
```

## File Methods

```python
f = open("file.txt", "r")

# Read entire file
content = f.read()

# Read one line
line = f.readline()

# Read all lines
lines = f.readlines()

# Write
f.write("text")

# Close
f.close()

# Always prefer using 'with' for automatic closing
```

## Next Steps

- [Core Builtins](core.md) - All built-in functions
- [File I/O](../stdlib/io.md) - Advanced file operations
