# Test if list append works correctly with references

class TestClass:
    def __init__(self):
        self.my_list = []

    def add_item_method1(self, item):
        # Method 1: Get reference first, then append
        list_ref = self.my_list
        list_ref.append(item)
        print(f"Method 1: Appended {item}, list_ref = {list_ref}")

    def add_item_method2(self, item):
        # Method 2: Append directly
        self.my_list.append(item)
        print(f"Method 2: Appended {item}, self.my_list = {self.my_list}")

print("Testing Method 1 (reference):")
obj1 = TestClass()
print(f"Before: obj1.my_list = {obj1.my_list}")
obj1.add_item_method1(42)
print(f"After: obj1.my_list = {obj1.my_list}")

print("\nTesting Method 2 (direct):")
obj2 = TestClass()
print(f"Before: obj2.my_list = {obj2.my_list}")
obj2.add_item_method2(99)
print(f"After: obj2.my_list = {obj2.my_list}")

print("\nTest complete!")
