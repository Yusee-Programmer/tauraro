#!/usr/bin/env python3
"""Comprehensive type conversion test for Tauraro"""

print("=== Type Conversion Test Suite ===\n")

# Test 1: int() conversions
print("Test 1: int() conversions")
a = int(42)
print(f"  int(42) = {a}")

b = int(3.14)
print(f"  int(3.14) = {b}")

c = int("123")
print(f"  int('123') = {c}")

d = int(True)
print(f"  int(True) = {d}")

e = int(False)
print(f"  int(False) = {e}")

# Test 2: float() conversions
print("\nTest 2: float() conversions")
f = float(42)
print(f"  float(42) = {f}")

g = float(3.14)
print(f"  float(3.14) = {g}")

h = float("2.5")
print(f"  float('2.5') = {h}")

i = float(True)
print(f"  float(True) = {i}")

# Test 3: str() conversions
print("\nTest 3: str() conversions")
j = str(42)
print(f"  str(42) = '{j}'")

k = str(3.14)
print(f"  str(3.14) = '{k}'")

l = str(True)
print(f"  str(True) = '{l}'")

m = str(False)
print(f"  str(False) = '{m}'")

# Test 4: bool() conversions
print("\nTest 4: bool() conversions")
n = bool(0)
print(f"  bool(0) = {n}")

o = bool(42)
print(f"  bool(42) = {o}")

p = bool(0.0)
print(f"  bool(0.0) = {p}")

q = bool(3.14)
print(f"  bool(3.14) = {q}")

r = bool("")
print(f"  bool('') = {r}")

s = bool("hello")
print(f"  bool('hello') = {s}")

# Test 5: Mixed arithmetic with type conversions
print("\nTest 5: Mixed arithmetic")
t = int(10) + int(5)
print(f"  int(10) + int(5) = {t}")

u = float(3.5) + float(2.5)
print(f"  float(3.5) + float(2.5) = {u}")

v = int(10) + float(2.5)
print(f"  int(10) + float(2.5) = {v}")

# Test 6: Type conversions in expressions
print("\nTest 6: Type conversions in expressions")
w = str(int("42") + int("8"))
print(f"  str(int('42') + int('8')) = '{w}'")

x = bool(int("0"))
print(f"  bool(int('0')) = {x}")

y = bool(int("42"))
print(f"  bool(int('42')) = {y}")

# Test 7: Chained conversions
print("\nTest 7: Chained conversions")
z = float(str(42))
print(f"  float(str(42)) = {z}")

aa = int(float("3.14"))
print(f"  int(float('3.14')) = {aa}")

# Test 8: Type preservation
print("\nTest 8: Type preservation")
val_int = 42
val_float = 3.14
val_str = "hello"
val_bool = True

print(f"  int var: {val_int}")
print(f"  float var: {val_float}")
print(f"  str var: {val_str}")
print(f"  bool var: {val_bool}")

# Test 9: Comparisons with type conversions
print("\nTest 9: Comparisons")
if int("42") == 42:
    print("  int('42') == 42: True")

if float("3.14") > 3:
    print("  float('3.14') > 3: True")

if bool(1):
    print("  bool(1): True")

# Test 10: Type compatibility
print("\nTest 10: Type compatibility")
def add_numbers(a, b):
    return a + b

result1 = add_numbers(10, 20)
print(f"  add(10, 20) = {result1}")

result2 = add_numbers(3.5, 2.5)
print(f"  add(3.5, 2.5) = {result2}")

result3 = add_numbers(int("5"), int("7"))
print(f"  add(int('5'), int('7')) = {result3}")

print("\n=== All Type Conversion Tests Completed ===")
