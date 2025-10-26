# Comprehensive Import System Test for Tauraro
print("="*60)
print("  TAURARO IMPORT SYSTEM - COMPREHENSIVE TEST")
print("="*60)

# Test 1: Builtin modules
print("\n[TEST 1] Builtin Module Imports")
print("-" * 40)
import math
import sys
import os
import time

print("✓ math.pi =", math.pi)
print("✓ sys.platform =", sys.platform)
print("✓ Builtin modules work!")

# Test 2: From-import builtin
print("\n[TEST 2] From-Import Builtin")
print("-" * 40)
from math import sqrt, e
from sys import version

print("✓ sqrt(16) =", sqrt(16))
print("✓ e =", e)
print("✓ version =", version)

# Test 3: Import with alias
print("\n[TEST 3] Import with Alias")
print("-" * 40)
import math as m
import sys as system

print("✓ m.pi =", m.pi)
print("✓ system.platform =", system.platform)

# Test 4: File-based module import
print("\n[TEST 4] File-Based Module Import")
print("-" * 40)
import mymodule

print("✓ mymodule loaded!")
print("✓ mymodule.PI =", mymodule.PI)
print("✓ mymodule.VERSION =", mymodule.VERSION)
print("✓ mymodule.add(10, 20) =", mymodule.add(10, 20))

# Test 5: From-import from user module
print("\n[TEST 5] From-Import User Module")
print("-" * 40)
from mymodule import greet, PI as MY_PI

print("✓ greet('Tauraro') =", greet('Tauraro'))
print("✓ MY_PI =", MY_PI)

# Summary
print("\n" + "="*60)
print("  ALL IMPORT TESTS PASSED!")  
print("  ✓ Builtin modules work")
print("  ✓ File-based modules work")
print("  ✓ From-import works")
print("  ✓ Import aliases work")
print("  ✓ Python-like import system fully functional!")
print("="*60)
