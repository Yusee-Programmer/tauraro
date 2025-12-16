#!/usr/bin/env python3
"""Comprehensive exception handling test for Tauraro"""

print("=== Exception Handling Test Suite ===\n")

# Test 1: Basic try-except
print("Test 1: Basic try-except")
try:
    x = 10 / 2
    print(f"  Success: {x}")
except:
    print("  Failed: Should not catch")

# Test 2: Exception with specific type
print("\nTest 2: Specific exception type")
try:
    result = 1 / 0
    print("  Failed: Should have raised exception")
except ZeroDivisionError:
    print("  Success: Caught ZeroDivisionError")

# Test 3: Exception with variable binding
print("\nTest 3: Exception with variable")
try:
    result = 1 / 0
except ZeroDivisionError as e:
    print(f"  Success: Caught exception: {e}")

# Test 4: Multiple exception handlers
print("\nTest 4: Multiple handlers")
try:
    items = [1, 2, 3]
    value = items[10]
except ZeroDivisionError:
    print("  Failed: Wrong handler")
except IndexError:
    print("  Success: Caught IndexError")
except:
    print("  Failed: Should catch IndexError specifically")

# Test 5: Try-except-else
print("\nTest 5: Try-except-else")
try:
    x = 10 / 2
except:
    print("  Failed: Should not except")
else:
    print("  Success: Else block executed")

# Test 6: Try-except-finally
print("\nTest 6: Try-except-finally")
try:
    x = 10 / 2
    print("  Try block executed")
except:
    print("  Failed: Should not except")
finally:
    print("  Success: Finally block executed")

# Test 7: Exception in finally
print("\nTest 7: Exception with finally (both execute)")
try:
    result = 1 / 0
except:
    print("  Except block executed")
finally:
    print("  Success: Finally executed even after exception")

# Test 8: Nested try blocks
print("\nTest 8: Nested try blocks")
try:
    print("  Outer try")
    try:
        print("    Inner try")
        result = 1 / 0
    except ZeroDivisionError:
        print("    Success: Inner except caught")
    print("  Outer try continues")
except:
    print("  Failed: Outer except should not trigger")

# Test 9: Re-raise exception
print("\nTest 9: Re-raise exception")
try:
    try:
        result = 1 / 0
    except:
        print("  Inner except, re-raising...")
        raise
except:
    print("  Success: Outer caught re-raised exception")

# Test 10: Raise new exception
print("\nTest 10: Raise new exception")
try:
    raise RuntimeError("Custom error message")
except RuntimeError:
    print("  Success: Caught RuntimeError")

# Test 11: Exception hierarchy (ArithmeticError catches ZeroDivisionError)
print("\nTest 11: Exception hierarchy")
try:
    result = 1 / 0
except ArithmeticError:
    print("  Success: Caught via parent class ArithmeticError")

# Test 12: Catch-all exception
print("\nTest 12: Catch-all")
try:
    result = 1 / 0
except:
    print("  Success: Catch-all handler worked")

# Test 13: Multiple nested finally blocks
print("\nTest 13: Multiple nested finally")
try:
    print("  Outer try")
    try:
        print("    Inner try")
    finally:
        print("    Inner finally")
finally:
    print("  Success: Outer finally")

print("\n=== All Exception Tests Completed ===")
