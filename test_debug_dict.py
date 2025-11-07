d = {"a": 1}
print(f"Type: {type(d)}")

# Try to access get as an attribute first
try:
    getter = d.get
    print(f"Got getter: {getter}")
except AttributeError as e:
    print(f"AttributeError: {e}")
