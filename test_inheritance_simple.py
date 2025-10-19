# Test simple inheritance
print("=" * 60)
print("Testing Simple Inheritance")
print("=" * 60)

# Test 1: Parent with __init__, child without
print("\nTest 1: Parent __init__, child without")
class Person:
    def __init__(self, name):
        self.name = name

class Employee(Person):
    def get_name(self):
        return self.name

emp = Employee("Alice")
print("Employee name:", emp.name)
print("Get name:", emp.get_name())

# Test 2: Both have __init__ (need explicit parent call)
print("\nTest 2: Both have __init__")
class Vehicle:
    def __init__(self, brand):
        self.brand = brand
        print("Vehicle __init__ called")

class Car(Vehicle):
    def __init__(self, brand, model):
        # Explicitly call parent __init__ using direct class call
        Vehicle.__init__(self, brand)
        self.model = model
        print("Car __init__ called")

car = Car("Toyota", "Camry")
print("Car brand:", car.brand)
print("Car model:", car.model)

print("\n" + "=" * 60)
print("INHERITANCE TESTS COMPLETED!")
print("=" * 60)
