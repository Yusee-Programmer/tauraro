# Design Patterns in Tauraro

Common design patterns and how to implement them in Tauraro.

## Creational Patterns

### Singleton

```python
class Singleton:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

# Usage
s1 = Singleton()
s2 = Singleton()
print(s1 is s2)  # True
```

### Factory

```python
class AnimalFactory:
    @staticmethod
    def create_animal(animal_type: str):
        if animal_type == "dog":
            return Dog()
        elif animal_type == "cat":
            return Cat()
        else:
            raise ValueError(f"Unknown animal: {animal_type}")
```

### Builder

```python
class Pizza:
    def __init__(self):
        self.size = None
        self.cheese = False
        self.pepperoni = False

class PizzaBuilder:
    def __init__(self):
        self.pizza = Pizza()

    def set_size(self, size):
        self.pizza.size = size
        return self

    def add_cheese(self):
        self.pizza.cheese = True
        return self

    def add_pepperoni(self):
        self.pizza.pepperoni = True
        return self

    def build(self):
        return self.pizza

# Usage
pizza = PizzaBuilder().set_size("large").add_cheese().build()
```

## Structural Patterns

### Decorator

```python
def timing_decorator(func):
    import time

    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f"{func.__name__} took {end - start:.4f}s")
        return result

    return wrapper

@timing_decorator
def slow_function():
    import time
    time.sleep(1)
```

### Adapter

```python
class OldSystem:
    def old_request(self):
        return "Old format data"

class NewSystem:
    def request(self):
        pass

class Adapter(NewSystem):
    def __init__(self, old_system):
        self.old_system = old_system

    def request(self):
        old_data = self.old_system.old_request()
        return f"Adapted: {old_data}"
```

## Behavioral Patterns

### Observer

```python
class Subject:
    def __init__(self):
        self._observers = []

    def attach(self, observer):
        self._observers.append(observer)

    def notify(self, event):
        for observer in self._observers:
            observer.update(event)

class Observer:
    def update(self, event):
        print(f"Received event: {event}")
```

### Strategy

```python
class SortStrategy:
    def sort(self, data):
        pass

class QuickSort(SortStrategy):
    def sort(self, data):
        # Quick sort implementation
        return sorted(data)

class MergeSort(SortStrategy):
    def sort(self, data):
        # Merge sort implementation
        return sorted(data)

class Sorter:
    def __init__(self, strategy: SortStrategy):
        self.strategy = strategy

    def sort_data(self, data):
        return self.strategy.sort(data)
```

## Async Patterns

### Async Iterator

```python
import asyncio

class AsyncCounter:
    def __init__(self, limit):
        self.limit = limit
        self.current = 0

    def __aiter__(self):
        return self

    async def __anext__(self):
        if self.current >= self.limit:
            raise StopAsyncIteration

        await asyncio.sleep(0.1)
        self.current += 1
        return self.current
```

### Producer-Consumer

```python
import asyncio

async def producer(queue, num_items):
    for i in range(num_items):
        await asyncio.sleep(0.1)
        await queue.put(f"Item {i}")
    await queue.put(None)  # Signal completion

async def consumer(queue):
    while True:
        item = await queue.get()
        if item is None:
            break
        print(f"Consumed: {item}")
```

## Next Steps

- [Best Practices](best-practices.md) - Coding best practices
- [Examples Index](index.md) - More examples
- [Classes](../language/classes.md) - OOP in Tauraro
