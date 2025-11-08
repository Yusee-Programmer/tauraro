# Operators

Tauraro supports all Python operators with full compatibility. When compiled with type annotations, operators are optimized to native operations.

## Arithmetic Operators

### Basic Arithmetic

```python
a = 10
b = 3

print(a + b)   # 13 - Addition
print(a - b)   # 7  - Subtraction
print(a * b)   # 30 - Multiplication
print(a / b)   # 3.333... - Division (always returns float)
print(a // b)  # 3  - Floor division
print(a % b)   # 1  - Modulo (remainder)
print(a ** b)  # 1000 - Exponentiation
```

### Optimized Arithmetic (With Types)

```python
# Type-annotated variables compile to native operations
def calculate(x: int, y: int) -> int:
    return x + y  # Compiled to: return x + y (native int64_t)

# vs untyped (slower when compiled)
def calculate_untyped(x, y):
    return x + y  # Uses boxed arithmetic
```

**Performance**: Native operations are **10-50x faster** when compiled!

### Unary Operators

```python
x = 10
print(+x)  # 10  - Unary plus
print(-x)  # -10 - Unary minus

y = 5
print(~y)  # -6  - Bitwise NOT
```

## Comparison Operators

### Basic Comparisons

```python
a = 10
b = 20

print(a == b)  # False - Equal to
print(a != b)  # True  - Not equal to
print(a < b)   # True  - Less than
print(a <= b)  # True  - Less than or equal
print(a > b)   # False - Greater than
print(a >= b)  # False - Greater than or equal
```

### Identity Operators

```python
x = [1, 2, 3]
y = [1, 2, 3]
z = x

print(x == y)    # True  - Same value
print(x is y)    # False - Different objects
print(x is z)    # True  - Same object
print(x is not y) # True - Different objects
```

### Membership Operators

```python
# in operator
numbers = [1, 2, 3, 4, 5]
print(3 in numbers)     # True
print(10 in numbers)    # False
print(10 not in numbers) # True

# Works with strings
text = "Hello, World!"
print("Hello" in text)  # True
print("Goodbye" in text) # False

# Works with dictionaries (checks keys)
data = {"name": "Alice", "age": 30}
print("name" in data)  # True
print("city" in data)  # False
```

### Chaining Comparisons

```python
x = 5
print(1 < x < 10)  # True
print(0 <= x <= 5)  # True
print(x == 5 == 5)  # True

# Equivalent to:
print((1 < x) and (x < 10))
```

## Logical Operators

### Boolean Logic

```python
a = True
b = False

print(a and b)  # False - Logical AND
print(a or b)   # True  - Logical OR
print(not a)    # False - Logical NOT

# Short-circuit evaluation
def expensive_check():
    print("Called!")
    return True

# 'expensive_check' is NOT called because 'a' is already False
result = False and expensive_check()

# 'expensive_check' IS called because first condition is True
result = True and expensive_check()
```

### Truthy and Falsy Values

```python
# Falsy values
print(bool(False))    # False
print(bool(None))     # False
print(bool(0))        # False
print(bool(0.0))      # False
print(bool(""))       # False
print(bool([]))       # False
print(bool({}))       # False

# Truthy values
print(bool(True))     # True
print(bool(1))        # True
print(bool("hello"))  # True
print(bool([1]))      # True
print(bool({"a": 1})) # True
```

### Conditional Expressions

```python
# Ternary operator
x = 10
result = "positive" if x > 0 else "non-positive"

# Using 'or' for defaults
name = input_name or "Unknown"

# Using 'and' for conditional execution
is_valid and process_data()
```

## Bitwise Operators

### Binary Operations

```python
a = 60  # 0011 1100
b = 13  # 0000 1101

print(a & b)   # 12  - 0000 1100 - AND
print(a | b)   # 61  - 0011 1101 - OR
print(a ^ b)   # 49  - 0011 0001 - XOR
print(~a)      # -61 - 1100 0011 - NOT
print(a << 2)  # 240 - 1111 0000 - Left shift
print(a >> 2)  # 15  - 0000 1111 - Right shift
```

### Bit Manipulation Examples

```python
# Set bit
def set_bit(num: int, pos: int) -> int:
    return num | (1 << pos)

# Clear bit
def clear_bit(num: int, pos: int) -> int:
    return num & ~(1 << pos)

# Toggle bit
def toggle_bit(num: int, pos: int) -> int:
    return num ^ (1 << pos)

# Check if bit is set
def is_bit_set(num: int, pos: int) -> bool:
    return (num & (1 << pos)) != 0
```

## Assignment Operators

### Compound Assignment

```python
x = 10

# Arithmetic
x += 5   # x = x + 5  => 15
x -= 3   # x = x - 3  => 12
x *= 2   # x = x * 2  => 24
x /= 4   # x = x / 4  => 6.0
x //= 2  # x = x // 2 => 3.0
x %= 2   # x = x % 2  => 1.0
x **= 3  # x = x ** 3 => 1.0

# Bitwise
x = 60
x &= 13  # x = x & 13
x |= 13  # x = x | 13
x ^= 13  # x = x ^ 13
x <<= 2  # x = x << 2
x >>= 2  # x = x >> 2
```

### Walrus Operator (Assignment Expression)

```python
# Assign and use in same expression (Python 3.8+)
if (n := len(data)) > 10:
    print(f"Processing {n} items")

# In while loops
while (line := file.readline()) != "":
    process(line)

# In list comprehensions
results = [y for x in data if (y := transform(x)) is not None]
```

## String Operators

### Concatenation

```python
first = "Hello"
last = "World"

# Using +
full = first + " " + last  # "Hello World"

# Using *
repeat = "Ha" * 3  # "HaHaHa"

# Using +=
message = "Hello"
message += " World"  # "Hello World"
```

### String Formatting

```python
name = "Alice"
age = 30

# f-strings (recommended)
msg = f"My name is {name} and I'm {age} years old"

# Format method
msg = "My name is {} and I'm {} years old".format(name, age)

# % operator (old style)
msg = "My name is %s and I'm %d years old" % (name, age)
```

## Sequence Operators

### Indexing and Slicing

```python
numbers = [1, 2, 3, 4, 5]

# Indexing
print(numbers[0])   # 1 - First element
print(numbers[-1])  # 5 - Last element
print(numbers[2])   # 3 - Third element

# Slicing
print(numbers[1:4])  # [2, 3, 4] - Elements 1-3
print(numbers[:3])   # [1, 2, 3] - First 3
print(numbers[2:])   # [3, 4, 5] - From index 2
print(numbers[::2])  # [1, 3, 5] - Every 2nd element
print(numbers[::-1]) # [5, 4, 3, 2, 1] - Reversed
```

### Concatenation and Repetition

```python
# Lists
list1 = [1, 2, 3]
list2 = [4, 5, 6]
combined = list1 + list2  # [1, 2, 3, 4, 5, 6]
repeated = list1 * 2      # [1, 2, 3, 1, 2, 3]

# Tuples
tuple1 = (1, 2)
tuple2 = (3, 4)
combined = tuple1 + tuple2  # (1, 2, 3, 4)
```

## Dictionary Operators

### Access and Membership

```python
data = {"name": "Alice", "age": 30, "city": "NYC"}

# Access
print(data["name"])  # "Alice"
print(data.get("name"))  # "Alice"
print(data.get("country", "USA"))  # "USA" (default)

# Membership
print("name" in data)  # True
print("country" in data)  # False

# Update
data["age"] = 31
data.update({"country": "USA", "state": "NY"})
```

### Dictionary Unpacking

```python
dict1 = {"a": 1, "b": 2}
dict2 = {"c": 3, "d": 4}

# Merge dictionaries
merged = {**dict1, **dict2}  # {"a": 1, "b": 2, "c": 3, "d": 4}

# Override values
overridden = {**dict1, "b": 20}  # {"a": 1, "b": 20}
```

## Operator Precedence

From highest to lowest priority:

1. `**` - Exponentiation
2. `+x`, `-x`, `~x` - Unary operators
3. `*`, `/`, `//`, `%` - Multiplication, division, modulo
4. `+`, `-` - Addition, subtraction
5. `<<`, `>>` - Bitwise shifts
6. `&` - Bitwise AND
7. `^` - Bitwise XOR
8. `|` - Bitwise OR
9. `==`, `!=`, `<`, `<=`, `>`, `>=`, `is`, `is not`, `in`, `not in` - Comparisons
10. `not` - Logical NOT
11. `and` - Logical AND
12. `or` - Logical OR

### Using Parentheses

```python
# Without parentheses (follows precedence)
result = 2 + 3 * 4  # 14 (multiplication first)

# With parentheses (override precedence)
result = (2 + 3) * 4  # 20

# Complex expression
result = (a + b) * (c - d) / e
```

## Custom Operators (Magic Methods)

### Arithmetic Operators

```python
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)

    def __str__(self):
        return f"Vector({self.x}, {self.y})"

v1 = Vector(1, 2)
v2 = Vector(3, 4)
print(v1 + v2)  # Vector(4, 6)
print(v1 * 2)   # Vector(2, 4)
```

### Comparison Operators

```python
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def __eq__(self, other):
        return self.age == other.age

    def __lt__(self, other):
        return self.age < other.age

    def __le__(self, other):
        return self.age <= other.age

alice = Person("Alice", 30)
bob = Person("Bob", 25)

print(alice > bob)  # True (30 > 25)
print(alice == bob) # False
```

## Performance Considerations

### Typed vs Untyped

```python
# Untyped - slower when compiled
def calculate(x, y):
    return x * 2 + y

# Typed - faster when compiled
def calculate_typed(x: int, y: int) -> int:
    return x * 2 + y  # Native operations!
```

### Operator Complexity

| Operator | Time Complexity | Notes |
|----------|----------------|-------|
| `+` (numbers) | O(1) | Constant time |
| `+` (strings) | O(n) | Depends on length |
| `+` (lists) | O(n) | Creates new list |
| `*` (numbers) | O(1) | Constant time |
| `*` (sequences) | O(n) | Depends on repetition |
| `in` (list) | O(n) | Linear search |
| `in` (set/dict) | O(1) | Hash lookup |
| `==` (numbers) | O(1) | Constant time |
| `==` (sequences) | O(n) | Element-wise comparison |

## Best Practices

### 1. Use Appropriate Operators

```python
# Good - clear intent
is_valid = x >= 0 and x <= 100

# Better - chained comparison
is_valid = 0 <= x <= 100
```

### 2. Avoid Complex Expressions

```python
# Bad - hard to read
result = ((a + b) * c - d / e) ** f % g

# Good - break into steps
sum_ab = a + b
product = sum_ab * c
quotient = d / e
difference = product - quotient
power = difference ** f
result = power % g
```

### 3. Use Type Annotations for Performance

```python
# Fast when compiled
def dot_product(a: List[float], b: List[float]) -> float:
    result: float = 0.0
    for i in range(len(a)):
        result += a[i] * b[i]
    return result
```

### 4. Leverage Short-Circuit Evaluation

```python
# Efficient - doesn't call expensive_check if condition is False
if is_enabled() and expensive_check():
    process()

# Use 'or' for defaults
value = cached_value or compute_value()
```

## Common Patterns

### Safe Division

```python
# Check before dividing
if b != 0:
    result = a / b

# Using conditional expression
result = a / b if b != 0 else 0
```

### Clamping Values

```python
# Clamp to range [min_val, max_val]
value = max(min_val, min(value, max_val))

# Using comparison chain
if not (min_val <= value <= max_val):
    value = min_val if value < min_val else max_val
```

### Swapping

```python
# Pythonic swap
a, b = b, a

# XOR swap (for integers only)
a ^= b
b ^= a
a ^= b
```

## Next Steps

- [Variables](variables.md) - Variable declaration and types
- [Control Flow](control-flow.md) - Using operators in conditions
- [Functions](functions.md) - Operator usage in functions
- [Classes](classes.md) - Operator overloading
- [Performance](../advanced/performance.md) - Optimization techniques
