# Fast OOP Benchmark - Demonstrates compilation speedup

class Calculator:
    def __init__(self, value):
        self.value = value

    def add(self, x):
        self.value = self.value + x
        return self.value

    def multiply(self, x):
        self.value = self.value * x
        return self.value

class AdvancedCalculator(Calculator):
    def __init__(self, value, multiplier):
        super().__init__(value)
        self.multiplier = multiplier

    def compute(self, x):
        result = self.add(x)
        result = self.multiply(self.multiplier)
        return result

print("=== Starting OOP Computation Benchmark ===")

# Create many objects and perform calculations
result = 0
calc = Calculator(1)
i = 0
while i < 100000:
    calc.add(1)
    calc.multiply(2)
    i = i + 1

print("Calculator test complete")

adv_calc = AdvancedCalculator(10, 3)
i = 0
while i < 100000:
    adv_calc.compute(5)
    i = i + 1

print("AdvancedCalculator test complete")
print("=== Benchmark Complete ===")
