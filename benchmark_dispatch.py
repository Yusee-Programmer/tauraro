# Benchmark for dispatch optimization
# Tests hot opcodes: LoadConst, LoadGlobal, BinaryAddRR, CallFunction, etc.

import time

def fibonacci(n):
    """Exercises: LoadFast, StoreFast, BinarySubRR, CompareLessRR, BinaryAddRR, ReturnValue"""
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

def list_operations():
    """Exercises: BuildList, ListAppend, SubscrLoad, LoadConst"""
    result = []
    for i in range(1000):
        result.append(i * 2)
    return sum(result)

def arithmetic_heavy():
    """Exercises: BinaryAddRR, BinarySubRR, BinaryMulRR, BinaryDivRR"""
    total = 0
    for i in range(10000):
        total = total + i * 2 - i / 2
    return total

def method_calls():
    """Exercises: CallMethod, LoadGlobal, StoreGlobal"""
    class Counter:
        def __init__(self):
            self.count = 0

        def increment(self):
            self.count = self.count + 1

    counter = Counter()
    for _ in range(1000):
        counter.increment()
    return counter.count

print("=== Dispatch Optimization Benchmark ===")
print()

# Fibonacci test
print("Test 1: Fibonacci (recursive calls + arithmetic)")
start = time.time()
result = fibonacci(20)
elapsed = time.time() - start
print(f"  Result: {result}")
print(f"  Time: {elapsed:.4f}s")
print()

# List operations test
print("Test 2: List Operations")
start = time.time()
result = list_operations()
elapsed = time.time() - start
print(f"  Result: {result}")
print(f"  Time: {elapsed:.4f}s")
print()

# Arithmetic test
print("Test 3: Arithmetic Heavy")
start = time.time()
result = arithmetic_heavy()
elapsed = time.time() - start
print(f"  Result: {result}")
print(f"  Time: {elapsed:.4f}s")
print()

# Method calls test
print("Test 4: Method Calls")
start = time.time()
result = method_calls()
elapsed = time.time() - start
print(f"  Result: {result}")
print(f"  Time: {elapsed:.4f}s")
print()

print("=== Benchmark Complete ===")
