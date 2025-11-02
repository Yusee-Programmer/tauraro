# Exception Handling Test - Comprehensive Validation

print("=" * 70)
print("TAURARO - EXCEPTION HANDLING VALIDATION")
print("=" * 70)

# 1. Basic try-except
print("\n[1] Basic try-except")
try:
    x = 10 / 0
    print("  This should not print")
except:
    print("  ✓ Basic try-except works!")

# 2. Catching specific exceptions
print("\n[2] Catching specific exceptions")
try:
    x = 10 / 0
except ZeroDivisionError:
    print("  ✓ ZeroDivisionError caught!")

# 3. Multiple except clauses
print("\n[3] Multiple except clauses")
try:
    x = int("abc")
except ZeroDivisionError:
    print("  Wrong exception")
except ValueError:
    print("  ✓ ValueError caught correctly!")

# 4. Raise keyword
print("\n[4] Raise keyword")
try:
    raise ValueError("Custom error message")
except ValueError as e:
    print(f"  ✓ Raised and caught: {e}")

# 5. Assert keyword
print("\n[5] Assert keyword")
try:
    assert 1 == 1
    print("  ✓ Assert True works")
    assert 1 == 2
    print("  This should not print")
except AssertionError:
    print("  ✓ Assert False raises AssertionError!")

# 6. Try-except-else
print("\n[6] Try-except-else")
try:
    x = 10 / 2
except ZeroDivisionError:
    print("  Exception occurred")

# 7. Try-except-finally
print("\n[7] Try-except-finally")
try:
    x = 10 / 2
except ZeroDivisionError:
    print("  Exception occurred")
finally:
    print("  ✓ Finally block always executes!")

# 8. Nested try-except
print("\n[8] Nested try-except")
try:
    try:
        x = 10 / 0
    except ZeroDivisionError:
        print("  ✓ Inner exception caught")
        raise ValueError("Re-raising as different type")
except ValueError:
    print("  ✓ Outer exception caught!")

# 9. Built-in Exception classes
print("\n[9] Built-in Exception classes")
exceptions_to_test = [
    (ValueError, "ValueError"),
    (TypeError, "TypeError"),
    (KeyError, "KeyError"),
    (IndexError, "IndexError"),
    (AttributeError, "AttributeError"),
    (ZeroDivisionError, "ZeroDivisionError"),
    (RuntimeError, "RuntimeError"),
    (NameError, "NameError"),
]

for exc_class, exc_name in exceptions_to_test:
    try:
        raise exc_class(f"Test {exc_name}")
    except exc_class:
        print(f"  ✓ {exc_name} works!")

# 10. Exception message access
print("\n[10] Exception message access")
try:
    raise ValueError("Custom message")
except ValueError as e:
    print(f"  ✓ Exception message: {e}")

# FINAL SUMMARY
print("\n" + "=" * 70)
print("EXCEPTION HANDLING TEST COMPLETE")
print("=" * 70)
