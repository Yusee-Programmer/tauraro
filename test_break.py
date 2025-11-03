# Test break statement

print("Testing break...")

i = 0
while i < 10:
    print(f"i = {i}")
    if i == 3:
        break
    i = i + 1

print("After break")
print("Break test complete!")
