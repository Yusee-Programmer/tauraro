#!/usr/bin/env python3
# Benchmark: Classes and OOP

class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def distance_squared(self) -> int:
        return self.x * self.x + self.y * self.y

    def add(self, other) -> None:
        self.x = self.x + other.x
        self.y = self.y + other.y

class Counter:
    def __init__(self, start: int):
        self.count = start

    def increment(self) -> None:
        self.count = self.count + 1

    def get_count(self) -> int:
        return self.count

class Rectangle:
    def __init__(self, width: int, height: int):
        self.width = width
        self.height = height

    def area(self) -> int:
        return self.width * self.height

    def perimeter(self) -> int:
        return 2 * (self.width + self.height)

def test_class_creation():
    i: int = 0
    total: int = 0
    while i < 1000000:
        p: Point = Point(i, i + 1)
        total = total + p.distance_squared()
        i = i + 1
    return total

def test_method_calls():
    c: Counter = Counter(0)
    i: int = 0
    while i < 10000000:
        c.increment()
        i = i + 1
    return c.get_count()

def test_object_interaction():
    p1: Point = Point(0, 0)
    i: int = 0
    while i < 1000000:
        p2: Point = Point(i, i)
        p1.add(p2)
        i = i + 1
    return p1.distance_squared()

def test_rectangles():
    total: int = 0
    i: int = 1
    while i < 100000:
        r: Rectangle = Rectangle(i, i + 1)
        total = total + r.area()
        i = i + 1
    return total

def main():
    print("Testing class creation and methods...")
    result1: int = test_class_creation()
    print(result1)

    print("Testing repeated method calls...")
    result2: int = test_method_calls()
    print(result2)

    print("Testing object interaction...")
    result3: int = test_object_interaction()
    print(result3)

    print("Testing rectangle calculations...")
    result4: int = test_rectangles()
    print(result4)

    print("All class tests passed!")

main()
