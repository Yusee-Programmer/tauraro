# Control Flow

Tauraro supports all standard Python control flow statements.

## Conditional Statements

### if Statement

```python
x = 10

if x > 0:
    print("Positive")
```

### if-else Statement

```python
if x > 0:
    print("Positive")
else:
    print("Non-positive")
```

### if-elif-else Statement

```python
if x > 0:
    print("Positive")
elif x < 0:
    print("Negative")
else:
    print("Zero")
```

### Nested Conditions

```python
age = 25
has_license = True

if age >= 18:
    if has_license:
        print("Can drive")
    else:
        print("Need license")
else:
    print("Too young")
```

### Conditional Expressions (Ternary)

```python
# value_if_true if condition else value_if_false
result = "Even" if x % 2 == 0 else "Odd"

# Nested ternary
sign = "positive" if x > 0 else "negative" if x < 0 else "zero"
```

## Loops

### while Loop

```python
# Basic while loop
count = 0
while count < 5:
    print(count)
    count += 1

# While with else
x = 0
while x < 3:
    print(x)
    x += 1
else:
    print("Loop completed normally")

# Infinite loop
while True:
    line = input("Enter command: ")
    if line == "quit":
        break
    process(line)
```

### for Loop

```python
# Iterate over sequence
for item in [1, 2, 3, 4, 5]:
    print(item)

# Iterate over string
for char in "hello":
    print(char)

# Iterate over range
for i in range(10):
    print(i)

# Iterate over dict
person = {"name": "Alice", "age": 30}
for key in person:
    print(key, person[key])

for key, value in person.items():
    print(f"{key}: {value}")

# For with else
for i in range(5):
    print(i)
else:
    print("Loop completed normally")
```

### Loop Control

#### break - Exit loop early

```python
for i in range(10):
    if i == 5:
        break
    print(i)  # Prints 0-4

# break prevents else clause
for i in range(10):
    if i == 5:
        break
else:
    print("This won't print")
```

#### continue - Skip to next iteration

```python
for i in range(10):
    if i % 2 == 0:
        continue
    print(i)  # Prints only odd numbers
```

#### pass - Do nothing placeholder

```python
for i in range(10):
    pass  # Will be implemented later

if x > 0:
    pass
else:
    print("Non-positive")
```

## Advanced Iteration

### enumerate()

Get index and value while iterating.

```python
fruits = ["apple", "banana", "cherry"]
for i, fruit in enumerate(fruits):
    print(f"{i}: {fruit}")
# 0: apple
# 1: banana
# 2: cherry

# Start from different index
for i, fruit in enumerate(fruits, start=1):
    print(f"{i}: {fruit}")
```

### zip()

Iterate over multiple sequences in parallel.

```python
names = ["Alice", "Bob", "Charlie"]
ages = [25, 30, 35]

for name, age in zip(names, ages):
    print(f"{name} is {age}")

# Zip three lists
scores = [85, 90, 95]
for name, age, score in zip(names, ages, scores):
    print(f"{name}, {age}, scored {score}")
```

### reversed()

Iterate in reverse order.

```python
for i in reversed(range(5)):
    print(i)  # 4, 3, 2, 1, 0

for char in reversed("hello"):
    print(char)  # o, l, l, e, h
```

### sorted()

Iterate in sorted order.

```python
for item in sorted([3, 1, 4, 1, 5]):
    print(item)  # 1, 1, 3, 4, 5

# Reverse sort
for item in sorted([3, 1, 4], reverse=True):
    print(item)  # 4, 3, 1

# Sort by key
words = ["apple", "pie", "zoo", "a"]
for word in sorted(words, key=len):
    print(word)  # a, pie, zoo, apple
```

## Comprehensions

### List Comprehensions

```python
# Basic
squares = [x**2 for x in range(10)]

# With condition
evens = [x for x in range(10) if x % 2 == 0]

# With transformation
upper = [s.upper() for s in ["hello", "world"]]

# Nested
matrix = [[i*j for j in range(3)] for i in range(3)]

# Flatten
nested = [[1, 2], [3, 4], [5, 6]]
flat = [x for sublist in nested for x in sublist]
# [1, 2, 3, 4, 5, 6]
```

### Dict Comprehensions

```python
# Basic
squares = {x: x**2 for x in range(5)}
# {0: 0, 1: 1, 2: 4, 3: 9, 4: 16}

# With condition
evens = {x: x**2 for x in range(10) if x % 2 == 0}

# From two lists
keys = ["a", "b", "c"]
values = [1, 2, 3]
d = {k: v for k, v in zip(keys, values)}

# Swap keys and values
original = {"a": 1, "b": 2}
swapped = {v: k for k, v in original.items()}
```

### Set Comprehensions

```python
# Basic
unique_lengths = {len(word) for word in ["a", "ab", "abc", "b"]}
# {1, 2, 3}

# With condition
even_squares = {x**2 for x in range(10) if x % 2 == 0}
```

### Generator Expressions

Memory-efficient iterators.

```python
# Generator expression (lazy evaluation)
gen = (x**2 for x in range(1000000))

# Use in iteration
for value in gen:
    if value > 100:
        break

# Convert to list if needed
squares = list(x**2 for x in range(10))
```

## Exception Handling

### try-except

```python
try:
    x = int(input("Enter number: "))
    result = 10 / x
    print(result)
except ValueError:
    print("Invalid number")
except ZeroDivisionError:
    print("Cannot divide by zero")
```

### Multiple Exceptions

```python
# Catch multiple exception types
try:
    risky_operation()
except (ValueError, TypeError) as e:
    print(f"Error: {e}")
```

### try-except-else

```python
try:
    file = open("data.txt")
except FileNotFoundError:
    print("File not found")
else:
    # Runs if no exception occurred
    content = file.read()
    file.close()
```

### try-except-finally

```python
try:
    file = open("data.txt")
    data = file.read()
except FileNotFoundError:
    print("File not found")
finally:
    # Always runs, even if exception occurred
    if 'file' in locals():
        file.close()
```

### Raising Exceptions

```python
# Raise exception
if x < 0:
    raise ValueError("x must be non-negative")

# Re-raise current exception
try:
    risky_operation()
except Exception:
    log_error()
    raise  # Re-raise the same exception
```

### Custom Exceptions

```python
class ValidationError(Exception):
    pass

def validate_age(age):
    if age < 0:
        raise ValidationError("Age cannot be negative")
    if age > 150:
        raise ValidationError("Age too large")
    return True

try:
    validate_age(-5)
except ValidationError as e:
    print(f"Validation failed: {e}")
```

## Context Managers

### with Statement

Automatically handle setup and cleanup.

```python
# File handling
with open("file.txt", "r") as f:
    content = f.read()
# File automatically closed

# Multiple context managers
with open("input.txt") as fin, open("output.txt", "w") as fout:
    fout.write(fin.read())
```

## Pattern Matching (match statement)

*Note: If implemented in Tauraro*

```python
def process_command(command):
    match command:
        case "quit":
            return "Exiting"
        case "help":
            return "Help text"
        case ["move", x, y]:
            return f"Moving to {x}, {y}"
        case _:
            return "Unknown command"
```

## Best Practices

1. **Use comprehensions** over map/filter when readable
2. **Use enumerate()** instead of range(len())
3. **Use with** for file operations
4. **Catch specific exceptions**, not bare `except:`
5. **Use break/continue** to simplify loop logic
6. **Prefer else clause** for loop completion checks
7. **Use generator expressions** for large sequences

## Performance Tips

- List comprehensions are faster than loops
- `any()` and `all()` short-circuit
- Generator expressions save memory
- Use `break` to exit loops early
- Type annotations enable C compilation optimizations

## Common Patterns

### Early Return

```python
def process(value):
    if not value:
        return None

    if value < 0:
        return "Negative"

    # Main logic
    return value * 2
```

### Loop-and-a-half

```python
while True:
    line = input("Enter text: ")
    if not line:
        break
    process(line)
```

### Iterator Protocol

```python
# Manual iteration
it = iter([1, 2, 3])
while True:
    try:
        item = next(it)
        print(item)
    except StopIteration:
        break
```

## Next Steps

- [Functions](functions.md)
- [Classes and OOP](classes.md)
- [Exceptions](exceptions.md)
- [Performance Optimization](../advanced/performance.md)
