# Simple benchmark without timing module
# Just test that arithmetic works correctly

def bench_arithmetic():
    total = 0
    for i in range(100):
        total = total + i
    return total

def bench_sub():
    total = 1000
    for i in range(100):
        total = total - i
    return total

print("Testing TaggedValue integration...")
result1 = bench_arithmetic()
result2 = bench_sub()
print(f"Add result: {result1}")
print(f"Sub result: {result2}")

# Expected: 4950, 1000 - 4950 = -3950
if result1 == 4950:
    print("✓ Addition working correctly")
else:
    print(f"✗ Addition failed: expected 4950, got {result1}")

if result2 == -3950:
    print("✓ Subtraction working correctly")
else:
    print(f"✗ Subtraction failed: expected -3950, got {result2}")
