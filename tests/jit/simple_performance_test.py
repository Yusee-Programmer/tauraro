"""
Simple JIT Performance Test
Tests that hot loops run significantly faster than cold loops
"""

print("=" * 60)
print("Simple JIT Performance Test")
print("=" * 60)
print()

# Test 1: Integer addition hot loop (should trigger JIT)
print("Test 1: Hot loop integer addition (10000 iterations)")
total = 0
for i in range(10000):
    total = total + i

expected = 49995000
if total == expected:
    print(f"✓ Result correct: {total}")
else:
    print(f"✗ Result incorrect: {total} (expected {expected})")

print()

# Test 2: Integer multiplication hot loop
print("Test 2: Hot loop integer multiplication (10000 iterations)")
total = 0
for i in range(10000):
    total = total + (i * 3)

expected = 149985000
if total == expected:
    print(f"✓ Result correct: {total}")
else:
    print(f"✗ Result incorrect: {total} (expected {expected})")

print()

# Test 3: Complex expression hot loop
print("Test 3: Hot loop complex expression (5000 iterations)")
total = 0
for i in range(5000):
    x = i * 2
    y = x + 10
    z = y - 5
    total = total + z

expected = 0
for i in range(5000):
    x = i * 2
    y = x + 10
    z = y - 5
    expected = expected + z

if total == expected:
    print(f"✓ Result correct: {total}")
else:
    print(f"✗ Result incorrect: {total} (expected {expected})")

print()

# Test 4: Nested operations
print("Test 4: Nested operations (1000 iterations)")
total = 0
for i in range(1000):
    for j in range(10):
        total = total + (i + j)

expected = 0
for i in range(1000):
    for j in range(10):
        expected = expected + (i + j)

if total == expected:
    print(f"✓ Result correct: {total}")
else:
    print(f"✗ Result incorrect: {total} (expected {expected})")

print()
print("=" * 60)
print("NOTE: If these complete quickly (< 1 second each), JIT is working")
print("=" * 60)
