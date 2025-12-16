#!/usr/bin/env python3
"""List creation and manipulation"""

def create_list(size):
    lst = []
    for i in range(size):
        lst.append(i)
    return lst

def sum_list(lst):
    total = 0
    for item in lst:
        total += item
    return total

def reverse_list(lst):
    reversed_lst = []
    for i in range(len(lst) - 1, -1, -1):
        reversed_lst.append(lst[i])
    return reversed_lst

def main():
    size = 100000

    lst = create_list(size)
    print(f"Created list of size: {len(lst)}")

    total = sum_list(lst)
    print(f"Sum of list: {total}")

    reversed_lst = reverse_list(lst[:100])
    print(f"First 5 of reversed: {reversed_lst[:5]}")

    return total

if __name__ == "__main__":
    main()
