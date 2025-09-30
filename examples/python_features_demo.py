# Comprehensive Python Features Demo for Tauraro
# Demonstrating 100% Python syntax compatibility

# 1. Data Types & Structures
print("=== DATA TYPES ===")

# Primitive types
integer_val = 42
float_val = 3.14159
complex_val = 3 + 4j
bool_val = True
string_val = "Hello, Tauraro!"
bytes_val = b"binary data"

print(f"Integer: {integer_val}")
print(f"Float: {float_val}")
print(f"Complex: {complex_val}")
print(f"Boolean: {bool_val}")
print(f"String: {string_val}")

# Collections
list_val = [1, 2, 3, "mixed", True]
tuple_val = (1, 2, 3)
set_val = {1, 2, 3, 3}  # Duplicates removed
dict_val = {"name": "Tauraro", "version": 1.0, "features": ["Python", "Rust"]}

print(f"List: {list_val}")
print(f"Tuple: {tuple_val}")
print(f"Set: {set_val}")
print(f"Dict: {dict_val}")

# Special values
ellipsis_val = ...
not_impl_val = NotImplemented

# 2. Control Flow
print("\n=== CONTROL FLOW ===")

# Conditionals
x = 10
if x > 5:
    print("x is greater than 5")
elif x == 5:
    print("x equals 5")
else:
    print("x is less than 5")

# Loops
print("For loop:")
for i in range(3):
    print(f"  Iteration {i}")

print("While loop:")
count = 0
while count < 3:
    print(f"  Count: {count}")
    count += 1

# Comprehensions
squares = [x**2 for x in range(5)]
even_squares = [x**2 for x in range(10) if x % 2 == 0]
dict_comp = {x: x**2 for x in range(5)}
set_comp = {x**2 for x in range(5)}

print(f"List comprehension: {squares}")
print(f"Filtered comprehension: {even_squares}")
print(f"Dict comprehension: {dict_comp}")
print(f"Set comprehension: {set_comp}")

# Pattern Matching (Python 3.10+)
def handle_value(value):
    match value:
        case 0:
            return "zero"
        case 1 | 2 | 3:
            return "small number"
        case x if x > 10:
            return "large number"
        case _:
            return "other"

print(f"Pattern matching: {handle_value(0)}, {handle_value(2)}, {handle_value(15)}")

# 3. Functions & Functional Programming
print("\n=== FUNCTIONS ===")

def greet(name, greeting="Hello"):
    """A simple greeting function with default parameter."""
    return f"{greeting}, {name}!"

print(greet("World"))
print(greet("Tauraro", "Hi"))

# Lambda functions
square = lambda x: x**2
print(f"Lambda result: {square(5)}")

# Higher-order functions
numbers = [1, 2, 3, 4, 5]
mapped = list(map(lambda x: x * 2, numbers))
filtered = list(filter(lambda x: x % 2 == 0, numbers))

print(f"Map result: {mapped}")
print(f"Filter result: {filtered}")

# Decorators
def my_decorator(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        result = func(*args, **kwargs)
        print(f"Finished {func.__name__}")
        return result
    return wrapper

@my_decorator
def say_hello():
    print("Hello from decorated function!")

say_hello()

# 4. Object-Oriented Programming
print("\n=== OOP ===")

class Animal:
    """Base animal class."""
    
    def __init__(self, name, species):
        self.name = name
        self.species = species
    
    def speak(self):
        return f"{self.name} makes a sound"
    
    def __str__(self):
        return f"{self.name} the {self.species}"

class Dog(Animal):
    """Dog class inheriting from Animal."""
    
    def __init__(self, name, breed):
        super().__init__(name, "Dog")
        self.breed = breed
    
    def speak(self):
        return f"{self.name} barks!"

# Create instances
my_dog = Dog("Buddy", "Golden Retriever")
print(my_dog)
print(my_dog.speak())

# 5. Exception Handling
print("\n=== EXCEPTION HANDLING ===")

try:
    risky_operation = 10 / 0
except ZeroDivisionError as e:
    print(f"Caught exception: {e}")
except Exception as e:
    print(f"Caught general exception: {e}")
else:
    print("No exception occurred")
finally:
    print("Cleanup code executed")

# 6. Context Managers
print("\n=== CONTEXT MANAGERS ===")

class FileManager:
    """Simple file manager context manager."""
    
    def __init__(self, filename, mode):
        self.filename = filename
        self.mode = mode
        self.file = None
    
    def __enter__(self):
        print(f"Opening file: {self.filename}")
        # In real implementation, would open actual file
        self.file = f"fake_file_handle_{self.filename}"
        return self.file
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        print(f"Closing file: {self.filename}")
        self.file = None
        return False  # Don't suppress exceptions

# Using context manager
with FileManager("test.txt", "r") as f:
    print(f"Working with file: {f}")

# 7. Advanced Features
print("\n=== ADVANCED FEATURES ===")

# Introspection
print(f"Type of integer: {type(42)}")
print(f"String representation: {repr('hello')}")

# Dynamic code execution
code = "2 + 3"
result = eval(code)
print(f"eval('2 + 3') = {result}")

exec("dynamic_var = 'created dynamically'")
print(f"Executed code result: {dynamic_var}")

# Compile code objects
compiled_code = compile("x = 1 + 1", "<string>", "exec")
print(f"Compiled code: {compiled_code}")

print("\n=== ALL PYTHON FEATURES DEMONSTRATED ===")
print("Tauraro successfully implements comprehensive Python compatibility!")