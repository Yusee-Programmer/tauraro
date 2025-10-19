# Test f-string functionality

name = "Alice"
age = 30

# Test basic f-string
greeting = f"Hello, {name}!"
print(greeting)

# Test f-string with multiple expressions
info = f"{name} is {age} years old"
print(info)

# Test f-string with expressions
calc = f"Next year {name} will be {age + 1}"
print(calc)

# Test f-string in a class context
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def introduce(self):
        return f"Hi, I'm {self.name} and I'm {self.age} years old"

person = Person("Bob", 25)
print(person.introduce())

print("F-string tests completed!")