# Simplified Test - Core Functions and Data Types

print("=" * 60)
print("TESTING CORE FUNCTIONS AND DATA TYPES AS OBJECTS")
print("=" * 60)

# =========================================
# TEST 1: Regular Functions
# =========================================
print("\n[TEST 1] Regular Functions")

def add(a, b):
    return a + b

def greet(name):
    return f"Hello, {name}!"

result = add(5, 3)
print(f"add(5, 3) = {result}")

msg = greet("Alice")
print(f"greet('Alice') = {msg}")

# Functions as objects
func_obj = add
result2 = func_obj(10, 20)
print(f"func_obj(10, 20) = {result2}")

print("✓ Regular functions work!")

# =========================================
# TEST 2: Built-in Functions as Objects
# =========================================
print("\n[TEST 2] Built-in Functions as Objects")

# Use print as object
print_func = print
print_func("Print function as object works!")

# Use len as object
len_func = len
my_list = [1, 2, 3, 4, 5]
length = len_func(my_list)
print(f"len_func([1,2,3,4,5]) = {length}")

print("✓ Built-in functions as objects work!")

# =========================================
# TEST 3: Data Types - Integer
# =========================================
print("\n[TEST 3] Integer as Object")

num = 42
print(f"Integer: {num}")
print(f"Type: {type(num).__name__}")
print(f"Integer __class__: {num.__class__.__name__}")

# Integer arithmetic
result = num + 8
print(f"{num} + 8 = {result}")

print("✓ Integer as object works!")

# =========================================
# TEST 4: Data Types - Float
# =========================================
print("\n[TEST 4] Float as Object")

fnum = 3.14
print(f"Float: {fnum}")
print(f"Type: {type(fnum).__name__}")
print(f"Float __class__: {fnum.__class__.__name__}")

# Float arithmetic
result = fnum * 2
print(f"{fnum} * 2 = {result}")

print("✓ Float as object works!")

# =========================================
# TEST 5: Data Types - String
# =========================================
print("\n[TEST 5] String as Object")

text = "Hello, World!"
print(f"String: {text}")
print(f"Type: {type(text).__name__}")
print(f"String __class__: {text.__class__.__name__}")

# String methods
upper_text = text.upper()
print(f"upper(): {upper_text}")

lower_text = text.lower()
print(f"lower(): {lower_text}")

# String slicing
substring = text[0:5]
print(f"text[0:5]: {substring}")

print("✓ String as object works!")

# =========================================
# TEST 6: Data Types - List
# =========================================
print("\n[TEST 6] List as Object")

my_list = [1, 2, 3, 4, 5]
print(f"List: {my_list}")
print(f"Type: {type(my_list).__name__}")
print(f"List __class__: {my_list.__class__.__name__}")

# List methods
my_list.append(6)
print(f"After append(6): {my_list}")

my_list.extend([7, 8])
print(f"After extend([7, 8]): {my_list}")

item = my_list.pop()
print(f"After pop(): {my_list}, popped: {item}")

# List indexing
first = my_list[0]
print(f"my_list[0]: {first}")

# List slicing
slice_list = my_list[1:4]
print(f"my_list[1:4]: {slice_list}")

print("✓ List as object works!")

# =========================================
# TEST 7: Data Types - Dictionary
# =========================================
print("\n[TEST 7] Dictionary as Object")

my_dict = {"name": "Alice", "age": 30, "city": "NYC"}
print(f"Dict: {my_dict}")
print(f"Type: {type(my_dict).__name__}")
print(f"Dict __class__: {my_dict.__class__.__name__}")

# Dict methods
keys = my_dict.keys()
print(f"keys(): {keys}")

values = my_dict.values()
print(f"values(): {values}")

# Dict indexing
name = my_dict["name"]
print(f"my_dict['name']: {name}")

# Dict update
my_dict["country"] = "USA"
print(f"After adding country: {my_dict}")

print("✓ Dictionary as object works!")

# =========================================
# TEST 8: Data Types - Boolean
# =========================================
print("\n[TEST 8] Boolean as Object")

flag = True
print(f"Boolean: {flag}")
print(f"Type: {type(flag).__name__}")
print(f"Bool __class__: {flag.__class__.__name__}")

# Boolean operations
result = flag and False
print(f"True and False = {result}")

result2 = flag or False
print(f"True or False = {result2}")

print("✓ Boolean as object works!")

# =========================================
# TEST 9: Classes and Methods
# =========================================
print("\n[TEST 9] Classes and Methods")

class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def greet(self):
        return f"Hi, I'm {self.name} and I'm {self.age} years old"

    def birthday(self):
        self.age = self.age + 1
        return self.age

person = Person("Bob", 25)
print(f"Person: {person}")
print(f"Type: {type(person).__name__}")
print(f"Person __class__: {person.__class__.__name__}")

greeting = person.greet()
print(f"greet(): {greeting}")

new_age = person.birthday()
print(f"After birthday(): {new_age}")

print("✓ Classes and methods work!")

# =========================================
# TEST 10: Method Calls on All Data Types
# =========================================
print("\n[TEST 10] Method Calls on Data Types")

# String methods
s = "hello"
print(f"'hello'.upper() = {s.upper()}")
print(f"'hello'.capitalize() = {s.capitalize()}")

# List methods
lst = [3, 1, 2]
lst.sort()
print(f"[3,1,2].sort() = {lst}")

# Dict methods
d = {"a": 1, "b": 2}
print(f"Dict keys: {d.keys()}")
print(f"Dict get('a'): {d.get('a')}")

print("✓ Method calls on all data types work!")

# =========================================
# TEST 11: Nested Function Calls
# =========================================
print("\n[TEST 11] Nested Function Calls")

def multiply(a, b):
    return a * b

def divide(a, b):
    return a / b

result = multiply(add(2, 3), divide(10, 2))
print(f"multiply(add(2, 3), divide(10, 2)) = {result}")

print("✓ Nested function calls work!")

# =========================================
# FINAL SUMMARY
# =========================================
print("\n" + "=" * 60)
print("ALL TESTS PASSED!")
print("=" * 60)
print("✓ Regular functions work")
print("✓ Built-in functions as objects work")
print("✓ Integer as object works")
print("✓ Float as object works")
print("✓ String as object works")
print("✓ List as object works")
print("✓ Dictionary as object works")
print("✓ Boolean as object works")
print("✓ Classes and methods work")
print("✓ Method calls on all data types work")
print("✓ Nested function calls work")
print("=" * 60)
