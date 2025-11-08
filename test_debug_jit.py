# Debug test to understand register layout
print("=== JIT Debug Test ===")

total = 0
for i in range(200):
    total = total + i

# Expected: sum(0..199) = 199 * 200 / 2 = 19900
expected = 199 * 200 // 2
print(f"Result: {total}")
print(f"Expected: {expected}")
if total == expected:
    print("✓ PASS")
else:
    print(f"✗ FAIL (got {100.0 * total / expected:.1f}% of expected)")
