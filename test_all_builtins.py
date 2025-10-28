import math
import time
import random

print("=== Testing Math Module ===")
print("sqrt(16) =", math.sqrt(16))
print("pow(2, 3) =", math.pow(2, 3))
print("sin(0) =", math.sin(0))

print("\n=== Testing Time Module ===")
start = time.time()
print("Current time:", start)
print("Sleeping for 0.1 seconds...")
time.sleep(0.1)
end = time.time()
print("Time elapsed:", end - start)

print("\n=== Testing Random Module ===")
random.seed(42)
print("random():", random.random())
print("random():", random.random())
print("randint(1, 10):", random.randint(1, 10))
print("randint(1, 10):", random.randint(1, 10))

print("\n=== All Tests Complete! ===")
