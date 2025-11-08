# Simplified tuple unpacking test
print("Test 1: Direct tuple unpacking")
x, y = (10, 20)
print("x =", x)
print("y =", y)

print("\nTest 2: Function returning tuple")
def get_tuple():
    return (30, 40)

a, b = get_tuple()
print("a =", a)
print("b =", b)
