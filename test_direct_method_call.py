# Test to understand why direct method calls fail
d = {"a": 1, "b": 2}

print("=== Test 1: Two-step (stored method) ===")
try:
    getter = d.get
    print(f"Method stored: {getter}")
    result = getter("a")
    print(f"Success: getter('a') = {result}")
except Exception as e:
    print(f"Failed: {e}")

print("\n=== Test 2: Direct call ===")
try:
    result = d.get("a")
    print(f"Success: d.get('a') = {result}")
except Exception as e:
    print(f"Failed: {e}")

print("\n=== Test 3: Check attribute ===")
try:
    print(f"d.get exists: {hasattr(d, 'get')}")
    attr = d.get
    print(f"d.get type: {type(attr)}")
except Exception as e:
    print(f"Failed checking attribute: {e}")
