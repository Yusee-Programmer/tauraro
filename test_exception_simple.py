print("Test 1: Raising built-in Exception")
try:
    raise Exception("This is an error")
except Exception as e:
    print(f"Caught: {e}")

print("\nTest 2: Simple raise without catch")
raise Exception("Uncaught error")
