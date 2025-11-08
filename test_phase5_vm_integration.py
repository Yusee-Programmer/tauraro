# Phase 5 VM Integration Test
# This test verifies that the CraneliftJIT compiler is properly integrated with the VM

print("Phase 5.1: VM Integration Test")
print("=" * 50)

# Test 1: Simple arithmetic loop (will trigger JIT at 10,000 iterations)
print("\nTest 1: Simple Arithmetic Loop (10,000 iterations)")
total = 0
for i in range(10000):
    total = total + i
print(f"Result: {total}")
print(f"Expected: 49995000")
if total == 49995000:
    print("✓ Test 1 PASSED")
else:
    print("✗ Test 1 FAILED")

# Test 2: List operations in hot loop
print("\nTest 2: List Operations (1,000 iterations)")
items = [10, 20, 30]
total2 = 0
for i in range(1000):
    idx = i % 3
    total2 = total2 + items[idx]
print(f"Result: {total2}")
print(f"Expected: 19990")
if total2 == 19990:
    print("✓ Test 2 PASSED")
else:
    print("✗ Test 2 FAILED")

# Test 3: Nested arithmetic
print("\nTest 3: Nested Arithmetic (5,000 iterations)")
x = 0
y = 0
for i in range(5000):
    x = x + i
    y = y + x
print(f"Result x: {x}, y: {y}")
print(f"Expected x: 12497500, y: 20833332500")
if x == 12497500 and y == 20833332500:
    print("✓ Test 3 PASSED")
else:
    print("✗ Test 3 FAILED")

print("\n" + "=" * 50)
print("Phase 5.1 VM Integration: All tests complete")
print("\nNote: JIT compilation should trigger at 10,000 iterations")
print("Check stderr for 'JIT: Compiled loop' messages")
