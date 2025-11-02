# Complete Functionality Test - All Functions and Data Types as Objects

print("=" * 60)
print("TESTING ALL FUNCTIONS AND DATA TYPES AS OBJECTS")
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
assert result == 8, "Regular function failed"

msg = greet("Alice")
print(f"greet('Alice') = {msg}")
assert msg == "Hello, Alice!", "String formatting in function failed"

# Functions as objects
func_obj = add
print(f"Function as object: {func_obj}")
result2 = func_obj(10, 20)
print(f"func_obj(10, 20) = {result2}")
assert result2 == 30, "Function object call failed"

print("✓ Regular functions work!")

# =========================================
# TEST 2: Closures
# =========================================
print("\n[TEST 2] Closures")

def outer(x):
    def inner(y):
        return x + y
    return inner

closure = outer(5)
result = closure(10)
print(f"closure(10) where x=5: {result}")
assert result == 15, "Closure failed"

print("✓ Closures work!")

# =========================================
# TEST 3: Lambda Functions
# =========================================
print("\n[TEST 3] Lambda Functions")

square = lambda x: x * x
result = square(7)
print(f"lambda square(7) = {result}")
assert result == 49, "Lambda function failed"

# Lambda as object
lam_obj = square
result2 = lam_obj(4)
print(f"lam_obj(4) = {result2}")
assert result2 == 16, "Lambda object call failed"

print("✓ Lambda functions work!")

# =========================================
# TEST 4: Built-in Functions as Objects
# =========================================
print("\n[TEST 4] Built-in Functions as Objects")

# Use print as object
print_func = print
print_func("Print function as object works!")

# Use len as object
len_func = len
my_list = [1, 2, 3, 4, 5]
length = len_func(my_list)
print(f"len_func([1,2,3,4,5]) = {length}")
assert length == 5, "Built-in function object failed"

print("✓ Built-in functions as objects work!")

# =========================================
# TEST 5: Data Types - Integer
# =========================================
print("\n[TEST 5] Integer as Object")

num = 42
print(f"Integer: {num}")
print(f"Type: {type(num).__name__}")
print(f"Integer __class__: {num.__class__.__name__}")

# Integer arithmetic
result = num + 8
print(f"{num} + 8 = {result}")
assert result == 50, "Integer addition failed"

print("✓ Integer as object works!")

# =========================================
# TEST 6: Data Types - Float
# =========================================
print("\n[TEST 6] Float as Object")

fnum = 3.14
print(f"Float: {fnum}")
print(f"Type: {type(fnum).__name__}")
print(f"Float __class__: {fnum.__class__.__name__}")

# Float arithmetic
result = fnum * 2
print(f"{fnum} * 2 = {result}")
assert result == 6.28, "Float multiplication failed"

print("✓ Float as object works!")

# =========================================
# TEST 7: Data Types - String
# =========================================
print("\n[TEST 7] String as Object")

text = "Hello, World!"
print(f"String: {text}")
print(f"Type: {type(text).__name__}")
print(f"String __class__: {text.__class__.__name__}")

# String methods
upper_text = text.upper()
print(f"upper(): {upper_text}")
assert upper_text == "HELLO, WORLD!", "String upper() failed"

lower_text = text.lower()
print(f"lower(): {lower_text}")
assert lower_text == "hello, world!", "String lower() failed"

# String slicing
substring = text[0:5]
print(f"text[0:5]: {substring}")
assert substring == "Hello", "String slicing failed"

print("✓ String as object works!")

# =========================================
# TEST 8: Data Types - List
# =========================================
print("\n[TEST 8] List as Object")

my_list = [1, 2, 3, 4, 5]
print(f"List: {my_list}")
print(f"Type: {type(my_list).__name__}")
print(f"List __class__: {my_list.__class__.__name__}")

# List methods
my_list.append(6)
print(f"After append(6): {my_list}")
assert len(my_list) == 6, "List append failed"

my_list.extend([7, 8])
print(f"After extend([7, 8]): {my_list}")
assert len(my_list) == 8, "List extend failed"

item = my_list.pop()
print(f"After pop(): {my_list}, popped: {item}")
assert item == 8 and len(my_list) == 7, "List pop failed"

# List indexing
first = my_list[0]
print(f"my_list[0]: {first}")
assert first == 1, "List indexing failed"

# List slicing
slice_list = my_list[1:4]
print(f"my_list[1:4]: {slice_list}")
assert len(slice_list) == 3, "List slicing failed"

print("✓ List as object works!")

# =========================================
# TEST 9: Data Types - Dictionary
# =========================================
print("\n[TEST 9] Dictionary as Object")

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
assert name == "Alice", "Dict indexing failed"

# Dict update
my_dict["country"] = "USA"
print(f"After adding country: {my_dict}")
assert "country" in my_dict, "Dict update failed"

print("✓ Dictionary as object works!")

# =========================================
# TEST 10: Data Types - Boolean
# =========================================
print("\n[TEST 10] Boolean as Object")

flag = True
print(f"Boolean: {flag}")
print(f"Type: {type(flag).__name__}")
print(f"Bool __class__: {flag.__class__.__name__}")

# Boolean operations
result = flag and False
print(f"True and False = {result}")
assert result == False, "Boolean AND failed"

result2 = flag or False
print(f"True or False = {result2}")
assert result2 == True, "Boolean OR failed"

print("✓ Boolean as object works!")

# =========================================
# TEST 11: Decorators
# =========================================
print("\n[TEST 11] Decorators")

def my_decorator(func):
    def wrapper():
        print("Before function call")
        result = func()
        print("After function call")
        return result
    return wrapper

@my_decorator
def say_hello():
    print("Hello from decorated function!")
    return "success"

result = say_hello()
print(f"Decorator result: {result}")
assert result == "success", "Decorator failed"

print("✓ Decorators work!")

# =========================================
# TEST 12: Classes and Methods
# =========================================
print("\n[TEST 12] Classes and Methods")

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
assert "Bob" in greeting and "25" in greeting, "Method call failed"

new_age = person.birthday()
print(f"After birthday(): {new_age}")
assert new_age == 26, "Method mutation failed"

print("✓ Classes and methods work!")

# =========================================
# TEST 13: Method Calls on All Data Types
# =========================================
print("\n[TEST 13] Method Calls on Data Types")

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
# TEST 14: Nested Function Calls
# =========================================
print("\n[TEST 14] Nested Function Calls")

def multiply(a, b):
    return a * b

def divide(a, b):
    return a / b

result = multiply(add(2, 3), divide(10, 2))
print(f"multiply(add(2, 3), divide(10, 2)) = {result}")
assert result == 25.0, "Nested function calls failed"

print("✓ Nested function calls work!")

# =========================================
# TEST 15: Function with Multiple Returns
# =========================================
print("\n[TEST 15] Functions with Multiple Returns")

def get_stats(numbers):
    total = sum(numbers)
    count = len(numbers)
    return total, count

nums = [1, 2, 3, 4, 5]
total, count = get_stats(nums)
print(f"get_stats([1,2,3,4,5]) = total: {total}, count: {count}")
assert total == 15 and count == 5, "Multiple return values failed"

print("✓ Functions with multiple returns work!")

# =========================================
# FINAL SUMMARY
# =========================================
print("\n" + "=" * 60)
print("ALL TESTS PASSED!")
print("=" * 60)
print("✓ Regular functions work")
print("✓ Closures work")
print("✓ Lambda functions work")
print("✓ Built-in functions as objects work")
print("✓ Integer as object works")
print("✓ Float as object works")
print("✓ String as object works")
print("✓ List as object works")
print("✓ Dictionary as object works")
print("✓ Boolean as object works")
print("✓ Decorators work")
print("✓ Classes and methods work")
print("✓ Method calls on all data types work")
print("✓ Nested function calls work")
print("✓ Functions with multiple returns work")
print("=" * 60)
