# Test type annotations

print("Testing type annotations...")

# Test 1: Function with type annotations
def greet(name: str) -> str:
    return f"Hello, {name}!"

result = greet("World")
print(f"Result: {result}")

# Test 2: Class with type-annotated __init__
class Person:
    def __init__(self, name: str, age: int):
        self.name = name
        self.age = age

person = Person("Alice", 30)
print(f"Person: {person.name}, age {person.age}")

print("Type annotations test complete!")
