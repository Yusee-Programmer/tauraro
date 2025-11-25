# Python OOP Benchmark

# Point class for 2D geometry
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def distance_squared(self):
        return self.x * self.x + self.y * self.y
    
    def add(self, other):
        return Point(self.x + other.x, self.y + other.y)
    
    def scale(self, factor):
        return Point(self.x * factor, self.y * factor)

# Counter class for accumulation
class Counter:
    def __init__(self, start):
        self.value = start
    
    def increment(self):
        self.value = self.value + 1
    
    def add(self, n):
        self.value = self.value + n
    
    def get(self):
        return self.value

# BankAccount for state management
class BankAccount:
    def __init__(self, balance):
        self.balance = balance
        self.transactions = 0
    
    def deposit(self, amount):
        self.balance = self.balance + amount
        self.transactions = self.transactions + 1
    
    def withdraw(self, amount):
        if self.balance >= amount:
            self.balance = self.balance - amount
            self.transactions = self.transactions + 1
            return True
        return False
    
    def get_balance(self):
        return self.balance

# === BENCHMARKS ===
print("=== PYTHON OOP BENCHMARK ===")

# Test 1: Object creation and method calls
print("Test 1: Point operations")
total = 0
i = 0
while i < 1000:
    p = Point(i, i * 2)
    total = total + p.distance_squared()
    i = i + 1
print(total)

# Test 2: Counter increments
print("Test 2: Counter increments")
c = Counter(0)
j = 0
while j < 10000:
    c.increment()
    j = j + 1
print(c.get())

# Test 3: Object chaining
print("Test 3: Point chaining")
p1 = Point(1, 1)
k = 0
while k < 500:
    p1 = p1.add(Point(1, 1))
    k = k + 1
print(p1.x)
print(p1.y)

# Test 4: Bank transactions
print("Test 4: Bank transactions")
account = BankAccount(1000)
m = 0
while m < 1000:
    account.deposit(10)
    account.withdraw(5)
    m = m + 1
print(account.get_balance())
print(account.transactions)

print("=== OOP BENCHMARK COMPLETE ===")
