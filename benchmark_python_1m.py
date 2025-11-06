"""Python Benchmark - 1 million increments"""

class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

counter = Counter()
i = 0
while i < 1000000:
    counter.increment()
    i = i + 1

print(counter.count)
