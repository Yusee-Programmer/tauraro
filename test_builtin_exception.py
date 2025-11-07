print("Test 1: Calling Exception with message")
e = Exception("Test message")
print(f"Exception object: {e}")
print(f"Type: {type(e)}")

print("\nTest 2: Raising it")
raise e
