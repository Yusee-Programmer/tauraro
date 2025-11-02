# Specific Exception Type Test

print("=" * 50)
print("SPECIFIC EXCEPTION TYPE TEST")
print("=" * 50)

# Test 1: Catch specific exception
print("\n[1] Catch ZeroDivisionError")
try:
    x = 10 / 0
except ZeroDivisionError:
    print("  ✓ ZeroDivisionError caught!")

# Test 2: Catch ValueError
print("\n[2] Catch ValueError")
try:
    raise ValueError("Test error")
except ValueError:
    print("  ✓ ValueError caught!")

# Test 3: Multiple except clauses
print("\n[3] Multiple except clauses")
try:
    raise TypeError("Type error")
except ValueError:
    print("  Wrong exception")
except TypeError:
    print("  ✓ TypeError caught correctly!")

# Test 4: Exception with message
print("\n[4] Exception with message (as e)")
try:
    raise RuntimeError("Custom message")
except RuntimeError as e:
    print(f"  ✓ Message: {e}")

# Test 5: IndexError
print("\n[5] IndexError")
try:
    mylist = [1, 2, 3]
    x = mylist[10]
except IndexError:
    print("  ✓ IndexError caught!")

# Test 6: KeyError
print("\n[6] KeyError")
try:
    mydict = {"a": 1}
    x = mydict["z"]
except KeyError:
    print("  ✓ KeyError caught!")

print("\n" + "=" * 50)
print("TEST COMPLETE")
print("=" * 50)
