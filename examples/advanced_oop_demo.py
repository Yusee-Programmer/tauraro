class Vehicle:
    def __init__(self, brand, model):
        self.brand = brand
        self.model = model
        self.is_running = False
    
    def start(self):
        self.is_running = True
        return f"{self.brand} {self.model} is starting"
    
    def stop(self):
        self.is_running = False
        return f"{self.brand} {self.model} is stopping"
    
    def status(self):
        state = "running" if self.is_running else "stopped"
        return f"{self.brand} {self.model} is {state}"

class Car(Vehicle):
    def __init__(self, brand, model, doors):
        super().__init__(brand, model)
        self.doors = doors
    
    def honk(self):
        return "Beep beep!"
    
    def info(self):
        return f"{self.brand} {self.model} with {self.doors} doors"

class Motorcycle(Vehicle):
    def __init__(self, brand, model, engine_size):
        super().__init__(brand, model)
        self.engine_size = engine_size
    
    def wheelie(self):
        return "Popping a wheelie!"
    
    def info(self):
        return f"{self.brand} {self.model} with {self.engine_size}cc engine"

# Create instances
car = Car("Toyota", "Camry", 4)
motorcycle = Motorcycle("Harley-Davidson", "Street 750", 750)

# Test polymorphism
vehicles = [car, motorcycle]
for vehicle in vehicles:
    print(vehicle.start())
    print(vehicle.status())
    
    # Call specific methods based on type
    if isinstance(vehicle, Car):
        print(vehicle.honk())
        print(vehicle.info())
    elif isinstance(vehicle, Motorcycle):
        print(vehicle.wheelie())
        print(vehicle.info())
    
    print(vehicle.stop())
    print(vehicle.status())
    print("---")