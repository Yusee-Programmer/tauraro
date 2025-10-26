# Test star imports
print("="*60)
print("  TESTING STAR IMPORTS")
print("="*60)

# Test 1: Star import from builtin module
print("\n[TEST 1] Star Import from math")
print("-" * 40)
from math import *
print("✓ Star import successful!")
print("✓ pi =", pi)
print("✓ e =", e)
print("✓ sqrt(16) =", sqrt(16))

# Test 2: Star import from custom module
print("\n[TEST 2] Star Import from mathutils.tr")
print("-" * 40)
from mathutils import *
print("✓ Star import successful!")
print("✓ square(5) =", square(5))
print("✓ cube(3) =", cube(3))
print("✓ E =", E)
print("✓ GOLDEN_RATIO =", GOLDEN_RATIO)

print("\n" + "="*60)
print("  STAR IMPORT TESTS PASSED!")
print("="*60)
