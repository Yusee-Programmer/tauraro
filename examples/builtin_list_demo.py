# Demonstration that list is a built-in data type in Tauraro
# This works without any imports, just like in Python

print("=== Tauraro Built-in List Demo ===")

# Create a list using the built-in list() function
my_list = list()
print(f"Empty list: {my_list}")

# Add elements to the list
my_list.append("Hello")
my_list.append("Tauraro")
my_list.append("World")
print(f"After appending: {my_list}")

# Use list literal syntax (if supported)
another_list = [1, 2, 3, 4, 5]
print(f"List literal: {another_list}")

# Test list operations
print(f"Length of list: {len(my_list)}")
print(f"First element: {my_list[0]}")
print(f"Last element: {my_list[-1]}")

# Test list methods
my_list.insert(1, "Beautiful")
print(f"After insert: {my_list}")

my_list.remove("Beautiful")
print(f"After remove: {my_list}")

popped = my_list.pop()
print(f"Popped element: {popped}")
print(f"After pop: {my_list}")

# Test list slicing (if supported)
numbers = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
print(f"Numbers list: {numbers}")
print(f"Slice [2:5]: {numbers[2:5]}")
print(f"Slice [::2]: {numbers[::2]}")

# Test list comprehension (if supported)
squares = [x**2 for x in range(5)]
print(f"Squares: {squares}")

# Test list sorting
unsorted = [3, 1, 4, 1, 5, 9, 2, 6]
unsorted.sort()
print(f"Sorted list: {unsorted}")

# Test list reversal
unsorted.reverse()
print(f"Reversed list: {unsorted}")

print("\n=== Demo Complete ===")
print("List is working as a built-in data type in Tauraro!")