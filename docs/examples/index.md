# Code Examples

This section provides practical examples of Tauraro programming patterns and best practices.

## Basic Examples

### Hello World

```python
print("Hello, World!")
```

### Variables and Types

```python
# Dynamic typing (no annotation)
x = 42
x = "now a string"  # OK

# Static typing (with annotation) - ENFORCED at runtime
count: int = 10
count = 20          # OK
count = "error"     # ERROR! Type mismatch

# Multiple assignments
a, b, c = 1, 2, 3
x = y = z = 0
```

### User Input

```python
name = input("Enter your name: ")
age = int(input("Enter your age: "))
print(f"Hello {name}, you are {age} years old")
```

## Data Structures

### Working with Lists

```python
# Create and manipulate lists
numbers = [1, 2, 3, 4, 5]
numbers.append(6)
numbers.extend([7, 8, 9])
numbers.insert(0, 0)

# List comprehension
squares = [x**2 for x in numbers]
evens = [x for x in numbers if x % 2 == 0]

# Nested lists
matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
for row in matrix:
    for item in row:
        print(item, end=" ")
```

### Dictionary Operations

```python
# Create dictionary
person = {
    "name": "Alice",
    "age": 30,
    "city": "NYC"
}

# Access and modify
person["age"] = 31
person["email"] = "alice@example.com"

# Iterate
for key, value in person.items():
    print(f"{key}: {value}")

# Dictionary comprehension
squares = {x: x**2 for x in range(10)}
```

### Set Operations

```python
# Set operations
a = {1, 2, 3, 4}
b = {3, 4, 5, 6}

print(a | b)    # Union: {1, 2, 3, 4, 5, 6}
print(a & b)    # Intersection: {3, 4}
print(a - b)    # Difference: {1, 2}
print(a ^ b)    # Symmetric difference: {1, 2, 5, 6}

# Remove duplicates from list
numbers = [1, 2, 2, 3, 3, 3, 4]
unique = list(set(numbers))
```

## Control Flow Examples

### Conditional Logic

```python
def check_grade(score):
    if score >= 90:
        return "A"
    elif score >= 80:
        return "B"
    elif score >= 70:
        return "C"
    elif score >= 60:
        return "D"
    else:
        return "F"

# Ternary operator
status = "pass" if score >= 60 else "fail"
```

### Loop Patterns

```python
# Iterate with index
fruits = ["apple", "banana", "cherry"]
for i, fruit in enumerate(fruits):
    print(f"{i}: {fruit}")

# Parallel iteration
names = ["Alice", "Bob", "Charlie"]
ages = [25, 30, 35]
for name, age in zip(names, ages):
    print(f"{name} is {age}")

# While loop with sentinel
while True:
    line = input("Enter command (quit to exit): ")
    if line == "quit":
        break
    process(line)
```

## Function Examples

### Basic Functions

```python
def greet(name: str) -> str:
    """Greet a person by name."""
    return f"Hello, {name}!"

def calculate_area(width: float, height: float) -> float:
    """Calculate rectangle area."""
    return width * height

def fibonacci(n: int) -> int:
    """Calculate nth Fibonacci number."""
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)
```

### Higher-Order Functions

```python
# Function that returns function
def make_multiplier(n):
    return lambda x: x * n

times_3 = make_multiplier(3)
print(times_3(10))  # 30

# Function that takes function
def apply_operation(func, values):
    return [func(x) for x in values]

squares = apply_operation(lambda x: x**2, [1, 2, 3, 4])
```

### Decorators

```python
def timer(func):
    """Time function execution."""
    import time
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f"{func.__name__} took {end - start:.4f} seconds")
        return result
    return wrapper

@timer
def slow_function():
    import time
    time.sleep(1)
    return "Done"
```

## Object-Oriented Examples

### Basic Class

```python
class BankAccount:
    def __init__(self, owner: str, balance: float = 0.0):
        self.owner = owner
        self._balance = balance  # Private attribute

    def deposit(self, amount: float):
        if amount > 0:
            self._balance += amount
            return True
        return False

    def withdraw(self, amount: float):
        if 0 < amount <= self._balance:
            self._balance -= amount
            return True
        return False

    @property
    def balance(self):
        return self._balance

    def __str__(self):
        return f"Account({self.owner}, ${self._balance:.2f})"

# Usage
account = BankAccount("Alice", 1000.0)
account.deposit(500)
account.withdraw(200)
print(account)  # Account(Alice, $1300.00)
```

### Inheritance Example

```python
class Animal:
    def __init__(self, name: str):
        self.name = name

    def speak(self):
        return "Some sound"

class Dog(Animal):
    def __init__(self, name: str, breed: str):
        super().__init__(name)
        self.breed = breed

    def speak(self):
        return "Woof!"

class Cat(Animal):
    def speak(self):
        return "Meow!"

# Usage
dog = Dog("Buddy", "Golden Retriever")
cat = Cat("Whiskers")
print(f"{dog.name}: {dog.speak()}")  # Buddy: Woof!
print(f"{cat.name}: {cat.speak()}")  # Whiskers: Meow!
```

### Using Properties

```python
class Temperature:
    def __init__(self, celsius: float = 0.0):
        self._celsius = celsius

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("Temperature below absolute zero!")
        self._celsius = value

    @property
    def fahrenheit(self):
        return self._celsius * 9/5 + 32

    @fahrenheit.setter
    def fahrenheit(self, value):
        self.celsius = (value - 32) * 5/9

# Usage
temp = Temperature(25)
print(temp.celsius)      # 25.0
print(temp.fahrenheit)   # 77.0
temp.fahrenheit = 100
print(temp.celsius)      # 37.77...
```

## File Operations

### Reading Files

```python
# Read entire file
with open("data.txt", "r") as f:
    content = f.read()

# Read line by line
with open("data.txt", "r") as f:
    for line in f:
        print(line.strip())

# Read all lines
with open("data.txt", "r") as f:
    lines = f.readlines()
```

### Writing Files

```python
# Write text
with open("output.txt", "w") as f:
    f.write("Hello, World!\n")
    f.write("Second line\n")

# Append to file
with open("log.txt", "a") as f:
    f.write("New log entry\n")

# Write multiple lines
lines = ["Line 1\n", "Line 2\n", "Line 3\n"]
with open("output.txt", "w") as f:
    f.writelines(lines)
```

### CSV Processing

```python
import csv

# Read CSV
with open("data.csv", "r") as f:
    reader = csv.reader(f)
    header = next(reader)
    for row in reader:
        print(row)

# Write CSV
data = [
    ["Name", "Age", "City"],
    ["Alice", "30", "NYC"],
    ["Bob", "25", "LA"]
]
with open("output.csv", "w") as f:
    writer = csv.writer(f)
    writer.writerows(data)
```

### JSON Processing

```python
import json

# Write JSON
data = {
    "name": "Alice",
    "age": 30,
    "hobbies": ["reading", "coding"]
}
with open("data.json", "w") as f:
    json.dump(data, f, indent=2)

# Read JSON
with open("data.json", "r") as f:
    data = json.load(f)
    print(data["name"])
```

## Common Patterns

### Singleton Pattern

```python
class Database:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance.connection = None
        return cls._instance

    def connect(self):
        if self.connection is None:
            self.connection = "Connected"
        return self.connection

db1 = Database()
db2 = Database()
print(db1 is db2)  # True
```

### Iterator Pattern

```python
class Countdown:
    def __init__(self, start):
        self.current = start

    def __iter__(self):
        return self

    def __next__(self):
        if self.current <= 0:
            raise StopIteration
        self.current -= 1
        return self.current + 1

for i in Countdown(5):
    print(i)  # 5, 4, 3, 2, 1
```

### Context Manager Pattern

```python
class Timer:
    def __enter__(self):
        import time
        self.start = time.time()
        return self

    def __exit__(self, *args):
        import time
        self.end = time.time()
        print(f"Elapsed: {self.end - self.start:.4f}s")

with Timer():
    # Code to time
    import time
    time.sleep(1)
```

## Algorithm Examples

### Sorting

```python
# Bubble sort
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]
    return arr

# Quick sort
def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quick_sort(left) + middle + quick_sort(right)
```

### Searching

```python
# Binary search
def binary_search(arr, target):
    left, right = 0, len(arr) - 1
    while left <= right:
        mid = (left + right) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1

# Linear search
def linear_search(arr, target):
    for i, val in enumerate(arr):
        if val == target:
            return i
    return -1
```

### Data Structures

```python
# Stack
class Stack:
    def __init__(self):
        self.items = []

    def push(self, item):
        self.items.append(item)

    def pop(self):
        return self.items.pop() if self.items else None

    def peek(self):
        return self.items[-1] if self.items else None

    def is_empty(self):
        return len(self.items) == 0

# Queue
class Queue:
    def __init__(self):
        self.items = []

    def enqueue(self, item):
        self.items.insert(0, item)

    def dequeue(self):
        return self.items.pop() if self.items else None

    def is_empty(self):
        return len(self.items) == 0
```

## Real-World Examples

### Web Scraper (pseudo-code)

```python
import urllib.request
import re

def fetch_page(url):
    with urllib.request.urlopen(url) as response:
        return response.read().decode('utf-8')

def extract_links(html):
    pattern = r'href="(https?://[^"]+)"'
    return re.findall(pattern, html)

url = "https://example.com"
html = fetch_page(url)
links = extract_links(html)
for link in links:
    print(link)
```

### Calculator

```python
class Calculator:
    def __init__(self):
        self.history = []

    def add(self, a: float, b: float) -> float:
        result = a + b
        self.history.append(f"{a} + {b} = {result}")
        return result

    def subtract(self, a: float, b: float) -> float:
        result = a - b
        self.history.append(f"{a} - {b} = {result}")
        return result

    def multiply(self, a: float, b: float) -> float:
        result = a * b
        self.history.append(f"{a} * {b} = {result}")
        return result

    def divide(self, a: float, b: float) -> float:
        if b == 0:
            raise ValueError("Cannot divide by zero")
        result = a / b
        self.history.append(f"{a} / {b} = {result}")
        return result

    def show_history(self):
        for entry in self.history:
            print(entry)

calc = Calculator()
calc.add(5, 3)
calc.multiply(4, 7)
calc.show_history()
```

### Contact Manager

```python
class Contact:
    def __init__(self, name: str, phone: str, email: str):
        self.name = name
        self.phone = phone
        self.email = email

    def __str__(self):
        return f"{self.name} - {self.phone} - {self.email}"

class ContactManager:
    def __init__(self):
        self.contacts = []

    def add_contact(self, contact: Contact):
        self.contacts.append(contact)

    def find_by_name(self, name: str):
        return [c for c in self.contacts if name.lower() in c.name.lower()]

    def list_all(self):
        for contact in self.contacts:
            print(contact)

    def remove_contact(self, name: str):
        self.contacts = [c for c in self.contacts if c.name != name]

# Usage
manager = ContactManager()
manager.add_contact(Contact("Alice Smith", "555-1234", "alice@example.com"))
manager.add_contact(Contact("Bob Jones", "555-5678", "bob@example.com"))
manager.list_all()
```

## Next Steps

- [Best Practices](best-practices.md)
- [Design Patterns](patterns.md)
- [Performance Tips](../advanced/performance.md)
- [Complete Language Reference](../language/syntax.md)
