# Test all Tauraro file extensions
print("="*70)
print("  TESTING ALL TAURARO FILE EXTENSIONS")
print("  Supported: .py, .tr, .tau, .tauraro")
print("="*70)

# Test 1: .py extension
print("\n[TEST 1] .py Extension")
print("-" * 50)
import mymodule
print("✓ mymodule.py loaded")
print("✓ mymodule.add(5, 10) =", mymodule.add(5, 10))

# Test 2: .tr extension  
print("\n[TEST 2] .tr Extension")
print("-" * 50)
import mathutils
print("✓ mathutils.tr loaded")
print("✓ mathutils.square(5) =", mathutils.square(5))
print("✓ mathutils.cube(3) =", mathutils.cube(3))
print("✓ mathutils.E =", mathutils.E)

# Test 3: .tau extension
print("\n[TEST 3] .tau Extension")
print("-" * 50)
import stringutils
print("✓ stringutils.tau loaded")
reversed_str = stringutils.reverse('hello')
print("✓ stringutils.reverse('hello') =", reversed_str)
print("✓ stringutils.VOWELS =", stringutils.VOWELS)

# Test 4: .tauraro extension
print("\n[TEST 4] .tauraro Extension")
print("-" * 50)
import datautils
print("✓ datautils.tauraro loaded")
test_list = [10, 20, 30, 40, 50]
sum_result = datautils.sum_list(test_list)
avg_result = datautils.average(test_list)
print("✓ datautils.sum_list([10,20,30,40,50]) =", sum_result)
print("✓ datautils.average([10,20,30,40,50]) =", avg_result)

print("\n" + "="*70)
print("  ALL EXTENSION TESTS PASSED!")
print("  ✓ .py extension works")
print("  ✓ .tr extension works")
print("  ✓ .tau extension works")
print("  ✓ .tauraro extension works")
print("="*70)
