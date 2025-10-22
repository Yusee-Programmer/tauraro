# Comprehensive Type Enforcement Test Suite for Tauraro
# Tests static typing features with type annotations

print("=" * 50)
print("Type Enforcement Test Suite")
print("=" * 50)

# Test 1: Simple type enforcement with integers
print("\n[Test 1] Simple integer type enforcement")
try:
    age: int = 20
    print(f"✓ Successfully assigned int value to age: {age}")

    # This should fail
    age = "twenty"  # TypeError expected
    print("✗ FAILED: Should have raised TypeError for age = '20'")
except Exception as e:
    if "TypeError" in str(e):
        print(f"✓ Correctly caught type error: {e}")
    else:
        print(f"✗ Unexpected error: {e}")

# Test 2: String type enforcement
print("\n[Test 2] String type enforcement")
try:
    name: str = "Alice"
    print(f"✓ Successfully assigned str value to name: {name}")

    # This should fail
    name = 123  # TypeError expected
    print("✗ FAILED: Should have raised TypeError for name = 123")
except Exception as e:
    if "TypeError" in str(e):
        print(f"✓ Correctly caught type error: {e}")
    else:
        print(f"✗ Unexpected error: {e}")

# Test 3: Float type enforcement
print("\n[Test 3] Float type enforcement")
try:
    price: float = 19.99
    print(f"✓ Successfully assigned float value to price: {price}")

    # This should fail
    price = "nineteen ninety-nine"  # TypeError expected
    print("✗ FAILED: Should have raised TypeError for price = '19.99'")
except Exception as e:
    if "TypeError" in str(e):
        print(f"✓ Correctly caught type error: {e}")
    else:
        print(f"✗ Unexpected error: {e}")

# Test 4: Bool type enforcement
print("\n[Test 4] Bool type enforcement")
try:
    is_active: bool = True
    print(f"✓ Successfully assigned bool value to is_active: {is_active}")

    # This should fail
    is_active = "true"  # TypeError expected
    print("✗ FAILED: Should have raised TypeError for is_active = 'true'")
except Exception as e:
    if "TypeError" in str(e):
        print(f"✓ Correctly caught type error: {e}")
    else:
        print(f"✗ Unexpected error: {e}")

# Test 5: List type enforcement
print("\n[Test 5] List type enforcement")
try:
    numbers: list = [1, 2, 3, 4, 5]
    print(f"✓ Successfully assigned list value to numbers: {numbers}")

    # This should fail
    numbers = "not a list"  # TypeError expected
    print("✗ FAILED: Should have raised TypeError for numbers = 'not a list'")
except Exception as e:
    if "TypeError" in str(e):
        print(f"✓ Correctly caught type error: {e}")
    else:
        print(f"✗ Unexpected error: {e}")

# Test 6: Dict type enforcement
print("\n[Test 6] Dict type enforcement")
try:
    person: dict = {"name": "Bob", "age": 30}
    print(f"✓ Successfully assigned dict value to person: {person}")

    # This should fail
    person = ["Bob", 30]  # TypeError expected
    print("✗ FAILED: Should have raised TypeError for person = ['Bob', 30]")
except Exception as e:
    if "TypeError" in str(e):
        print(f"✓ Correctly caught type error: {e}")
    else:
        print(f"✗ Unexpected error: {e}")

# Test 7: Function return type enforcement
print("\n[Test 7] Function return type enforcement")
try:
    def get_age() -> int:
        return 25

    result = get_age()
    print(f"✓ Function returned correct type: {result}")

    def get_name() -> str:
        return 123  # This should fail

    try:
        bad_result = get_name()
        print("✗ FAILED: Should have raised TypeError in get_name")
    except Exception as e:
        if "TypeError" in str(e):
            print(f"✓ Correctly caught function return type error: {e}")
        else:
            print(f"? Unexpected error: {e}")
except Exception as e:
    print(f"? Error during function test: {e}")

# Test 8: Dynamic typing (no type annotation)
print("\n[Test 8] Dynamic typing (no type hints)")
try:
    dynamic_var = 42
    print(f"✓ dynamic_var = {dynamic_var} (int)")

    dynamic_var = "now a string"
    print(f"✓ dynamic_var = '{dynamic_var}' (str)")

    dynamic_var = [1, 2, 3]
    print(f"✓ dynamic_var = {dynamic_var} (list)")

    print("✓ Dynamic typing works correctly (no type enforcement)")
except Exception as e:
    print(f"✗ Unexpected error in dynamic typing: {e}")

# Test 9: Mixed static and dynamic in same script
print("\n[Test 9] Mixed static and dynamic typing")
try:
    typed_value: int = 100
    untyped_value = 200

    print(f"✓ typed_value (static): {typed_value}")
    print(f"✓ untyped_value (dynamic): {untyped_value}")

    # Dynamic can change types
    untyped_value = "changed to string"
    print(f"✓ untyped_value changed: '{untyped_value}'")

    # Static cannot change types
    try:
        typed_value = "this should fail"
        print("✗ FAILED: Static typed variable accepted wrong type")
    except Exception as e:
        if "TypeError" in str(e):
            print(f"✓ Static type enforcement working: {e}")
        else:
            print(f"? Unexpected error: {e}")
except Exception as e:
    print(f"✗ Error in mixed typing test: {e}")

# Test 10: Reassignment with correct type
print("\n[Test 10] Reassignment with correct type")
try:
    count: int = 10
    print(f"✓ Initial count: {count}")

    count = 20  # This should work (same type)
    print(f"✓ Updated count: {count}")

    count = 30  # This should also work
    print(f"✓ Updated count again: {count}")
except Exception as e:
    print(f"✗ Unexpected error: {e}")

print("\n" + "=" * 50)
print("Type Enforcement Test Suite Complete")
print("=" * 50)
