"""
Core Optimizations Test (VM-compatible)
"""

print("=== CORE OPTIMIZATIONS TEST ===")
print()

# Test 1: Integer operations
print("Test 1: Integer Operations")
int_result = 0
for i in range(1000):
    int_result = int_result + 1
print("Integer sum:", int_result)
print()

# Test 2: Float operations
print("Test 2: Float Operations")
float_result = 0.0
for i in range(1000):
    float_result = float_result + 1.5
print("Float sum:", float_result)
print()

# Test 3: While loop
print("Test 3: While Loop")
counter = 0
i = 0
while i < 100:
    counter = counter + 1
    i = i + 1
print("While counter:", counter)
print()

# Test 4: If/elif/else
print("Test 4: Conditionals")
positive = 0
negative = 0
zero = 0

for i in range(100):
    x = i - 50
    if x > 0:
        positive = positive + 1
    elif x < 0:
        negative = negative + 1
    else:
        zero = zero + 1

print("Positive:", positive, "Negative:", negative, "Zero:", zero)
print()

# Test 5: Nested loops
print("Test 5: Nested Loops")
nested_sum = 0
for i in range(50):
    for j in range(50):
        nested_sum = nested_sum + 1
print("Nested sum:", nested_sum)
print()

# Test 6: Mixed operations
print("Test 6: Mixed Int/Float")
int_sum = 0
float_sum = 0.0
for i in range(100):
    int_sum = int_sum + 1
    float_sum = float_sum + 1.5
print("Int:", int_sum, "Float:", float_sum)
print()

# Test 7: Comparisons
print("Test 7: Comparisons")
compare_count = 0
for i in range(100):
    if i < 50:
        compare_count = compare_count + 1
    if i > 25:
        compare_count = compare_count + 1
    if i == 75:
        compare_count = compare_count + 1
print("Comparison count:", compare_count)
print()

print("=== ALL TESTS COMPLETE ===")
