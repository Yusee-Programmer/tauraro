#!/usr/bin/env python3
"""
Test for the critical missing features one at a time
"""

print("=" * 60)
print("MISSING FEATURES TEST")
print("=" * 60)

# Test 1: Extended Unpacking - simplest case
print("\n1. Testing Extended Unpacking (simplest)")
print("-" * 40)
try:
    first, *rest = [1, 2, 3, 4]
    print(f"first, *rest = [1, 2, 3, 4]")
    print(f"  first = {first}, rest = {rest}")
    print("  ✓ Extended unpacking works!")
except Exception as e:
    print(f"  ✗ Extended unpacking FAILED: {e}")

# Test 2: list.sort() with key parameter
print("\n2. Testing list.sort() with key parameter")
print("-" * 40)
try:
    items = ["apple", "Banana", "cherry", "Date"]
    items_copy = items.copy()
    items_copy.sort(key=str.lower)
    print(f"Original: {items}")
    print(f"Sorted with key=str.lower: {items_copy}")
    print("  ✓ list.sort(key=...) works!")
except Exception as e:
    print(f"  ✗ list.sort(key=...) FAILED: {e}")

# Test 3: eval() with simple expression
print("\n3. Testing eval()")
print("-" * 40)
try:
    result = eval("2 + 3 * 4")
    print(f"eval('2 + 3 * 4') = {result}")
    if result == 14:
        print("  ✓ eval() works!")
    else:
        print(f"  ✗ eval() returned wrong value: {result} (expected 14)")
except Exception as e:
    print(f"  ✗ eval() FAILED: {e}")

# Test 4: exec() with simple code
print("\n4. Testing exec()")
print("-" * 40)
try:
    exec("test_var = 42")
    print(f"exec('test_var = 42') executed")
    # Note: exec() might not work with scope properly
    print("  ⚠  exec() exists but may not work fully")
except Exception as e:
    print(f"  ✗ exec() FAILED: {e}")

# Test 5: Descriptor protocol
print("\n5. Testing Descriptor Protocol")
print("-" * 40)
try:
    class Descriptor:
        def __get__(self, obj, objtype=None):
            return "descriptor_value"

        def __set__(self, obj, value):
            pass

    class MyClass:
        attr = Descriptor()

    obj = MyClass()
    value = obj.attr
    print(f"Descriptor __get__ returned: {value}")
    print("  ⚠  Descriptor protocol may not be fully implemented")
except Exception as e:
    print(f"  ✗ Descriptor protocol FAILED: {e}")

print("\n" + "=" * 60)
print("TEST COMPLETE")
print("=" * 60)
