# Simple list append test

# Test 1: Module level list
print("Test 1: Module level list")
my_list = []
print(f"Before: {my_list}")
my_list.append(42)
print(f"After: {my_list}")

# Test 2: Inside a function
print("\nTest 2: Inside a function")
def test_func():
    local_list = []
    print(f"  Before: {local_list}")
    local_list.append(99)
    print(f"  After: {local_list}")
    return local_list

result = test_func()
print(f"  Returned: {result}")

# Test 3: Inside a method
print("\nTest 3: Inside a method")
class TestClass:
    def test_method(self):
        method_list = []
        print(f"  Before: {method_list}")
        method_list.append(77)
        print(f"  After: {method_list}")
        return method_list

obj = TestClass()
result2 = obj.test_method()
print(f"  Returned: {result2}")

print("\nAll tests complete!")
