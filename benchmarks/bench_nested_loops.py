#!/usr/bin/env python3
# Benchmark: Nested Loops - 1 Billion Total Iterations

def nested_sum(n: int) -> int:
    total: int = 0
    i: int = 0
    while i < n:
        j: int = 0
        while j < n:
            total = total + 1
            j = j + 1
        i = i + 1
    return total

result: int = nested_sum(31623)
print(result)
