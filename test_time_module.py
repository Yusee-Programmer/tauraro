import time

print("Testing time.time() output:")
t1 = time.time()
print(f"time.time() = {t1}")
print(f"Type check: {type(t1)}")

# Small loop
total = 0
for i in range(100):
    total = total + i

t2 = time.time()
print(f"time.time() after loop = {t2}")
print(f"Difference = {t2 - t1}")
print(f"Difference type = {type(t2 - t1)}")
