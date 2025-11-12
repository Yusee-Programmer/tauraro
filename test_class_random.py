import random

class TestClass:
    def __init__(self):
        self.data = []
        self.init_data()

    def init_data(self):
        print("[init_data] Starting...")
        names = ["Alice", "Bob", "Charlie"]
        print(f"[init_data] names = {names}, len = {len(names)}")

        for i in range(5):
            name = random.choice(names)
            print(f"[init_data] Iteration {i}: chose {name}")
            self.data.append({"id": i, "name": name})

        print(f"[init_data] Done. Created {len(self.data)} items")

print("Creating TestClass instance...")
obj = TestClass()
print(f"obj.data = {obj.data}")
print("Test passed!")
