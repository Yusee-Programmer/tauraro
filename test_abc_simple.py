#!/usr/bin/env tauraro

print("Testing abc module...")

import abc

print("abc module imported:", abc)
print("Type of abc:", type(abc))

# Try to access ABCMeta
try:
    abcmeta = abc.ABCMeta
    print("ABCMeta found:", abcmeta)
except Exception as e:
    print("Error accessing ABCMeta:", e)

# Try to access ABC
try:
    abc_class = abc.ABC
    print("ABC found:", abc_class)
except Exception as e:
    print("Error accessing ABC:", e)

# Test hasattr
try:
    has_abcmeta = hasattr(abc, 'ABCMeta')
    print("hasattr(abc, 'ABCMeta'):", has_abcmeta)
except Exception as e:
    print("Error with hasattr:", e)

print("Test complete!")
