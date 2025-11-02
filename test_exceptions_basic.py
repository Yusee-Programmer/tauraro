# Basic Exception Handling Test

print("=" * 50)
print("BASIC EXCEPTION HANDLING TEST")
print("=" * 50)

# Test 1: Basic try-except
print("\n[1] Basic try-except")
try:
    x = 10 / 0
    print("  This should not print")
except:
    print("  ✓ Basic exception caught!")

# Test 2: Raise keyword
print("\n[2] Raise keyword")
try:
    raise ValueError("Test error")
except:
    print("  ✓ Raised exception caught!")

# Test 3: Assert keyword
print("\n[3] Assert keyword")
try:
    assert 1 == 1
    print("  ✓ Assert True works")
except:
    print("  Assert True failed")

try:
    assert 1 == 2
    print("  This should not print")
except:
    print("  ✓ Assert False caught!")

print("\n" + "=" * 50)
print("TEST COMPLETE")
print("=" * 50)
