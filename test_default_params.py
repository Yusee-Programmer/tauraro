# Test default parameters

def test_defaults(a=10, b=20, c=30):
    print(f"a = {a}, b = {b}, c = {c}")
    return a + b + c

print("Test 1: No arguments")
result1 = test_defaults()
print(f"Result: {result1}\n")

print("Test 2: One argument")
result2 = test_defaults(100)
print(f"Result: {result2}\n")

print("Test 3: All arguments")
result3 = test_defaults(1, 2, 3)
print(f"Result: {result3}\n")
