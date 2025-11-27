#!/usr/bin/env python3
# Benchmark: Arithmetic Operations - 1 Billion Iterations

def arithmetic_sum(n: int) -> int:
    total: int = 0
    i: int = 0
    while i < n:
        total = total + (i * 3 - i // 2 + i % 7)
        i = i + 1
    return total

result: int = arithmetic_sum(1000000000)
print(result)
