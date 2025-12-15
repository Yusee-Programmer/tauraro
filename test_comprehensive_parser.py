#!/usr/bin/env tauraro
"""Comprehensive parser test suite"""

def test_return_statements():
    """Test various return statement patterns"""
    print("=== Return Statements ===")

    def returns_nothing():
        return

    def returns_single():
        return 42

    def returns_tuple():
        return 1, 2, 3

    def returns_tuple_with_trailing_comma():
        return 1, 2,

    def returns_nested():
        return (1, 2), (3, 4)

    # Test all return types
    result = returns_nothing()
    print("returns_nothing: OK")

    result = returns_single()
    print(f"returns_single: {result}")

    result = returns_tuple()
    print(f"returns_tuple: {result}")

    result = returns_tuple_with_trailing_comma()
    print(f"returns_with_trailing_comma: {result}")

    result = returns_nested()
    print(f"returns_nested: {result}")

def test_tuple_unpacking():
    """Test tuple unpacking in various contexts"""
    print("=== Tuple Unpacking ===")

    # Basic unpacking
    a, b = 1, 2
    print(f"Basic: a={a}, b={b}")

    # Unpacking from function return
    def get_pair():
        return 10, 20

    x, y = get_pair()
    print(f"From function: x={x}, y={y}")

    # Nested unpacking
    (a, b), c = (1, 2), 3
    print(f"Nested: a={a}, b={b}, c={c}")

    # Unpacking in for loop
    pairs = [(1, 2), (3, 4), (5, 6)]
    for i, j in pairs:
        print(f"Loop: i={i}, j={j}")

def test_assignments():
    """Test various assignment patterns"""
    print("=== Assignments ===")

    # Simple assignment
    x = 42
    print(f"Simple: x={x}")

    # Multiple assignment
    a = b = c = 10
    print(f"Multiple: a={a}, b={b}, c={c}")

    # Augmented assignment
    x = 5
    x += 10
    x *= 2
    print(f"Augmented: x={x}")

    # List assignment
    arr = [1, 2, 3]
    arr[0] = 99
    print(f"List assign: {arr}")

def test_control_flow():
    """Test control flow statements"""
    print("=== Control Flow ===")

    # If statements
    x = 10
    if x > 5:
        print("If: OK")
    elif x > 0:
        print("Elif: Should not reach")
    else:
        print("Else: Should not reach")

    # For loops
    total = 0
    for i in [1, 2, 3, 4, 5]:
        total += i
    print(f"For loop: total={total}")

    # While loops
    count = 0
    while count < 3:
        count += 1
    print(f"While loop: count={count}")

    # Break and continue
    for i in range(10):
        if i == 2:
            continue
        if i == 5:
            break
        pass
    print("Break/continue: OK")

def test_expressions():
    """Test various expression types"""
    print("=== Expressions ===")

    # Arithmetic
    result = 2 + 3 * 4 - 1
    print(f"Arithmetic: {result}")

    # Comparison
    result = (5 > 3) and (10 < 20)
    print(f"Comparison: {result}")

    # Boolean logic
    result = True and False or True
    print(f"Boolean: {result}")

    # Ternary expression
    x = 5
    result = "positive" if x > 0 else "negative"
    print(f"Ternary: {result}")

def test_collections():
    """Test collection literals and operations"""
    print("=== Collections ===")

    # Lists
    lst = [1, 2, 3, 4, 5]
    print(f"List: {lst}")

    # Tuples
    tpl = (1, 2, 3)
    print(f"Tuple: {tpl}")

    # Dicts
    dct = {"a": 1, "b": 2}
    print(f"Dict: {dct}")

    # List comprehension
    squares = [x * x for x in range(5)]
    print(f"List comp: {squares}")

def test_functions():
    """Test function definitions"""
    print("=== Functions ===")

    # Simple function
    def simple(x):
        return x * 2

    print(f"Simple func: {simple(5)}")

    # Multiple parameters
    def multi(a, b, c):
        return a + b + c

    print(f"Multi params: {multi(1, 2, 3)}")

    # Default parameters
    def with_default(x, y=10):
        return x + y

    print(f"Default params: {with_default(5)}")

    # Return tuple
    def ret_tuple():
        return 1, 2, 3

    a, b, c = ret_tuple()
    print(f"Return tuple: a={a}, b={b}, c={c}")

def test_string_operations():
    """Test string literals and operations"""
    print("=== String Operations ===")

    # Basic strings
    s1 = "hello"
    s2 = 'world'
    print(f"Strings: {s1} {s2}")

    # F-strings
    x = 42
    s = f"Value is {x}"
    print(f"F-string: {s}")

    # Concatenation
    result = "hello" + " " + "world"
    print(f"Concat: {result}")

def test_classes():
    """Test class definitions"""
    print("=== Classes ===")

    class Point:
        def __init__(self, x, y):
            self.x = x
            self.y = y

        def get_coords(self):
            return self.x, self.y

    p = Point(10, 20)
    x, y = p.get_coords()
    print(f"Class: x={x}, y={y}")

def main():
    """Run all parser tests"""
    test_return_statements()
    test_tuple_unpacking()
    test_assignments()
    test_control_flow()
    test_expressions()
    test_collections()
    test_functions()
    test_string_operations()
    test_classes()
    print("\n=== All Parser Tests Passed ===")

main()
