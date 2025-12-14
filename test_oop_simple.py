# Simple OOP test with classes
class Animal:
    def __init__(self: Animal, name: str) -> int:
        self.name: str = name
        return 0

    def speak(self: Animal) -> str:
        return "Some sound"

class Dog(Animal):
    def __init__(self: Dog, name: str, breed: str) -> int:
        super().__init__(name)
        self.breed: str = breed
        return 0

    def speak(self: Dog) -> str:
        return "Woof!"

    def get_info(self: Dog) -> str:
        info: str = self.name + " is a " + self.breed
        return info

def main() -> int:
    dog: Dog = Dog("Buddy", "Golden Retriever")
    sound: str = dog.speak()
    print(sound)

    info: str = dog.get_info()
    print(info)

    return 0

main()
