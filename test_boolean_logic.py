#!/usr/bin/env tauraro

print("Testing boolean logic...")

# Test 1: Simple boolean
result = True
print("result =", result)
print("not result =", not result)

if not result:
    print("ERROR: This should not print!")
    raise Exception("Boolean logic failed")
else:
    print("GOOD: Boolean logic works correctly")

# Test 2: hasattr
import abc
has_abcmeta = hasattr(abc, 'ABCMeta')
print("\nhasattr(abc, 'ABCMeta') =", has_abcmeta)
print("not hasattr(abc, 'ABCMeta') =", not has_abcmeta)

if not has_abcmeta:
    print("ERROR: hasattr returned False but should be True!")
    raise Exception("ABCMeta not found")
else:
    print("GOOD: ABCMeta found")

print("\nAll tests passed!")
