print("Testing bitwise OR...")
x = 0x10000000 | 0x00CF0000
print(f"Result: {x}")
print(f"Type: {type(x)}")
if x == None:
    print("ERROR: Result is None!")
else:
    print(f"SUCCESS: x = {x}")
