import time

print("Testing time module...")
start = time.time()
print(f"Current time: {start}")

# Do some work
total = 0
for i in range(1000000):
    total += i

end = time.time()
print(f"Sum: {total}")
print(f"Elapsed time: {end - start} seconds")
