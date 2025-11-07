print("Test 1: Custom exception class")

class MyError(Exception):
    pass

print("Created MyError class")

try:
    raise MyError("Custom error message")
except MyError as e:
    print(f"Caught custom exception: {e}")
    print(f"Type: {type(e)}")

print("\nTest 2: Uncaught custom exception")
raise MyError("Uncaught custom error")
