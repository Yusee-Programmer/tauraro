#!/usr/bin/env python3
"""
Benchmark: Prime Sieve (Eratosthenes)
Tests: Array operations, loops, memory allocation
"""
import time
import sys

def sieve_of_eratosthenes(limit):
    """Find all primes up to limit"""
    if limit < 2:
        return []

    # Create boolean array
    is_prime = [True] * (limit + 1)
    is_prime[0] = False
    is_prime[1] = False

    # Sieve
    for i in range(2, int(limit ** 0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit + 1, i):
                is_prime[j] = False

    # Collect primes
    primes = [i for i in range(limit + 1) if is_prime[i]]
    return primes

def main():
    limit = 1000000 if len(sys.argv) < 2 else int(sys.argv[1])

    start = time.time()
    primes = sieve_of_eratosthenes(limit)
    elapsed = time.time() - start

    print(f"Found {len(primes)} primes up to {limit}")
    print(f"Last 5 primes: {primes[-5:]}")
    print(f"Time: {elapsed:.4f} seconds")
    return elapsed

if __name__ == "__main__":
    main()
