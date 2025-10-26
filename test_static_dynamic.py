# Test file for static and dynamic typing features

# Static typing example
def add_numbers(a: int, b: int) -> int:
    return a + b

# Dynamic typing example
def add_dynamic(a, b):
    return a + b

# Test with explicit types
x: int = 10
y: int = 20
z = add_numbers(x, y)
print(f"Static typing result: {z}")

# Test with inferred types
a = 15
b = 25
c = add_dynamic(a, b)
print(f"Dynamic typing result: {c}")

# Test with mixed types
d: float = 3.14
e = 42
f = d + e
print(f"Mixed types result: {f}")