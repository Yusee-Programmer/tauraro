# Test that list is a built-in data type in Tauraro
# This should work without any imports

# Create a list using the built-in list() function
my_list = list()
my_list.append(1)
my_list.append(2)
my_list.append(3)

print("List contents:", my_list)
print("List length:", len(my_list))

# Test list indexing
print("First element:", my_list[0])
print("Last element:", my_list[-1])

# Test list methods
my_list.insert(1, 99)
print("After insert:", my_list)

my_list.remove(99)
print("After remove:", my_list)

popped = my_list.pop()
print("Popped element:", popped)
print("After pop:", my_list)

# Test list comprehension (if supported)
squared = [x * x for x in my_list]
print("Squared elements:", squared)

print("Test passed! List is working as a built-in data type.")