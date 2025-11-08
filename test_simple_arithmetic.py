# Simple arithmetic test for baseline performance
import time

print("Testing arithmetic performance (no JIT)...")

start = time.time()
total = 0
for i in range(100000):
    total = total + i
end = time.time()

print(f"Sum: {total}")
print(f"Expected: {99999 * 100000 // 2}")
print(f"Time: {end - start:.3f} seconds")

if total == 99999 * 100000 // 2:
    print("✓ CORRECT")
else:
    print("✗ INCORRECT")
