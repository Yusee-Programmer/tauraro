#!/usr/bin/env tauraro

name = "World"
value = 42

# Test f-string
result = f"Hello {name}!"
print(result)

# Test f-string with expression
result2 = f"Value is {value}"
print(result2)

# Test f-string with multiple parts
result3 = f"Name: {name}, Value: {value}"
print(result3)

print("F-strings working!")
