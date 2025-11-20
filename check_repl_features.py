#!/usr/bin/env python3
"""
Simple test to check which REPL features work and which don't
"""

import subprocess
import sys
import os

def run_repl_test(code):
    """Run code in REPL and return output"""
    try:
        process = subprocess.Popen(
            ['./target/release/tauraro.exe', 'repl'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        input_data = code + "\nexit()\n"
        stdout, stderr = process.communicate(input=input_data)
        return stdout, stderr
    except Exception as e:
        return "", str(e)

print("TAURARO REPL FEATURE CHECK")
print("=" * 60)

# Test 1: Simple expression
print("\n[1] Simple expression (print(1 + 2))")
out, err = run_repl_test("print(1 + 2)")
if "3" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 2: Multiline if
print("\n[2] Multiline if statement")
code = """if True:
    print('yes')"""
out, err = run_repl_test(code)
if "yes" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")
    print(f"  Stderr: {err[-200:]}")

# Test 3: For loop
print("\n[3] For loop")
code = """for i in range(3):
    print(i)"""
out, err = run_repl_test(code)
if "0" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 4: Function definition
print("\n[4] Function definition")
code = """def add(a, b):
    return a + b
print(add(2, 3))"""
out, err = run_repl_test(code)
if "5" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")
    print(f"  Stderr: {err[-200:]}")

# Test 5: Try-except
print("\n[5] Try-except")
code = """try:
    x = 1 / 0
except:
    print('caught')"""
out, err = run_repl_test(code)
if "caught" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 6: Class definition
print("\n[6] Class definition")
code = """class Point:
    def __init__(self, x):
        self.x = x
p = Point(5)
print(p.x)"""
out, err = run_repl_test(code)
if "5" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 7: Lambda
print("\n[7] Lambda")
out, err = run_repl_test("f = lambda x: x * 2\nprint(f(5))")
if "10" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 8: List comprehension
print("\n[8] List comprehension")
out, err = run_repl_test("x = [i*2 for i in range(3)]\nprint(x)")
if "[" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 9: Exception chaining
print("\n[9] Exception chaining")
code = """try:
    raise ValueError('orig')
except ValueError as e:
    raise RuntimeError('wrapped') from e"""
out, err = run_repl_test(code)
if "RuntimeError" in (out + err):
    print("  WORKS (raised exception as expected)")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

# Test 10: Generator
print("\n[10] Generator")
code = """def gen():
    yield 1
    yield 2
print(list(gen()))"""
out, err = run_repl_test(code)
if "[1, 2]" in out or "1" in out:
    print("  WORKS")
else:
    print("  FAILS")
    print(f"  Output: {out[-200:]}")

print("\n" + "=" * 60)
print("Check above to see which features need REPL improvements")
