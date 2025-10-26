# Comprehensive Import System Test
print("=== Testing Tauraro Import System ===\n")

# Test 1: Import builtin modules
print("--- Test 1: Builtin Modules ---")
import math
import sys
import os
import time
import random

print("math.pi:", math.pi)
print("sys.platform:", sys.platform)
print("Builtin modules imported successfully!\n")

# Test 2: From-import from builtin modules
print("--- Test 2: From-Import Builtin ---")
from math import sqrt, e
from sys import version

print("sqrt(16):", sqrt(16))
print("e:", e)
print("version:", version)
print("From-import works!\n")

# Test 3: Import with alias
print("--- Test 3: Import with Alias ---")
import math as m
import sys as system

print("m.pi:", m.pi)
print("system.platform:", system.platform)
print("Import alias works!\n")

# Test 4: Multiple from-imports
print("--- Test 4: Multiple From-Imports ---")
from math import pi, e, sqrt
from time import time

print("pi:", pi)
print("e:", e)
print("sqrt(25):", sqrt(25))
print("time():", time())
print("Multiple from-imports work!\n")

# Test 5: Star import (import *)
# This might not be implemented yet
# print("--- Test 5: Star Import ---")
# from math import *
# print("pi from star import:", pi)

print("=== Import System Tests Complete ===")
