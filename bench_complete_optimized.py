# Complete Optimized Benchmark - All Features with TaggedValue
# Tests arithmetic, comparisons, conditionals, functions, and classes

print("=" * 60)
print("COMPLETE TAURARO OPTIMIZED BENCHMARK")
print("With TaggedValue Fast Paths for All Operations")
print("=" * 60)
print()

print("1. ARITHMETIC OPERATIONS (TaggedValue)")
print("-" * 60)
total = 0
for i in range(500000):
    total = total + i - 1 + 2
print(f"   Arithmetic (500K): {total}")

result_mul = 100
for i in range(50000):
    result_mul = (result_mul * 2) / 2
print(f"   Mul/Div (50K): {result_mul}")
print()

print("2. COMPARISON OPERATIONS (TaggedValue)")
print("-" * 60)
cmp_count = 0
for i in range(100000):
    if i < 50000:
        cmp_count = cmp_count + 1
    if i >= 75000:
        cmp_count = cmp_count + 1
    if i == 50000:
        cmp_count = cmp_count + 1
print(f"   Comparisons (300K): {cmp_count}")
print()

print("3. CONDITIONAL CONTROL FLOW")
print("-" * 60)
if_total = 0
for i in range(50000):
    if i < 25000:
        if_total = if_total + 1
    else:
        if_total = if_total + 2
print(f"   If-else (50K): {if_total}")

while_sum = 0
counter = 0
while counter < 25000:
    while_sum = while_sum + counter
    counter = counter + 1
print(f"   While loop (25K): {while_sum}")
print()

print("4. FUNCTION CALLS")
print("-" * 60)
def fast_add(x, y):
    return x + y

def fast_compare(a, b):
    if a < b:
        return a
    else:
        return b

func_result = 0
for i in range(25000):
    func_result = fast_add(func_result, i)
print(f"   Function calls (25K): {func_result}")

min_result = 0
for i in range(25000):
    min_result = fast_compare(min_result, i)
print(f"   Function with compare (25K): {min_result}")
print()

print("5. CLASS OPERATIONS")
print("-" * 60)
class FastCounter:
    def __init__(self, start):
        self.value = start

    def increment_if(self, threshold):
        if self.value < threshold:
            self.value = self.value + 1
        return self.value

c = FastCounter(0)
for i in range(25000):
    c.increment_if(50000)
print(f"   Class with conditionals (25K): {c.value}")
print()

print("=" * 60)
print("BENCHMARK COMPLETE!")
print("=" * 60)
print()
print("Performance Summary:")
print("  - All arithmetic operations: TaggedValue fast path")
print("  - All comparisons: TaggedValue fast path")
print("  - All conditionals: Optimized with fast comparisons")
print("  - Functions: Benefiting from internal optimizations")
print("  - Classes: Benefiting from method optimizations")
