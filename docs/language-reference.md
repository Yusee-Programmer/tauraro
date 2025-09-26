# TauraroLang Language Reference

**Complete reference for TauraroLang syntax, semantics, and language features**

## Table of Contents

1. [Overview](#overview)
2. [Lexical Structure](#lexical-structure)
3. [Data Types](#data-types)
4. [Variables and Constants](#variables-and-constants)
5. [Operators](#operators)
6. [Control Flow](#control-flow)
7. [Functions](#functions)
8. [Classes and Objects](#classes-and-objects)
9. [Collections](#collections)
10. [Error Handling](#error-handling)
11. [Async Programming](#async-programming)
12. [Type System](#type-system)
13. [Memory Management](#memory-management)
14. [Modules and Imports](#modules-and-imports)

## Overview

TauraroLang is a dynamically typed programming language with optional static typing support. It features a clean, expressive syntax inspired by modern programming languages while maintaining simplicity and readability.

### Key Characteristics

- **Dynamic Typing**: Variables can hold values of any type
- **Optional Static Typing**: Add type annotations for better tooling and performance
- **Expression-Based**: Most constructs are expressions that return values
- **Memory Safe**: Automatic memory management with manual control options
- **Interoperable**: Seamless integration with C and Python

## Lexical Structure

### Comments

```tauraro
// Single-line comment

/*
   Multi-line comment
   Can span multiple lines
*/

/// Documentation comment
/// Used for generating documentation
```

### Identifiers

Identifiers must start with a letter or underscore, followed by letters, digits, or underscores:

```tauraro
valid_identifier
_private_var
MyClass
variable123
```

### Keywords

Reserved keywords in TauraroLang:

```
aiki        // function definition
fitar       // return statement
idan        // if statement
kuma        // else statement
don         // for loop
yayin       // while loop
dauki       // assignment/let
matsayin    // type assertion
async       // async function
await       // await expression
extern      // external function
export      // export declaration
import      // import statement
class       // class definition
struct      // structure definition
enum        // enumeration
match       // pattern matching
try         // exception handling
catch       // exception handler
finally     // cleanup block
break       // loop break
continue    // loop continue
true        // boolean true
false       // boolean false
null        // null value
none        // none value
```

### Literals

#### Integer Literals
```tauraro
42          // decimal
0x2A        // hexadecimal
0o52        // octal
0b101010    // binary
1_000_000   // with separators
```

#### Float Literals
```tauraro
3.14
2.5e10
1.5E-5
.5          // 0.5
5.          // 5.0
```

#### String Literals
```tauraro
"Hello, World!"
'Single quotes'
"""
Multi-line string
with line breaks
"""
f"Formatted string with {variable}"
r"Raw string with \n literal backslashes"
```

#### Boolean and Special Literals
```tauraro
true
false
null
none
```

## Data Types

### Primitive Types

#### Integer (`int`)
64-bit signed integers:
```tauraro
let age = 25
let negative = -100
let large = 9223372036854775807
```

#### Float (`float`)
64-bit floating-point numbers:
```tauraro
let pi = 3.14159
let scientific = 1.23e-4
let infinity = float("inf")
```

#### Boolean (`bool`)
```tauraro
let is_active = true
let is_complete = false
```

#### String (`str`)
UTF-8 encoded strings:
```tauraro
let name = "TauraroLang"
let greeting = f"Hello, {name}!"
let multiline = """
    This is a
    multi-line string
"""
```

#### None Type
```tauraro
let empty = none
let uninitialized = null
```

### Collection Types

#### List (`list`)
Ordered, mutable collections:
```tauraro
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "hello", true, 3.14]
let empty_list = []

// List operations
numbers.append(6)
numbers[0] = 10
let first = numbers[0]
let length = len(numbers)
```

#### Dictionary (`dict`)
Key-value mappings:
```tauraro
let person = {
    "name": "Alice",
    "age": 30,
    "city": "New York"
}

// Dictionary operations
person["email"] = "alice@example.com"
let name = person["name"]
let keys = person.keys()
```

#### Tuple
Immutable ordered collections:
```tauraro
let coordinates = (10, 20)
let rgb = (255, 128, 0)
let single = (42,)  // Single element tuple

// Tuple unpacking
let (x, y) = coordinates
```

## Variables and Constants

### Variable Declaration

```tauraro
// Basic declaration
let x = 42
let name = "TauraroLang"

// Multiple assignment
let a, b, c = 1, 2, 3
let (x, y) = (10, 20)

// Type annotations (optional)
let age: int = 25
let pi: float = 3.14159
let active: bool = true
```

### Assignment Operators

```tauraro
let x = 10

// Compound assignment
x += 5      // x = x + 5
x -= 3      // x = x - 3
x *= 2      // x = x * 2
x /= 4      // x = x / 4
x %= 3      // x = x % 3
x **= 2     // x = x ** 2
```

### Constants

```tauraro
// Constants (by convention, use UPPER_CASE)
let PI = 3.14159
let MAX_SIZE = 1000
```

## Operators

### Arithmetic Operators

```tauraro
let a = 10
let b = 3

let sum = a + b         // 13
let diff = a - b        // 7
let product = a * b     // 30
let quotient = a / b    // 3.333...
let remainder = a % b   // 1
let power = a ** b      // 1000

// Unary operators
let neg = -a            // -10
let pos = +a            // 10
```

### Comparison Operators

```tauraro
let x = 10
let y = 20

x == y      // false (equality)
x != y      // true (inequality)
x < y       // true (less than)
x <= y      // true (less than or equal)
x > y       // false (greater than)
x >= y      // false (greater than or equal)
```

### Logical Operators

```tauraro
let a = true
let b = false

a && b      // false (logical AND)
a || b      // true (logical OR)
!a          // false (logical NOT)

// Short-circuit evaluation
let result = a && expensive_function()  // Only calls function if a is true
```

### Bitwise Operators

```tauraro
let x = 12  // 1100 in binary
let y = 10  // 1010 in binary

x & y       // 8 (1000) - bitwise AND
x | y       // 14 (1110) - bitwise OR
x ^ y       // 6 (0110) - bitwise XOR
~x          // -13 - bitwise NOT
x << 2      // 48 (110000) - left shift
x >> 2      // 3 (11) - right shift
```

### String Operators

```tauraro
let first = "Hello"
let second = "World"

let greeting = first + " " + second  // "Hello World"
let repeated = "Ha" * 3              // "HaHaHa"

// String formatting
let name = "Alice"
let age = 30
let message = f"My name is {name} and I'm {age} years old"
```

### Membership Operators

```tauraro
let numbers = [1, 2, 3, 4, 5]
let text = "Hello, World!"

2 in numbers        // true
6 in numbers        // false
"Hello" in text     // true
"Goodbye" in text   // false
```

## Control Flow

### Conditional Statements

#### If-Else
```tauraro
let age = 18

idan age >= 18:
    print("Adult")
kuma idan age >= 13:
    print("Teenager")
kuma:
    print("Child")

// Ternary operator
let status = age >= 18 ? "Adult" : "Minor"
```

#### Match Expressions
```tauraro
let value = 42

let result = match value:
    0 -> "zero"
    1 -> "one"
    2..10 -> "small"
    11..100 -> "medium"
    _ -> "large"

// Pattern matching with types
let data = [1, 2, 3]
match data:
    [] -> print("Empty list")
    [x] -> print(f"Single element: {x}")
    [x, y] -> print(f"Two elements: {x}, {y}")
    [x, ...rest] -> print(f"First: {x}, Rest: {rest}")
```

### Loops

#### For Loops
```tauraro
// Iterate over range
don i in range(5):
    print(i)  // 0, 1, 2, 3, 4

// Iterate over collection
let fruits = ["apple", "banana", "orange"]
don fruit in fruits:
    print(fruit)

// Enumerate with index
don i, fruit in enumerate(fruits):
    print(f"{i}: {fruit}")

// Dictionary iteration
let person = {"name": "Alice", "age": 30}
don key, value in person.items():
    print(f"{key}: {value}")
```

#### While Loops
```tauraro
let count = 0
yayin count < 5:
    print(count)
    count += 1

// Infinite loop with break
yayin true:
    let input = get_input()
    idan input == "quit":
        break
    process(input)
```

#### Loop Control
```tauraro
don i in range(10):
    idan i == 3:
        continue  // Skip iteration
    idan i == 7:
        break     // Exit loop
    print(i)
```

## Functions

### Function Definition

```tauraro
// Basic function
aiki greet(name):
    fitar f"Hello, {name}!"

// Function with default parameters
aiki power(base, exponent = 2):
    fitar base ** exponent

// Function with type annotations
aiki add(a: int, b: int) -> int:
    fitar a + b

// Variable arguments
aiki sum_all(*args):
    let total = 0
    don arg in args:
        total += arg
    fitar total

// Keyword arguments
aiki create_person(**kwargs):
    fitar {
        "name": kwargs.get("name", "Unknown"),
        "age": kwargs.get("age", 0)
    }
```

### Function Calls

```tauraro
// Basic call
let message = greet("Alice")

// Named arguments
let result = power(base=2, exponent=3)

// Unpacking arguments
let numbers = [1, 2, 3, 4, 5]
let total = sum_all(*numbers)

// Unpacking keyword arguments
let person_data = {"name": "Bob", "age": 25}
let person = create_person(**person_data)
```

### Lambda Functions

```tauraro
// Anonymous functions
let square = lambda x: x * x
let add = lambda a, b: a + b

// Higher-order functions
let numbers = [1, 2, 3, 4, 5]
let squares = map(lambda x: x * x, numbers)
let evens = filter(lambda x: x % 2 == 0, numbers)
```

### Closures

```tauraro
aiki make_counter():
    let count = 0
    
    aiki increment():
        count += 1
        fitar count
    
    fitar increment

let counter = make_counter()
print(counter())  // 1
print(counter())  // 2
```

## Classes and Objects

### Class Definition

```tauraro
class Person:
    // Class variable
    species = "Homo sapiens"
    
    // Constructor
    aiki __init__(self, name, age):
        self.name = name
        self.age = age
    
    // Instance method
    aiki greet(self):
        fitar f"Hello, I'm {self.name}"
    
    // Class method
    @classmethod
    aiki from_string(cls, person_str):
        name, age = person_str.split(",")
        fitar cls(name, int(age))
    
    // Static method
    @staticmethod
    aiki is_adult(age):
        fitar age >= 18
    
    // Property
    @property
    aiki description(self):
        fitar f"{self.name} is {self.age} years old"
```

### Inheritance

```tauraro
class Student(Person):
    aiki __init__(self, name, age, student_id):
        super().__init__(name, age)
        self.student_id = student_id
    
    aiki greet(self):
        fitar f"Hi, I'm {self.name}, student #{self.student_id}"
    
    aiki study(self, subject):
        fitar f"{self.name} is studying {subject}"
```

### Object Usage

```tauraro
// Create instances
let person = Person("Alice", 30)
let student = Student("Bob", 20, "S12345")

// Method calls
print(person.greet())
print(student.study("Mathematics"))

// Property access
print(person.description)

// Class and static methods
let person2 = Person.from_string("Charlie,25")
print(Person.is_adult(17))  // false
```

## Collections

### List Operations

```tauraro
let numbers = [1, 2, 3, 4, 5]

// Access and modification
numbers[0] = 10
numbers.append(6)
numbers.insert(2, 99)
numbers.remove(3)
let popped = numbers.pop()

// Slicing
let subset = numbers[1:4]    // [2, 99, 4]
let reversed = numbers[::-1] // Reverse order

// List comprehensions
let squares = [x * x for x in range(10)]
let evens = [x for x in numbers if x % 2 == 0]
```

### Dictionary Operations

```tauraro
let person = {"name": "Alice", "age": 30}

// Access and modification
person["city"] = "New York"
let name = person.get("name", "Unknown")
person.update({"email": "alice@example.com", "phone": "123-456-7890"})

// Dictionary comprehensions
let squares = {x: x*x for x in range(5)}
let filtered = {k: v for k, v in person.items() if len(str(v)) > 3}
```

### Set Operations

```tauraro
let set1 = {1, 2, 3, 4, 5}
let set2 = {4, 5, 6, 7, 8}

// Set operations
let union = set1 | set2         // {1, 2, 3, 4, 5, 6, 7, 8}
let intersection = set1 & set2  // {4, 5}
let difference = set1 - set2    // {1, 2, 3}
let symmetric_diff = set1 ^ set2 // {1, 2, 3, 6, 7, 8}

// Set methods
set1.add(9)
set1.remove(1)
let is_subset = set1.issubset(set2)
```

## Error Handling

### Try-Catch Blocks

```tauraro
try:
    let result = risky_operation()
    print(f"Success: {result}")
catch ValueError as e:
    print(f"Value error: {e}")
catch Exception as e:
    print(f"General error: {e}")
finally:
    print("Cleanup code")
```

### Raising Exceptions

```tauraro
aiki divide(a, b):
    idan b == 0:
        raise ValueError("Cannot divide by zero")
    fitar a / b

aiki validate_age(age):
    idan age < 0:
        raise ValueError("Age cannot be negative")
    idan age > 150:
        raise ValueError("Age seems unrealistic")
```

### Custom Exceptions

```tauraro
class CustomError(Exception):
    aiki __init__(self, message, code):
        super().__init__(message)
        self.code = code

try:
    raise CustomError("Something went wrong", 500)
catch CustomError as e:
    print(f"Error {e.code}: {e}")
```

## Async Programming

### Async Functions

```tauraro
async aiki fetch_data(url):
    let response = await http_get(url)
    let data = await response.json()
    fitar data

async aiki process_urls(urls):
    let tasks = []
    don url in urls:
        tasks.append(fetch_data(url))
    
    let results = await gather(*tasks)
    fitar results
```

### Async Context Managers

```tauraro
async aiki async_file_operation():
    async with open_async("file.txt") as f:
        let content = await f.read()
        fitar content
```

### Generators and Async Generators

```tauraro
// Generator function
aiki fibonacci():
    let a, b = 0, 1
    yayin true:
        yield a
        a, b = b, a + b

// Async generator
async aiki async_counter(max_count):
    don i in range(max_count):
        await sleep(1)
        yield i
```

## Type System

### Type Annotations

```tauraro
// Variable annotations
let name: str = "Alice"
let age: int = 30
let scores: list[int] = [85, 92, 78]
let person: dict[str, any] = {"name": "Bob", "age": 25}

// Function annotations
aiki process_data(data: list[dict[str, any]]) -> dict[str, int]:
    let result: dict[str, int] = {}
    don item in data:
        result[item["name"]] = item["score"]
    fitar result
```

### Generic Types

```tauraro
// Generic function
aiki identity<T>(value: T) -> T:
    fitar value

// Generic class
class Container<T>:
    aiki __init__(self, value: T):
        self.value = value
    
    aiki get(self) -> T:
        fitar self.value

let int_container = Container<int>(42)
let str_container = Container<str>("hello")
```

### Union Types

```tauraro
// Union type annotation
aiki process_id(id: int | str) -> str:
    idan type(id) == int:
        fitar f"ID: {id:06d}"
    kuma:
        fitar f"ID: {id.upper()}"

// Optional types
aiki find_user(name: str) -> User | none:
    // Search logic here
    fitar found_user or none
```

### Type Checking

```tauraro
// Runtime type checking
aiki safe_divide(a: any, b: any) -> float:
    idan not isinstance(a, (int, float)) or not isinstance(b, (int, float)):
        raise TypeError("Arguments must be numbers")
    idan b == 0:
        raise ValueError("Cannot divide by zero")
    fitar float(a) / float(b)
```

## Memory Management

### Automatic Memory Management

TauraroLang uses automatic garbage collection for most memory management:

```tauraro
// Objects are automatically cleaned up when no longer referenced
aiki create_large_data():
    let data = [i for i in range(1000000)]
    fitar data  // Memory will be freed when data goes out of scope
```

### Manual Memory Control

For performance-critical code, manual memory management is available:

```tauraro
// Explicit memory allocation
let buffer = allocate(1024)  // Allocate 1KB
buffer.write(0, b"Hello")
let data = buffer.read(0, 5)
deallocate(buffer)  // Explicit cleanup

// RAII-style resource management
with managed_resource() as resource:
    resource.use()
    // Resource automatically cleaned up
```

### Weak References

```tauraro
import weakref

class Parent:
    aiki __init__(self):
        self.children = []

class Child:
    aiki __init__(self, parent):
        self.parent = weakref.ref(parent)  // Weak reference to avoid cycles
        parent.children.append(self)
```

## Modules and Imports

### Module Definition

```tauraro
// math_utils.tr
export aiki add(a, b):
    fitar a + b

export aiki multiply(a, b):
    fitar a * b

export let PI = 3.14159

// Private function (not exported)
aiki _internal_helper():
    fitar "helper"
```

### Import Statements

```tauraro
// Import entire module
import math_utils
let result = math_utils.add(2, 3)

// Import specific functions
import {add, multiply} from math_utils
let sum = add(5, 7)
let product = multiply(3, 4)

// Import with alias
import math_utils as math
let pi_value = math.PI

// Import all exports
import * from math_utils
```

### Package Structure

```
my_package/
├── __init__.tr
├── core.tr
├── utils/
│   ├── __init__.tr
│   ├── helpers.tr
│   └── validators.tr
└── tests/
    └── test_core.tr
```

```tauraro
// Import from package
import my_package.core
import {validate_email} from my_package.utils.validators
```

---

This language reference provides a comprehensive overview of TauraroLang's syntax and features. For more detailed examples and tutorials, see the [Getting Started Guide](getting-started.md) and [API Documentation](api-reference.md).