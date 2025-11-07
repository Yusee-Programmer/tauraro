# Conditional and loop benchmark
# Tests if/while with comparison conditions

print("=== Conditional Operations Benchmark ===")
print()

# Test if-else chains with comparisons
print("1. If-Else Chains")
total = 0
for i in range(100000):
    if i < 25000:
        total = total + 1
    elif i < 50000:
        total = total + 2
    elif i < 75000:
        total = total + 3
    else:
        total = total + 4
print(f"   Total from if-else: {total}")

# Test while loops with conditions
print("2. While Loops with Conditions")
counter = 0
sum_val = 0
while counter < 50000:
    sum_val = sum_val + counter
    counter = counter + 1
print(f"   Sum from while: {sum_val}")

# Test nested conditionals
print("3. Nested Conditionals")
nested_count = 0
for i in range(10000):
    if i > 5000:
        if i < 7500:
            if i % 2 == 0:
                nested_count = nested_count + 1
print(f"   Nested condition count: {nested_count}")

# Test loop with early break
print("4. Loop with Break Condition")
break_value = 0
for i in range(100000):
    if i > 75000:
        break_value = i
        break
    break_value = break_value + 1
print(f"   Break value: {break_value}")

# Test loop with continue
print("5. Loop with Continue Condition")
continue_sum = 0
for i in range(50000):
    if i % 2 == 0:
        continue
    continue_sum = continue_sum + i
print(f"   Continue sum (odds): {continue_sum}")

print()
print("All conditional tests complete!")
