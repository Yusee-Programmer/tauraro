# Quick test for all optimization types

# Test 1: Integer (already working)
print("Test 1: Integer")
total = 0
for i in range(100):
    total = total + 1
print("Integer result:", total)
print()

# Test 2: Float
print("Test 2: Float")
ftotal = 0.0
for i in range(100):
    ftotal = ftotal + 1.5
print("Float result:", ftotal)
print()

# Test 3: String
print("Test 3: String")
s = "Hello"
s2 = " World"
result = s + s2
print("String result:", result)
print()

# Test 4: Mixed
print("Test 4: Mixed")
a = 10
b = 20.5
c = a + a
d = b + b
print("Mixed results:", c, d)
