#!/usr/bin/env tauraro
"""Test various parser cases"""

def returns_tuple():
    return 1, 2

def test_cases():
    # Test 1: Tuple unpack from function
    a, b = returns_tuple()
    print("Test 1: OK")

    # Test 2: If without else
    x = 5
    if x > 0:
        print("Test 2: OK")

    # Test 3: Nested if without else
    if x > 0:
        if x < 10:
            print("Test 3: OK")

    # Test 4: Multiple assignments
    c = d = e = 10
    print("Test 4: OK")

    # Test 5: For loop with range
    for i in range(3):
        print(f"Test 5: {i}")

    # Test 6: While with break
    count = 0
    while count < 3:
        count = count + 1
        if count == 2:
            break
    print("Test 6: OK")

test_cases()
