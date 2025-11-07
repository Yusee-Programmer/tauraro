# Debug: Show what bytecode is generated for a simple loop
total = 0
for i in range(15000):
    total = total + i

print(f"Total: {total}")
