#!/usr/bin/env python3
# Benchmark: Basic Data Types and Operations

def test_integers() -> int:
    total: int = 0
    i: int = 0
    while i < 10000000:
        total = total + i
        i = i + 1
    return total

def test_floats() -> float:
    total: float = 0.0
    i: int = 0
    while i < 10000000:
        total = total + float(i) * 1.5
        i = i + 1
    return total

def test_strings() -> int:
    result: str = ""
    i: int = 0
    while i < 100000:
        result = "test" + str(i)
        i = i + 1
    return len(result)

def test_booleans() -> int:
    count: int = 0
    i: int = 0
    flag: bool = False
    while i < 10000000:
        if i % 2 == 0:
            flag = True
        else:
            flag = False
        if flag:
            count = count + 1
        i = i + 1
    return count

def main():
    print("Testing integers...")
    result1: int = test_integers()
    print(result1)

    print("Testing floats...")
    result2: float = test_floats()
    print(result2)

    print("Testing strings...")
    result3: int = test_strings()
    print(result3)

    print("Testing booleans...")
    result4: int = test_booleans()
    print(result4)

    print("All basic type tests passed!")

main()
