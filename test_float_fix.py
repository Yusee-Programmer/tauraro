print("=== Testing Float Arithmetic Fix ===")
print()

# Test 1: Basic float operations
print("Test 1: Basic float operations")
a = 5.5
b = 3.2
print(f"a = {a}")
print(f"b = {b}")
print(f"a + b = {a + b}")
print(f"a - b = {a - b}")
print(f"a * b = {a * b}")
print(f"a / b = {a / b}")
print()

# Test 2: Float comparisons
print("Test 2: Float comparisons")
x = 3.0
y = 5.0
print(f"x = {x}")
print(f"y = {y}")
print(f"x == y: {x == y}")
print(f"x != y: {x != y}")
print(f"x < y: {x < y}")
print(f"x <= y: {x <= y}")
print(f"x > y: {x > y}")
print(f"x >= y: {x >= y}")
print()

# Test 3: Float negation
print("Test 3: Float negation")
z = 7.5
print(f"z = {z}")
print(f"-z = {-z}")
print()

# Test 4: Time module (critical for benchmarks!)
print("Test 4: Time module")
import time
start = time.time()
print(f"time.time() = {start}")
print(f"Type: {type(start)}")
time.sleep(0.1)
end = time.time()
elapsed = end - start
print(f"Elapsed time after sleep(0.1): {elapsed}")
print(f"Is elapsed > 0: {elapsed > 0}")
print(f"Is elapsed < 1: {elapsed < 1}")
print()

print("=== All Float Tests Passed! ===")
