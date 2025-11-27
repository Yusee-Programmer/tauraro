#!/usr/bin/env python3
# Benchmark: Bitwise Operations - 1 Billion Iterations

def bitwise_sum(n: int) -> int:
    total: int = 0
    i: int = 1
    while i <= n:
        total = total + ((i << 1) + (i >> 1) + (i & 15))
        i = i + 1
    return total

result: int = bitwise_sum(1000000000)
print(result)
