#!/usr/bin/env python3
# Benchmark: Lists and Collections

def test_list_creation():
    i: int = 0
    total: int = 0
    while i < 100000:
        lst = [1, 2, 3, 4, 5]
        total = total + len(lst)
        i = i + 1
    return total

def test_list_append():
    lst = []
    i: int = 0
    while i < 1000000:
        lst.append(i)
        i = i + 1
    return len(lst)

def test_list_access():
    lst = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    total: int = 0
    i: int = 0
    while i < 10000000:
        idx: int = i % 10
        total = total + lst[idx]
        i = i + 1
    return total

def test_list_modification():
    lst = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    i: int = 0
    while i < 1000000:
        idx: int = i % 10
        lst[idx] = i
        i = i + 1
    total: int = 0
    j: int = 0
    while j < 10:
        total = total + lst[j]
        j = j + 1
    return total

def test_list_iteration():
    lst = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    total: int = 0
    i: int = 0
    while i < 1000000:
        j: int = 0
        while j < len(lst):
            total = total + lst[j]
            j = j + 1
        i = i + 1
    return total

def main():
    print("Testing list creation...")
    result1: int = test_list_creation()
    print(result1)

    print("Testing list append...")
    result2: int = test_list_append()
    print(result2)

    print("Testing list access...")
    result3: int = test_list_access()
    print(result3)

    print("Testing list modification...")
    result4: int = test_list_modification()
    print(result4)

    print("Testing list iteration...")
    result5: int = test_list_iteration()
    print(result5)

    print("All list tests passed!")

main()
