# Test input() function fix
print("Testing input() with prompt...")
name = input("Enter your name: ")
print("You entered:", name)
print("Type check:", type(name))

# Test input() without prompt
print("\nTesting input() without prompt...")
age = input()
print("You entered:", age)

print("\nREPL input() test completed!")
