#!/usr/bin/env python3
"""Test script to verify FFI modules can be imported and used"""

import math
import sys
import os
import time
import random
import json

def test_math():
    """Test math module functions"""
    print("Testing math module...")
    result = math.sqrt(16)
    print(f"  math.sqrt(16) = {result}")
    result = math.sin(0)
    print(f"  math.sin(0) = {result}")
    print(f"  math.pi = {math.pi}")
    print("  Math module OK!")

def test_sys():
    """Test sys module"""
    print("\nTesting sys module...")
    print(f"  sys.platform = {sys.platform}")
    print(f"  sys.version = {sys.version}")
    print("  Sys module OK!")

def test_os():
    """Test os module"""
    print("\nTesting os module...")
    cwd = os.getcwd()
    print(f"  os.getcwd() = {cwd}")
    print(f"  os.name = {os.name}")
    print("  OS module OK!")

def test_time():
    """Test time module"""
    print("\nTesting time module...")
    t = time.time()
    print(f"  time.time() = {t}")
    print("  Time module OK!")

def test_random():
    """Test random module"""
    print("\nTesting random module...")
    r = random.random()
    print(f"  random.random() = {r}")
    ri = random.randint(1, 10)
    print(f"  random.randint(1, 10) = {ri}")
    print("  Random module OK!")

def test_json():
    """Test json module"""
    print("\nTesting json module...")
    data = {"name": "test", "value": 42}
    json_str = json.dumps(data)
    print(f"  json.dumps({data}) = {json_str}")
    print("  JSON module OK!")

if __name__ == "__main__":
    print("=" * 60)
    print("FFI Modules Test Suite")
    print("=" * 60)

    try:
        test_math()
        test_sys()
        test_os()
        test_time()
        test_random()
        test_json()

        print("\n" + "=" * 60)
        print("All tests passed!")
        print("=" * 60)
    except Exception as e:
        print(f"\n[ERROR] Test failed: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
