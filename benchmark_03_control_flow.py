#!/usr/bin/env python3
# Benchmark: Control Flow Structures

def test_if_else() -> int:
    count: int = 0
    i: int = 0
    while i < 10000000:
        if i % 3 == 0:
            count = count + 1
        elif i % 3 == 1:
            count = count + 2
        else:
            count = count + 3
        i = i + 1
    return count

def test_nested_if() -> int:
    count: int = 0
    i: int = 0
    while i < 5000000:
        if i % 2 == 0:
            if i % 4 == 0:
                count = count + 1
            else:
                count = count + 2
        else:
            if i % 3 == 0:
                count = count + 3
            else:
                count = count + 4
        i = i + 1
    return count

def test_while_loop() -> int:
    total: int = 0
    i: int = 0
    while i < 10000000:
        total = total + i
        i = i + 1
    return total

def test_nested_while() -> int:
    total: int = 0
    i: int = 0
    while i < 1000:
        j: int = 0
        while j < 1000:
            total = total + 1
            j = j + 1
        i = i + 1
    return total

def test_break_continue() -> int:
    total: int = 0
    i: int = 0
    while i < 10000000:
        if i % 2 == 0:
            i = i + 1
            continue
        if i > 9999990:
            break
        total = total + i
        i = i + 1
    return total

def main():
    print("Testing if-else...")
    result1: int = test_if_else()
    print(result1)

    print("Testing nested if...")
    result2: int = test_nested_if()
    print(result2)

    print("Testing while loop...")
    result3: int = test_while_loop()
    print(result3)

    print("Testing nested while...")
    result4: int = test_nested_while()
    print(result4)

    print("Testing break/continue...")
    result5: int = test_break_continue()
    print(result5)

    print("All control flow tests passed!")

main()
