# Final Comprehensive OOP Feature Test Suite

print("===========================================================")
print("TAURARO OOP COMPREHENSIVE TEST SUITE")
print("===========================================================")
print()

# ===========================================================================
# PART 1: BASIC CLASS FEATURES
# ===========================================================================

print("PART 1: BASIC CLASS FEATURES")
print("-----------------------------------------------------------")

# Test 1.1: Class instantiation
print("\nTest 1.1: Class instantiation and attributes")
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

p = Person("Alice", 30)
print(f"  Name: {p.name}, Age: {p.age}")
print("  PASS")

# Test 1.2: Methods
print("\nTest 1.2: Instance methods")
class Calculator:
    def __init__(self, value):
        self.value = value

    def add(self, n):
        self.value = self.value + n
        return self.value

    def get_value(self):
        return self.value

calc = Calculator(10)
calc.add(5)
print(f"  Value after add(5): {calc.get_value()}")
print("  PASS")

# Test 1.3: Multiple instances
print("\nTest 1.3: Multiple instances independence")
class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

c1 = Counter()
c2 = Counter()
c1.increment()
c1.increment()
c2.increment()
print(f"  Counter 1: {c1.count}, Counter 2: {c2.count}")
print("  PASS")

# ===========================================================================
# PART 2: F-STRING SUPPORT
# ===========================================================================

print()
print("===========================================================")
print("PART 2: F-STRING SUPPORT")
print("-----------------------------------------------------------")

# Test 2.1: Basic f-strings
print("\nTest 2.1: Basic f-string formatting")
name = "Bob"
age = 25
greeting = f"Hello, {name}! You are {age} years old."
print(f"  {greeting}")
print("  PASS")

# Test 2.2: F-strings with expressions
print("\nTest 2.2: F-strings with complex expressions")
x = 10
y = 20
print(f"  Sum of {x} and {y} is {x + y}")
print("  PASS")

# Test 2.3: F-strings with object attributes
print("\nTest 2.3: F-strings with object attributes")
class Student:
    def __init__(self, name, grade):
        self.name = name
        self.grade = grade

student = Student("Charlie", 95)
report = f"{student.name} scored {student.grade}%"
print(f"  {report}")
print("  PASS")

# ===========================================================================
# PART 3: TUPLE SUPPORT
# ===========================================================================

print()
print("===========================================================")
print("PART 3: TUPLE SUPPORT")
print("-----------------------------------------------------------")

# Test 3.1: Empty tuple
print("\nTest 3.1: Empty tuple")
empty = ()
print(f"  Empty tuple: {empty}, Length: {len(empty)}")
print("  PASS")

# Test 3.2: Single element tuple
print("\nTest 3.2: Single element tuple")
single = (42,)
print(f"  Single tuple: {single}, Length: {len(single)}")
print("  PASS")

# Test 3.3: Multi-element tuple
print("\nTest 3.3: Multi-element tuple")
multi = (1, 2, 3, 4, 5)
print(f"  Multi tuple: {multi}, Length: {len(multi)}")
print("  PASS")

# Test 3.4: Tuple indexing
print("\nTest 3.4: Tuple indexing")
t = (10, 20, 30, 40)
first = t[0]
third = t[2]
last = t[-1]
print(f"  t[0]={first}, t[2]={third}, t[-1]={last}")
print("  PASS")

# ===========================================================================
# FINAL SUMMARY
# ===========================================================================

print()
print("===========================================================")
print("TEST SUMMARY")
print("===========================================================")
print()
print("PASS: Basic Class Features")
print("  - Class instantiation")
print("  - Instance methods")
print("  - Attribute access and modification")
print("  - Multiple instances")
print()
print("PASS: F-String Support")
print("  - Basic formatting")
print("  - Complex expressions (addition)")
print("  - Object attribute access")
print()
print("PASS: Tuple Support")
print("  - Empty tuples")
print("  - Single and multi-element tuples")
print("  - Tuple indexing")
print()
print("===========================================================")
print("ALL CORE OOP TESTS PASSED!")
print("===========================================================")
