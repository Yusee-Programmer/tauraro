d = {"a": 1, "b": 2}
print(f"Dict: {d}")

# Test getting the method
getter = d.get
print(f"Getter: {getter}")

# Test calling it
try:
    result = getter("a")
    print(f"getter('a') = {result}")
except Exception as e:
    print(f"Error calling getter('a'): {e}")

# Test calling with default
try:
    result = getter("c", 99)
    print(f"getter('c', 99) = {result}")
except Exception as e:
    print(f"Error calling getter('c', 99): {e}")

# Test direct call
try:
    result = d.get("b")
    print(f"d.get('b') = {result}")
except Exception as e:
    print(f"Error calling d.get('b'): {e}")
