#!/usr/bin/env python3
# Benchmark: Common Algorithms

def bubble_sort(arr, n: int) -> None:
    i: int = 0
    while i < n:
        j: int = 0
        while j < n - i - 1:
            if arr[j] > arr[j + 1]:
                temp = arr[j]
                arr[j] = arr[j + 1]
                arr[j + 1] = temp
            j = j + 1
        i = i + 1

def test_bubble_sort():
    iterations: int = 0
    while iterations < 100:
        arr = [64, 34, 25, 12, 22, 11, 90, 88, 45, 50, 23, 36, 18, 77, 29]
        bubble_sort(arr, 15)
        iterations = iterations + 1
    return arr[0]

def factorial(n: int) -> int:
    if n <= 1:
        return 1
    return n * factorial(n - 1)

def test_factorial():
    total: int = 0
    i: int = 1
    while i <= 15:
        total = total + factorial(i)
        i = i + 1
    return total

def gcd(a: int, b: int) -> int:
    while b != 0:
        temp: int = b
        b = a % b
        a = temp
    return a

def test_gcd():
    total: int = 0
    i: int = 1
    while i < 10000:
        total = total + gcd(i * 123, i * 456)
        i = i + 1
    return total

def is_prime(n: int) -> bool:
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0:
        return False
    i: int = 3
    while i * i <= n:
        if n % i == 0:
            return False
        i = i + 2
    return True

def test_prime_counting():
    count: int = 0
    i: int = 2
    while i < 100000:
        if is_prime(i):
            count = count + 1
        i = i + 1
    return count

def main():
    print("Testing bubble sort...")
    result1: int = test_bubble_sort()
    print(result1)

    print("Testing factorial...")
    result2: int = test_factorial()
    print(result2)

    print("Testing GCD...")
    result3: int = test_gcd()
    print(result3)

    print("Testing prime counting...")
    result4: int = test_prime_counting()
    print(result4)

    print("All algorithm tests passed!")

main()
