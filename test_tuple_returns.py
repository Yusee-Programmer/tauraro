# Test tuple return and unpacking

print("=== Testing Tuple Returns ===")

# Test 1: Simple tuple return
def get_coordinates():
    x = 10
    y = 20
    return (x, y)

print("\n1. Simple tuple return:")
result = get_coordinates()
print(f"Result: {result}")

# Test 2: Tuple unpacking
def get_name_and_age():
    return ("Alice", 30)

print("\n2. Tuple unpacking:")
name, age = get_name_and_age()
print(f"Name: {name}, Age: {age}")

# Test 3: Multiple values
def calculate_stats(n):
    total = n * (n + 1) // 2
    avg = total / n
    return (total, avg, n)

print("\n3. Multiple return values:")
total, average, count = calculate_stats(10)
print(f"Total: {total}, Average: {average}, Count: {count}")

# Test 4: Nested unpacking
def get_point_and_color():
    return ((5, 10), "red")

print("\n4. Nested tuple:")
point, color = get_point_and_color()
print(f"Point: {point}, Color: {color}")

print("\n=== Tests Complete ===")
