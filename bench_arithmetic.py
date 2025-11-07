# Arithmetic benchmark only
a = 0
b = 0
c = 0
for i in range(1000000):
    a = i + 1
    b = a - 5
    c = b + 10
print(f"Result: a={a}, b={b}, c={c}")
