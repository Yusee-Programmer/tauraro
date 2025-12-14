#!/usr/bin/env python3
# Benchmark: Arithmetic Operations

def test_addition() -> int:
    total: int = 0
    i: int = 0
    while i < 50000000:
        total = total + i
        i = i + 1
    return total

def test_multiplication() -> int:
    total: int = 1
    i: int = 1
    while i < 1000000:
        total = (total * i) % 1000000007
        i = i + 1
    return total

def test_division() -> float:
    total: float = 1000000.0
    i: int = 1
    while i < 1000000:
        total = total / 1.0001
        i = i + 1
    return total

def test_modulo() -> int:
    total: int = 0
    i: int = 0
    while i < 10000000:
        total = total + (i % 997)
        i = i + 1
    return total

def test_mixed_ops() -> int:
    total: int = 0
    i: int = 0
    while i < 10000000:
        total = total + (i * 3 - i // 2 + i % 7)
        i = i + 1
    return total

def main():
    print("Testing addition...")
    result1: int = test_addition()
    print(result1)

    print("Testing multiplication...")
    result2: int = test_multiplication()
    print(result2)

    print("Testing division...")
    result3: float = test_division()
    print(result3)

    print("Testing modulo...")
    result4: int = test_modulo()
    print(result4)

    print("Testing mixed operations...")
    result5: int = test_mixed_ops()
    print(result5)

    print("All arithmetic tests passed!")

main()
