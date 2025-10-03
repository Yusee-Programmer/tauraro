# Expressions

This chapter explains the syntax and semantics of expressions in Tauraro.

## Arithmetic Conversions

When a description of an arithmetic operator below uses the phrase "the numeric arguments are converted to a common type," this means that the operator implementation for built-in types works as follows:

- If either argument is a complex number, the other is converted to complex
- Otherwise, if either argument is a floating point number, the other is converted to floating point
- Otherwise, both must be integers and no conversion is necessary

Some additional rules apply for certain operators.

## Atoms

Atoms are the most basic elements of expressions. The simplest atoms are identifiers or literals.

### Identifiers

An identifier occurring as an atom is a name. See section Identifiers and Keywords for lexical definition and section Naming and Binding for documentation of naming and binding.

### Literals

Python supports string and bytes literals and various numeric literals:

```tauraro
# Numeric literals
42              # Integer
3.14            # Float
3 + 4j          # Complex

# String literals
"Hello"         # String
b"Hello"        # Bytes
"""Multiline
string"""       # Multiline string

# Boolean and None
True
False
None
```

### Parenthesized Forms

A parenthesized form is an optional expression list enclosed in parentheses:

```tauraro
# Simple grouping
result = (2 + 3) * 4

# Tuple literal
single_element = (42,)  # Note the comma
multiple_elements = (1, 2, 3)

# Empty tuple
empty = ()
```

### Displays for Lists, Sets, and Dictionaries

These are the mutable container types: lists, sets, and dictionaries.

#### List Displays

```tauraro
# Empty list
empty = []

# List with elements
numbers = [1, 2, 3, 4]

# List comprehension
squares = [x**2 for x in range(10)]

# Nested list
matrix = [[1, 2], [3, 4]]
```

#### Set Displays

```tauraro
# Empty set
empty = set()

# Set with elements
colors = {"red", "green", "blue"}

# Set comprehension
squares = {x**2 for x in range(10)}
```

#### Dictionary Displays

```tauraro
# Empty dictionary
empty = {}

# Dictionary with key-value pairs
person = {"name": "Alice", "age": 30}

# Dictionary comprehension
squares = {x: x**2 for x in range(10)}

# Using dict() constructor
person = dict(name="Alice", age=30)
```

## Primaries

Primaries represent the most tightly bound operations of the language. Their syntax is:

```
primary: atom | attributeref | subscription | slicing | call
```

### Attribute References

An attribute reference is a primary followed by a period and a name:

```tauraro
# Accessing object attributes
person.name
math.pi
list.append

# Calling methods
person.introduce()
numbers.append(42)
```

### Subscriptions

A subscription selects an item of a sequence (string, tuple or list) or mapping (dictionary) object:

```tauraro
# List subscription
numbers = [1, 2, 3, 4]
first = numbers[0]
last = numbers[-1]

# Dictionary subscription
person = {"name": "Alice", "age": 30}
name = person["name"]

# String subscription
text = "Hello"
first_char = text[0]
```

### Slicings

A slicing selects a range of items in a sequence object:

```tauraro
# Basic slicing
numbers = [0, 1, 2, 3, 4, 5]
subset = numbers[1:4]        # [1, 2, 3]
from_start = numbers[:3]     # [0, 1, 2]
to_end = numbers[3:]         # [3, 4, 5]
copy = numbers[:]            # [0, 1, 2, 3, 4, 5]

# Slicing with step
evens = numbers[::2]         # [0, 2, 4]
reverse = numbers[::-1]      # [5, 4, 3, 2, 1, 0]

# Slicing with negative indices
middle = numbers[-4:-1]      # [2, 3, 4]
```

### Calls

A call calls a callable object (e.g., a function):

```tauraro
# Simple function call
result = abs(-5)

# Call with keyword arguments
person = dict(name="Alice", age=30)

# Call with * and ** unpacking
args = [1, 2, 3]
result = max(*args)

kwargs = {"name": "Bob", "age": 25}
person = dict(**kwargs)

# Method call
text = "hello"
upper_text = text.upper()
```

## The Power Operator

The power operator binds more tightly than unary operators on its left; it binds less tightly than unary operators on its right:

```tauraro
# These are equivalent
-2**2     # -4 (-(2**2))
(-2)**2   # 4

# Right associative
2**3**2   # 512 (2**(3**2))
(2**3)**2 # 64
```

## Unary Arithmetic and Bitwise Operations

All unary arithmetic and bitwise operations have the same priority level:

```tauraro
# Unary plus
+x

# Unary minus
-x

# Bitwise NOT
~x

# Boolean NOT
not x
```

## Binary Arithmetic Operations

The binary arithmetic operations have the conventional priority levels:

```tauraro
# Exponentiation (right associative)
**

# Multiplication, matrix multiplication, division, floor division, remainder
* @ / // %

# Addition and subtraction
+ -

# Bitwise shifts
<< >>

# Bitwise AND
&

# Bitwise XOR
^

# Bitwise OR
|
```

Examples:

```tauraro
# Order of operations
result = 2 + 3 * 4      # 14
result = (2 + 3) * 4    # 20

# Mixed operations
result = 2**3 * 4       # 32
result = 2 * 3**4       # 162
```

## Comparisons

Unlike C, all comparison operations have the same priority, which is lower than that of any arithmetic, shifting or bitwise operation:

```tauraro
# Simple comparisons
x < y
x > y
x <= y
x >= y
x == y
x != y

# Identity comparisons
x is y
x is not y

# Membership tests
x in y
x not in y
```

Comparisons can be chained:

```tauraro
# These are equivalent
1 < 2 < 3
1 < 2 and 2 < 3

# More complex chaining
0 <= x < 100
```

## Boolean Operations

Boolean operations have the lowest priority of all Python operations:

```tauraro
# NOT (highest priority)
not x

# AND
x and y

# OR (lowest priority)
x or y
```

Short-circuit evaluation:

```tauraro
# 'and' short-circuits: if x is false, y is not evaluated
x and y

# 'or' short-circuits: if x is true, y is not evaluated
x or y
```

## Conditional Expressions

Conditional expressions (sometimes called a "ternary operator") have the lowest priority of all Python operations:

```tauraro
# Syntax: expression_if_true if condition else expression_if_false
result = "positive" if x > 0 else "non-positive"

# Equivalent to:
if x > 0:
    result = "positive"
else:
    result = "non-positive"
```

## Lambda Expressions

Lambda expressions yield function objects:

```tauraro
# Simple lambda
square = lambda x: x**2

# Lambda with multiple parameters
add = lambda x, y: x + y

# Lambda in higher-order functions
numbers = [1, 2, 3, 4, 5]
squared = list(map(lambda x: x**2, numbers))
evens = list(filter(lambda x: x % 2 == 0, numbers))
```

## Expression Lists

Expression lists are used in several contexts: multiple assignment, function calls, and return statements.

```tauraro
# Multiple assignment
a, b, c = 1, 2, 3

# Function call with multiple arguments
print("Hello", "World", sep=" ")

# Return multiple values
def coordinates():
    return 10, 20

x, y = coordinates()
```

## Evaluation Order

Python evaluates expressions from left to right. Notice that while evaluating an assignment, the right-hand side is evaluated before the left-hand side.

In the following lines, expressions will be evaluated in the order of their line numbers:

```tauraro
# Evaluation order demonstration
spam(1)             # 1
spam(1, 2)          # 1, 2
spam(1, some_keyword=2)  # 1, 2
spam(1, 2, 3)       # 1, 2, 3
spam(1, 2, 3, some_keyword=4)  # 1, 2, 3, 4
```

## Operator Precedence

The following table summarizes the operator precedence in Python, from highest precedence (most binding) to lowest precedence (least binding):

| Operator | Description |
|----------|-------------|
| `(expressions...)`, `[expressions...]`, `{key: value...}`, `{expressions...}` | Binding or parenthesized expression, list display, dictionary display, set display |
| `x[index]`, `x[index:index]`, `x(arguments...)`, `x.attribute` | Subscription, slicing, call, attribute reference |
| `await x` | Await expression |
| `**` | Exponentiation |
| `+x`, `-x`, `~x` | Positive, negative, bitwise NOT |
| `*`, `@`, `/`, `//`, `%` | Multiplication, matrix multiplication, division, floor division, remainder |
| `+`, `-` | Addition and subtraction |
| `<<`, `>>` | Bitwise shifts |
| `&` | Bitwise AND |
| `^` | Bitwise XOR |
| `|` | Bitwise OR |
| `in`, `not in`, `is`, `is not`, `<`, `<=`, `>`, `>=`, `!=`, `==` | Comparisons, including membership tests and identity tests |
| `not x` | Boolean NOT |
| `and` | Boolean AND |
| `or` | Boolean OR |
| `if – else` | Conditional expression |
| `lambda` | Lambda expression |
| `:=` | Assignment expression |

## Augmented Assignment Statements

Augmented assignment statements combine an operation and an assignment:

```tauraro
# Basic augmented assignments
x += 1      # x = x + 1
x -= 1      # x = x - 1
x *= 2      # x = x * 2
x /= 2      # x = x / 2
x //= 2     # x = x // 2
x %= 3      # x = x % 3
x **= 2     # x = x ** 2
x &= 3      # x = x & 3
x |= 3      # x = x | 3
x ^= 3      # x = x ^ 3
x >>= 1     # x = x >> 1
x <<= 1     # x = x << 1
```

For targets which are attribute references, the same caveat about class and instance attributes applies as for regular assignments.

## Generator Expressions

Generator expressions are surrounded by parentheses and return an iterator:

```tauraro
# Generator expression
squares = (x**2 for x in range(10))

# Using the generator
for square in squares:
    print(square)

# Converting to list
squares_list = list(x**2 for x in range(10))
```

Generator expressions are more memory-efficient than list comprehensions for large datasets.

## Yield Expressions

The `yield` expression is used when defining a generator function:

```tauraro
def countdown(n):
    while n > 0:
        yield n
        n -= 1

# Using the generator
for number in countdown(5):
    print(number)
```

The `yield from` expression is used to delegate to another generator:

```tauraro
def generator1():
    yield 1
    yield 2

def generator2():
    yield from generator1()
    yield 3

# Using the combined generator
for value in generator2():
    print(value)  # Prints 1, 2, 3
```

## Await Expressions

The `await` expression is used in async functions:

```tauraro
import asyncio

async def fetch_data():
    # Simulate async operation
    await asyncio.sleep(1)
    return "Data"

async def main():
    data = await fetch_data()
    print(data)

# Run the async function
asyncio.run(main())
```

## Assignment Expressions

The `:=` operator (walrus operator) assigns a value to a variable as part of an expression:

```tauraro
# Traditional way
data = input("Enter data: ")
if len(data) > 10:
    print(f"Long input: {data}")

# Using assignment expression
if (data := input("Enter data: ")) and len(data) > 10:
    print(f"Long input: {data}")

# In list comprehensions
numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
filtered = [y for x in numbers if (y := x**2) > 20]
print(filtered)  # [25, 36, 49, 64, 81, 100]
```