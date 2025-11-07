# Test inline method caching optimization
# This test repeatedly calls methods to trigger cache hits

class Counter:
    def __init__(self):
        self.value = 0

    def increment(self):
        self.value = self.value + 1

    def get_value(self):
        return self.value

# Create instance
c = Counter()

# Call methods repeatedly to test caching
# First call: cache miss, second+ calls: cache hits
for i in range(5):
    c.increment()
    print(c.get_value())

print("Final value:", c.get_value())
