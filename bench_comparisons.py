# Comparison operations benchmark
# Tests all comparison operators with TaggedValue fast path

print("=== Comparison Operations Benchmark ===")
print()

# Test less than (<)
print("1. Less Than (<)")
count_lt = 0
for i in range(100000):
    if i < 50000:
        count_lt = count_lt + 1
print(f"   Count < 50000: {count_lt}")

# Test less than or equal (<=)
print("2. Less Than or Equal (<=)")
count_le = 0
for i in range(100000):
    if i <= 50000:
        count_le = count_le + 1
print(f"   Count <= 50000: {count_le}")

# Test greater than (>)
print("3. Greater Than (>)")
count_gt = 0
for i in range(100000):
    if i > 50000:
        count_gt = count_gt + 1
print(f"   Count > 50000: {count_gt}")

# Test greater than or equal (>=)
print("4. Greater Than or Equal (>=)")
count_ge = 0
for i in range(100000):
    if i >= 50000:
        count_ge = count_ge + 1
print(f"   Count >= 50000: {count_ge}")

# Test equality (==)
print("5. Equality (==)")
count_eq = 0
for i in range(100000):
    if i == 50000:
        count_eq = count_eq + 1
print(f"   Count == 50000: {count_eq}")

# Test not equal (!=)
print("6. Not Equal (!=)")
count_ne = 0
for i in range(100000):
    if i != 50000:
        count_ne = count_ne + 1
print(f"   Count != 50000: {count_ne}")

print()
print("All comparison tests complete!")
print(f"Total operations: 600,000")
