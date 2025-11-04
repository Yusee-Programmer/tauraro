# Test module-level function parameters

def module_func(a, b, c):
    print(f"module_func: a={a}, b={b}, c={c}")
    return a + b + c

result = module_func(100, 200, 300)
print(f"Result: {result}")
