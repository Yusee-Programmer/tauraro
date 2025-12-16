#!/usr/bin/env python3
# Benchmark: Simple Loop - 1 Billion Iterations

def loop_sum(n: int) -> int:
    total: int = 0
    i: int = 0
    while i < n:
        total = total + 1
        i = i + 1
    return total

result: int = loop_sum(1000000000)
print(result)
