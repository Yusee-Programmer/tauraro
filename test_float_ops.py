# Test float operations
print("Testing float arithmetic:")

a = 1762587865.241701
b = 1762587865.241854
print(f"a = {a} (type: {type(a)})")
print(f"b = {b} (type: {type(b)})")

c = b - a
print(f"b - a = {c} (type: {type(c)})")
print(f"Expected: ~0.000153 (type: float)")

# Simple float operations
x = 5.5
y = 2.3
print(f"\nx = {x}, y = {y}")
print(f"x + y = {x + y} (type: {type(x + y)})")
print(f"x - y = {x - y} (type: {type(x - y)})")
print(f"x * y = {x * y} (type: {type(x * y)})")
print(f"x / y = {x / y} (type: {type(x / y)})")
