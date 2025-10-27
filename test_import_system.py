# Test program with both user-defined and builtin imports
import mymath
import math

print("Testing User-Defined Module (mymath):")
print("square(5) =", mymath.square(5))
print("cube(3) =", mymath.cube(3))
print("add(10, 20) =", mymath.add(10, 20))
print("mymath.PI =", mymath.PI)

print("\nTesting Builtin Module (math):")
print("math.sqrt(16) =", math.sqrt(16))
print("math.pow(2, 3) =", math.pow(2, 3))
print("math.pi =", math.pi)

print("\nMixed operations:")
result = mymath.square(math.sqrt(16))
print("mymath.square(math.sqrt(16)) =", result)
