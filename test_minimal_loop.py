#!/usr/bin/env tauraro
# Minimal test to reproduce the infinite loop

print("Starting minimal test...")

passed = 0
failed = 0
errors = []

def test_module(name, test_func):
    global passed, failed, errors
    try:
        test_func()
        print(f"✓ {name}")
        passed += 1
        return True
    except Exception as e:
        print(f"✗ {name} - ERROR: {e}")
        failed += 1
        errors.append((name, str(e)))
        return False

def test_abc():
    import abc
    if not hasattr(abc, 'ABCMeta'):
        raise Exception("ABCMeta not found")
    if not hasattr(abc, 'ABC'):
        raise Exception("ABC not found")

print("Calling test_module...")
test_module("abc", test_abc)

print("After test_module call")
print(f"Passed: {passed}, Failed: {failed}")

print("End of script")
